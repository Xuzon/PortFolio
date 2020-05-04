///<summary>
/// This class will manage rotator of both arms, adding randomly angles to axis
/// and set it to the UCrazyArmsAnimator that it will override the rotation of the arm bones
///</summary>

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrazyArmsAnimator.h"
#include "IKCrazyArmsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLOFFAME_API UIKCrazyArmsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIKCrazyArmsComponent();

protected:
	///<summary>
	/// Get the animator class of this owner, it need to be an ACharacter
	///</summary>
	virtual void BeginPlay() override;
	//arm rotators
	FRotator rightArmRotator;
	FRotator leftArmRotator;
	//speed and angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrazyArms)
	float animSpeed = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CrazyArms)
	float maxAngle = 90;
	//x = 0, y = 1, z = 2
	int rightAxis = 0;
	int leftAxis = 0;
	//Blueprint animator ref
	UCrazyArmsAnimator* animator = nullptr;

	//store what direction I'm adding
	FVector rightArmDir = FVector(1,1,1);
	FVector leftArmDir = FVector(1,1,1);

	///<summary>
	///By the axis we are changing change its angle using euler
	///</summary>
	void AddToRotator(FRotator& rotator, int& axis, float angleToAdd, FVector& dir);
	///<summary>
	/// Add the "angleToAdd" to the angle, if reached max, flip, also change the direction and randomly change axis
	///</summary>
	void AddToAngle(float& angle, float angleToAdd, int& axis, float& dir);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	///<summary>
	/// Add the angle of this frame by speed and delta time to the rotators
	///</summary>
	UFUNCTION(BlueprintCallable, Category = "Methods")
	void DoAnim(float DeltaTime);
		
};
