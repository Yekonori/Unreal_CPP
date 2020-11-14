// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CPPGameMode.h"
#include "Unreal_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_CPPGameMode::AUnreal_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
