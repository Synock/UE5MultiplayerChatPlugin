// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "ChatBoxWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseFocusRequired);
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

	UFUNCTION(BlueprintCallable)
	void ReturnFocus();

	static constexpr TCHAR CommandTriggerCharacter = '/';
	static constexpr TCHAR ServerCommandTriggerCharacter = '#';

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatDataType(EGlobalMessageType Type, const FString& Message);

	UFUNCTION(BlueprintCallable)
	bool HasFocus() const;

	UFUNCTION(BlueprintCallable)
	void SetChatFocus();

	UPROPERTY(BlueprintAssignable)
	FReleaseFocusRequired OnReleaseFocusRequired;


};
