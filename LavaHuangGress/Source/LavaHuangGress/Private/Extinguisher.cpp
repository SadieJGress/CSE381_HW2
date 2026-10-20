#include "Extinguisher.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "SprinklerTarget.h"

AExtinguisher::AExtinguisher()
{
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    Movement->ProjectileGravityScale = 1.f;
}

void AExtinguisher::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bSpent) return;
    if (!IsValid(OtherActor) || Cast<ACharacter>(OtherActor)) return;

    if (ASprinklerTarget* Target = Cast<ASprinklerTarget>(OtherActor))
    {
        bSpent = true;
        Target->OnHitByThrowable();
        OnHitTarget();
        Destroy();
        return;
    }

    //miss occurred
    bSpent = true;
    Movement->StopMovementImmediately();
    Movement->Deactivate();

    Mesh->SetLinearDamping(1.f);
    Mesh->SetAngularDamping(2.f);
    Mesh->SetSimulatePhysics(true);

    SetLifeSpan(MissLifeSeconds);
}