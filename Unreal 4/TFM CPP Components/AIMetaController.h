// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIControllerTicker.h"
#include "AIMetaController.generated.h"


UCLASS()
class HELLOFFAME_API AAIMetaController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIMetaController();

protected:
	///<summary>
	/// Tickers list
	///</summary>
	TArray< UAIControllerTicker*> controllerList;
	///<summary>
	/// In what ticker I'm in
	///</summary>
	int listPointer = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	///<summary>
	/// How many controllers are ticked per tick of this
	///</summary>
	int controllersPerTick = 10;

	///<summary>
	/// Add a ticker to this controller
	///</summary>
	void SubscribeTicker(UAIControllerTicker* toAdd);
	
	///<summary>
	/// Remove a ticker from this controller
	///</summary>
	void UnSusbcribeTicker(UAIControllerTicker* toRemove);
};
