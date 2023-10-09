// Copyright 2023 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "ChatRichInlineHyperlinkDecorator.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatRichInlineHyperlinkDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Appearance)
	FHyperlinkStyle Style;

	UFUNCTION(BlueprintNativeEvent)
	void ClickFun(const FString& ID);

	UFUNCTION(BlueprintImplementableEvent)
	void BPHelperFunction(APlayerController* PlayerController, UGameInstance* GameInstance, const FString& ID);

	UChatRichInlineHyperlinkDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};
