// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AThrowable::AThrowable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	// Enable collision notifs
	Mesh->SetNotifyRigidBodyCollision(true);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Mesh;
	Movement->InitialSpeed = 0.f;
	Movement->MaxSpeed = ThrowSpeed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 1.f;
	Movement->Deactivate();
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(this, &AThrowable::HandleHit);
	}
	
}

void AThrowable::SetCarried(bool bCarried)
{
	if (bCarried)
	{
		Movement->Deactivate();
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Movement->Activate();
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AThrowable::Throw(const FVector& Direction)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetCarried(false);
	Movement->MaxSpeed = ThrowSpeed;
	Movement->Velocity = Direction.GetSafeNormal() * ThrowSpeed;

}

void AThrowable::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Override by children
}

