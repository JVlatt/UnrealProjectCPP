// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20;
}

bool UInventoryComponent::AddItem(class UItem* itemToAdd)
{
	if (Items.Num() >= Capacity || !itemToAdd) return false;

	itemToAdd->OwningInventory = this;
	itemToAdd->World = GetWorld();
	Items.Add(itemToAdd);
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(class UItem* itemToRemove)
{
	if (!itemToRemove) return false;

	itemToRemove->OwningInventory = nullptr;
	itemToRemove->World = nullptr;
	Items.RemoveSingle(itemToRemove);
	OnInventoryUpdated.Broadcast();

	return true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		Items.Add(Item);
	}
	
}


