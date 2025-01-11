// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "UI/ChatBoxWidget.h"
#include "UObject/Interface.h"
#include "HUDChatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UHUDChatInterface : public UInterface
{
	GENERATED_BODY()
};

//interface to the main HUD/UI
//Typically that's where you would override functions if you want multiple chat windows
class MULTIPLAYERCHATPLUGIN_API IHUDChatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual UChatBoxWidget* GetChatBoxWidget() = 0;

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatDataType(EGlobalMessageType Type, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatConstantData(EChatColor Color, EMessageCategories Category, int32 MessageIndex);

	//Get a constant Text string from somewhere
	UFUNCTION()
	virtual FText GetConstantTextFromId(int32 StringID);

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatConstantWithVariablesData(EChatColor Color, EMessageCategories Category, int32 MessageIndex, const TArray<FString> Array);
};
