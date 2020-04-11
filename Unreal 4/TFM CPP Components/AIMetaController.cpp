// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMetaController.h"


// Sets default values
AAIMetaController::AAIMetaController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIMetaController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIMetaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < controllersPerTick; ++i, ++listPointer)
	{
		if (listPointer >= controllerList.Num())
		{
			listPointer = 0;
			return;
		}
		controllerList[listPointer]->callback.Broadcast();
	}
}

///<summary>
/// Add a ticker to this controller
///</summary>
void AAIMetaController::SubscribeTicker(UAIControllerTicker* toAdd)
{
	controllerList.Add(toAdd);
}

///<summary>
	/// Remove a ticker from this controller
	///</summary>
void AAIMetaController::UnSusbcribeTicker(UAIControllerTicker* toRemove)
{
	controllerList.Remove(toRemove);
}

