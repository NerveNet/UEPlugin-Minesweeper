// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"


DECLARE_LOG_CATEGORY_EXTERN(LogMinesweeperRuntime, All, All);




class MINESWEEPERRUNTIME_API FMinesweeperRuntimeModule : public IModuleInterface
{
public:
	static inline FName GetPluginName() { return TEXT("Minesweeper"); }
	static inline FName GetModuleName() { return TEXT("MinesweeperRuntime"); }
	static inline FMinesweeperRuntimeModule& Get() { return FModuleManager::LoadModuleChecked<FMinesweeperRuntimeModule>(GetModuleName()); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded(GetModuleName()); }

    static inline const FPluginDescriptor& GetPluginDescriptor() { return IPluginManager::Get().FindPlugin(GetPluginName().ToString())->GetDescriptor(); }
    
    
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
