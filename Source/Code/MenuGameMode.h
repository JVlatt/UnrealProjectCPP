// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MenuGameMode.generated.h"
/**
 * 
 */
UCLASS()
class CODE_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> MainMenuUIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* MenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<USaveGame> SaveClass;
public: 
	UFUNCTION(BlueprintCallable)
		void DeleteAtSlot(FString slot);
	UFUNCTION(BlueprintCallable)
		void LoadAtSlot(FString slot);

	virtual void BeginPlay() override;
};
