// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->OnComponentHit.AddDynamic(this, &AProjectile::OnCompHit);
	RootComponent = sphereComp;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &AProjectile::OnOtherHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOtherHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector location = Hit.ImpactPoint;
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(Hit.Location, Hit.Location + Hit.Normal);
	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(location,FRotator());
	UMaterialInterface* mat = FindObject<UMaterialInterface>(NULL, TEXT("/Game/Materials/Splash"));
	if (!mat)
		mat = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/Materials/Splash"));
	decal->SetDecalMaterial(mat);
	decal->SetActorRotation(rotation);
	decal->SetActorScale3D(FVector(0.5f));
	Destroy();
}

void AProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UMaterialInterface* mat = FindObject<UMaterialInterface>(NULL, TEXT("/Game/Materials/Splash"));
		if (!mat)
			mat = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/Materials/Splash"));

		FVector location = UKismetMathLibrary::InverseTransformLocation(OtherComp->GetComponentTransform(), Hit.Location);
		UGameplayStatics::SpawnDecalAttached(mat, FVector(0.5f), OtherComp);
	}
}

