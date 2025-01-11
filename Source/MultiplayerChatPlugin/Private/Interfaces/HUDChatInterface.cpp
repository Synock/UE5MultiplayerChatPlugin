// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/HUDChatInterface.h"

#include "ConstantTextManager.h"


void IHUDChatInterface::AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message)
{
	if (UChatBoxWidget* ChatBoxWidget = GetChatBoxWidget())
	{
		ChatBoxWidget->AddChatData(Color, Category, Message);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IHUDChatInterface::AddChatDataType(EGlobalMessageType Type, const FString& Message)
{
	if (UChatBoxWidget* ChatBoxWidget = GetChatBoxWidget())
	{
		ChatBoxWidget->AddChatDataType(Type, Message);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IHUDChatInterface::AddChatConstantData(EChatColor Color, EMessageCategories Category, int32 MessageIndex)
{
	AddChatData(Color, Category, GetConstantTextFromId(MessageIndex).ToString());
}

//----------------------------------------------------------------------------------------------------------------------

FText IHUDChatInterface::GetConstantTextFromId(int32 StringID)
{
	return ConstantTextManager::GetTextByID(StringID);
}
//----------------------------------------------------------------------------------------------------------------------

void IHUDChatInterface::AddChatConstantWithVariablesData(EChatColor Color, EMessageCategories Category, int32 MessageId,
	const TArray<FString> Variables)
{
	FText Message = GetConstantTextFromId(MessageId);
	FString MessageString = Message.ToString();


	for (int32 i = 0; i < Variables.Num(); i++)
	{
		FString Target = "%" + FString::FromInt(i+1);
		MessageString.ReplaceInline(*Target, *Variables[i]);
	}
	AddChatData(Color, Category, MessageString);
}
