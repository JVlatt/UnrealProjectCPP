// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ItemData.h"
#include "InventoryComponent.h"
#include "Engine/DataTable.h"

UItem::UItem()
{
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UItem::InitData(int id, UDataTable* dataTable)
{
	ItemDisplayName = FText::FromString(dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->ItemName);
	ItemDescription = FText::FromString(dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->Description);
	Thumbnail = dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->TexturePath;
	UseActionText = FText::FromString(dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->UseText);
	HealAmount = dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->AdditionalHP;
	ArmorAmount = dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->Armor;
	AttackAmount = dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->Damages;
}

void UItem::Use(class ACodeCharacter* Character)
{
	
}