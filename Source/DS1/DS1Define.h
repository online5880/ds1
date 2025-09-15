#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class EDS1AttributeType : uint8
{
	Stamina,
	Health,
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right,
	Up,
	Down,
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None,
	Left,
	Right,
	Up,
	Down,
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None,
	SwordShield,
	TwoHanded,
	MeleeFists
};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	MainCollision,
	SecondCollision
};

UENUM(BlueprintType)
enum class EDS1AIBehavior : uint8
{
	Idle,			// 기본
	Patrol,			// 순찰
	MeleeAttack,	// 근접공격
	Approach,		// 따라가기
};