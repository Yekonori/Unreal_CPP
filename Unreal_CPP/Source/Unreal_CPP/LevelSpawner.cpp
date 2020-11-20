// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSpawner.h"

// Sets default values
ALevelSpawner::ALevelSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelSpawner::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ALevelSpawner::OverlapEnds);
}

// Called when the game starts or when spawned
void ALevelSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSpawner::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor == Cast<AActor>(MyCharacter) && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}
}

void ALevelSpawner::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor == Cast<AActor>(MyCharacter) && LevelToUnload != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, false);
	}
}
