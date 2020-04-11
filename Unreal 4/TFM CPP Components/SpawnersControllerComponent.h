// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "SpawnerComponent.h"
#include "SpawnersControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API USpawnersControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnersControllerComponent();

protected:
	TArray<USpawnerComponent*> spawners;
	USpawnerComponent* currentSpawner;
	AActor* player;
	bool onSafe = false;

	///<summary>
	/// Remove the USpawnerComponent of the actor
	///</summary>
	UFUNCTION(BlueprintCallable)
	void OnDestroyedSpawner(AActor* actor);
	///<summary>
	/// Get and store all USpawnerComponents from the class given the setup
	///</summary>
	void GetSpawners();
	///<summary>
	/// Set current spawner state with checks
	///</summary>
	void SetCurrentSpawnerState(bool state);
	///<summary>
	/// Iterate over my actors, return the closest one
	///</summary>
	USpawnerComponent* GetClosestSpawner(AActor* to) const;
	///<summary>
	/// Get all the spawners in the world, get the player, set the nearest spawner
	/// with this class setup
	///</summary>
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "ControllerSettings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> spawnersClassToSearch;
	UPROPERTY(EditAnywhere, Category = "ControllerSettings")
		bool startSpawning = true;

	///<summary>
	/// Get the nearest spawner, if it is different than the current spawner
	/// deactivate the current spawner, set the new as current spawner, activate it
	///</summary>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	///<summary>
	/// Deactivate current spawner, set flag
	///</summary>
	UFUNCTION(BlueprintCallable)
		void OnEnterSafeZone();
	///<summary>
	/// Set flag, activate current spawner
	///</summary>
	UFUNCTION(BlueprintCallable)
		void OnExitSafeZone();
		
};
