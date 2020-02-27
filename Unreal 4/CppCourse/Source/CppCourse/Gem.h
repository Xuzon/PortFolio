// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Placeable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Gem.generated.h"


UCLASS()
class CPPCOURSE_API AGem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGem();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlaceable* placeable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* sphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
