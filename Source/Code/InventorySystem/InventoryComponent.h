// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CODE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int Capacity;
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class UItem*> DefaultItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
		TArray<class UItem*> Items;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventoryUpdated OnInventoryUpdated;

	bool AddItem(class UItem* itemToAdd);
	bool RemoveItem(class UItem* itemToRemove);

};
