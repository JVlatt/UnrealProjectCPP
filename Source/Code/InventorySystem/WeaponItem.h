// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CodeCharacter.h"
#include "Item.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class CODE_API UWeaponItem : public UItem
{
	GENERATED_BODY()
protected:

	virtual void Use(class ACodeCharacter* Character) override;
};
