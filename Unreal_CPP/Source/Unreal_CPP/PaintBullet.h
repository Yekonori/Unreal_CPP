// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>

#include "PaintBullet.generated.h"

UCLASS()
class UNREAL_CPP_API APaintBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* BulletMovement;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMaterialInterface* DeferredDecal;

private:
	bool gardeFou = false;
};
