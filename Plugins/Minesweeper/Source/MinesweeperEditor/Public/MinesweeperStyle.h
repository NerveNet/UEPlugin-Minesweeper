// Copyright 2022 Brad Monahan. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Styling/ISlateStyle.h"
#include "Styling/SlateStyle.h"

struct FSlateFontInfo;
struct FSlateIcon;
struct FSlateBrush;




class MINESWEEPEREDITOR_API FMinesweeperStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static const ISlateStyle& Get() { return *StyleSet; }
	static FName GetStyleSetName() { return FName(TEXT("MinesweeperStyle")); }

	static const FLinearColor GetColor(const FName& InName) { return StyleSet->GetSlateColor(InName).GetSpecifiedColor(); }
	static const FSlateFontInfo GetFontStyle(const FName& InName) { return StyleSet->GetFontStyle(InName); }
	static const FSlateIcon GetIcon(const FName& InName) { return FSlateIcon(GetStyleSetName(), InName); }
	//static const FSlateBrush* GetBrush(const FName& InName) { return StyleSet->GetBrush(InName); }
	static const FSlateBrush* GetBrush(const FName& InName) { return FSlateIcon(GetStyleSetName(), InName).GetIcon(); }


private:
	static TSharedPtr<FSlateStyleSet> StyleSet;

	static FString InResources(const FString& InRelativePath, const ANSICHAR* InExtension);
	static FString InEngineContent(const FString& InRelativePath, const ANSICHAR* InExtension);
	static FString InEditorSlate(const FString& InRelativePath, const ANSICHAR* InExtension);
	static FString InSlateFonts(const FString& InRelativePath, const ANSICHAR* InExtension);

};