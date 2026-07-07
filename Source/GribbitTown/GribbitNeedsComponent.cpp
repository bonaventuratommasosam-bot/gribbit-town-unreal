#include "GribbitNeedsComponent.h"

UGribbitNeedsComponent::UGribbitNeedsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	RebuildNeedMap();
}

void UGribbitNeedsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGribbitNeedsComponent, Hunger);
	DOREPLIFETIME(UGribbitNeedsComponent, Energy);
	DOREPLIFETIME(UGribbitNeedsComponent, Fun);
	DOREPLIFETIME(UGribbitNeedsComponent, Social);
	DOREPLIFETIME(UGribbitNeedsComponent, Hygiene);
	DOREPLIFETIME(UGribbitNeedsComponent, Bladder);
}

void UGribbitNeedsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only the server advances needs; clients receive replicated values.
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		DecayNeed(Hunger,  HungerDecay,  DeltaTime);
		DecayNeed(Energy,  EnergyDecay,  DeltaTime);
		DecayNeed(Fun,     FunDecay,     DeltaTime);
		DecayNeed(Social,  SocialDecay,  DeltaTime);
		DecayNeed(Hygiene, HygieneDecay, DeltaTime);
		DecayNeed(Bladder, BladderDecay, DeltaTime);
	}
}

void UGribbitNeedsComponent::DecayNeed(float& Need, float Rate, float DeltaTime)
{
	const float Previous = Need;
	Need = FMath::Clamp(Need - Rate * DeltaTime, 0.f, 100.f);
	if (Previous >= 20.f && Need < 20.f)
	{
		OnNeedCritical.Broadcast(Previous <= Need ? NAME_None : GetFName());
	}
}

void UGribbitNeedsComponent::ModifyNeed(FName NeedName, float Amount)
{
	float* Value = NeedMap.Find(NeedName);
	if (!Value) return;
	*Value = FMath::Clamp(*Value + Amount, 0.f, 100.f);
}

float UGribbitNeedsComponent::GetNeed(FName NeedName) const
{
	const float* Value = NeedMap.Find(NeedName);
	return Value ? *Value : 0.f;
}

float UGribbitNeedsComponent::GetAverageNeed() const
{
	float Sum = Hunger + Energy + Fun + Social + Hygiene + Bladder;
	return Sum / 6.f;
}

bool UGribbitNeedsComponent::IsCritical(FName NeedName, float Threshold) const
{
	return GetNeed(NeedName) < Threshold;
}

void UGribbitNeedsComponent::RebuildNeedMap()
{
	NeedMap.Empty();
	NeedMap.Add("Hunger",  &Hunger);
	NeedMap.Add("Energy",  &Energy);
	NeedMap.Add("Fun",     &Fun);
	NeedMap.Add("Social",  &Social);
	NeedMap.Add("Hygiene", &Hygiene);
	NeedMap.Add("Bladder", &Bladder);
}
