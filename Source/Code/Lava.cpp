// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CodeGameMode.h"

// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = boxComp;
}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ALava::OnOtherHit);
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALava::OnOtherHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleToSpawn, Hit.Location, FRotator());

		killTimer = FTimerHandle();

		OtherActor->DisableInput(GetWorld()->GetFirstPlayerController());
		GetWorldTimerManager().SetTimer(killTimer, this, &ALava::KillPlayer, 0.1f, true, 0.3f);
	}
}

void ALava::KillPlayer()
{
	ACodeGameMode* gM = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());
	if (gM)
		gM->Respawn();

		GetWorldTimerManager().ClearTimer(killTimer);
}

