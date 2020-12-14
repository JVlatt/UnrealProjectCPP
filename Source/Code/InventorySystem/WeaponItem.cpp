// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

void UWeaponItem::Use(class ACodeCharacter* Character)
{
	if (Character)
	{
		Character->Damages += AttackAmount;
	}
	if (OwningInventory)
		OwningInventory->RemoveItem(this);
}