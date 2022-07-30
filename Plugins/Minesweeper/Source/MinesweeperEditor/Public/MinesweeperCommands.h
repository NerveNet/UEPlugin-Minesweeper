// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperStyle.h"




class MINESWEEPEREDITOR_API FMinesweeperCommands : public TCommands<FMinesweeperCommands>
{
public:
	FMinesweeperCommands()
		: TCommands<FMinesweeperCommands>(
			TEXT("Minesweeper"),
			NSLOCTEXT("Contexts", "NerveNetDevToolsCommands", "NerveNet DevTools"),
			NAME_None,
			FMinesweeperStyle::GetStyleSetName()
		)
	{ }


	//~ Begin TCommands<> Interface
	virtual void RegisterCommands() override;
	//~ End TCommands<> Interface


	TSharedPtr<FUICommandInfo> ToggleMinesweeperWindowMode;

};

