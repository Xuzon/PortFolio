// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BehaviourTreesGameMode.h"
#include "BehaviourTreesPlayerController.h"
#include "BehaviourTreesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABehaviourTreesGameMode::ABehaviourTreesGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABehaviourTreesPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}