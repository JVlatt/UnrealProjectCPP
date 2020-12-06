// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodeGameMode.h"
#include "CodeCharacter.h"
#include "CustomSaveClass.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACodeGameMode::ACodeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACodeGameMode::BeginPlay()
{
	if (UGameplayStatics::DoesSaveGameExist("0", 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Save Loaded"));
		LoadAtSlot("0");
	}
}

void ACodeGameMode::ToggleInventory()
{
	if (InventoryUIClass)
	{
		if (!InventoryWidget)
		{
			InventoryWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), InventoryUIClass);
			if (!InventoryWidget)
				return;
			InventoryWidget->AddToViewport();
		}
	}
	if (isInventoryOpened)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		isInventoryOpened = false;
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		isInventoryOpened = true;
		
	}
}

void ACodeGameMode::Pause()
{
	if (PauseUIClass)
	{
		if (!PauseWidget)
		{
			PauseWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseUIClass);
			if (!PauseWidget)
				return;
			PauseWidget->AddToViewport();
		}
	}
	if (GetWorld()->GetFirstPlayerController()->IsPaused())
	{
		PauseWidget->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}
	else
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}

void ACodeGameMode::Respawn()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->Destroy();
	RestartPlayerAtPlayerStart(GetWorld()->GetFirstPlayerController(),FindPlayerStart(GetWorld()->GetFirstPlayerController()));
}

void ACodeGameMode::SaveAtSlot(FString slot)
{
	USaveGame* newSave = UGameplayStatics::CreateSaveGameObject(SaveClass);
	UCustomSaveClass* castedSave = Cast<UCustomSaveClass>(newSave);
	if (castedSave != nullptr)
	{
		castedSave->playerPosition = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	}
	UGameplayStatics::SaveGameToSlot(castedSave, slot, 0);
}

void ACodeGameMode::DeleteAtSlot(FString slot)
{
	UGameplayStatics::DeleteGameInSlot(slot, 0);
}

void ACodeGameMode::LoadAtSlot(FString slot)
{
	if (UGameplayStatics::DoesSaveGameExist(slot, 0))
	{
		USaveGame* loadedSave = UGameplayStatics::LoadGameFromSlot(slot, 0);
		UCustomSaveClass* castedSave = Cast<UCustomSaveClass>(loadedSave);
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorLocation(castedSave->playerPosition);
	}
}


