// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "ChatLineWidget.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatLineWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	FText FormatMessage(EChatColor Color, const FString& Message);

public:
};
