#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Gribbit Town game module.
 * Low-poly closed world for the GRiBBiTS (frog inhabitants).
 */
class FGribbitTownModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
