// Copyright 2022 Brad Monahan. All Rights Reserved.

#include "MinesweeperStyle.h"
#include "MinesweeperEditorModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/StyleColors.h"
//#include "Styling/StarshipCoreStyle.h"




#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )
#define TTF_CORE_FONT(RelativePath, ...) FSlateFontInfo( Style->RootToCoreContentDir(RelativePath, TEXT(".ttf") ), __VA_ARGS__)

const FVector2D Icon8x8(8.0f, 8.0f);
const FVector2D Icon12x12(12.0f, 12.0f);
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon32x32(32.0f, 32.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);
const FVector2D Icon128x128(128.0f, 128.0f);
const FVector2D Icon256x256(256.0f, 256.0f);
const FVector2D Icon512x512(512.0f, 512.0f);


TSharedPtr<FSlateStyleSet> FMinesweeperStyle::StyleInstance = nullptr;


void FMinesweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
		ReloadTextures();
	}
}

void FMinesweeperStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}


void FMinesweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}


const FLinearColor FMinesweeperStyle::GetColor(const FName& InName)
{
	return FMinesweeperStyle::Get().GetSlateColor(InName).GetSpecifiedColor();
}
const FSlateFontInfo FMinesweeperStyle::GetFont(const FName& InName)
{
	return FMinesweeperStyle::Get().GetFontStyle(InName);
}
const FSlateIcon FMinesweeperStyle::GetIcon(const FName& InName)
{
	return FSlateIcon(FMinesweeperStyle::GetStyleSetName(), InName);
}
const FSlateBrush* FMinesweeperStyle::GetBrush(const FName& InName)
{
	return FSlateIcon(FMinesweeperStyle::GetStyleSetName(), InName).GetIcon();
}


TSharedRef<FSlateStyleSet> FMinesweeperStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	
	// Path Constants
	const FString EngineSlateDirectory = FPaths::EngineContentDir() / TEXT("Slate");
	const FString EngineSlateCommonDirectory = EngineSlateDirectory / TEXT("Common");
	const FString EngineSlateFontsDirectory = EngineSlateDirectory / TEXT("Fonts");
	
	const FString EngineEditorSlateDirectory = FPaths::EngineContentDir() / TEXT("Editor/Slate");
	const FString EngineEditorSlateIconsDirectory = EngineSlateDirectory / TEXT("Icons");
	
	const FString PluginResourcesDirectory = IPluginManager::Get().FindPlugin(FMinesweeperEditorModule::GetPluginName().ToString())->GetBaseDir() / TEXT("Resources");
	Style->SetContentRoot(PluginResourcesDirectory);


	const FSlateColor DefaultForeground = FEditorStyle::GetSlateColor("DefaultForeground");
	const FSlateColor InvertedForeground = FEditorStyle::GetSlateColor("InvertedForeground");
	const FSlateColor SelectorColor = FEditorStyle::GetSlateColor("SelectorColor");
	const FSlateColor SelectionColor = FEditorStyle::GetSlateColor("SelectionColor");
	const FSlateColor SelectionColor_Inactive = FEditorStyle::GetSlateColor("SelectionColor_Inactive");
	const FSlateColor SelectionColor_Pressed = FEditorStyle::GetSlateColor("SelectionColor_Pressed");


	#pragma region COLORS

	const FLinearColor TitleColor = FLinearColor(1.0f, 1.0f, 1.0f);
	const FLinearColor HighScoreListHeaderColor = FLinearColor(1.0f, 1.0f, 1.0f);

	Style->Set("Color.Title", HighScoreListHeaderColor);
	Style->Set("Color.HighScoreListHeader", HighScoreListHeaderColor);

	Style->Set("Color.OneNeighborText", FLinearColor(0.0f, 0.0f, 1.0f));
	Style->Set("Color.TwoNeighborText", FLinearColor(0.0f, 1.0f, 0.0f));
	Style->Set("Color.ThreeNeighborText", FLinearColor(1.0f, 0.0f, 0.0f));
	Style->Set("Color.FourNeighborText", FLinearColor(0.0f, 0.0f, 0.5f));
	Style->Set("Color.FiveNeighborText", FLinearColor(0.0f, 0.5f, 0.0f));
	Style->Set("Color.SixNeighborText", FLinearColor(0.5f, 0.0f, 0.0f));
	Style->Set("Color.SevenNeighborText", FLinearColor(0.0f, 0.0f, 0.25f));
	Style->Set("Color.EightNeighborText", FLinearColor(0.0f, 0.25f, 0.0f));

	Style->Set("Color.Win", FLinearColor(0.0f, 1.0f, 0.0f));
	Style->Set("Color.Lose", FLinearColor(1.0f, 0.0f, 0.0f));

	#pragma endregion


	#pragma region FONTS

	FSlateFontInfo RobotoFont = FSlateFontInfo(EngineSlateFontsDirectory / TEXT("Roboto-Regular.ttf"), 10);
	FSlateFontInfo RobotoBoldFont = FSlateFontInfo(EngineSlateFontsDirectory / TEXT("Roboto-Bold.ttf"), 10);

	Style->Set("Font.Roboto", RobotoFont);
	Style->Set("Font.Roboto.Bold", RobotoBoldFont);

	FSlateFontInfo RobotoFontWithOutline = FSlateFontInfo(EngineSlateFontsDirectory / TEXT("Roboto-Bold.ttf"), 10);
	RobotoFontWithOutline.OutlineSettings.OutlineColor = FLinearColor::Black;
	RobotoFontWithOutline.OutlineSettings.OutlineSize = 1.0f;
	Style->Set("Font.Roboto.BlackOutline", RobotoFontWithOutline);

	#pragma endregion


	#pragma region TEXT STYLES
	
	FTextBlockStyle EngineNormalText = FEditorStyle::GetWidgetStyle<FTextBlockStyle>("NormalText");

	FTextBlockStyle NormalTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoFont)
		.SetFontSize(10)
		.SetColorAndOpacity(FStyleColors::Foreground)
		.SetShadowOffset(FVector2D::UnitVector);
	Style->Set("Text.Normal", NormalTextStyle);

	FTextBlockStyle NormalBoldTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetFontSize(10)
		.SetColorAndOpacity(FStyleColors::Foreground)
		.SetShadowOffset(FVector2D::UnitVector);
	Style->Set("Text.Normal.Bold", NormalBoldTextStyle);
	
	FTextBlockStyle SmallTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(8)
		.SetShadowOffset(FVector2D(0.0f, 0.0f));
	Style->Set("Text.Small", SmallTextStyle);

	FTextBlockStyle VerySmallTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(7)
		.SetShadowOffset(FVector2D(0.0f, 0.0f));
	Style->Set("Text.VerySmall", VerySmallTextStyle);

	FTextBlockStyle TinyTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFontSize(6)
		.SetShadowOffset(FVector2D(0.0f, 0.0f));
	Style->Set("Text.Tiny", TinyTextStyle);
	
	FTextBlockStyle TitleTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetColorAndOpacity(TitleColor)
		.SetFontSize(28)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetShadowOffset(FVector2D(2.0f));
	Style->Set("Text.Title", TitleTextStyle);
	

	Style->Set("Text.DefaultButton", Style->GetWidgetStyle<FTextBlockStyle>("Text.Normal"));


	FTextBlockStyle HeaderLargeTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetColorAndOpacity(TitleColor)
		.SetFontSize(20)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetShadowOffset(FVector2D(1.5f));
	Style->Set("Text.HeaderLarge", HeaderLargeTextStyle);

	FTextBlockStyle MineCountTextStyle = FTextBlockStyle(EngineNormalText)
		.SetFont(RobotoBoldFont)
		.SetColorAndOpacity(TitleColor)
		.SetFontSize(20)
		.SetShadowColorAndOpacity(FLinearColor::Black)
		.SetShadowOffset(FVector2D(1.0f));
	Style->Set("Text.MineCount", MineCountTextStyle);

	FTextBlockStyle WinLoseTextStyle = FTextBlockStyle(NormalTextStyle)
		.SetFont(RobotoFontWithOutline)
		.SetFontSize(18)
		.SetShadowOffset(FVector2D(0.0f, 0.0f));
	Style->Set("Text.WinLose", WinLoseTextStyle);
	

	FTextBlockStyle HighScoreListHeaderTextStyle = NormalTextStyle
		.SetFontSize(10)
		.SetColorAndOpacity(FLinearColor::Black)
		.SetShadowColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.2f))
		.SetShadowOffset(FVector2D(1.0f))
		.SetFont(RobotoBoldFont);
	Style->Set("HighScoreList.Text.Header", HighScoreListHeaderTextStyle);

	for (int32 rank = 1; rank <= 10; rank++)
	{
		FString strName = "HighScoreList.Text.Rank" + FString::FromInt(rank);
		int32 fontSizeIncrease = FMath::FloorToInt32((10.0f - rank) / 2.0f);
		Style->Set(*strName, FTextBlockStyle(NormalTextStyle)
			.SetFont(RobotoFont)
			.SetFontSize(7 + fontSizeIncrease)
			.SetColorAndOpacity(FStyleColors::Foreground)
			.SetShadowColorAndOpacity(FStyleColors::ForegroundInverted.GetSpecifiedColor())
			.SetShadowOffset(FVector2D(1.0f)));
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
	Style->Set("DefaultButton", DefaultButtonStyle);


	// Toggle Button CheckBox Style
	FCheckBoxStyle ToggleButtonStyle;
	ToggleButtonStyle.CheckBoxType = ESlateCheckBoxType::ToggleButton;
	// unchecked
	ToggleButtonStyle.UncheckedImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.UncheckedImage.TintColor = FLinearColor(0.01f, 0.01f, 0.01f, 1.0f);
	// unchecked hovered
	ToggleButtonStyle.UncheckedHoveredImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.UncheckedHoveredImage.TintColor = FLinearColor(0.02f, 0.02f, 0.02f, 1.0f);
	// unchecked pressed
	ToggleButtonStyle.UncheckedPressedImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.UncheckedPressedImage.TintColor = FLinearColor(0.02f, 0.02f, 0.02f, 1.0f);
	// checked
	ToggleButtonStyle.CheckedImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.CheckedImage.TintColor = FLinearColor(0.0f, 0.1f, 0.04f, 1.0f);
	// checked hovered
	ToggleButtonStyle.CheckedHoveredImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.CheckedHoveredImage.TintColor = FLinearColor(0.0f, 0.12f, 0.06f, 1.0f);
	// checked pressed
	ToggleButtonStyle.CheckedPressedImage.DrawAs = ESlateBrushDrawType::Box;
	ToggleButtonStyle.CheckedPressedImage.TintColor = FLinearColor(0.0f, 0.12f, 0.06f, 1.0f);
	Style->Set("ToggleButton", ToggleButtonStyle);
	
	const FCheckBoxStyle ToggleButtonCheckboxStyle = FCheckBoxStyle()
		.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
		.SetUncheckedImage(FSlateRoundedBoxBrush(FStyleColors::Secondary, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetUncheckedHoveredImage(FSlateRoundedBoxBrush(SelectionColor, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetUncheckedPressedImage(FSlateRoundedBoxBrush(SelectionColor_Pressed, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetCheckedImage(FSlateRoundedBoxBrush(SelectionColor_Pressed, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetCheckedHoveredImage(FSlateRoundedBoxBrush(SelectionColor, 4.0f, FStyleColors::InputOutline, 1.0f))
		.SetCheckedPressedImage(FSlateRoundedBoxBrush(SelectionColor_Pressed, 4.0f, FStyleColors::InputOutline, 1.0f));
	Style->Set("ToggleButtonCheckbox", ToggleButtonCheckboxStyle);

	#pragma endregion


	#pragma region BRUSHES
	
    Style->Set("ToolbarButton", new IMAGE_BRUSH("Mine_32x", Icon40x40));
	Style->Set("ToolbarButton.Small", new IMAGE_BRUSH("Mine_32x", Icon16x16));

	FLinearColor RoundedPanelColor = FStyleColors::AccentWhite.GetSpecifiedColor();
	RoundedPanelColor.A = 0.1f;
	Style->Set("RoundedPanel", new FSlateRoundedBoxBrush(RoundedPanelColor, 8.0f));

	Style->Set("ClosedCell", new IMAGE_BRUSH("ClosedCell_32x", Icon32x32));
	Style->Set("OpenCell", new IMAGE_BRUSH("OpenCell_32x", Icon32x32));
	Style->Set("OpenCell.Mine", new IMAGE_BRUSH("OpenCell_Mine_32x", Icon32x32));
	Style->Set("Mine", new IMAGE_BRUSH("Mine_32x", Icon32x32));
	Style->Set("Flag", new IMAGE_BRUSH("Flag_32x", Icon32x32));

	Style->Set("MrSmile.Alive", new IMAGE_BRUSH("MrSmile_Alive_64x", Icon64x64));
	Style->Set("MrSmile.Dead", new IMAGE_BRUSH("MrSmile_Dead_64x", Icon64x64));

	Style->Set("ToggleWindowMode", new IMAGE_BRUSH("ToggleWindowMode_32x", Icon20x20));


	FLinearColor HighScoreListHeaderBackgroundColor = FStyleColors::AccentWhite.GetSpecifiedColor();
	HighScoreListHeaderBackgroundColor.A = 0.4f;
	Style->Set("HighScoreList.Header", new FSlateRoundedBoxBrush(HighScoreListHeaderBackgroundColor, 4.0f));

	Style->Set("Border.Win", new FSlateRoundedBoxBrush(FLinearColor(0, 0, 0, 0), 2.0f, FLinearColor(0, 1, 0, 1), 1.0f));
	Style->Set("Border.Lose", new FSlateRoundedBoxBrush(FLinearColor(0, 0, 0, 0), 2.0f, FLinearColor(1, 0, 0, 1), 1.0f));

	#pragma endregion


	return Style;
}




#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

