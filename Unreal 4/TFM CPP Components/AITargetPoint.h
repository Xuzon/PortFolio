///<summary>
/// This class is a point that will spread around an actor,
/// it would also manage the movement of the pawn
///</summary>

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AITargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFFAME_API AAITargetPoint : public ATargetPoint
{
	GENERATED_BODY()

private:
	FVector CalculateWaypointPerpendicularPosition();
	APawn* controlledFan;
	bool canUpdate = false;

public:
	void Update();
	void SetControlledPawn(APawn* pawn);
	void SetUpdate(bool toSet);
	void DestroyWaypoint();
};
