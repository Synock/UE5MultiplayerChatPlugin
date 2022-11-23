// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "HUDChatInterface.h"
#include "UObject/Interface.h"
#include "PlayerChatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerChatInterface : public UInterface
{
	GENERATED_BODY()
};

//this interface is to be attached to the player controllers
class MULTIPLAYERCHATPLUGIN_API IPlayerChatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual IHUDChatInterface* GetChatHUD() = 0;

	UFUNCTION(Client, unreliable, Category = "Chat|Display")
	virtual void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(Client, unreliable, Category = "Chat|Display")
	virtual void AddChatDataType(EGlobalMessageType Type, const FString& Message);

};
