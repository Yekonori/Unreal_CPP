// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Unreal_CPPGameMode.generated.h"

UCLASS(minimalapi)
class AUnreal_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnreal_CPPGameMode();

	void CreateNewThirdCharacter(AController* controller, FVector spawnVectorLocation, FRotator spawnRotator);
};



