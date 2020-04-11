// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "PlayerMovementComponent.generated.h"


#define FIXED_TIME 0.02f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMovementComponent();

protected:
	//accumulated time to do my own fixed update
	float accumulatedTime = 0;
	//cm up that you give when jumping
	float gravityOffset = 10.f;
	//the needed capsule to get the height
	UCapsuleComponent* capsule;
	//standard ray parameters
	FCollisionQueryParams rayParams;
	//animator to change variables
	UAnimInstance* animator;
	//am I grounded?
	bool isGrounded = true;
	//current gravity velocity
	float currentGravity = 0;
	//current momentum (will use it when airbone)
	FVector momentum;
	FVector lastGroundMovement;
	//current momentum air speed
	float airSpeed;

	// Called when the game starts
	virtual void BeginPlay() override;

	///<summary>
	/// Calculate the desired movement (world relative) given the Input vector
	///</summary>
	bool CalculateDesiredMovement(const FVector inputVector, float &magnitude, FVector &moveDir, float deltaTime);

	///<summary>
	/// Wrapper to call move and rotate at same time
	///</summary>
	void MoveAndRotate(FVector moveDir, float magnitude, float deltaTime);
	
	///<summary>
	/// Move to the moveDir, checking for stairs to climb
	///</summary>
	void MoveDirection(const FVector myPos, FVector moveDir, float magnitude, float deltaTime);
	
	///<summary>
	/// Ground movement, check for slopes I can climb
	///</summary>
	bool GroundMovement(const FVector myPos, FVector& result, const FVector startRay, 
		const FVector endRay, FHitResult& hit, const float halfDistance);

	///<summary>
	/// In Air movement
	///</summary>
	bool AirBoneMovement(const FVector myPos, FVector& result, const FVector desiredPos,
		FHitResult& hit);
	
	///<summary>
	/// Rotate with interpolation until my forward reach the vector moveDir
	///</summary>
	void Rotate(FVector myPos, FVector moveDir);

	///<summary>
	/// Will add jump speed if grounded
	///</summary>
	UFUNCTION(BlueprintCallable, Category="Movement")
	void Jump();	
	
	///<summary>
	/// Gravity jump/fall
	///</summary>
	void Gravity(float deltaTime);	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//properties that will be assigned from blueprints keyboard/gamepad events
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float horizontalAxis = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float verticalAxis = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool traceAgainstComplex = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TEnumAsByte<ECollisionChannel> collisionChannel = ECollisionChannel::ECC_WorldStatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UCameraComponent* cam = nullptr;

	//Basic movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float speed = 800;		
	//min sqr threshold to  move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float threshold = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float rotationSpeed = 1;
	//slope and stairs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float stepStair = 40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float maxSlopeAngle = 45;

	//Jump properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float jumpForce = 500;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump", meta = (ClampMax = 1, ClampMin = 0 ))
	float airControl = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float gravityAcceleration = 981;
	
	///<summary>
	/// Get last realized movement
	///</summary>
	UFUNCTION(BlueprintCallable, Category = "Getters")
	FVector GetLastMovement() const;	


	///<summary>
	/// Get last realized movement
	///</summary>
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetAirVelocity();	
	
	///<summary>
	/// Get last realized movement
	///</summary>
	UFUNCTION(BlueprintCallable, Category = "Getters")
	bool GetIsGrounded() const;

	
};
