// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperCommands.h"


#define LOCTEXT_NAMESPACE "Minesweeper"




void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(ToggleMinesweeperWindowMode,
		"Minesweeper",
		"Why work when you can play Minesweeper???",
		EUserInterfaceActionType::Button, 
		FInputChord(EKeys::M, false, true, false, false)
	);
}




#undef LOCTEXT_NAMESPACE
