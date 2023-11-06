// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MinesweeperDifficulty.h"
#include "MinesweeperGame.h"
#include "MinesweeperVisualTheme.h"
#include "MinesweeperStatics.generated.h"

class UTexture2D;
class UMinesweeperGridCanvas;




/**
 * Minesweeper Blueprint Function Library
 */
UCLASS(ClassGroup = "Minesweeper")
class MINESWEEPERRUNTIME_API UMinesweeperStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static inline const TCHAR* DefaultClosedCellTexturePath() { return TEXT("/Minesweeper/ClosedCell_64x.ClosedCell_64x"); }
	static inline const TCHAR* DefaultOpenCellTexturePath() { return TEXT("/Minesweeper/OpenCell_64x.OpenCell_64x"); }
	static inline const TCHAR* DefaultOpenCellMineTexturePath() { return TEXT("/Minesweeper/OpenCell_Mine_64x.OpenCell_Mine_64x"); }
	static inline const TCHAR* DefaultMineTexturePath() { return TEXT("/Minesweeper/Mine_64x.Mine_64x"); }
	static inline const TCHAR* DefaultFlagTexturePath() { return TEXT("/Minesweeper/Flag_64x.Flag_64x"); }
	static inline const TCHAR* DefaultHoverCellTexturePath() { return TEXT("/Minesweeper/HoverCell_64x.HoverCell_64x"); }
	static inline const TCHAR* DefaultCellFontPath() { return TEXT("/Minesweeper/CellFont.CellFont"); }


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultClosedCellTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultOpenCellTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultOpenCellMineTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultMineTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultFlagTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UTexture2D* DefaultHoverCellTexture();

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static UFont* DefaultCellFont();


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE float DefaultCellDrawSize() { return 28.0f; }

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE float MinCellDrawSize() { return 10.0f; }

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE float MaxCellDrawSize() { return 64.0f; }

	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static float ClampCellDrawSize(const int32 Size);


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FLinearColor DefaultHoverCellValidColor() { return FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); }
	
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FLinearColor DefaultHoverCellInvalidColor() { return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); }
	
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FSlateColor DefaultNeighborMineCountColor(const int32 MineCount);


	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FMinesweeperVisualTheme DefaultVisualTheme();




	/** Creates a render texture canvas that draws a Minesweeper game object's data and logic. */
	UFUNCTION(BlueprintCallable, Category = "Minesweeper", Meta = (WorldContext = "WorldContextObject"))
		static UMinesweeperGridCanvas* CreateMinesweeperGridCanvas(UObject* WorldContextObject, UMinesweeperGame* Game, const FMinesweeperVisualTheme& VisualTheme);


	/** Beginner Difficulty: 9x9 grid, 10 mines (10 / 81 = 0.12345% probability) */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FMinesweeperDifficulty BeginnerDifficulty() { return FMinesweeperDifficulty(9, 9, 10); }
	
	/** Intermediate Difficulty: 16x16 grid, 40 mines (40 / 256 = 0.15625% probability) */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FMinesweeperDifficulty IntermediateDifficulty() { return FMinesweeperDifficulty(16, 16, 40); }
	
	/** Expert Difficulty: 30x16 grid, 99 mines (99 / 576 = 0.17187% probability) */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FMinesweeperDifficulty ExpertDifficulty() { return FMinesweeperDifficulty(30, 16, 99); }

	/** Returns the max difficulty for a game. */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE FMinesweeperDifficulty MaxDifficulty() { return FMinesweeperDifficulty(UMinesweeperGame::MaxGridSize, UMinesweeperGame::MaxGridSize, UMinesweeperGame::MaxMineCount); }


	/** Returns true for valid difficulty settings. */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static bool IsDifficultyValid(const FMinesweeperDifficulty& InDifficulty);

	/** Returns true if the difficulty settings are beginner level (9x9 grid with 10 mines). */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE bool IsBeginnerDifficulty(const FMinesweeperDifficulty& InDifficulty) { return InDifficulty == BeginnerDifficulty(); }

	/** Returns true if the difficulty settings are intermediate level (16x16 grid with 40 mines). */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE bool IsIntermediateDifficulty(const FMinesweeperDifficulty& InDifficulty) { return InDifficulty == IntermediateDifficulty(); }

	/** Returns true if the difficulty settings are a expert level (30x16 grid with 99 mines). */
	UFUNCTION(BlueprintPure, Category = "Minesweeper")
		static FORCEINLINE bool IsExpertDifficulty(const FMinesweeperDifficulty& InDifficulty) { return InDifficulty == ExpertDifficulty(); }

};

