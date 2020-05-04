
#include "IKCrazyArmsComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UIKCrazyArmsComponent::UIKCrazyArmsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

///<summary>
/// Get the animator class of this owner, it need to be an ACharacter
///</summary>
void UIKCrazyArmsComponent::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* character = Cast<ACharacter>(GetOwner());
	if (character == nullptr || character->GetMesh() == nullptr || character->GetMesh()->AnimClass == nullptr)
	{
		return;
	}
	animator = Cast<UCrazyArmsAnimator>(character->GetMesh()->GetAnimInstance());
}


// Called every frame
void UIKCrazyArmsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DoAnim(DeltaTime);
}

///<summary>
///By the axis we are changing change its angle using euler
///</summary>
void UIKCrazyArmsComponent::AddToRotator(FRotator& rotator, int& axis, float angleToAdd, FVector& dir)
{
	FVector euler = rotator.Euler();
	switch (axis)
	{
	default:
		AddToAngle(euler.X, angleToAdd, axis, dir.X);
		break;
	case 1:
		AddToAngle(euler.Y, angleToAdd, axis, dir.Y);
		break;
	case 2:
		AddToAngle(euler.Z, angleToAdd, axis, dir.Z);
		break;
	}
	rotator = rotator.MakeFromEuler(euler);
}

///<summary>
/// Add the "angleToAdd" to the angle, if reached max, flip, also change the direction and randomly change axis
///</summary>
void UIKCrazyArmsComponent::AddToAngle(float& angle, float angleToAdd, int& axis, float& dir)
{
	float sign = dir < 0 ? -1 : 1;
	angle += angleToAdd * sign;
	//if reached max angle flip direction and change axis to rotate
	if (abs(angle) > maxAngle)
	{
		dir *= -1;
		axis = FMath::RandRange(0, 2);
	}
}

///<summary>
/// Add the angle of this frame by speed and delta time to the rotators
///</summary>
void UIKCrazyArmsComponent::DoAnim(float DeltaTime)
{
	if (animator == nullptr)
	{
		return;
	}
	float angleToAdd = DeltaTime * animSpeed;
	AddToRotator(rightArmRotator, rightAxis, angleToAdd, rightArmDir);
	AddToRotator(leftArmRotator, leftAxis, angleToAdd, leftArmDir);
	animator->LeftArmRotator = leftArmRotator;
	animator->RightArmRotator = rightArmRotator;
}

