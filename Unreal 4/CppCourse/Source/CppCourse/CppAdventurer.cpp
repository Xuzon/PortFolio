// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAdventurer.h"

ACppAdventurer::ACppAdventurer()
{
	PrimaryActorTick.bCanEverTick = true;
	auto move = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	move->MaxSpeed = 1200.f;
	CreateDefaultSubobject<UInputComponent>(TEXT("Input"));
	CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("P"));
	CreateDefaultSubobject<UTransporter>(TEXT("T"));
}

void ACppAdventurer::Tick(float DeltaTime)
{
}