// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "UObject/Object.h"
#include "ChatBoxWidget.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatBoxWidget : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatDataType(EGlobalMessageType Type, const FString& Message);
};
