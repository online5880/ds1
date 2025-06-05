#include "Items/DS1PickupItem.h"

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

void ADS1PickupItem::Interact(AActor* Interactor)
{
	GEngine->AddOnScreenDebugMessage(0,1.5f,FColor::Cyan,FString::Printf(TEXT("ADS1PickupItem Interact")));
}

