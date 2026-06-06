// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "ChatBoxWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReleaseFocusRequired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatSelectionChanged, const FString&, SelectedOption);
/**
 *
 */
UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* TextBox = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* ChatSelection = nullptr;

	UFUNCTION(BlueprintCallable)
	void HandleTextEnter();

	UFUNCTION(BlueprintCallable)
	void ReturnFocus();

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleChatSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	static constexpr TCHAR CommandTriggerCharacter = '/';
	static constexpr TCHAR ServerCommandTriggerCharacter = '#';

public:
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SubmitCurrentText();

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintImplementableEvent)
	void AddChatDataType(EGlobalMessageType Type, const FString& Message);

	UFUNCTION(BlueprintCallable)
	bool HasFocus() const;

	UFUNCTION(BlueprintCallable)
	void SetChatFocus();

	UFUNCTION(BlueprintPure, Category = "Chat")
	FString GetSelectedChatOption() const;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SetSelectedChatOption(const FString& Option);

	UPROPERTY(BlueprintAssignable)
	FReleaseFocusRequired OnReleaseFocusRequired;

	UPROPERTY(BlueprintAssignable)
	FChatSelectionChanged OnChatSelectionChanged;


};
