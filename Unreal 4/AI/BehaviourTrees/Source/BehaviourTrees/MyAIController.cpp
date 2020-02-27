// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "MyTargetPoint.h"
#include "EngineUtils.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


void AMyAIController::UpdateNextTargetPoint()
{
	UBlackboardComponent* blackboard = BrainComponent->GetBlackboardComponent();

	int32 targetPointIndex = blackboard->GetValueAsInt("TargetPointIndex");

	if (targetPointIndex >= 4)
	{
		targetPointIndex = 0;
		blackboard->SetValueAsInt("TargetPointIndex", targetPointIndex);
	}

	for (TActorIterator<AMyTargetPoint> it(GetWorld()); it; ++it)
	{
		AMyTargetPoint* target = *it;
		if (target->position == targetPointIndex)
		{
			blackboard->SetValueAsVector("TargetPointPosition", target->GetActorLocation());
			break;
		}
	}

	blackboard->SetValueAsInt("TargetPointIndex", targetPointIndex + 1);
}
