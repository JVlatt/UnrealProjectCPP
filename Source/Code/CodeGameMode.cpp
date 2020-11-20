// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodeGameMode.h"
#include "CodeCharacter.h"
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
