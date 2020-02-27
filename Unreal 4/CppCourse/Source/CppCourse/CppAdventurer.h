// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Transporter.h"
#include "CppAdventurer.generated.h"

/**
 * 
 */
UCLASS()
class CPPCOURSE_API ACppAdventurer : public ADefaultPawn
{
	GENERATED_BODY()
public:
	ACppAdventurer();
	void Tick(float DeltaTime);
};
