// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintBullet.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
APaintBullet::APaintBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
    static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    UStaticMesh* Mesh = MeshAsset.Object;
    BulletMesh->SetStaticMesh(Mesh);

    BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
    BulletMovement->InitialSpeed = 3000.0f;
    BulletMovement->MaxSpeed = 3000.0f;

    BulletMesh->SetSimulatePhysics(true);
    BulletMesh->SetNotifyRigidBodyCollision(true);
    BulletMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
    BulletMesh->OnComponentHit.AddDynamic(this, &APaintBullet::OnHit);
    RootComponent = BulletMesh;

    SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalComp(TEXT("Material'/Game/Materials/PaintShot.PaintShot'"));
    DeferredDecal = DecalComp.Object;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void APaintBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APaintBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaintBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr && !gardeFou)
    {
        gardeFou = true;
        FActorSpawnParameters SpawnParams;
        ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.Location, Hit.GetActor()->GetActorTransform().GetRotation().Rotator(), SpawnParams);
        UE_LOG(LogTemp, Warning, TEXT("Projectile Collided"));
        // Set DecalMaterial, LifeSpan and DecalSize
        if (decal)
        {
            int rndFrame = FMath::RandRange(0, 3);
            FLinearColor color = FLinearColor::MakeRandomColor();
            UMaterialInstanceDynamic* rndMaterial = UMaterialInstanceDynamic::Create(DeferredDecal, this);

            rndMaterial->SetScalarParameterValue("Frame", rndFrame);
            rndMaterial->SetVectorParameterValue("Color", color);

            decal->SetDecalMaterial(rndMaterial);

            decal->SetLifeSpan(10.0f);
            decal->GetDecal()->DecalSize = FVector(64.0f, 64.0f, 64.0f);
            decal->SetActorRotation(Hit.Normal.Rotation());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
        }
    }

    this->Destroy();
    //SetLifeSpan(0.0f);
}