// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CustomSaveClass.h"

void AMenuGameMode::DeleteAtSlot(FString slot)
{
	if (UGameplayStatics::DoesSaveGameExist(slot, 0))
	{
		UGameplayStatics::DeleteGameInSlot(slot, 0);
	}
}

void AMenuGameMode::LoadAtSlot(FString slot)
{
	if (UGameplayStatics::DoesSaveGameExist(slot, 0))
	{
		UGameplayStatics::SaveGameToSlot(UGameplayStatics::LoadGameFromSlot(slot, 0), "0", 0);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		UGameplayStatics::OpenLevel(GetWorld(), "Level1");
	}
}

void AMenuGameMode::BeginPlay()
{
	if (MainMenuUIClass)
	{
		if (!MenuWidget)
		{
			MenuWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), MainMenuUIClass);
			if (!MenuWidget)
				return;
			MenuWidget->AddToViewport();
		}
	}
}
