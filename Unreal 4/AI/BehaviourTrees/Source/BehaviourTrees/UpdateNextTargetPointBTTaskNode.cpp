// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateNextTargetPointBTTaskNode.h"
#include "MyAIController.h"

FString UUpdateNextTargetPointBTTaskNode::GetStaticDescription() const
{
	return TEXT("Refresh the next waypoint to use");
}

EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AMyAIController* controller = Cast<AMyAIController>(ownerComp.GetOwner());
	controller->UpdateNextTargetPoint();


}