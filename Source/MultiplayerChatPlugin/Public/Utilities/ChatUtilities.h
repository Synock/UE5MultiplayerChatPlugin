// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "ChatUtilities.generated.h"

UCLASS()
class MULTIPLAYERCHATPLUGIN_API UChatUtilities: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Chat")
	static FMessageMetaData GetMessageMetaFromType(EGlobalMessageType Type);

	//Try to add chat data from a player Controller
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatData(APlayerController* PC, EChatColor Color, EMessageCategories Category, const FString & Message);

	//Try to add chat data from a player Controller
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatConstantData(APlayerController* PC, EChatColor Color, EMessageCategories Category, int32 MessageId);

	//Try to add chat data from a player Controller
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatConstantWithVariablesData(APlayerController* PC, EChatColor Color, EMessageCategories Category, int32 MessageId, const TArray<FString>& Variables);

	//helper function that allows to display chat from a character
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatDataFromAvatar(ACharacter* Character, EChatColor Color, EMessageCategories Category, const FString & Message);

	//helper function that allows to display chat from a character
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatConstantDataFromAvatar(ACharacter* Character, EChatColor Color, EMessageCategories Category, int32 MessageId);

	//helper function that display a message in yellow with DEBUG - in front of it
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatDebugData(APlayerController* PC, EMessageCategories Category, const FString & Message);

	//Try to add chat data from a player Controller
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatDataType(APlayerController* PC, EGlobalMessageType Type, const FString & Message);

	//helper function that allows to display chat from a character
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddChatDataTypeFromAvatar(ACharacter* Character, EGlobalMessageType Type, const FString & Message);

	//helper function that allows to log stuff in an area
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddLogInArea(AGameModeBase* GameMode, EGlobalMessageType Type, const FString & Message, const FVector& Origin, float Range = 100.);

	//helper function that allows to log stuff in an area around a player, excluding him
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddLogAroundPlayer(APlayerController* PC, EGlobalMessageType Type, const FString & Message, float Range = 100.);

	//helper function that allows to log stuff in an area around a character, if it's a player excluding him
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddLogAroundAvatar(AActor* Character, EGlobalMessageType Type, const FString& Message, float Range = 100.);

	//helper function that allows to log stuff in an area around a character, if it's a player excluding him
	UFUNCTION(BlueprintCallable, Category="Chat")
	static void AddConstantLogAroundAvatar(AActor* Character, EChatColor Color, EMessageCategories Category, int32 MessageId, float Range = 100.);

};
