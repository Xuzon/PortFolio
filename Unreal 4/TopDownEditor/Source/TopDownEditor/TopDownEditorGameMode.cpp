// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TopDownEditorGameMode.h"
#include "TopDownEditorPlayerController.h"
#include "TopDownEditorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATopDownEditorGameMode::ATopDownEditorGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownEditorPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}