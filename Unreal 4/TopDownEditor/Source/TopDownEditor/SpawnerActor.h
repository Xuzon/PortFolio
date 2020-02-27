// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tickable.h"
#include "Templates/SubclassOf.h"
#include "SpawnerActor.generated.h"

UENUM(BlueprintType)
enum SpawnDir { Dir_Up, Dir_Down, Dir_Left, Dir_Right };

UCLASS()
class TOPDOWNEDITOR_API ASpawnerActor : public AActor, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnerActor();
	UFUNCTION(BlueprintCallable)
	void SpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//meta=(DisplayName="Offset") -> en unreal se ve como Offset en la UI
	// UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayName = "Offset"))
	FTransform offset;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayName = "Offset"))
		TEnumAsByte<SpawnDir> dir;

	UPROPERTY(EditAnywhere, Category = "Spawner")
		TSubclassOf<AActor> classToSpawn;

	FColor arrowColor;

	AActor* spawnedActor;

	UFUNCTION(BlueprintCallable)
		bool HasSpawned() const;
	UFUNCTION(BlueprintCallable)
		void DestroyActor();
	UFUNCTION(BlueprintImplementableEvent)
		void OnActorSpawnChanged(bool spawned);

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& event) override;
#endif

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsTickableInEditor() const override;
	//return {}; devuelve el tipo con el constructor por defecto
	TStatId GetStatId() const override { return {}; }
};
