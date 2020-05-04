// Fill out your copyright notice in the Description page of Project Settings.


#include "AITargetPoint.h"
#include "DrawDebugHelpers.h"

void AAITargetPoint::SetUpdate(bool toSet)
{
	canUpdate = toSet;
}

void AAITargetPoint::Update()
{
	if (!canUpdate)
	{
		return;
	}
	FVector toSet = CalculateWaypointPerpendicularPosition();
	SetActorLocation(toSet);
	DrawDebugSphere(GetWorld(), toSet, 25, 12, FColor::Purple);
	if (!IsValid(controlledFan) || controlledFan->IsHidden())
	{
		DestroyWaypoint();
	}
}

void AAITargetPoint::SetControlledPawn(APawn* pawn)
{
	controlledFan = pawn;
}

FVector AAITargetPoint::CalculateWaypointPerpendicularPosition()
{
	//TODO
	return FVector::ZeroVector;
}

void AAITargetPoint::DestroyWaypoint()
{
	this->Destroy();
}
