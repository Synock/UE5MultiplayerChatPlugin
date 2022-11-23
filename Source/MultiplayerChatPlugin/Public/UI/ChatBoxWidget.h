// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "ChatBoxWidget.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	UEditableTextBox* TextBox = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UComboBoxString* ChatSelection = nullptr;

	UFUNCTION(BlueprintCallable)
	void HandleTextEnter();



public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatDataType(EGlobalMessageType Type, const FString& Message);
};
