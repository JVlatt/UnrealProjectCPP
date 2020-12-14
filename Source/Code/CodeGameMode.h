// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "CodeGameMode.generated.h"

UCLASS(minimalapi)
class ACodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> PauseUIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* PauseWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> InventoryUIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* InventoryWidget;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<USaveGame> SaveClass;

public:
	bool isInventoryOpened;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDataTable* ItemsDataTable;

	ACodeGameMode();
	UFUNCTION(BlueprintCallable)
	void Pause();
	void Respawn();
	UFUNCTION(BlueprintCallable)
	void SaveAtSlot(FString slot);
	UFUNCTION(BlueprintCallable)
	void DeleteAtSlot(FString slot);
	UFUNCTION(BlueprintCallable)
	void LoadAtSlot(FString slot);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

};



