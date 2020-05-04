///<summary>
/// this class is used to subscribe to the MetaController and let my "callback" tick
/// managed by the meta controller, so I can manage how many actors, components etc... are called
/// each frame
///</summary>
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIControllerTicker.generated.h"

class AAIMetaController;

///<summary>
/// Declare a type of multicasting delegate called, FOnTicked, 
/// we need the F because unreal reflection system needs it
///</summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTicked);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API UAIControllerTicker : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIControllerTicker();

protected:
	///<summary>
	/// Get the reference to the meta controller using Unreal API to search for the class
	/// If found subscribe this ticker to the meta controller 
	///</summary>
	virtual void BeginPlay() override;
	AAIMetaController* metaController;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	///<summary>
	/// callback to call from AIMetaController
	///</summary>
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnTicked callback;
		
};
