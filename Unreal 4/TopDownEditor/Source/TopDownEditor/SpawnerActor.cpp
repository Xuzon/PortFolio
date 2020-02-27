// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerActor.h"
#include "Engine/World.h"
#include "Engine/DocumentationActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASpawnerActor::ASpawnerActor() : offset(FVector(0, 0, 100)), arrowColor(FColor::Red), spawnedActor(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnerActor::SpawnActor()
{
	FTransform spawnedTransform = GetTransform();
	switch(dir)
	{
		case Dir_Up:
			offset = FTransform(FVector(0,0,200));
		break;
		case Dir_Down:
				offset = FTransform(FVector(0,0,-200));
		break;
		case Dir_Left:
				offset = FTransform(FVector(0,-200,0));
		break;
		case Dir_Right:
				offset = FTransform(FVector(0,200,0));
		break;
	}
	spawnedTransform = offset * spawnedTransform;
	spawnedActor = GetWorld()->SpawnActor<AActor>(classToSpawn.Get(), spawnedTransform);
	OnActorSpawnChanged(HasSpawned());
}

bool ASpawnerActor::HasSpawned() const
{
	return spawnedActor != nullptr;
}

void ASpawnerActor::DestroyActor()
{
	if(spawnedActor != nullptr)
	{
		spawnedActor->Destroy();
		spawnedActor = nullptr;
		OnActorSpawnChanged(HasSpawned());
	}
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
// 	Super::Tick(DeltaTime);
// #if ENABLE_DRAW_DEBUG && WITH_EDITOR
// 	if (IsSelectedInEditor())
// 	{
// 		FTransform spawnedTransform = GetTransform();
// 		FVector start = spawnedTransform.GetTranslation();
// 		spawnedTransform = offset * spawnedTransform;
// 		FVector end = spawnedTransform.GetTranslation();
// 		DrawDebugDirectionalArrow(GetWorld(), start, end, 100.0f, arrowColor);
// 	}
// #endif
}

bool ASpawnerActor::IsTickableInEditor() const
{
	return true;
}

#if WITH_EDITOR
void ASpawnerActor::PostEditChangeProperty(FPropertyChangedEvent &event)
{
	Super::PostEditChangeProperty(event);
	if(event.Property == nullptr)
	{
		return;
	}
	auto classifiedName = GET_MEMBER_NAME_CHECKED(ASpawnerActor, classToSpawn);
	if (event.GetPropertyName().IsEqual(classifiedName))
	{
		arrowColor = classToSpawn != nullptr ? FColor::Cyan : FColor::Red;
	}
}
#endif
