// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomSaveClass.generated.h"

/**
 *
 */
UCLASS()
class CODE_API UCustomSaveClass : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere)
	FVector playerPosition;
};
