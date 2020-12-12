// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SavePoint.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API USavePoint : public USaveGame
{
	GENERATED_BODY()

public :

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FTransform PlayerTransform;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SaveSlotName = "Test";

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

    USavePoint();
	
};
