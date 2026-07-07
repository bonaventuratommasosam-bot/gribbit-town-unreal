#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GribbitNeedsWidget.generated.h"

class UGribbitNeedsComponent;
class UProgressBar;

/**
 * UGribbitNeedsWidget
 *
 * C++ base for the needs HUD (Feature 2). Builds a small native HUD with
 * six progress bars, so the generated WBP_NeedsBars works without designer
 * wiring.
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

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// Resolves the local player's needs component. Returns true if bound.
	UFUNCTION(BlueprintCallable, Category="Needs")
	bool TryBindLocalNeeds();

private:
	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarHunger;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarEnergy;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarFun;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarSocial;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarHygiene;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> BarBladder;

	void BuildNativeLayout();
	void AddNeedRow(class UVerticalBox* Root, const FText& Label, TObjectPtr<UProgressBar>& OutBar);
	void UpdateNativeBars() const;
};
