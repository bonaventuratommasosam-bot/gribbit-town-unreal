#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GribbitNeedsWidget.generated.h"

class UGribbitNeedsComponent;

/**
 * UGribbitNeedsWidget
 *
 * C++ base for the needs HUD (Feature 2). The Blueprint child
 * (WBP_NeedsBars) adds the actual progress bars; this base binds to the
 * local player's needs component and ticks the values every frame.
 *
 * Override BindNeeds() in Blueprint to wire your UProgressBar widgets.
 */
UCLASS(Abstract, Blueprintable)
class GRIBBITTOWN_API UGribbitNeedsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// The needs component this widget is displaying (set in native tick).
	UPROPERTY(BlueprintReadOnly, Category="Needs")
	TObjectPtr<UGribbitNeedsComponent> BoundNeeds;

	// Called each frame with the latest need values. Override in Blueprint.
	UFUNCTION(BlueprintImplementableEvent, Category="Needs")
	void OnNeedsUpdated(float Hunger, float Energy, float Fun, float Social, float Hygiene, float Bladder);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// Resolves the local player's needs component. Returns true if bound.
	UFUNCTION(BlueprintCallable, Category="Needs")
	bool TryBindLocalNeeds();
};
