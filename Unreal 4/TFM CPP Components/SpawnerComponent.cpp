#include "SpawnerComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

///<summary>
/// Intitialize the targets, initialize the pool, set the spawn timer
///</summary>
void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeTargets();
	FillPool();
	//set timer
	GetWorld()->GetTimerManager().SetTimer(timeHandler, this, &USpawnerComponent::Spawn, delay, true, 0.0f);
	if (!willSpawn)
	{
		GetWorld()->GetTimerManager().PauseTimer(timeHandler);
	}
}

///<summary>
/// If had no assigned targets get self or attached targets
///</summary>
void USpawnerComponent::InitializeTargets()
{
	// If had assigned targets don't mess with it
	if (targets.Num() > 0)
	{
		return;
	}
	//Check if have attached actors or I'm the target
	TArray<AActor *> attachedActors;
	GetOwner()->GetAttachedActors(attachedActors, true);
	if (attachedActors.Num() == 0)
	{
		targets.Add(GetOwner());
	}
	else
	{
		for (auto child : attachedActors)
		{
			targets.Add(child);
		}
	}
}

///<summary>
/// Callback that is called when an object I spawned is destroyed
///</summary>
void USpawnerComponent::OnDestroyedPooledActor(AActor *actor)
{
	UE_LOG(LogTemp, Warning, TEXT("SOMETHING HAS KILLED A FAN"));
	pool.Remove(actor);
}

///<summary>
/// Spawn and fill the pool, disable non desired actors
///</summary>
void USpawnerComponent::FillPool()
{
	FActorSpawnParameters params = FActorSpawnParameters();
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//Instantiate the maximum amount of actors. Disable the ones not wanted initially.
	for (int i = 0; i < maxActors; i++)
	{
		AActor *toAdd = GetWorld()->SpawnActor<AActor>(
			spawnableBlueprint.Get(),
			FTransform(targets[FMath::RandRange(0, targets.Num() - 1)]->GetActorLocation() + spawnOffset),
			params);
		if (toAdd == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn: %d"), i);
			continue;
		}
		pool.Add(toAdd);
		toAdd->OnDestroyed.AddDynamic(this, &USpawnerComponent::OnDestroyedPooledActor);
		if (i >= initialActors)
		{
			DeactivateActor(toAdd);
		}
		UE_LOG(LogTemp, Warning, TEXT("Spawnea %d"), i);
	}
}

///<summary>
/// Activate "actorsPerTick" disabled actors of the pool at one targets position
///</summary>
void USpawnerComponent::Spawn()
{
	if (!willSpawn)
	{
		return;
	}
	for (int i = 0; i < actorsPerTick; i++)
	{
		FVector pos = targets[FMath::RandRange(0, targets.Num() - 1)]->GetActorLocation() + spawnOffset;
		SpawnOnPos(pos);
	}
}

///<summary>
/// Spawn a deactivated actor in the position
///</summary>
void USpawnerComponent::SpawnOnPos(FVector pos)
{
	pool.Remove(nullptr);
	//Activate a fan from the pool
	for (auto a : pool)
	{
		if (!IsValid(a))
		{
			UE_LOG(LogTemp, Warning, TEXT("NULL PTR ON SPAWN, THIS WILL NEVER HAVE TO BE TRIGGERED"));
			continue;
		}
		if (a->IsHidden())
		{

			UE_LOG(LogTemp, Warning, TEXT("Spawnea timer"));
			FRotator rotation = FRotator(0.f, 0.f, 0.f);
			ActivateActor(a, pos, rotation);
			break;
		}
	}
}

///<summary>
/// Wrapper to teleport and activate one actor
///</summary>
void USpawnerComponent::ActivateActor(AActor *actor, const FVector posToSpawn, const FRotator rotation)
{
	if (!IsValid(actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL PTR ON ACTIVATE ACTOR, THIS WILL NEVER HAVE TO BE TRIGGERED"));
		return;
	}
	ChangeActorState(actor, true);
	actor->TeleportTo(posToSpawn, rotation, false, true);
	onSpawnedActor.Broadcast(actor);
}

///<summary>
/// Static wrapper to deactivate one actor
///</summary>
void USpawnerComponent::DeactivateActor(AActor *actor)
{
	ChangeActorState(actor, false);
}

///<summary>
//Activates or stops the spawner (timer and flag)
///</summary>
void USpawnerComponent::SetSpawnerActive(bool active)
{
	//check trying to set same state
	if (willSpawn == active)
	{
		return;
	}
	willSpawn = active;
	if (!active)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(timeHandler))
		{
			GetWorld()->GetTimerManager().PauseTimer(timeHandler);
		}
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(timeHandler))
		{
			GetWorld()->GetTimerManager().UnPauseTimer(timeHandler);
		}
	}
}

///<summary>
/// Change the state of all actors in pool
///</summary>
void USpawnerComponent::ChangeAllState(bool state)
{
	int selectedPoint = 0;
	for (int i = 0; i < pool.Num(); ++i)
	{
		AActor *actor = pool[i];
		if (!IsValid(actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("NULL PTR ON CHANGE ALL STATE, THIS WILL NEVER HAVE TO BE TRIGGERED"));
			continue;
		}
		FVector posToSpawn = targets[selectedPoint++]->GetActorLocation();
		if (selectedPoint >= targets.Num())
		{
			selectedPoint = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Chosen location: %s"), *posToSpawn.ToString());
		ChangeActorState(actor, state);
		actor->TeleportTo(posToSpawn, FRotator::ZeroRotator);
	}
}

///<summary>
/// Change the state of an actor, hide/unhide it, disable/enable collision, tick and components
///</summary>
void USpawnerComponent::ChangeActorState(AActor *actor, bool state)
{
	if (!IsValid(actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("WARNING:: CHANGED STATE IN POOL OF DESTROYED ACTOR"));
		return;
	}
	//Set actor visible in game and enable ticking and collisions
	actor->SetActorHiddenInGame(!state);
	actor->SetActorEnableCollision(state);
	actor->SetActorTickEnabled(state);
	for (auto component : actor->GetComponents())
	{
		component->SetComponentTickEnabled(state);
	}
}