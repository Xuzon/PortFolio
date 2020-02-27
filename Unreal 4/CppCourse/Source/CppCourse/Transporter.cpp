// Fill out your copyright notice in the Description page of Project Settings.


#include "Transporter.h"

// Sets default values for this component's properties
UTransporter::UTransporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UTransporter::BeginPlay()
{
	Super::BeginPlay();
	UInputComponent* input = GetOwner()->FindComponentByClass<UInputComponent>();
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	input->BindAction("pick", IE_Pressed, this, &UTransporter::Pick);
	input->BindAction("pick", IE_Released, this, &UTransporter::Drop);
	// ...
}


// Called every frame
void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector location;
	FRotator rotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(location, rotation);

	DrawDebugLine(
		GetWorld(),
		location,
		location + rotation.Vector() * 100,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		10.f
	);
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(location + rotation.Vector() * 100);
	}
	// ...
}

void UTransporter::Pick()
{
	FVector location;
	FRotator rotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(location, rotation);

	FCollisionQueryParams params
	(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		hit,
		location,
		location + rotation.Vector() * 100,
		ECollisionChannel::ECC_PhysicsBody,
		params
	);
	if (AActor* other = hit.GetActor())
	{
		physicsHandle->GrabComponentAtLocation
		(
			hit.GetComponent(),
			NAME_None,
			hit.GetComponent()->GetOwner()->GetActorLocation()
		);
	}
}

void UTransporter::Drop()
{
	physicsHandle->ReleaseComponent();
}

