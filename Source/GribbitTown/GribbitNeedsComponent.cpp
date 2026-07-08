#include "GribbitNeedsComponent.h"
#include "Engine/Engine.h"

UGribbitNeedsComponent::UGribbitNeedsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UGribbitNeedsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGribbitNeedsComponent, Hunger);
	DOREPLIFETIME(UGribbitNeedsComponent, Energy);
	DOREPLIFETIME(UGribbitNeedsComponent, Fun);
	DOREPLIFETIME(UGribbitNeedsComponent, Social);
}

void UGribbitNeedsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_Authority)
	{
		// Only server decays needs
		Hunger = FMath::Clamp(Hunger - 0.8f * DeltaTime, 0.f, 100.f);
		Energy = FMath::Clamp(Energy - 0.5f * DeltaTime, 0.f, 100.f);
		Fun    = FMath::Clamp(Fun    - 0.6f * DeltaTime, 0.f, 100.f);
		Social = FMath::Clamp(Social - 0.4f * DeltaTime, 0.f, 100.f);
	}

	// Show needs on screen (client + server for now)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Hunger: %.1f"), Hunger));
		GEngine->AddOnScreenDebugMessage(2, 0.0f, FColor::Cyan,   FString::Printf(TEXT("Energy: %.1f"), Energy));
		GEngine->AddOnScreenDebugMessage(3, 0.0f, FColor::Green,  FString::Printf(TEXT("Fun: %.1f"), Fun));
		GEngine->AddOnScreenDebugMessage(4, 0.0f, FColor::Magenta, FString::Printf(TEXT("Social: %.1f"), Social));
	}
}

void UGribbitNeedsComponent::ModifyNeed(FName NeedName, float Amount)
{
	if (GetOwnerRole() != ROLE_Authority) return;

	if (NeedName == "Hunger") Hunger = FMath::Clamp(Hunger + Amount, 0.f, 100.f);
	else if (NeedName == "Energy") Energy = FMath::Clamp(Energy + Amount, 0.f, 100.f);
	else if (NeedName == "Fun")    Fun    = FMath::Clamp(Fun    + Amount, 0.f, 100.f);
	else if (NeedName == "Social") Social = FMath::Clamp(Social + Amount, 0.f, 100.f);
}
