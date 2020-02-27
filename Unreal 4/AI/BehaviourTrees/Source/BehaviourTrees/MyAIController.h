// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BEHAVIOURTREES_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable, Category = "Work")
    void UpdateNextTargetPoint();
};
