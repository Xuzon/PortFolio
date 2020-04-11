// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


///<summary>
/// Get the capsule component of the player
///</summary>
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	capsule = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	rayParams = FCollisionQueryParams(FName(TEXT("InteractTrace")), true, NULL);
	rayParams.bTraceComplex = traceAgainstComplex;
	rayParams.bReturnPhysicalMaterial = false;
	rayParams.AddIgnoredActor(GetOwner());
}


///<summary>
/// Calculate desired movement from the input received in the frame, then call move and rotate if needed
///</summary>
void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//sanity check, we need the camera for the movement
	if (cam == nullptr)
	{
		return;
	}
	accumulatedTime += DeltaTime;
	while (accumulatedTime >= FIXED_TIME)
	{
		accumulatedTime -= FIXED_TIME;
		Gravity(FIXED_TIME);
		FVector inputVector(verticalAxis, horizontalAxis, 0);
		//don't move or rotate if didn't reach threshold and grounded, or on air and withouth momentum
		if (inputVector.SizeSquared() < threshold && (isGrounded || momentum.SizeSquared() < threshold))
		{
			lastGroundMovement = FVector::ZeroVector;
			continue;
		}
		//Calculate the desired movement filling with the input vector and desired speed
		float magnitude = speed * FIXED_TIME;
		FVector moveDir;
		if (!CalculateDesiredMovement(inputVector, magnitude, moveDir, FIXED_TIME))
		{
			continue;
		}
		MoveAndRotate(moveDir, magnitude, FIXED_TIME);
	}
	//reset frame input
	verticalAxis = 0;
	horizontalAxis = 0;
}


///<summary>
/// Calculate the desired movement (world relative) given the Input vector
///</summary>
bool UPlayerMovementComponent::CalculateDesiredMovement(const FVector inputVector, float& magnitude, FVector& moveDir, float deltaTime)
{
	moveDir = FVector::ZeroVector;
	//need the camera vectors projected with player up
	moveDir += UKismetMathLibrary::ProjectVectorOnToPlane(cam->GetForwardVector(), FVector::UpVector) * inputVector.X;
	moveDir += UKismetMathLibrary::ProjectVectorOnToPlane(cam->GetRightVector(), FVector::UpVector) * inputVector.Y;
	moveDir.Normalize();

	//if not grounded the momentum will have some talk with me
	if (!isGrounded)
	{
		//calculate if the input tries to go hold the momentum
		//(the user is inputting the reverse of the momentum)
		float inputSize = inputVector.Size();
		inputSize *= UKismetMathLibrary::SignOfFloat(FVector::DotProduct(moveDir, momentum));
		//hold the momentum lerping with the user input
		if (inputSize < 0)
		{
			float t = FMath::Clamp(abs(inputSize), 0.f, 1.f);
			airSpeed = FMath::Lerp(airSpeed, 0.0f, t);
			UE_LOG(LogTemp, Warning, TEXT("Countering momentum %f :: %f"), inputSize, airSpeed);
		}
		//apply the momentum
		moveDir = FMath::Lerp(moveDir, momentum, airSpeed);
		//moveDir = moveDir * airControl + momentum * (1 - airControl) * airSpeed;
		//change the movement magnitude if still affected by momentum
		if (airSpeed > 0)
		{
			magnitude *= airSpeed;
		}
		//maybe we countered the momentum to a threshold we don't move
		if (moveDir.SizeSquared() < threshold)
		{
			return false;
		}
		moveDir.Normalize();
	}
	return true;

}
///<summary>
/// Wrapper to call move and rotate at same time
///</summary>
void UPlayerMovementComponent::MoveAndRotate(FVector moveDir, float magnitude, float deltaTime)
{
	FVector myPos = GetOwner()->GetActorLocation();
	MoveDirection(myPos, moveDir, magnitude, deltaTime);
	Rotate(myPos, moveDir);
}

///<summary>
/// Move to the moveDir, checking for stairs to climb
///</summary>
void UPlayerMovementComponent::MoveDirection(const FVector myPos, FVector moveDir, float magnitude, float deltaTime)
{
	//get the height from the center of the player to the floor
	float half = capsule->GetScaledCapsuleHalfHeight();
	//desired position
	FVector endPos = myPos + moveDir * magnitude;

	//initialize the hit result
	FHitResult hit = FHitResult(ForceInit);

	//Check for walls, with the magnitude and do not overlap my capsule
	FVector wallCheck = myPos + moveDir * (magnitude + capsule->GetScaledCapsuleRadius());
	FVector capsuleOffSet = FVector::UpVector * capsule->GetScaledCapsuleHalfHeight() / 2.f;
	//Check for walls using Raycasting, if used capsule casting had some problems entering colliders
	if (GetOwner()->GetWorld()->LineTraceSingleByChannel
	(
		hit,
		myPos,
		wallCheck,
		collisionChannel,
		rayParams
	))
	{
		if (hit.Actor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hitting with %s"), *hit.Actor->GetName());
		}

		//if hit a wall that's it is in front o me don't move 
		//so, I have to check I'm not stuck, so, if the dot between normal and move dir
		//is positive it means it is behind me, don't mind that
		if (FVector::DotProduct(hit.Normal, moveDir) <= 0)
		{
			return;
		}
		//if hit a wall  adjust the endpos to don't overlap the wall (using capsule radius)
		/*FVector incidence = (hit.ImpactPoint - myPos);
		incidence.Normalize();
		endPos = hit.ImpactPoint - incidence * capsule->GetScaledCapsuleRadius();*/
	}

	FVector floorMovePos = endPos - FVector::UpVector * (half + stepStair);
	FVector result;
	if (isGrounded)
	{
		if (GroundMovement(myPos, result, endPos, floorMovePos, hit, half))
		{
			lastGroundMovement = result - myPos;
			lastGroundMovement.Z = 0;
			GetOwner()->SetActorLocation(result);
		}
	}
	else
	{
		if (AirBoneMovement(myPos, result, endPos, hit))
		{
			GetOwner()->SetActorLocation(result);
		}
	}
}

///<summary>
/// Ground movement, check for slopes I can climb
///</summary>
bool UPlayerMovementComponent::GroundMovement(const FVector myPos, FVector& result, const FVector startRay,
	const FVector endRay, FHitResult& hit, const float halfDistance)
{

	//raycasting to try to walk glued to the ground	
	bool haveHit = GetOwner()->GetWorld()->LineTraceSingleByChannel
	(
		hit,
		startRay,
		endRay,
		collisionChannel,
		rayParams
	);
	//If my racyast hit something check I can climb that
	if (haveHit)
	{
		//dis shit is in radians
		float angle = FMath::Acos(FVector::DotProduct(FVector::UpVector, hit.ImpactNormal));
		angle = UKismetMathLibrary::RadiansToDegrees(angle);
		//the new desired position, impact point + the capsule offset
		FVector buff = hit.ImpactPoint + FVector::UpVector * halfDistance;
		if (angle > maxSlopeAngle&& buff.Z > myPos.Z)
		{
			//don't move, I can't climb that shit
			UE_LOG(LogTemp, Warning, TEXT("Can't climb"));
			return false;
		}
		else
		{
			//check don't stepping a lot 
			//(for example, jumping and the center of the mass of the player is above the ground
			//but we don't want to move that much)
			if (abs(buff.Z - myPos.Z) < stepStair)
			{
				//Walk glued to the ground but with desired magnitude
				FVector dir = buff - myPos;
				dir.Normalize();
				float magnitude = (startRay - myPos).Size();
				result = myPos + dir * magnitude;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		//moving freely (Im grounded but trying to walk to an area with no ground)
		result = startRay;
		return true;
	}
}

///<summary>
/// Air movement, add momentum
///</summary>
bool UPlayerMovementComponent::AirBoneMovement(const FVector myPos, FVector& result, const FVector desiredPos,
	FHitResult& hit)
{
	result = desiredPos;
	return true;
}

///<summary>
/// Rotate with interpolation until my forward reach the vector moveDir
///</summary>
void UPlayerMovementComponent::Rotate(FVector myPos, FVector moveDir)
{
	//don't rotate while driving through the momentum
	if (!isGrounded && airSpeed > 0)
	{
		return;
	}
	FVector endPos = myPos + moveDir;
	FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(myPos, endPos);
	newRotator = FMath::Lerp(GetOwner()->GetActorRotation(), newRotator, rotationSpeed);
	GetOwner()->SetActorRotation(newRotator, ETeleportType::TeleportPhysics);
}

///<summary>
/// Will add jump speed if grounded
///</summary>
void UPlayerMovementComponent::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Try to jump:%i"), isGrounded);
	if (isGrounded)
	{
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FVector::UpVector * gravityOffset);
		isGrounded = false;
		currentGravity = jumpForce;
		if (lastGroundMovement.SizeSquared() > threshold)
		{
			momentum = lastGroundMovement;
			momentum.Normalize();
			airSpeed = 1 - airControl;
		}
		else
		{
			momentum = FVector::ZeroVector;
			airSpeed = 0;
		}
	}
}

///<summary>
/// Gravity jump/fall
///</summary>
void UPlayerMovementComponent::Gravity(float deltaTime)
{
	//calculate vector parameters
	FVector myPos = GetOwner()->GetActorLocation();
	FVector endPos = myPos;
	FVector offsetVector = FVector::UpVector * (capsule->GetScaledCapsuleHalfHeight() + gravityOffset - 1);

	//gravity acceleration
	endPos.Z += currentGravity * deltaTime;
	currentGravity -= gravityAcceleration * deltaTime;

	//Get physics parameters
	FHitResult hit = FHitResult(ForceInit);
	UWorld* world = GetOwner()->GetWorld();

	//check if jumping don't pass through the ceilings, and stop current gravity jump
	if (currentGravity > 0 && world->LineTraceSingleByChannel(
		hit,
		myPos,
		endPos + offsetVector,
		collisionChannel,
		rayParams
	))
	{
		endPos = myPos;
		currentGravity = -0;
	}

	//check I have a ground below me
	bool toGround = world->LineTraceSingleByChannel
	(
		hit,
		myPos,
		(endPos - offsetVector),
		collisionChannel,
		rayParams
	);
	//if changed ground debug
	if (toGround != isGrounded)
	{
		isGrounded = toGround;
		UE_LOG(LogTemp, Warning, TEXT("Changed grounded:%i"), isGrounded);
	}
	//if grounded reset the gravity acceleration, momentum and stick to the ground
	//if not, gravity, do your work
	if (isGrounded)
	{
		currentGravity = 0;
		airSpeed = 0;
		momentum = FVector::ZeroVector;
		lastGroundMovement = FVector::ZeroVector;
		FVector pos = hit.ImpactPoint + FVector::UpVector * capsule->GetScaledCapsuleHalfHeight();
		GetOwner()->SetActorLocation(pos, false);
	}
	else
	{
		GetOwner()->SetActorLocation(endPos, false);
	}
}


FVector UPlayerMovementComponent::GetLastMovement() const
{
	return lastGroundMovement;
}

bool UPlayerMovementComponent::GetIsGrounded() const
{
	return isGrounded;
}

void UPlayerMovementComponent::ResetAirVelocity()
{
	this->currentGravity = 0;
}