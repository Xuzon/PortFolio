///<summary>
/// This class will Spawn a pool of actors at the beginning and use the pool to spawn periodically
/// some actors if active
///</summary>
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "SpawnerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedActor,class AActor*, spawnedActor);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API USpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerComponent();

protected:
	///<summary>
	/// Intitialize the targets, initialize the pool, set the spawn timer
	///</summary>
	virtual void BeginPlay() override;
private:
	FTimerHandle timeHandler;

	//Pool that stores all the actors to be spawned
	TArray<AActor*> pool;

	///<summary>
	/// Callback that is called when an object I spawned is destroyed
	///</summary>
	UFUNCTION(BlueprintCallable)
	void OnDestroyedPooledActor(AActor* actor);

	///<summary>
	/// If had no assigned targets get self or attached targets
	///</summary>
	void InitializeTargets();

	///<summary>
	/// Spawn and fill the pool, disable non desired actors
	///</summary>
	void FillPool();

	///<summary>
	/// Wrapper to teleport and activate one actor
	///</summary>
	void ActivateActor(AActor* actor, const FVector posToSpawn, const FRotator rotation);

	///<summary>
	/// Change the state of an actor, hide/unhide it, disable/enable collision, tick and components
	///</summary>
	static void ChangeActorState(AActor* actor, bool state);

public:


	//Points where it's possible to spawn actors
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		TArray<AActor*> targets;
	//Amount fo initial actors
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int initialActors = 3;
	//Actors that will be spawned on each tick
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int actorsPerTick = 1;
	//Delay between actor spawns
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		float delay = 3.f;
	//Max number of actors to be spawned
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int maxActors = 10;
	//Type of actor to be spawned
	UPROPERTY(EditAnywhere, Category = "Spawn Settings", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> spawnableBlueprint;	
	//Offset that I've got when spawning
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		FVector spawnOffset = FVector(100.f,100.f,0.f);
	//True to activate the spawn, false to disable it
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		bool willSpawn;
	///<summary>
	/// Disable any given actor and its components, it doesn't need to be spawned with a spawner
	///</summary>
	UFUNCTION(BlueprintCallable)
		static void DeactivateActor(AActor* actor);
	///<summary>
	//Activates or stops the spawner
	///</summary>
	UFUNCTION(BlueprintCallable)
		void SetSpawnerActive(bool active);
	///<summary>
	/// Change the state of all actors in pool
	///</summary>
	UFUNCTION(BlueprintCallable)
		void ChangeAllState(bool state);
	///<summary>
	/// Activate "actorsPerTick" disabled actors of the pool at one targets position
	///</summary>
	UFUNCTION(BlueprintCallable)
		void Spawn();
	///<summary>
	/// Spawn a deactivated actor in the position
	///</summary>
	UFUNCTION(BlueprintCallable)
		void SpawnOnPos(FVector pos);

	///<summary>
	/// On spawned actor event
	///</summary>
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnSpawnedActor onSpawnedActor;
};
