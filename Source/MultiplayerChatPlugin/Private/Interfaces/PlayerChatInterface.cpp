// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/PlayerChatInterface.h"


// Add default functionality here for any IPlayerChatInterface functions that are not pure virtual.

void IPlayerChatInterface::AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message)
{
	if (IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatData(Color, Category, Message);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::AddChatDataType(EGlobalMessageType Type, const FString& Message)
{
	if (IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatDataType(Type, Message);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::AreaSpeak(const FString& Message, float Range)
{
	SelfHearingSpeak(EGlobalMessageType::Say, Message);

	Server_AreaSpeak(Message,Range);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::ShoutSpeak(const FString& Message)
{
	SelfHearingSpeak(EGlobalMessageType::Shout, Message);

	Server_ShoutSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::OOCSpeak(const FString& Message)
{
	SelfHearingSpeak(EGlobalMessageType::OOC, Message);

	Server_OOCSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

bool IPlayerChatInterface::IsInGroup() const
{
	return false;
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::GroupSpeak(const FString& Message)
{
	if(!IsInGroup())
	{
		AddChatData(EChatColor::White,  EMessageCategories::Chat, "You are not in a group. Talking to yourself again???");
		return;
	}

	SelfHearingSpeak(EGlobalMessageType::Group, Message);

	Server_GroupSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::SelfHearingSpeak(EGlobalMessageType Type, const FString& Message)
{
	const EChatColor Color = EChatColor::White;

	const FString ReworkedMessage = "'" + Message + "'";
	switch (Type)
	{
	default:
	case EGlobalMessageType::Unknown: break;
	case EGlobalMessageType::Say:
		AddChatData(Color,  EMessageCategories::Chat, "You say, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Group:
		AddChatData(Color, EMessageCategories::Chat, "You tell your party, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Raid:
		AddChatData(Color, EMessageCategories::Chat, "You tell your party, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Shout:
		AddChatData(Color, EMessageCategories::Chat, "You shout, " + ReworkedMessage);
		break;
	case EGlobalMessageType::OOC:
		AddChatData(Color, EMessageCategories::Chat, "You say out of character, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Tell:
		AddChatData(Color, EMessageCategories::Chat, "You told, " + ReworkedMessage);
		break;
	}
}
