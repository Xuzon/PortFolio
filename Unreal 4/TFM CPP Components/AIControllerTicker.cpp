// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerTicker.h"
#include "AIMetaController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAIControllerTicker::UAIControllerTicker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAIControllerTicker::BeginPlay()
{
	Super::BeginPlay();
	metaController = Cast<AAIMetaController>(UGameplayStatics::GetActorOfClass(this->GetOwner()->GetWorld(), AAIMetaController::StaticClass()));
	if (metaController != nullptr)
	{
		metaController->SubscribeTicker(this);
	}
}


// Called every frame
void UAIControllerTicker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

