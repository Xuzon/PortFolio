// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrazyArmsAnimator.h"
#include "IKCrazyArmsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API UIKCrazyArmsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIKCrazyArmsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//arm rotators
	FRotator rightArmRotator;
	FRotator leftArmRotator;
	//speed and angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrazyArms)
	float animSpeed = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrazyArms)
	float maxAngle = 90;
	//x = 0, y = 1, z = 2
	int rightAxis = 0;
	int leftAxis = 0;
	//Blueprint animator ref
	UCrazyArmsAnimator* animator = nullptr;

	//store what direction I'm adding
	FVector rightArmDir = FVector(1,1,1);
	FVector leftArmDir = FVector(1,1,1);

	void AddToRotator(FRotator& rotator, int& axis, float angleToAdd, FVector& dir);
	void AddToAngle(float& angle, float angleToAdd, int& axis, float& dir);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "Methods")
	void DoAnim(float DeltaTime);
		
};
