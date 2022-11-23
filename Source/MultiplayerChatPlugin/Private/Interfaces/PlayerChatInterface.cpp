// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/PlayerChatInterface.h"


// Add default functionality here for any IPlayerChatInterface functions that are not pure virtual.

void IPlayerChatInterface::AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message)
{
	if(IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatData(Color, Category, Message);
	}
}

void IPlayerChatInterface::AddChatDataType(EGlobalMessageType Type, const FString& Message)
{
	if(IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatDataType(Type, Message);
	}
}
