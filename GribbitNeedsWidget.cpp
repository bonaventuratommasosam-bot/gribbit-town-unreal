#include "GribbitNeedsWidget.h"
#include "GribbitNeedsComponent.h"
#include "GribbitCharacter.h"
#include "GribbitPlayerController.h"

bool UGribbitNeedsWidget::TryBindLocalNeeds()
{
	AGribbitPlayerController* PC = GetOwningPlayer<AGribbitPlayerController>();
	if (!PC) return false;

	AGribbitCharacter* Char = Cast<AGribbitCharacter>(PC->GetPawn());
	if (!Char) return false;

	BoundNeeds = Char->FindComponentByClass<UGribbitNeedsComponent>();
	return BoundNeeds != nullptr;
}

void UGribbitNeedsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!BoundNeeds && !TryBindLocalNeeds())
	{
		return;
	}

	OnNeedsUpdated(
		BoundNeeds->Hunger,
		BoundNeeds->Energy,
		BoundNeeds->Fun,
		BoundNeeds->Social,
		BoundNeeds->Hygiene,
		BoundNeeds->Bladder);
}
