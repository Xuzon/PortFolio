// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Openable.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPPCOURSE_API UOpenable : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void Open();
	UPROPERTY(EditAnywhere)	float _angle = 90.0f;
	UPROPERTY(EditAnywhere) float _secondsToClose = 1.0f;
	UPROPERTY(EditAnywhere)	ATriggerVolume* _opener;

	float _secondsRemaining = 0.0f;
};