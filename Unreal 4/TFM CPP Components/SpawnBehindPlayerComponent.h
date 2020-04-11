// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "SpawnerComponent.h"
#include "SpawnBehindPlayerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API USpawnBehindPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnBehindPlayerComponent();

protected:
	float lastTimeSpawned = 0;
	bool active = true;
	USpawnerComponent* spawner = nullptr;
	APlayerController* playerController = nullptr;
	AActor* actorToFollow = nullptr;
	// Called when the game starts
	virtual void BeginPlay() override;
	FVector GetNotScreenPos(FVector dir) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		float timeBetweenSpawns = 5;	
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int numberOfSpawns = 5;	
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int maxDistanceToSpawnInMeters = 40;
	UFUNCTION(BlueprintCallable)
		void Start();	
	UFUNCTION(BlueprintCallable)
		void Stop();
	UFUNCTION(BlueprintCallable)
		void SetActor(AActor* actor);

		
};