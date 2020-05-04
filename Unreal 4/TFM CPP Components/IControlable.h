///<summary>
/// Interface that we need in the HordeAICOntroller's pawns
/// We need a bool that give us if it is following a player and the character movement of the pawn
///</summary
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/Interface.h"
#include "IControlable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIControlable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLOFFAME_API IIControlable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	bool IsFollowingPlayer() const;
	UFUNCTION(BlueprintNativeEvent)
	UCharacterMovementComponent* GetCharacterMovement() const;
	UFUNCTION(BlueprintNativeEvent)
	void InterfaceTest() const;
};
