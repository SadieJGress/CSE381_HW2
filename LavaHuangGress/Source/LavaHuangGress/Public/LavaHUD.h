#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LavaHUD.generated.h"

UCLASS()
class LAVAHUANGGRESS_API ALavaHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;
	void TriggerDamageFlash() { FlashAlpha = 0.6f; }

protected:
	float FlashAlpha = 0.f;
};
