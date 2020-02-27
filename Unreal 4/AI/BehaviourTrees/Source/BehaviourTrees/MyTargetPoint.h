// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "MyTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class BEHAVIOURTREES_API AMyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ejercicio")
	int32 position;
};
