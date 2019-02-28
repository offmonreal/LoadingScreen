// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SSimpleLoadingScreen.h"

#include "SScaleBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "SSafeZone.h"
#include "SThrobber.h"
#include "SDPIScaler.h"
#include "Engine/Texture2D.h"
#include "Engine/UserInterfaceSettings.h"

#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

/////////////////////////////////////////////////////
// SSimpleLoadingScreen

void SSimpleLoadingScreen::Construct(const FArguments& InArgs, const FLoadingScreenDescription& InScreenDescription)
{
	LastComputedDPIScale = 1.0f;

	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();

	const FSlateFontInfo& TipFont = Settings->TipFont;
	const FSlateFontInfo& LoadingFont = Settings->LoadingFont;

	TSharedRef<SOverlay> Root = SNew(SOverlay);

	//Если ессть картинки
	if ( InScreenDescription.Images.Num() > 0 )
	{
		//Рандом выбор картинки
		int32 ImageIndex = FMath::RandRange(0, InScreenDescription.Images.Num() - 1);

		//Загрузка в память
		const FStringAssetReference& ImageAsset = InScreenDescription.Images[ImageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();

		//Конвертируем в объект
		if ( UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject) )
		{
			//Размер картинки
			FIntPoint TextureSize = LoadingImage->GetImportedSize();

			//Создаем кисть компонет
			DynamicBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage, FVector2D((float)TextureSize.X, (float)TextureSize.Y));

			//Создаем Slate кисть
			SlateBrush = DynamicBrush->GetSlateBrush();

			Root->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SScaleBox)
				.Stretch(InScreenDescription.ImageStretch)
				[
					SNew(SImage)
					.Image(SlateBrush)//Назначаем Slate кисть
				]
			];
		}
	}

	TSharedRef<SWidget> TipWidget = SNullWidget::NullWidget;

	//Подсказки-сообщения
	if (InScreenDescription.bEnableTips && Settings->Tips.Num() > 0 )
	{
		//Рандом выбор подсказки-сообщения
		int32 TipIndex = FMath::RandRange(0, Settings->Tips.Num() - 1);

		//Создаем виджет ТекстБлок
		TipWidget = SNew(STextBlock)
			.WrapTextAt(Settings->TipWrapAt)
			.Font(TipFont)
			.Text(Settings->Tips[TipIndex]);
	}

	Root->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Bottom)
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor(FLinearColor(0, 0, 0, 0.75))
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		[
			SNew(SSafeZone)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.IsTitleSafe(true)
			[
				SNew(SDPIScaler)
				.DPIScale(this, &SSimpleLoadingScreen::GetDPIScale)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.Padding(FMargin(25, 0.0f, 0, 0))
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SCircularThrobber)
						// Преобразовать размер шрифта в пиксели, pixel_size = point_size * resolution / 72, затем наполовину уменьшить радиус
						.Radius((LoadingFont.Size * 96.0f/72.0f) / 2.0f)
					]

					+ SHorizontalBox::Slot()
					.Padding(FMargin(40.0f, 0.0f, 0, 0))
					.AutoWidth()
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(InScreenDescription.LoadingText)
						.Font(LoadingFont)
					]

					+ SHorizontalBox::Slot()
					.FillWidth(1)
					.HAlign(HAlign_Fill)
					[
						SNew(SSpacer)
						.Size(FVector2D(1.0f, 1.0f))
					]

					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(FMargin(10.0f))
					[
						TipWidget
					]
				]
			]
		]
	];

	ChildSlot
	[
		Root
	];
}

void SSimpleLoadingScreen::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	const FVector2D& LocalSize = AllottedGeometry.GetLocalSize();
	FIntPoint Size((int32)LocalSize.X, (int32)LocalSize.Y);
	const float NewScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);

	if ( NewScale != LastComputedDPIScale )
	{
		LastComputedDPIScale = NewScale;
		SlatePrepass(1.0f);
	}
}

float SSimpleLoadingScreen::GetDPIScale() const
{
	return LastComputedDPIScale;
}

#undef LOCTEXT_NAMESPACE
