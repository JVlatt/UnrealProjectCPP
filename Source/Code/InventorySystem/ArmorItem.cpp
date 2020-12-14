// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"

void UArmorItem::Use(class ACodeCharacter* Character)
{
	if (Character)
	{
		Character->Armor += ArmorAmount;
	}
	if (OwningInventory)
		OwningInventory->RemoveItem(this);
}
