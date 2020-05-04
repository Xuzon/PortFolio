#include "SpawnersControllerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnersControllerComponent::USpawnersControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


///<summary>
/// Get all the spawners in the world, get the player, set the nearest spawner
/// with this class setup
///</summary>
void USpawnersControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	GetSpawners();
	player = Cast<AActor>(this->GetOwner()->GetWorld()->GetFirstPlayerController()->GetPawn());
	currentSpawner = GetClosestSpawner(player);
	SetCurrentSpawnerState(startSpawning);
}


///<summary>
/// Get the nearest spawner, if it is different than the current spawner
/// deactivate the current spawner, set the new as current spawner, activate it
///</summary>
void USpawnersControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	USpawnerComponent* closestSpawner = GetClosestSpawner(player);
	if (closestSpawner != currentSpawner)
	{
		SetCurrentSpawnerState(false);
		currentSpawner = closestSpawner;
		SetCurrentSpawnerState(true);
	}
}

///<summary>
/// Set current spawner state with checks
///</summary>
void USpawnersControllerComponent::SetCurrentSpawnerState(bool toSet)
{
	if (currentSpawner != nullptr && !onSafe)
	{
		currentSpawner->SetSpawnerActive(toSet);
	}
}

///<summary>
/// Deactivate current spawner, set flag
///</summary>
void USpawnersControllerComponent::OnEnterSafeZone()
{
	SetCurrentSpawnerState(false);
	onSafe = true;
}

///<summary>
/// Set flag, activate current spawner
///</summary>
void USpawnersControllerComponent::OnExitSafeZone()
{
	onSafe = false;
	SetCurrentSpawnerState(true);
}

///<summary>
/// Get and store all USpawnerComponents from the class given the setup
///</summary>
void USpawnersControllerComponent::GetSpawners()
{
	TArray<AActor*> spawnersActors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), spawnersClassToSearch, spawnersActors);
	for (AActor* actor : spawnersActors)
	{
		USpawnerComponent* spawner = Cast<USpawnerComponent>(actor->GetComponentByClass(USpawnerComponent::StaticClass()));
		if (spawner != nullptr)
		{
			spawners.Add(spawner);
			//add watchdog just in case SOMEONE destroy the spawner
			spawner->GetOwner()->OnDestroyed.AddDynamic(this, &USpawnersControllerComponent::OnDestroyedSpawner);
		}
	}
}

///<summary>
/// Remove the USpawnerComponent of the actor
///</summary>
void USpawnersControllerComponent::OnDestroyedSpawner(AActor* actor)
{
	USpawnerComponent* toRemove = Cast<USpawnerComponent>(actor->GetComponentByClass(USpawnerComponent::StaticClass()));
	spawners.Remove(toRemove);
}

///<summary>
/// Iterate over my actors, return the closest one
///</summary>
USpawnerComponent* USpawnersControllerComponent::GetClosestSpawner(AActor* to) const
{
	USpawnerComponent* toRet = nullptr;
	float minDis = -1;
	if (to == nullptr)
	{
		return toRet;
	}
	for (USpawnerComponent* spawner : spawners)
	{
		float dis = (spawner->GetOwner()->GetActorLocation() - to->GetActorLocation()).SizeSquared();
		if (minDis == -1 || dis < minDis)
		{
			minDis = dis;
			toRet = spawner;
		}
	}
	return toRet;
}

