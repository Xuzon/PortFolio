// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeAIController.h"

AHordeAIController::AHordeAIController()
{
	ticker = CreateDefaultSubobject<UAIControllerTicker>(TEXT("Ticker"));
	ticker->callback.AddDynamic(this, &AHordeAIController::MyTick);
}

AHordeAIController::~AHordeAIController()
{
	if (IsValid(waypoint))
	{
		waypoint->DestroyWaypoint();
	}
}

void AHordeAIController::MyTick()
{
	if (!IsValid(pawn) || !pawn->GetClass()->ImplementsInterface(UIControlable::StaticClass()))
	{
		return;
	}
	if (IIControlable::Execute_IsFollowingPlayer(pawn))
	{
		if (followingPlayer)
		{
			KeepFollowingPlayer();
		}
		else
		{
			followingPlayer = true;
			InitializeWaypoint();
			IIControlable::Execute_GetCharacterMovement(pawn)->MaxWalkSpeed = 500;
			KeepFollowingPlayer();
		}
	}
	else
	{
		if (followingPlayer)
		{
			IIControlable::Execute_GetCharacterMovement(pawn)->StopActiveMovement();
			waypoint->DestroyWaypoint();
			followingPlayer = false;
		}
	}
}

void AHordeAIController::BeginPlay()
{
	pawn = Super::GetPawn();
	if (pawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HordeAIController:: I don't have any pawn"));
	}
	if (pawn->GetClass()->ImplementsInterface(UIControlable::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("HordeAIController:: Pawn does not implement IControlable"));
	}
}

void AHordeAIController::KeepFollowingPlayer()
{
	MoveToActor(waypoint, 1, true, true, true, 0, true);
}

void AHordeAIController::InitializeWaypoint()
{
	//TODO
}

FVector AHordeAIController::CalculateWaypointPerpendicularPosition(float spread, float& initialDistance)
{
	//TODO
	return FVector::ZeroVector;
}