// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperEditorModule.h"
#include "MinesweeperCommands.h"
#include "MinesweeperStyle.h"
#include "MinesweeperSettings.h"
#include "Slate/SMinesweeperWindow.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SScaleBox.h"


#define LOCTEXT_NAMESPACE "FMinesweeperEditorModule"

DEFINE_LOG_CATEGORY(LogMinesweeperEditor);




void FMinesweeperEditorModule::StartupModule()
{
	FMinesweeperStyle::Initialize();
	
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	
	
    // create and map UI commands
    FMinesweeperCommands::Register();
    CommandList = MakeShareable(new FUICommandList);
    
	const FMinesweeperCommands& MinesweeperCommands = FMinesweeperCommands::Get();
	CommandList->MapAction(MinesweeperCommands.ToggleMinesweeperWindowMode, FExecuteAction::CreateRaw(this, &FMinesweeperEditorModule::ToggleMinesweeperWindowMode));
	
	
	// add toolbar extension
	if (UMinesweeperSettings::GetConst()->ShowToolbarButton)
	{
		ToolbarButtonExtender = MakeShareable(new FExtender);
		ToolbarButtonExtension = ToolbarButtonExtender->AddToolBarExtension("Content", EExtensionHook::After, CommandList, FToolBarExtensionDelegate::CreateRaw(this, &FMinesweeperEditorModule::BuildToolbarButton));
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarButtonExtender);
	}
	

    // add menu item to display "Minesweeper" under the "Windows" menu
    ToolMenuExtender = MakeShareable(new FExtender());
    ToolMenuExtender->AddMenuExtension("LevelEditor", EExtensionHook::After, CommandList, FMenuExtensionDelegate::CreateRaw(this, &FMinesweeperEditorModule::BuildWindowsMenu));
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(ToolMenuExtender);
	
	
    // register tab spawner
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		GetMinesweeperDockTabName(),
        FOnSpawnTab::CreateLambda([&](const FSpawnTabArgs&) -> TSharedRef<SDockTab> { return CreateGenericDockTabForGame(); }))
        .SetMenuType(ETabSpawnerMenuType::Hidden)
        .SetDisplayName(GetMinesweeperLabel())
        .SetIcon(FMinesweeperStyle::GetIcon("Mine"));
}

void FMinesweeperEditorModule::ShutdownModule()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));


	// unregister tab spawners
	FGlobalTabmanager::Get()->UnregisterTabSpawner(GetMinesweeperDockTabName());
	
	
	// remove menu extension
	LevelEditorModule.GetMenuExtensibilityManager()->RemoveExtender(ToolMenuExtender);
		
		
	// remove toolbar extension
	if (ToolbarButtonExtender.IsValid())
	{
		ToolbarButtonExtender->RemoveExtension(ToolbarButtonExtension.ToSharedRef());
		LevelEditorModule.GetToolBarExtensibilityManager()->RemoveExtender(ToolbarButtonExtender);

		ToolbarButtonExtender.Reset();
		ToolbarButtonExtension.Reset();
	}
	
	
    // unregister UI commands
	const FMinesweeperCommands& MinesweeperCommands = FMinesweeperCommands::Get();
	if (CommandList->IsActionMapped(MinesweeperCommands.ToggleMinesweeperWindowMode)) CommandList->UnmapAction(MinesweeperCommands.ToggleMinesweeperWindowMode);

    CommandList.Reset();
    FMinesweeperCommands::Unregister();
    	
    	
	FMinesweeperStyle::Shutdown();
}


FText FMinesweeperEditorModule::GetMinesweeperLabel()
{
	return LOCTEXT("MinesweeperLabel", "Minesweeper");
}

FText FMinesweeperEditorModule::GetMinesweeperTooltip()
{
	return LOCTEXT("MinesweeperTooltip", "Why work when you can play Minesweeper???");
}


void FMinesweeperEditorModule::BuildToolbarButton(FToolBarBuilder& Builder)
{
	const FMinesweeperCommands& MinesweeperCommands = FMinesweeperCommands::Get();

	Builder.AddToolBarButton(
		MinesweeperCommands.ToggleMinesweeperWindowMode,
		NAME_None,
		GetMinesweeperLabel(),
		GetMinesweeperLabel(),
		FMinesweeperStyle::GetIcon("ToolbarButton")
	);
}

void FMinesweeperEditorModule::BuildWindowsMenu(FMenuBuilder& MenuBuilder)
{
	const FMinesweeperCommands& MinesweeperCommands = FMinesweeperCommands::Get();

	MenuBuilder.BeginSection(/*ExtensionHook=*/"EditorGames", LOCTEXT("EditorGamesHeadingLabel", "Editor Games"));
	{
		MenuBuilder.AddMenuEntry(
			MinesweeperCommands.ToggleMinesweeperWindowMode,
			NAME_None,
			GetMinesweeperLabel(),
			GetMinesweeperTooltip(),
			FMinesweeperStyle::GetIcon("ToolbarButton.Small")
		);
	}
	MenuBuilder.EndSection();
}


#pragma region Window / DockTab Management

TSharedRef<SWindow> FMinesweeperEditorModule::CreateGenericWindowForGame()
{
	if (!MinesweeperGame.IsValid())
	{
		MinesweeperGame = SNew(SMinesweeperWindow);
	}
	return SNew(SWindow)
		.Title(FMinesweeperEditorModule::GetMinesweeperLabel())
		.CreateTitleBar(true)
		.SaneWindowPlacement(true)
		.bDragAnywhere(false)
		.FocusWhenFirstShown(true)
		.HasCloseButton(true)
		.SupportsMaximize(false)
		.SupportsMinimize(true)
		.IsInitiallyMaximized(false)
		.IsInitiallyMinimized(false)
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.SizingRule(ESizingRule::Autosized)
		.ShouldPreserveAspectRatio(false)
		[
			MinesweeperGame.ToSharedRef()
		];
}

TSharedRef<SDockTab> FMinesweeperEditorModule::CreateGenericDockTabForGame()
{
	if (!MinesweeperGame.IsValid())
	{
		MinesweeperGame = SNew(SMinesweeperWindow);
	}
	return
		SNew(SDockTab)
		.Label(GetMinesweeperLabel())
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SScaleBox)
			.Stretch(EStretch::ScaleToFit)
			.StretchDirection(EStretchDirection::DownOnly)
			.HAlign(HAlign_Left).VAlign(VAlign_Top)
			[
				MinesweeperGame.ToSharedRef()
			]
		];
}


void FMinesweeperEditorModule::AddWindowToSlateApplication(TSharedRef<SWindow> InWindow)
{
	TSharedPtr<SWindow> rootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (rootWindow.IsValid())
		FSlateApplication::Get().AddWindowAsNativeChild(InWindow, rootWindow.ToSharedRef());
	else
		FSlateApplication::Get().AddWindow(InWindow);
}


bool FMinesweeperEditorModule::IsMinesweeperWindowVisible()
{
	if (!StandaloneParentWindow.IsValid()) return false;

	TArray<TSharedRef<SWindow>> windows;
	FSlateApplication::Get().GetAllVisibleWindowsOrdered(windows);

	for (const TSharedRef<SWindow> window : windows)
	{
		if (window == StandaloneParentWindow.ToSharedRef())
		{
			return true;
		}
	}

	return false;
}

void FMinesweeperEditorModule::OpenMinesweeperWindow()
{
	if (IsMinesweeperDockTabOpen()) return;

	if (IsMinesweeperWindowVisible())
	{
		StandaloneParentWindow->BringToFront();
	}
	else
	{
		StandaloneParentWindow = CreateGenericWindowForGame();
		AddWindowToSlateApplication(StandaloneParentWindow.ToSharedRef());
		//RecenterMinesweeperWindow();
	}
}

void FMinesweeperEditorModule::CloseMinesweeperWindow(const bool bInForceImmediately)
{
	if (!StandaloneParentWindow.IsValid()) return;

	if (bInForceImmediately)
		StandaloneParentWindow->DestroyWindowImmediately();
	else
		StandaloneParentWindow->RequestDestroyWindow();

	MinesweeperGame.Reset();
	StandaloneParentWindow.Reset();
}


bool FMinesweeperEditorModule::IsMinesweeperDockTabOpen() const
{
	return FGlobalTabmanager::Get()->FindExistingLiveTab(FTabId(GetMinesweeperDockTabName())).IsValid();
}

void FMinesweeperEditorModule::OpenMinesweeperDockTab()
{
	if (IsMinesweeperWindowVisible()) return;
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId(GetMinesweeperDockTabName()));
}

void FMinesweeperEditorModule::CloseMinesweeperDockTab()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FTabId(GetMinesweeperDockTabName()))->RequestCloseTab();
}


void FMinesweeperEditorModule::ToggleMinesweeperWindowMode()
{
	UMinesweeperSettings* settings = UMinesweeperSettings::Get();

	if (IsMinesweeperWindowVisible())
	{
		// create new dock tab and close standalone window
		TSharedPtr<SDockTab> dockTab = FGlobalTabmanager::Get()->TryInvokeTab(FTabId(GetMinesweeperDockTabName()));
		if (dockTab.IsValid())
		{
			MinesweeperGame->AssignParentWidget(dockTab);
			CloseMinesweeperWindow();
		}

		// save last window mode
		settings->UseDockTab = true;
	}
	else if (IsMinesweeperDockTabOpen())
	{
		// create new standalone window and close dock tab
		StandaloneParentWindow = CreateGenericWindowForGame();
		AddWindowToSlateApplication(StandaloneParentWindow.ToSharedRef());
		//RecenterMinesweeperWindow();

		if (StandaloneParentWindow.IsValid())
		{
			MinesweeperGame->AssignParentWidget(StandaloneParentWindow);
			CloseMinesweeperDockTab();
		}

		// save last window mode
		settings->UseDockTab = false;
	}

	// if any previous game may have been open, make sure we close it
	if (MinesweeperGame.IsValid())
	{
		MinesweeperGame.Reset();
	}

	// open if neither is already open
	if (settings->UseDockTab)
		OpenMinesweeperDockTab();
	else
		OpenMinesweeperWindow();
}

void FMinesweeperEditorModule::RecenterMinesweeperWindow()
{
	if (IsMinesweeperWindowVisible())
	{
		const FVector2D viewportSize = GEditor->GetActiveViewport()->GetSizeXY();
		StandaloneParentWindow->MoveWindowTo((viewportSize - StandaloneParentWindow->GetTickSpaceGeometry().GetLocalSize()) / 2.0f);
	}
	else if (IsMinesweeperDockTabOpen())
	{
		const FVector2D viewportSize = GEditor->GetActiveViewport()->GetSizeXY();
		TSharedPtr<SWindow> dockTabParentWindow = FGlobalTabmanager::Get()->TryInvokeTab(FTabId(GetMinesweeperDockTabName()))->GetParentWindow();
		dockTabParentWindow->MoveWindowTo((viewportSize - StandaloneParentWindow->GetTickSpaceGeometry().GetLocalSize()) / 2.0f);
	}
}

#pragma endregion




#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperEditorModule, Minesweeper)
