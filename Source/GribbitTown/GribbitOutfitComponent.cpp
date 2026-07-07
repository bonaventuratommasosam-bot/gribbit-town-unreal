#include "GribbitOutfitComponent.h"

UGribbitOutfitComponent::UGribbitOutfitComponent()
{
}

void UGribbitOutfitComponent::SetOutfit(FName OutfitID)
{
	CurrentOutfit = OutfitID;
	// TODO: Apply visual changes (Mesh, Materials, Accessories)
	UE_LOG(LogTemp, Log, TEXT("Gribbit changed outfit to: %s"), *OutfitID.ToString());
}
