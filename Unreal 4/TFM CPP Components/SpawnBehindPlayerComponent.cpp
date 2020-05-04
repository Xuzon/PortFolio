// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBehindPlayerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationDataInterface.h"

// Sets default values for this component's properties
USpawnBehindPlayerComponent::USpawnBehindPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USpawnBehindPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	playerController = this->GetOwner()->GetWorld()->GetFirstPlayerController();
	spawner = Cast<USpawnerComponent>(GetOwner()->GetComponentByClass(USpawnerComponent::StaticClass()));
}


///<summary>
/// Periodically, after "timeBetweenSpawns" use the spawner to spawn "numberOfSpawns" actors outside the screen
///</summary>
void USpawnBehindPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (actorToFollow == nullptr || spawner == nullptr || playerController == nullptr  || !active)
	{
		return;
	}
	float timeSinceLastSpawn = this->GetOwner()->GetGameTimeSinceCreation() - lastTimeSpawned;
	if (timeSinceLastSpawn > timeBetweenSpawns)
	{
		lastTimeSpawned = this->GetOwner()->GetGameTimeSinceCreation();
		for (int i = 0; i < numberOfSpawns; ++i)
		{
			FVector nonScreenPos = GetNotScreenPos(-actorToFollow->GetActorForwardVector());
			spawner->SpawnOnPos(nonScreenPos);
		}
	}
}

///<summary>
/// Get a position outside of the screen direction "dir" from "actor to follow" into the Navmesh
///</summary>
FVector USpawnBehindPlayerComponent::GetNotScreenPos(FVector dir) const
{
	FVector toRet = FVector::ZeroVector;
	//iterate over diferent positions from the actor and check it is outside the screen
	for (int i = 0; i < maxDistanceToSpawnInMeters; ++i)
	{
		toRet = actorToFollow->GetActorLocation() + dir * i * 100;
		FVector2D screenPos;
		if(!playerController->ProjectWorldLocationToScreen(toRet, screenPos))
		{
			UE_LOG(LogTemp, Warning, TEXT("Calculated non screen position"));
			break;
		}
		toRet = FVector::ZeroVector;
	}
	//if didn't get outside screen pos, get the max position and log
	if (toRet.SizeSquared() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Position to screen too short"));
		toRet = actorToFollow->GetActorLocation() + dir * maxDistanceToSpawnInMeters * 100;
	}
	//project the point into the navmesh, as I'm using it for AI
	const UNavigationSystemBase* navigation = this->GetOwner()->GetWorld()->GetNavigationSystem();
	if (navigation != nullptr && navigation->GetMainNavData() != nullptr)
	{
		FNavLocation navHit;
		if (navigation->GetMainNavData()->ProjectPoint(toRet, navHit, FVector(1000, 1000, 1000)))
		{
			toRet = navHit.Location;
		}
	}
	return toRet;
}


void USpawnBehindPlayerComponent::Start()
{
	active = true;
}

void USpawnBehindPlayerComponent::Stop()
{
	active = false;
}

void USpawnBehindPlayerComponent::SetActor(AActor* actor)
{
	actorToFollow = actor;
}


