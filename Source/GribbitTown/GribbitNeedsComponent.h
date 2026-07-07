#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GribbitNeedsComponent.generated.h"

/**
 * UGribbitNeedsComponent
 *
 * Sims-like needs for a Gribbit (frog character).
 * Needs: Hunger, Energy, Fun, Social, Hygiene, Bladder. Range 0..100.
 * Decays over time (server-authoritative, replicated to clients).
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRIBBITTOWN_API UGribbitNeedsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGribbitNeedsComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- Needs (0..100). Higher = better satisfied. ---
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Hunger = 80.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Energy = 80.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Fun = 70.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Social = 60.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Hygiene = 75.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Needs")
	float Bladder = 85.f;

	// --- Decay rates per second (how fast each need falls). ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float HungerDecay = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float EnergyDecay = 0.20f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float FunDecay = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float SocialDecay = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float HygieneDecay = 0.18f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Needs")
	float BladderDecay = 0.30f;

	// --- API ---
	UFUNCTION(BlueprintCallable, Category="Needs")
	void ModifyNeed(FName NeedName, float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Needs")
	float GetNeed(FName NeedName) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Needs")
	float GetAverageNeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Needs")
	bool IsCritical(FName NeedName, float Threshold = 20.f) const;

	// Blueprint event fired when a need crosses below Threshold (once per drop).
	// Blueprint event fired when a need crosses below Threshold (once per drop).
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNeedCriticalSignature, FName, NeedName);

	UPROPERTY(BlueprintAssignable, Category="Needs")
	FOnNeedCriticalSignature OnNeedCritical;

private:
	void DecayNeed(float& Need, float Rate, float DeltaTime);
	TMap<FName, float*> NeedMap;
	void RebuildNeedMap();
};
