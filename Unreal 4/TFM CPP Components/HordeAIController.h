///<summary>
/// The AI controller of our horde pawns, we need to diseminate the pawns over the area
/// not follow only the player, as it does an ugly view where all the horde
/// is concentrated in a point following the player
///</summary>

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITargetPoint.h"
#include "AIControllerTicker.h"
#include "IControlable.h"
#include "HordeAIController.generated.h"

/**
 * 
 */
UCLASS()
class HELLOFFAME_API AHordeAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UAIControllerTicker* ticker = nullptr;
	APawn* pawn = nullptr;
	AAITargetPoint* waypoint = nullptr;
	bool followingPlayer = false;

	void KeepFollowingPlayer();
	void InitializeWaypoint();
	FVector CalculateWaypointPerpendicularPosition(float spread, float& initialDistance);
public:

	virtual void BeginPlay() override;

	UFUNCTION()
	void MyTick();

	AHordeAIController();
	~AHordeAIController();
};
