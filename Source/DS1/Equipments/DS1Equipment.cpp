#include "DS1Equipment.h"

#include "GameFramework/Character.h"

ADS1Equipment::ADS1Equipment()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ADS1Equipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADS1Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADS1Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void ADS1Equipment::EquipItem()
{
}

void ADS1Equipment::UnequipItem()
{
}

void ADS1Equipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),SocketName);
		}
	}
}

