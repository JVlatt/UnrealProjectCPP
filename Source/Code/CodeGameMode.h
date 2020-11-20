// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CodeGameMode.generated.h"

UCLASS(minimalapi)
class ACodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> PauseUIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* PauseWidget;

public:
	ACodeGameMode();
	UFUNCTION(BlueprintCallable)
	void Pause();
	void Respawn();
};



