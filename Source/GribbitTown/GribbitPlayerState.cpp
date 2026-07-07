#include "GribbitPlayerState.h"
#include "Net/UnrealNetwork.h"

AGribbitPlayerState::AGribbitPlayerState()
{
	bReplicates = true;
}

void AGribbitPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGribbitPlayerState, CharacterID);
	DOREPLIFETIME(AGribbitPlayerState, DisplayName);
}

void AGribbitPlayerState::SetCharacterPreset(FName InCharacterID, const FString& InDisplayName)
{
	if (HasAuthority())
	{
		CharacterID = InCharacterID;
		DisplayName = InDisplayName;
	}
}

void AGribbitPlayerState::OnRep_CharacterID()
{
	OnCharacterPresetChanged.Broadcast();
}

void AGribbitPlayerState::OnRep_DisplayName()
{
	OnCharacterPresetChanged.Broadcast();
}
