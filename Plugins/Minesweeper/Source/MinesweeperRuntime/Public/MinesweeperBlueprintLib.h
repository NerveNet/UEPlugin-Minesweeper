// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinesweeperDifficulty.h"
#include "MinesweeperBlueprintLib.generated.h"

class UMinesweeperGridCanvas;
class UMinesweeperGame;




/**
 * Minesweeper Blueprint Function Library
 */
UCLASS(ClassGroup = "Minesweeper")
class MINESWEEPERRUNTIME_API UMinesweeperBlueprintLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Minesweeper", Meta = (WorldContext = "WorldContextObject"))
		static UMinesweeperGridCanvas* CreateMinesweeperGridCanvas(UObject* WorldContextObject, UMinesweeperGame* Game, const float CellDrawSize);


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FMinesweeperDifficulty BeginnerDifficulty();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FMinesweeperDifficulty IntermediateDifficulty();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FMinesweeperDifficulty ExpertDifficulty();


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static bool IsBeginnerDifficulty(const FMinesweeperDifficulty& Difficulty);

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static bool IsIntermediateDifficulty(const FMinesweeperDifficulty& Difficulty);

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static bool IsExpertDifficulty(const FMinesweeperDifficulty& Difficulty);

};

