#pragma once

#include "DS1Define.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DS1TargetingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DS1_API UDS1TargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// LockOn 시킬 후보군을 감지할 영역의 크기(반지름)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	float TargetingRadius = 500.f;

	// LockOn시 카메라 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	float FaceLockOnRotationSpeed = 20.f;

	// 디버그 드로우 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Targeting")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	ACharacter* Character = nullptr;

	UPROPERTY()
	class UCameraComponent* Camera = nullptr;

	// 현재 타겟팅 중인 대상
	UPROPERTY()
	AActor* LockedTargetActor = nullptr;

	// LOckOn 상태 관리
	bool bIsLockOn = false;

public:
	UDS1TargetingComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// LockON On/Off
	void ToggleLockOn();

	// LockOn 대상 스위칭
	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	// LockOn 상태
	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

protected:
	// LockOn 대상 검색
	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;

	// 최종 LockOn 대상 찾기
	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;

	// 캐릭터가 카메라(ControlRotation)의 회전에 동기화 되도록 설정
	void OrientCamera() const;

	// 캐릭터가 이동 바얗ㅇ으로 회전하도록 설정
	void OrientMovement() const;

	// LockedOn 상태에서 카메라 회전 제어
	void FaceLockOnActor() const;

	// LockOn
	void LockOnTarget();

	// LockOn 해제
	void StopLockOn();
	
};
