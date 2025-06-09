#include "Items/DS1PickupItem.h"

#include "Equipments/DS1Equipment.h"

ADS1PickupItem::ADS1PickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupItemMesh"));
	SetRootComponent(Mesh);
	
}

void ADS1PickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}


void ADS1PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADS1PickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ADS1Equipment* CDO = EquipmentClass->GetDefaultObject<ADS1Equipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
		}
	}
}

void ADS1PickupItem::Interact(AActor* InteractionActor)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractionActor;

	ADS1Equipment* SpawnItem = GetWorld()->SpawnActor<ADS1Equipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
}

