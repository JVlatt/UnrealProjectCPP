// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "../CodeCharacter.h"
#include "HealItem.generated.h"

/**
 * 
 */
UCLASS()
class CODE_API UHealItem : public UItem
{
	GENERATED_BODY()
	
protected:

	virtual void Use(class ACodeCharacter* Character) override;

};
