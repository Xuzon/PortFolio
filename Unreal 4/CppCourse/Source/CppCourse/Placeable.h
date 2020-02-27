// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Placeable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlacedGem);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPCOURSE_API UPlaceable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlaceable();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* overlapable;
	UPROPERTY(BlueprintAssignable)
		FPlacedGem placedGem;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
