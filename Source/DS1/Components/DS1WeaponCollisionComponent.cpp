
#include "DS1WeaponCollisionComponent.h"

#include "Logging/StructuredLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1WeaponCollisionComponent)

UDS1WeaponCollisionComponent::UDS1WeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UDS1WeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UDS1WeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}

}

void UDS1WeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UDS1WeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UDS1WeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void UDS1WeaponCollisionComponent::AddIgnoreActor(AActor* Actor)
{
	if (Actor && !IgnoreActors.Contains(Actor))
	{
		IgnoreActors.Add(Actor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("액터가 null이거나 이미 IgnoreActors에 존재합니다: %s"), *GetNameSafe(Actor));
	}
}

void UDS1WeaponCollisionComponent::RemoveIgnoreActor(AActor* Actor)
{
	if (Actor && IgnoreActors.Contains(Actor))
	{
		IgnoreActors.Remove(Actor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("액터가 null이거나 IgnoreActors에 존재하지 않습니다: %s"), *GetNameSafe(Actor));
	}
}

bool UDS1WeaponCollisionComponent::CanHitActor(const AActor* Actor) const
{
	 return AlreadyHitActors.Contains(Actor) == false;
}

void UDS1WeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoreActors,
		DrawDebugType,
		OutHits,
		true
		);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor == nullptr)
			{
				continue;
			}

			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Emplace(HitActor);

				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}

