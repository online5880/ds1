#include "DS1EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/DS1Enemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DS1EnemyAIController)

ADS1EnemyAIController::ADS1EnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
}

void ADS1EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<ADS1Enemy>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget,0.1f,true);
}

void ADS1EnemyAIController::OnUnPossess()
{
	ControlledEnemy = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnUnPossess();
}

void ADS1EnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (OutActors.Contains(PlayerCharacter))
	{
		SetTarget(PlayerCharacter);
		ControlledEnemy->ToggleHealthBarVisibility(true);
	}
	else
	{
		SetTarget(nullptr);
		ControlledEnemy->ToggleHealthBarVisibility(false);
	}
}

void ADS1EnemyAIController::SetTarget(AActor* NewTarget) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("Target"), NewTarget);
	}
}
