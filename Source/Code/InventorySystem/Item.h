// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

/**
 *
 */
UCLASS(Abstract, BluePrintType, BluePrintable, EditInlineNew, DefaultToInstanced)
class CODE_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	UPROPERTY(Transient)
		class UWorld* World;

	virtual class UWorld* GetWorld() const { return World; };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (Multiline = true))
		FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
		float Weight;

	UPROPERTY()
		class UInventoryComponent* OwningInventory;

	virtual void Use(class ACodeCharacter* Character) PURE_VIRTUAL(UItem, );

	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class ACodeCharacter* Character);
	void InitData(int id, UDataTable* dataTable);
};
