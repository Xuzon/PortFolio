// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = _sphere;
	_sphere->InitSphereRadius(50.0f);
	_sphere->SetCollisionProfileName(TEXT("Pawn"));

	UStaticMeshComponent* sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh component"));
	sphereMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		sphereVisualAsset (TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	sphereMesh->SetStaticMesh(sphereVisualAsset.Object);

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FTransform spawnedTransform = GetTransform();
	spawnedTransform.AddToTranslation(FVector{ 100.0f, 0.f,0.f });
	AActor* newActor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), spawnedTransform);
	_spawned.Add(newActor);

	if ((_counterDelete -= DeltaTime) <= 0.0f)
	{
		_counterDelete = CD;
		for (auto& a : _spawned)
		{
			a->Destroy();
		}
		_spawned.Empty();
	}
}

