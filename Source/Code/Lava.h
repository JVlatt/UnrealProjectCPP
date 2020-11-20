// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lava.generated.h"

UCLASS()
class CODE_API ALava : public AActor
{
	GENERATED_BODY()

	FTimerHandle killTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ALava();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* particleToSpawn;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnOtherHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void KillPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* boxComp;
};
