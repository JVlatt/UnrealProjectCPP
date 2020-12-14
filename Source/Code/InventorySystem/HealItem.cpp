// Fill out your copyright notice in the Description page of Project Settings.


#include "HealItem.h"

void UHealItem::Use(class ACodeCharacter* Character)
{
	if (Character)
	{
		Character->Heal(HealAmount);
	}
	if (OwningInventory)
		OwningInventory->RemoveItem(this);
}
