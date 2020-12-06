// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ItemID;

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AdditionalHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Armor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Damages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Location;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* TexturePath;

};

