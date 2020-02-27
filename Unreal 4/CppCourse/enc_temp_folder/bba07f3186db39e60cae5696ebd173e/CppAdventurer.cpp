// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAdventurer.h"

ACppAdventurer::ACppAdventurer()
{
	PrimaryActorTick.bCanEverTick = true;
	auto move = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	move->MaxSpeed = 1200.f;
	CreateDefaultSubobject<UTransporter>(TEXT("T"));
	CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("P"));
}