#include "GribbitNeedsComponent.h"

UGribbitNeedsComponent::UGribbitNeedsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGribbitNeedsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Simple decay over time (prototype)
	Hunger = FMath::Clamp(Hunger - 0.5f * DeltaTime, 0.f, 100.f);
	Energy = FMath::Clamp(Energy - 0.3f * DeltaTime, 0.f, 100.f);
	Fun    = FMath::Clamp(Fun    - 0.4f * DeltaTime, 0.f, 100.f);
}

void UGribbitNeedsComponent::ModifyNeed(FName NeedName, float Amount)
{
	if (NeedName == "Hunger") Hunger = FMath::Clamp(Hunger + Amount, 0.f, 100.f);
	else if (NeedName == "Energy") Energy = FMath::Clamp(Energy + Amount, 0.f, 100.f);
	else if (NeedName == "Fun")    Fun    = FMath::Clamp(Fun    + Amount, 0.f, 100.f);
	// TODO: Expand for all needs
}
