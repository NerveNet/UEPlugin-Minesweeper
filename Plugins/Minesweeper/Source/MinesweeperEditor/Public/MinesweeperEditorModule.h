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
private:
	TSharedPtr<FUICommandList> CommandList;
	
    /** Extender for toolbar button. */
    TSharedPtr<FExtender> ToolbarButtonExtender;
    TSharedPtr<const FExtensionBase> ToolbarButtonExtension;

    /** Extender for "Windows" menu. */
    TSharedPtr<FExtender> ToolMenuExtender;

    /** Extender for LevelEditor main file menu dropdown. */
    TSharedPtr<FExtender> MainMenuExtender;
    
    
    /** Holds a pointer to the actual game slate widget. */
    TSharedPtr<SMinesweeper> MinesweeperGame;

    /** Holds a pointer to the game slate widget's active standalone window parent. */
    TSharedPtr<SWindow> MinesweeperWindow;


public:
	static inline FName GetPluginName() { return TEXT("Minesweeper"); }
	static inline FName GetModuleName() { return TEXT("MinesweeperEditor"); }
	static inline FMinesweeperEditorModule& Get() { return FModuleManager::LoadModuleChecked<FMinesweeperEditorModule>(GetModuleName()); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded(GetModuleName()); }

    static inline const FPluginDescriptor& GetPluginDescriptor() { return IPluginManager::Get().FindPlugin(GetPluginName().ToString())->GetDescriptor(); }
    
    
    /** Gets the name of the Minesweeper editor docking tab. */
    static inline const FName GetMinesweeperDockTabName() { return FName("MinesweeperTab"); }
    

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	
    static FText GetMinesweeperLabel();
    static FText GetMinesweeperTooltip();


private:
    void BuildToolbarButton(FToolBarBuilder& Builder);
    void BuildWindowsMenu(FMenuBuilder& MenuBuilder);
	
	
    /** Creates a generic SWindow parent for the game slate widget. */
    TSharedRef<SWindow> CreateGenericWindowForGame();

    /** Creates a generic SDockTab parent for the game slate widget. */
    TSharedRef<SDockTab> CreateGenericDockTabForGame();


    /** Helper function to add a window to the editor slate application. */
    void AddWindowToSlateApplication(TSharedRef<SWindow> InWindow);

    bool IsMinesweeperDockTabOpen() const;
    void OpenMinesweeperDockTab();
    void CloseMinesweeperDockTab();

    bool IsMinesweeperWindowVisible();
    void OpenMinesweeperWindow();
    void CloseMinesweeperWindow(const bool bInForceImmediately = false);

public:
    /** Opens the Minesweeper game with the last used window mode. If the Minesweeper window is already open, toggles between standalone and docking tab window modes. */
    void ToggleMinesweeperWindowMode();

    /** Experimental and does not work yet. Recenters the minesweeper parent window on screen. */
    void RecenterMinesweeperWindow();


};
