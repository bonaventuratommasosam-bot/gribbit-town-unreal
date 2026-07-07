#include "GribbitConsumable.h"
#include "GribbitCharacter.h"
#include "GribbitNeedsComponent.h"

AGribbitConsumable::AGribbitConsumable()
{
	InteractionPrompt = NSLOCTEXT("Gribbit", "Consume", "Eat / Use");
	InteractionRange = 300.f;
}

void AGribbitConsumable::OnInteract_Implementation(AGribbitCharacter* Interactor)
{
	if (!Interactor) return;

	UGribbitNeedsComponent* Needs = Interactor->FindComponentByClass<UGribbitNeedsComponent>();
	if (Needs)
	{
		for (const TPair<FName, float>& Pair : NeedAmounts)
		{
			Needs->ModifyNeed(Pair.Key, Pair.Value);
		}
	}

	if (bConsumeOnUse)
	{
		Destroy();
	}
}
