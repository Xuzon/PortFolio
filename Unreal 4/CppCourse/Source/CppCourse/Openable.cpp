// Fill out your copyright notice in the Description page of Project Settings.


#include "Openable.h"

// Sets default values for this component's properties
UOpenable::UOpenable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOpenable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if ((_secondsRemaining -= DeltaTime) <= 0)
	{
		GetOwner()->SetActorRotation(FRotator(0.f, 0.f, 0.f));
	}

	if (_opener && _opener->IsOverlappingActor(pawn))
	{
		Open();
	}

}

void UOpenable::Open()
{
	GetOwner()->SetActorRotation(FRotator(0.f, _angle, 0.f));
	_secondsRemaining = _secondsToClose;
}
