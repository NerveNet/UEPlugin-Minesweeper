// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"

class FUICommandList;
class SMinesweeper;
class SMinesweeperWindow;


DECLARE_LOG_CATEGORY_EXTERN(LogMinesweeperEditor, All, All);




class MINESWEEPEREDITOR_API FMinesweeperEditorModule : public IModuleInterface
{
    friend class UMinesweeperSettings;

private:
	TSharedPtr<FUICommandList> CommandList;
	
    /** Extender for toolbar button. */
    TSharedPtr<FExtender> ToolbarButtonExtender;
    TSharedPtr<const FExtensionBase> ToolbarButtonExtension;

    /** Extender for "Windows" menu. */
    TSharedPtr<FExtender> ToolMenuExtender;

    /** Extender for LevelEditor main file menu dropdown. */
    TSharedPtr<FExtender> MainMenuExtender;
    
    
    TSharedPtr<SMinesweeper> MinesweeperGame;
    TSharedPtr<SWindow> MinesweeperWindow;


public:
	static inline FName GetPluginName() { return TEXT("Minesweeper"); }
	static inline FName GetModuleName() { return TEXT("MinesweeperEditor"); }
	static inline FMinesweeperEditorModule& Get() { return FModuleManager::LoadModuleChecked<FMinesweeperEditorModule>(GetModuleName()); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded(GetModuleName()); }

    static inline const FPluginDescriptor& GetPluginDescriptor() { return IPluginManager::Get().FindPlugin(GetPluginName().ToString())->GetDescriptor(); }
    
    
    static inline const FName GetMinesweeperDockTabName() { return FName("MinesweeperTab"); }
    

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	
    static FText GetMinesweeperLabel();
    static FText GetMinesweeperTooltip();


private:
    void BuildToolbarButton(FToolBarBuilder& Builder);
    void BuildWindowsMenu(FMenuBuilder& MenuBuilder);
	
	
    TSharedRef<SWindow> CreateGenericWindowForGame();
    TSharedRef<SDockTab> CreateGenericDockTabForGame();


    /** Helper function to create window. */
    void AddWindowToSlateApplication(TSharedRef<SWindow> InWindow);

    bool IsMinesweeperDockTabOpen() const;
    void OpenMinesweeperDockTab();
    void CloseMinesweeperDockTab();

    bool IsMinesweeperWindowVisible();
    void OpenMinesweeperWindow();
    void CloseMinesweeperWindow(const bool bInForceImmediately = false);

public:
    void ToggleMinesweeperWindowMode();

    void RecenterMinesweeperWindow();


};
