// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ItemData.h"
#include "Engine/DataTable.h"

UItem::UItem()
{
	Weight = 1.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UItem::InitData(int id, UDataTable* dataTable)
{
	ItemDisplayName = FText::FromString(dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->ItemName);
	Thumbnail = dataTable->FindRow<FItemData>(FName(FString::FromInt(id)), "", false)->TexturePath;
}

