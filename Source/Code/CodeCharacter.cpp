// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "CodeGameMode.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "InventorySystem/Item.h"
#include "InventorySystem/InventoryComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACodeCharacter

ACodeCharacter::ACodeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	attachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPoint"));
	attachPoint->SetupAttachment(RootComponent);
	isHolding = false;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	attachPoint->SetupAttachment(RootComponent);

}

void ACodeCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ACodeCharacter::OnPunchNotify);
}
//////////////////////////////////////////////////////////////////////////
// Input

void ACodeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacter::Crouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACodeCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ACodeCharacter::Shoot);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ACodeCharacter::TogglePickup);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACodeCharacter::ToggleAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACodeCharacter::ReleaseAim);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACodeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACodeCharacter::MoveRight);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ACodeCharacter::TogglePause).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ACodeCharacter::ToggleInventory);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACodeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACodeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACodeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACodeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACodeCharacter::OnResetVR);
}

void ACodeCharacter::ToggleCrouch()
{
	if (isAiming)return;
	if (GetCharacterMovement()->IsCrouching())
	{
		animState = 0;
		UnCrouch();
	}
	else
	{
		animState = 1;
		Crouch();
	}
}

void ACodeCharacter::UseItem(UItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this);
	}
}

void ACodeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACodeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACodeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACodeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if (!isAiming)return;
	FRotator rotation = FRotator(0);
	rotation.Yaw = GetControlRotation().Yaw;
	SetActorRotation(rotation);
}

void ACodeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACodeCharacter::MoveForward(float Value)
{
	ZAxis = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACodeCharacter::MoveRight(float Value)
{
	XAxis = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACodeCharacter::ToggleAim()
{
	if (!isHolding) return;
	if (GetCharacterMovement()->IsCrouching())
		UnCrouch();

	isAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	animState = 2;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->SetJumpAllowed(false);
	if (GetWorldTimerManager().IsTimerActive(smoothCameraTimer))
		GetWorldTimerManager().ClearTimer(smoothCameraTimer);
	smoothCameraTimer = FTimerHandle();
	smoothCameraLerp = 0.0f;
	GetWorldTimerManager().SetTimer(smoothCameraTimer, this, &ACodeCharacter::ToggleAimCamera, 0.01f, true, 0);
}

void ACodeCharacter::ToggleAimCamera()
{
	smoothCameraLerp += 0.01f;
	FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), FVector(0, 60, 90), smoothCameraLerp));
	if (smoothCameraLerp >= 1)
		GetWorldTimerManager().ClearTimer(smoothCameraTimer);
}

void ACodeCharacter::ReleaseAimCamera()
{
	smoothCameraLerp += 0.01f;
	FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), FVector(0, 0, 0), smoothCameraLerp));
	if (smoothCameraLerp >= 1)
		GetWorldTimerManager().ClearTimer(smoothCameraTimer);
}

void ACodeCharacter::ReleaseAim()
{
	isAiming = false;
	animState = 0;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->SetJumpAllowed(true);
	if (GetWorldTimerManager().IsTimerActive(smoothCameraTimer))
		GetWorldTimerManager().ClearTimer(smoothCameraTimer);
	smoothCameraTimer = FTimerHandle();
	smoothCameraLerp = 0.0f;
	GetWorldTimerManager().SetTimer(smoothCameraTimer, this, &ACodeCharacter::ReleaseAimCamera, 0.01f, true, 0);
}

void ACodeCharacter::TogglePause()
{
	ACodeGameMode* gM = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());
	if (gM)
		gM->Pause();
}

void ACodeCharacter::ToggleInventory()
{
	ACodeGameMode* gM = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());
	if (gM)
		gM->ToggleInventory();
}

void ACodeCharacter::Shoot()
{
	if (isHolding)
	{
		if (!isAiming || isShooting) return;

		FVector location = GetActorLocation();
		FHitResult hitResult;


		FVector start = location;
		FVector end = start + (FollowCamera->GetForwardVector() * 5000);

		FCollisionQueryParams traceParams;
		traceParams.AddIgnoredActor(this);
		bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, traceParams);
		if (hit)
		{
			FActorSpawnParameters spawnParameters;
			AActor* actorRef = GetWorld()->SpawnActor<AActor>(projectile, location, Controller->GetControlRotation(), spawnParameters);
			isShooting = true;
			shootTimerHandle = FTimerHandle();
			GetWorldTimerManager().SetTimer(shootTimerHandle, this, &ACodeCharacter::ResetShoot, 0.1f, true, 1);
		}
	}
	else if(!GetMesh()->GetAnimInstance()->Montage_IsPlaying(PunchMontage))
	{
		
		GetMesh()->GetAnimInstance()->Montage_Play(PunchMontage);
	}
}

void ACodeCharacter::ResetShoot()
{
	isShooting = false;
	if (GetWorldTimerManager().IsTimerActive(shootTimerHandle))
		GetWorldTimerManager().ClearTimer(shootTimerHandle);
}

void ACodeCharacter::TogglePickup()
{
	if (isHolding)
	{
		Drop();
	}
	else {
		Pickup();
	}
}

void ACodeCharacter::Pickup()
{
	FVector location = GetActorLocation();
	FHitResult hitResult;

	FVector start = location;
	FVector end = start + (FollowCamera->GetForwardVector() * 100);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);
	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, traceParams);
	DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 2.0f);
	if (hit && hitResult.Actor->ActorHasTag("Pickup"))
	{
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		hitResult.Actor->AttachToComponent(attachPoint, rules);
		isHolding = true;
		holdItem = hitResult.Actor.Get();
	}
}

void ACodeCharacter::Drop()
{
	if (holdItem)
	{
		holdItem->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		isHolding = false;
	}
}

void ACodeCharacter::OnPunchNotify(FName notifyName, const FBranchingPointNotifyPayload& branching)
{
	if (notifyName == FName("Punch"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Punch Notify"));
		FRotator rotation = FRotator(0);
		rotation.Yaw = GetControlRotation().Yaw;
		SetActorRotation(rotation);
		TArray<TEnumAsByte<EObjectTypeQuery>> targetObjectsArray;
		TArray<AActor*> ignoredActors;
		TArray<AActor*> overlappedActors;
		targetObjectsArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ignoredActors.Add(this);
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetMesh()->GetSocketLocation(FName("RightHandSocket")), 100, targetObjectsArray, nullptr, ignoredActors, overlappedActors);
		for(AActor* AI : overlappedActors)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, AI->GetName());
			AI->TakeDamage(strength,FDamageEvent(),Controller,this);
		}
	}
}