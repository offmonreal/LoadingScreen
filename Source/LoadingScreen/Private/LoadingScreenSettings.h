// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "SScaleBox.h"
#include "MoviePlayer.h"
#include "Engine/DeveloperSettings.h"

#include "LoadingScreenSettings.generated.h"

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenDescription
{
	GENERATED_USTRUCT_BODY()

	FLoadingScreenDescription();

	/** Минимальное время, в течение которого должен быть открыт экран загрузки, -1, если минимального времени нет. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	float MinimumLoadingScreenDisplayTime;
	
	/** Если TRUE, экран загрузки исчезнет, как только загрузка будет завершена. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	bool bAutoCompleteWhenLoadingCompletes;
	
	/* Если TRUE, ВИДЕО могут быть пропущены, нажав на экран загрузки, пока загрузка не завершена. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	bool bMoviesAreSkippable;
	
	/** Если true, воспроизведение фильма продолжается до тех пор, пока не будет вызван Stop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	bool bWaitForManualStop;
	
	/** Должны ли мы просто воспроизводить, зацикливать и т. Д. ПРИМЕЧАНИЕ. Если тип воспроизведения - MT_LoadingLoop, MoviePlayer автоматически завершит работу в последнем фильме и загрузка завершится независимо от bAutoCompleteWhenLoadingCompletes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Loading)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/** Пути к фильмам Content/Movies/ directory without extension. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movies)
	TArray<FString> MoviePaths;

	/** Должны ли мы показать images/tips/loading text?  Как правило, вы хотите установить значение false, если вы просто хотите показать фильм */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Display)
	bool bShowUIOverlay;

	/** Текст отображается рядом с анимированной иконкой  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Display)
	FText LoadingText;

	/** Разрешать показ сообщений в правом нижнем углу  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Display)
	bool bEnableTips;

	/** Текстура отображается на экране загрузки в верхней части фильма. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Images, meta=(AllowedClasses="Texture2D"))
	TArray<FStringAssetReference> Images;

	/** Тип масштабирования, применяемый к изображениям. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Images)
	TEnumAsByte<EStretch::Type> ImageStretch;
};

/**
 * Настройки для простого загрузочного экрана плагина.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loading Screen"))
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:

	/** Экран запуска проекта. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription StartupScreen;

	/** Экран загрузки по умолчанию между картами. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription DefaultScreen;

	/** Шрифт для отображения подсказок. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	FSlateFontInfo TipFont;

	/** Шрифт для отображения при загрузке. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Display)
	FSlateFontInfo LoadingFont;

	/** Размер кончика до его переноса на следующую строку. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	float TipWrapAt;

	/** Подсказки для отображения на экране загрузки. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	TArray<FText> Tips;
};
