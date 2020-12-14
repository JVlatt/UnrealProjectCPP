// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "InventoryComponent.h"
#include "../CodeGameMode.h"
#include "../CodeCharacter.h"
#include "Components/SphereComponent.h"
#include "ItemData.h"
#include "HealItem.h"
#include "ArmorItem.h"
#include "WeaponItem.h"
// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::BeginOverlap);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
    FString Category = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode())->ItemsDataTable->FindRow<FItemData>(FName(FString::FromInt(itemId)), "", false)->Category;
    if (Category == "Heal")
    {
        itemData = NewObject<UHealItem>();
        itemData->InitData(itemId, Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode())->ItemsDataTable);
    }
    else if (Category == "Armor")
    {
        itemData = NewObject<UArmorItem>();
        itemData->InitData(itemId, Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode())->ItemsDataTable);
    }
    else if (Category == "Weapon")
    {
        itemData = NewObject<UWeaponItem>();
        itemData->InitData(itemId, Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode())->ItemsDataTable);
    }
    
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    ACodeCharacter* character = Cast<ACodeCharacter>(OtherActor);
    if (character)
    {
        character->Inventory->AddItem(itemData);
        Destroy();
    }
}
