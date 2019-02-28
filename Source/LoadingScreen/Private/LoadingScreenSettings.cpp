// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
//#include "Internationalization/Internationalization.h"
//#include "Internationalization/Culture.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

FLoadingScreenDescription::FLoadingScreenDescription()
	: MinimumLoadingScreenDisplayTime(-1)
	, bAutoCompleteWhenLoadingCompletes(true)
	, bMoviesAreSkippable(true)
	, bWaitForManualStop(false)
	, bShowUIOverlay(true)
	, bEnableTips(false)
	, LoadingText(LOCTEXT("Loading", "LOADING"))
	, ImageStretch(EStretch::ScaleToFit)
{

	/*FString test = FInternationalization::Get().GetCurrentLanguage().Get().GetName();

	if (test == "ru")
		return;
	else
	{
		//FInternationalization::Get().SetCurrentCulture("ru");
		int x = 200;
		x++;
		x = x * 200;
	}*/
}

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer): Super(Initializer)
{
	TipWrapAt = 1000.0f;

	if ( !IsRunningDedicatedServer() )
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(TEXT("/Engine/EngineFonts/Roboto"));
		TipFont = FSlateFontInfo(RobotoFontObj.Object, 20, FName("Normal"));
		LoadingFont = FSlateFontInfo(RobotoFontObj.Object, 32, FName("Bold"));
	}
}

#undef LOCTEXT_NAMESPACE
