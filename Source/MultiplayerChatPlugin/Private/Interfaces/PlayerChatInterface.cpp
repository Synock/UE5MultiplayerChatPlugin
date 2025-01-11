// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/PlayerChatInterface.h"


// Add default functionality here for any IPlayerChatInterface functions that are not pure virtual.

void IPlayerChatInterface::ProcessCommands(const FString& Command, const FString& Arguments)
{
	//Do something about the commands
}
//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::ProcessServerCommands(const FString& Command, const FString& Arguments)
{
	//Do something about the commands that have server impact
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message)
{
	if (IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatData(Color, Category, Message);
	}
}

void IPlayerChatInterface::AddChatConstantData(EChatColor Color, EMessageCategories Category, int32 MessageId)
{
	if (IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatConstantData(Color, Category, MessageId);
	}
}

void IPlayerChatInterface::AddChatConstantWithVariablesData(EChatColor Color, EMessageCategories Category,
	int32 MessageId, const TArray<FString>& Variables)
{
	if (IHUDChatInterface* ChatHUD = GetChatHUD())
	{
		ChatHUD->AddChatConstantWithVariablesData(Color, Category, MessageId, Variables);
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
	Server_AreaSpeak(Message, Range);
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

void IPlayerChatInterface::AuctionSpeak(const FString& Message)
{
	SelfHearingSpeak(EGlobalMessageType::Auction, Message);
	Server_AuctionSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

bool IPlayerChatInterface::IsInGroup() const
{
	return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool IPlayerChatInterface::IsInRaid() const
{
	return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool IPlayerChatInterface::IsInGuild() const
{
	return false;
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::GroupSpeak(const FString& Message)
{
	if (!IsInGroup())
	{
		AddChatData(EChatColor::White, EMessageCategories::Chat,
		            "You are not in a group. Talking to yourself again???");
		return;
	}

	SelfHearingSpeak(EGlobalMessageType::Group, Message);

	Server_GroupSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::TellSpeak(const FString& TargetName, const FString& Message)
{
	if (TargetName == GetChatName())
	{
		AddChatData(EChatColor::White, EMessageCategories::Chat, "Talking to yourself again?");
		return;
	}

	//this is displayed without any check, is it Legit?
	SelfHearingSpeak(EGlobalMessageType::Tell, Message, TargetName);
	Server_TellSpeak(TargetName, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::RaidSpeak(const FString& Message)
{
	if (!IsInGroup() || !IsInRaid())
	{
		AddChatData(EChatColor::White, EMessageCategories::Chat,
		            "You are not in a raid party.");
		return;
	}

	SelfHearingSpeak(EGlobalMessageType::Raid, Message);
	Server_RaidSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::GuildSpeak(const FString& Message)
{
	if (!IsInGuild())
	{
		AddChatData(EChatColor::White, EMessageCategories::Chat,
		            "You are not a member of any guild.");
		return;
	}

	SelfHearingSpeak(EGlobalMessageType::Guild, Message);
	Server_GuildSpeak(Message);
}

//----------------------------------------------------------------------------------------------------------------------

void IPlayerChatInterface::SelfHearingSpeak(EGlobalMessageType Type, const FString& Message, const FString& Target)
{
	const EChatColor Color = EChatColor::White;

	const FString ReworkedMessage = "'" + Message + "'";
	switch (Type)
	{
	default:
	case EGlobalMessageType::Unknown: break;
	case EGlobalMessageType::Say:
		AddChatData(Color, EMessageCategories::Chat, "You say, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Group:
		AddChatData(Color, EMessageCategories::Chat, "You tell your party, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Guild:
		AddChatData(Color, EMessageCategories::Chat, "You tell the guild, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Auction:
		AddChatData(Color, EMessageCategories::Chat, "You auction, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Raid:
		AddChatData(Color, EMessageCategories::Chat, "You tell your raid party, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Shout:
		AddChatData(Color, EMessageCategories::Chat, "You shout, " + ReworkedMessage);
		break;
	case EGlobalMessageType::OOC:
		AddChatData(Color, EMessageCategories::Chat, "You say out of character, " + ReworkedMessage);
		break;
	case EGlobalMessageType::Tell:
		AddChatData(Color, EMessageCategories::Chat, "You told " + Target + ", " + ReworkedMessage);
		break;
	}
}
