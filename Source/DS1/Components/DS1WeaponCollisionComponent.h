#pragma once

#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DS1WeaponCollisionComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

/**
 * 무기 충돌 컴포넌트
 * 이 컴포넌트는 무기의 충돌을 처리하고, 충돌 시 이벤트를 발생시킵니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DS1_API UDS1WeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHitActor OnHitActor;

protected:
	// 트레이스 시작 위치 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Collision")
	FName TraceStartSocketName = "WeaponStart";

	// 트레이스 끝 위치 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Collision")
	FName TraceEndSocketName = "WeaponEnd";

protected:
	// Sphere 크기(반지름)
	UPROPERTY(EditAnywhere, Category = "Collision")
	float TraceRadius = 20.f;

	// Trace 대상 오브젝트 타입
	UPROPERTY(EditAnywhere, Category = "Collision")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	// 충돌 제외 Actors
	UPROPERTY(EditAnywhere, Category = "Collision")
	TArray<AActor*> IgnoreActors;

	// 디버그 드로우 타입
	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	// 무기의 메시 컴포넌트
	// StaticMesh, SkeletalMesh 등
	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;

	// 이미 충돌한 액터 목록
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	bool bIsCollisionEnabled = false;

public:

	UDS1WeaponCollisionComponent();

protected:

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();
	
	void TurnOffCollision();

	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);

	void AddIgnoreActor(AActor* Actor);

	void RemoveIgnoreActor(AActor* Actor);

protected:
	bool CanHitActor(const AActor* Actor) const;
	void CollisionTrace();
};
