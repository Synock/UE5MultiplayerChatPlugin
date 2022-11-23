// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/HUDChatInterface.h"


void IHUDChatInterface::AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message)
{
	if (UChatBoxWidget* ChatBoxWidget = GetChatBoxWidget())
	{
		ChatBoxWidget->AddChatData(Color, Category, Message);
	}
}

void IHUDChatInterface::AddChatDataType(EGlobalMessageType Type, const FString& Message)
{
	if (UChatBoxWidget* ChatBoxWidget = GetChatBoxWidget())
	{
		ChatBoxWidget->AddChatDataType(Type, Message);
	}
}
