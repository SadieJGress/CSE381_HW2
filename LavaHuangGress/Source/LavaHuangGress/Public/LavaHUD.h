#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LavaHUD.generated.h"

UCLASS()
class LAVAHUANGGRESS_API ALavaHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALavaHUD();
	virtual void DrawHUD() override;
	void TriggerDamageFlash() { FlashAlpha = 0.6f; }

protected:
	float FlashAlpha = 0.f;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI Icons")
	TObjectPtr<UTexture2D> CushionPicture;

	UPROPERTY(EditDefaultsOnly, Category = "UI Icons")
	TObjectPtr<UTexture2D> ExtinguisherPicture;
};
