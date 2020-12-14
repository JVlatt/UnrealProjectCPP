// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "ItemActor.generated.h"

UCLASS()
class CODE_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItemActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
		int itemId;

	UItem* itemData;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
