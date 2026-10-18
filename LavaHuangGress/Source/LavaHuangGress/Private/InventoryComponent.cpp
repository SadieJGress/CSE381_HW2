// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Throwable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Max carry capacities
	FInventorySlot CushionSlot;
	CushionSlot.Capacity = 3;
	CushionSlot.Count = 0;
	Slots.Add(EItemSlot::Cushion, CushionSlot);

	FInventorySlot ExtinguisherSlot;
	ExtinguisherSlot.Capacity = 2;
	ExtinguisherSlot.Count = 0;
	Slots.Add(EItemSlot::Extinguisher, ExtinguisherSlot);
}



bool UInventoryComponent::AddItem(EItemSlot Slot)
{
	if (FInventorySlot* SlotData = Slots.Find(Slot))
	{
		if (SlotData->Count < SlotData->Capacity)
		{
			SlotData->Count++;
			if (Slot == EquippedSlot)
			{
				RefreshHeldActor();
			}
			return true;
		}
	}
	return false;
}

void UInventoryComponent::EquipSlot(EItemSlot Slot)
{
	if (Slots.Contains(Slot))
	{
		EquippedSlot = Slot;
		RefreshHeldActor();
	}
}

int32 UInventoryComponent::GetCount(EItemSlot Slot) const
{
	const FInventorySlot* SlotData = Slots.Find(Slot);
	return SlotData ? SlotData->Count : 0;
}

void UInventoryComponent::RefreshHeldActor()
{
	if (HeldActor)
	{
		HeldActor->Destroy();
		HeldActor = nullptr;
	}

	FInventorySlot* SlotData = Slots.Find(EquippedSlot);
	if (!SlotData || SlotData->Count <= 0 || !SlotData->ItemClass)
	{
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn held actor and attach to hand
	HeldActor = GetWorld()->SpawnActor<AThrowable>(SlotData->ItemClass, OwnerCharacter->GetActorTransform(), SpawnParams);
	if (HeldActor)
	{
		HeldActor->SetCarried(true);
		USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
		if (CharacterMesh)
		{
			HeldActor->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
		}
	}
}

void UInventoryComponent::ThrowEquipped()
{
	FInventorySlot* SlotData = Slots.Find(EquippedSlot);
	if (!SlotData || SlotData->Count <= 0 || !HeldActor)
	{
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	SlotData->Count--;

	// Determine throw based on camera
	FVector ThrowDirection = OwnerCharacter->GetControlRotation().Vector();
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PC && PC->PlayerCameraManager)
	{
		ThrowDirection = PC->PlayerCameraManager->GetCameraRotation().Vector();
	}

	// Hand item off
	AThrowable* ThrownItem = HeldActor;
	HeldActor = nullptr;

	ThrownItem->Throw(ThrowDirection);

	// Respawn next item
	RefreshHeldActor();
}
