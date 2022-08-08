// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperStyle.h"
#include "MinesweeperEditorModule.h"
#include "MinesweeperGridCanvas.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/StyleColors.h"

// Editor Style: C:\Epic Games\UE_5.0\Engine\Source\Editor\EditorStyle\Private\SlateEditorStyle.cpp


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FMinesweeperStyle::InEngineContent( RelativePath, ".png" ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( FMinesweeperStyle::InEngineContent( RelativePath, ".png" ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( FMinesweeperStyle::InEngineContent( RelativePath, ".png" ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( FMinesweeperStyle::InEngineContent( RelativePath, ".ttf" ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( FMinesweeperStyle::InEngineContent( RelativePath, ".otf" ), __VA_ARGS__ )

#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FMinesweeperStyle::InResources( RelativePath, ".png" ), __VA_ARGS__ )
#define IMAGE_PLUGIN_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush( FMinesweeperStyle::InResources(RelativePath, ".svg" ), __VA_ARGS__)
#define BOX_PLUGIN_BRUSH( RelativePath, ... ) FSlateBoxBrush( FMinesweeperStyle::InResources( RelativePath, ".png" ), __VA_ARGS__ )
#define BORDER_PLUGIN_BRUSH( RelativePath, ... ) FSlateBorderBrush( FMinesweeperStyle::InResources( RelativePath, ".png" ), __VA_ARGS__ )


const FVector2D Icon8x8(8.0f, 8.0f);
const FVector2D Icon12x12(12.0f, 12.0f);
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon32x32(32.0f, 32.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);


TSharedPtr<FSlateStyleSet> FMinesweeperStyle::StyleSet = nullptr;


FString FMinesweeperStyle::InResources(const FString& InRelativePath, const ANSICHAR* InExtension)
{
	static FString contentDir = IPluginManager::Get().FindPlugin(FMinesweeperEditorModule::GetPluginName().ToString())->GetBaseDir() / TEXT("Resources");
	return (contentDir / InRelativePath) + InExtension;
}

FString FMinesweeperStyle::InEngineContent(const FString& InRelativePath, const ANSICHAR* InExtension)
{
	static FString contentDir = FPaths::EngineContentDir();
	return (contentDir / InRelativePath) + InExtension;
}

FString FMinesweeperStyle::InEditorSlate(const FString& InRelativePath, const ANSICHAR* InExtension)
{
	static FString contentDir = FPaths::EngineContentDir() / TEXT("Editor/Slate");
	return (contentDir / InRelativePath) + InExtension;
}

FString FMinesweeperStyle::InSlateFonts(const FString& InRelativePath, const ANSICHAR* InExtension)
{
	static FString contentDir = FPaths::EngineContentDir() / TEXT("Slate/Fonts");
	return (contentDir / InRelativePath) + InExtension;
}


void FMinesweeperStyle::Initialize()
{
	if (StyleSet.IsValid()) return;

	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin(FMinesweeperEditorModule::GetPluginName().ToString());
	if (!plugin.IsValid()) return;


	// Unreal Engine Path Constants
	/*const FString EngineSlateDirectory = FPaths::EngineContentDir() / TEXT("Slate");
	const FString EngineSlateCommonDirectory = EngineSlateDirectory / TEXT("Common");
	const FString EngineSlateFontsDirectory = EngineSlateDirectory / TEXT("Fonts");
	//const FString EngineEditorSlateDirectory = FPaths::EngineContentDir() / TEXT("Editor/Slate");
	//const FString EngineEditorSlateIconsDirectory = EngineSlateDirectory / TEXT("Icons");
	*/
	const FString PluginDirectory = plugin->GetBaseDir();
	const FString PluginResourcesDirectory = PluginDirectory / TEXT("Resources");
	

	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(PluginResourcesDirectory);


	const FSlateColor DefaultForeground = FEditorStyle::GetSlateColor("DefaultForeground");
	const FSlateColor InvertedForeground = FEditorStyle::GetSlateColor("InvertedForeground");
	const FSlateColor SelectorColor = FEditorStyle::GetSlateColor("SelectorColor");
	const FSlateColor SelectionColor = FEditorStyle::GetSlateColor("SelectionColor");
	const FSlateColor SelectionColor_Inactive = FEditorStyle::GetSlateColor("SelectionColor_Inactive");
	const FSlateColor SelectionColor_Pressed = FEditorStyle::GetSlateColor("SelectionColor_Pressed");


	#pragma region COLORS

	const FLinearColor TitleColor = FLinearColor(1.0f, 1.0f, 1.0f);
	const FLinearColor HighScoreListHeaderColor = FLinearColor(1.0f, 1.0f, 1.0f);
	const FLinearColor DefaultTextShadowColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);

	StyleSet->Set("Color.Title", HighScoreListHeaderColor);
	StyleSet->Set("Color.HighScoreListHeader", HighScoreListHeaderColor);
	
	StyleSet->Set("Color.OneNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(1));
	StyleSet->Set("Color.TwoNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(2));
	StyleSet->Set("Color.ThreeNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(3));
	StyleSet->Set("Color.FourNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(4));
	StyleSet->Set("Color.FiveNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(5));
	StyleSet->Set("Color.SixNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(6));
	StyleSet->Set("Color.SevenNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(7));
	StyleSet->Set("Color.EightNeighborText", UMinesweeperGridCanvas::DefaultNeighborMineCountColor(8));

	StyleSet->Set("Color.Win", FLinearColor(0.0f, 1.0f, 0.0f));
	StyleSet->Set("Color.Lose", FLinearColor(1.0f, 0.0f, 0.0f));

	#pragma endregion


	#pragma region FONTS
	
	FSlateFontInfo RobotoFont = FSlateFontInfo(InEngineContent(TEXT("Slate/Fonts/Roboto-Regular"), ".ttf"), 10);
	FSlateFontInfo RobotoBoldFont = FSlateFontInfo(InEngineContent(TEXT("Slate/Fonts/Roboto-Bold"), ".ttf"), 10);

	StyleSet->Set("Font.Roboto", RobotoFont);
	StyleSet->Set("Font.Roboto.Bold", RobotoBoldFont);

	FSlateFontInfo RobotoFontWithOutline = RobotoFont;
	RobotoFontWithOutline.OutlineSettings.OutlineColor = FLinearColor::Black;
	RobotoFontWithOutline.OutlineSettings.OutlineSize = 1.0f;
	StyleSet->Set("Font.Roboto.BlackOutline", RobotoFontWithOutline);

	FSlateFontInfo RobotoBoltFontWithOutline = RobotoBoldFont;
	RobotoBoltFontWithOutline.OutlineSettings.OutlineColor = FLinearColor::Black;
	RobotoBoltFontWithOutline.OutlineSettings.OutlineSize = 1.0f;
	StyleSet->Set("Font.Roboto.Bold.BlackOutline", RobotoBoltFontWithOutline);

	#pragma endregion


	#pragma region TEXT STYLES
	
	FTextBlockStyle EngineNormalText = FEditorStyle::GetWidgetStyle<FTextBlockStyle>("NormalText");

	FTextBlockStyle NormalTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoFont)
		.SetFontSize(10)
		.SetColorAndOpacity(FStyleColors::Foreground)
		.SetShadowOffset(FVector2D(1.0f))
		.SetShadowColorAndOpacity(DefaultTextShadowColor);
	StyleSet->Set("Text.Normal", NormalTextStyle);

	FTextBlockStyle NormalBoldTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetFontSize(10)
		.SetColorAndOpacity(FStyleColors::Foreground)
		.SetShadowOffset(FVector2D(1.0f))
		.SetShadowColorAndOpacity(DefaultTextShadowColor);
	StyleSet->Set("Text.Normal.Bold", NormalBoldTextStyle);
	
	FTextBlockStyle SmallTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(8)
		.SetShadowOffset(FVector2D(0.0f));
	StyleSet->Set("Text.Small", SmallTextStyle);

	FTextBlockStyle VerySmallTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(7)
		.SetShadowOffset(FVector2D(0.0f));
	StyleSet->Set("Text.VerySmall", VerySmallTextStyle);

	FTextBlockStyle TinyTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(6)
		.SetShadowOffset(FVector2D(0.0f));
	StyleSet->Set("Text.Tiny", TinyTextStyle);
	

	FTextBlockStyle TitleTextStyle = FTextBlockStyle(NormalTextStyle)
		//.SetFont(RobotoBoltFontWithOutline)
		.SetFont(RobotoFont/*WithOutline*/)
		.SetFontSize(28)
		.SetColorAndOpacity(TitleColor)
		.SetShadowOffset(FVector2D(2.0f))
		.SetShadowColorAndOpacity(DefaultTextShadowColor);
	StyleSet->Set("Text.Title", TitleTextStyle);

	FTextBlockStyle TitleTextRedStyle = FTextBlockStyle(TitleTextStyle)
		.SetColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.0f));
	StyleSet->Set("Text.Title.Red", TitleTextRedStyle);
	

	StyleSet->Set("Text.DefaultButton", StyleSet->GetWidgetStyle<FTextBlockStyle>("Text.Normal"));


	FTextBlockStyle HeaderLargeTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetColorAndOpacity(TitleColor)
		.SetFontSize(20)
		.SetShadowOffset(FVector2D(1.5f))
		.SetShadowColorAndOpacity(DefaultTextShadowColor);
	StyleSet->Set("Text.HeaderLarge", HeaderLargeTextStyle);

	FTextBlockStyle MineCountTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetColorAndOpacity(TitleColor)
		.SetFontSize(20)
		.SetShadowOffset(FVector2D(1.0f))
		.SetShadowColorAndOpacity(DefaultTextShadowColor);
	StyleSet->Set("Text.MineCount", MineCountTextStyle);

	FTextBlockStyle WinLoseTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFont(RobotoBoltFontWithOutline)
		.SetFontSize(18)
		.SetShadowOffset(FVector2D(0.0f));
	StyleSet->Set("Text.WinLose", WinLoseTextStyle);
	

	FTextBlockStyle HighScoreListHeaderTextStyle = NormalTextStyle
		.SetFontSize(10)
		.SetColorAndOpacity(FLinearColor::Black)
		.SetShadowOffset(FVector2D(1.0f))
		.SetShadowColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.2f))
		.SetFont(RobotoBoldFont);
	StyleSet->Set("HighScoreList.Text.Header", HighScoreListHeaderTextStyle);

	for (int32 rank = 1; rank <= 10; ++rank)
	{
		FString strName = "HighScoreList.Text.Rank" + FString::FromInt(rank);
		int32 fontSizeIncrease = FMath::FloorToInt32((10.0f - rank) / 2.0f);
		StyleSet->Set(*strName, FTextBlockStyle(NormalTextStyle)
			.SetFont(RobotoFont)
			.SetFontSize(7 + fontSizeIncrease)
			.SetColorAndOpacity(FStyleColors::Foreground)
			//.SetShadowColorAndOpacity(FStyleColors::ForegroundInverted.GetSpecifiedColor())
			.SetShadowOffset(FVector2D(1.0f))
			.SetShadowColorAndOpacity(DefaultTextShadowColor));
	}

	#pragma endregion


	#pragma region BUTTON AND CHECKBOX STYLES

	const FButtonStyle DefaultButtonStyle = FButtonStyle()
		.SetNormal(FSlateRoundedBoxBrush(FStyleColors::Secondary, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetHovered(FSlateRoundedBoxBrush(FStyleColors::Hover, 4.0f, FStyleColors::Hover, 1.0f))
		.SetPressed(FSlateRoundedBoxBrush(FStyleColors::Header, 4.0f, FStyleColors::Hover, 1.0f))
		.SetNormalPadding(FMargin(2, 2, 2, 2))
		.SetPressedPadding(FMargin(2, 3, 2, 1))
		.SetNormalForeground(FStyleColors::ForegroundHover)
		.SetHoveredForeground(FStyleColors::ForegroundHover)
		.SetPressedForeground(FStyleColors::ForegroundHover)
		.SetDisabledForeground(FStyleColors::Foreground);
	StyleSet->Set("DefaultButton", DefaultButtonStyle);


	// Toggle Button CheckBox Style
	FCheckBoxStyle ToggleButtonStyle;
	ToggleButtonStyle.CheckBoxType = ESlateCheckBoxType::ToggleButton;
	// unchecked
	ToggleButtonStyle.UncheckedImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.UncheckedImage.TintColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.0f);

	#pragma endregion


	#pragma region BRUSHES

	StyleSet->Set("TransparentBorder", new FSlateColorBrush(FLinearColor::Transparent));
	
	StyleSet->Set("ToolbarButton", new IMAGE_PLUGIN_BRUSH("Mine_64x", Icon40x40));
	StyleSet->Set("ToolbarButton.Small", new IMAGE_PLUGIN_BRUSH("Mine_64x", Icon16x16));

	//StyleSet->Set("Settings", new IMAGE_BRUSH("Editor/Slate/Icons/GeneralTools/Settings_40x", Icon16x16));
	StyleSet->Set("Settings", new IMAGE_PLUGIN_BRUSH("Settings_40x", Icon16x16));

	FLinearColor RoundedPanelColor = FStyleColors::AccentWhite.GetSpecifiedColor();
	RoundedPanelColor.A = 0.1f;
	StyleSet->Set("RoundedPanel", new FSlateRoundedBoxBrush(RoundedPanelColor, 8.0f));

	StyleSet->Set("ClosedCell", new IMAGE_PLUGIN_BRUSH("ClosedCell_64x", Icon64x64));
	StyleSet->Set("OpenCell", new IMAGE_PLUGIN_BRUSH("OpenCell_64x", Icon64x64));
	StyleSet->Set("OpenCell.Mine", new IMAGE_PLUGIN_BRUSH("OpenCell_Mine_64x", Icon64x64));
	StyleSet->Set("Mine", new IMAGE_PLUGIN_BRUSH("Mine_64x", Icon64x64));
	StyleSet->Set("Flag", new IMAGE_PLUGIN_BRUSH("Flag_64x", Icon64x64));

	StyleSet->Set("MrSmile.Alive", new IMAGE_PLUGIN_BRUSH("MrSmile_Alive_64x", Icon64x64));
	StyleSet->Set("MrSmile.Dead", new IMAGE_PLUGIN_BRUSH("MrSmile_Dead_64x", Icon64x64));

	StyleSet->Set("ToggleWindowMode", new IMAGE_PLUGIN_BRUSH("ToggleWindowMode_32x", Icon20x20));


	FLinearColor HighScoreListHeaderBackgroundColor = FStyleColors::AccentWhite.GetSpecifiedColor();
	HighScoreListHeaderBackgroundColor.A = 0.4f;
	StyleSet->Set("HighScoreList.Header", new FSlateRoundedBoxBrush(HighScoreListHeaderBackgroundColor, 4.0f));

	StyleSet->Set("Border.Win", new FSlateRoundedBoxBrush(FLinearColor(0, 0, 0, 0), 2.0f, FLinearColor(0, 1, 0, 1), 1.0f));
	StyleSet->Set("Border.Lose", new FSlateRoundedBoxBrush(FLinearColor(0, 0, 0, 0), 2.0f, FLinearColor(1, 0, 0, 1), 1.0f));


	StyleSet->Set("GameHeaderTimeBorder", new FSlateRoundedBoxBrush(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f), FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), 1.0f));

	#pragma endregion


	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}


void FMinesweeperStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}




#undef IMAGE_PLUGIN_BRUSH
#undef IMAGE_PLUGIN_BRUSH_SVG
#undef BOX_PLUGIN_BRUSH
#undef BORDER_PLUGIN_BRUSH

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
#undef TTF_CORE_FONT
