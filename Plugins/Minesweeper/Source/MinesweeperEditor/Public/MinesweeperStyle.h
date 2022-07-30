// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateStyle.h"

struct FTextBlockStyle;
struct FSlateFontInfo;
struct FSlateIcon;
struct FSlateBrush;




class MINESWEEPEREDITOR_API FMinesweeperStyle
{
private:
	static TSharedPtr<FSlateStyleSet> StyleInstance;

	static TSharedRef<FSlateStyleSet> Create();


public:
	static const ISlateStyle& Get() { return *StyleInstance; }
	static FName GetStyleSetName() { return FName(TEXT("MinesweeperStyle")); }

	static void Initialize();
	static void Shutdown();
	
	/** Reloads textures used by slate renderer. */
	static void ReloadTextures();


	static const FLinearColor GetColor(const FName& InName);
	static const FSlateFontInfo GetFont(const FName& InName);
	static const FSlateIcon GetIcon(const FName& InName);
	static const FSlateBrush* GetBrush(const FName& InName);

};