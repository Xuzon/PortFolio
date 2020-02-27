// Fill out your copyright notice in the Description page of Project Settings.

#include "Gem.h"


// Sets default values
AGem::AGem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	placeable = CreateDefaultSubobject<UPlaceable>(TEXT("Placeable"));
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphere->InitSphereRadius(50.0f);
	sphere->SetCollisionProfileName(TEXT("Pawn"));
	UStaticMeshComponent* sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh component"));
	sphereMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		sphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	sphereMesh->SetStaticMesh(sphereVisualAsset.Object);

}

// Called when the game starts or when spawned
void AGem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

