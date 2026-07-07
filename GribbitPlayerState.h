#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GribbitPlayerState.generated.h"

/**
 * AGribbitPlayerState
 *
 * Per-player replicated state for multiplayer (Feature 5 foundation).
 * Holds the chosen character preset ID and the displayed name, so every
 * client can render the correct frog and label above the head.
 */
UCLASS()
class GRIBBITTOWN_API AGribbitPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGribbitPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Key into DT_Characters (e.g. "ChillPete", "SheriffBuck").
	UPROPERTY(ReplicatedUsing=OnRep_CharacterID, EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FName CharacterID = NAME_None;

	// Display name shown above the frog.
	UPROPERTY(ReplicatedUsing=OnRep_DisplayName, EditAnywhere, BlueprintReadWrite, Category="Gribbit")
	FString DisplayName;

	UFUNCTION(BlueprintCallable, Category="Gribbit")
	void SetCharacterPreset(FName InCharacterID, const FString& InDisplayName);

	UFUNCTION(BlueprintPure, Category="Gribbit")
	FName GetCharacterID() const { return CharacterID; }

	UFUNCTION(BlueprintPure, Category="Gribbit")
	FString GetDisplayName() const { return DisplayName; }

protected:
	UFUNCTION()
	void OnRep_CharacterID();

	UFUNCTION()
	void OnRep_DisplayName();

	// Blueprint hook when the preset changes (swap mesh/outfit).
	UPROPERTY(BlueprintAssignable, Category="Gribbit")
	FOnCharacterPresetChangedSignature OnCharacterPresetChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterPresetChangedSignature);
};
