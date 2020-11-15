// Copyright Epic Games, Inc. All Rights Reserved.

#include "Unreal_CPPGameMode.h"
#include "Unreal_CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Engine/Engine.h"

AUnreal_CPPGameMode::AUnreal_CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AUnreal_CPPGameMode::CreateNewThirdCharacter(AController* controller, FVector spawnVectorLocation, FRotator spawnRotator)
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, controller, spawnVectorLocation, spawnRotator]()
		{
			APawn* Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, spawnVectorLocation, spawnRotator);
			if (Pawn) {
				controller->Possess(Pawn);
			}
		}, 2, false);
}
