#include "Items/DS1PickupItem.h"

#include "DS1Define.h"
#include "Equipments/DS1Equipment.h"

ADS1PickupItem::ADS1PickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupItemMesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
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
			Mesh->SetSimulatePhysics(true);
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

