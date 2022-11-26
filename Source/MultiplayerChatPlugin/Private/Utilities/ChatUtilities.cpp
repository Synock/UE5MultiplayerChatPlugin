// Copyright 2022 Maximilien (Synock) Guislain


#include "Utilities/ChatUtilities.h"

#include "GameFramework/Character.h"
#include "Interfaces/PlayerChatInterface.h"


FMessageMetaData UChatUtilities::GetMessageMetaFromType(EGlobalMessageType Type)
{
	switch (Type)
	{
	default:
	case EGlobalMessageType::Unknown:return {};
	case EGlobalMessageType::Say: return {EChatColor::White, EMessageCategories::Chat, EMessageReach::Area};
	case EGlobalMessageType::Group:return {EChatColor::LightBlue, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Raid:return {EChatColor::LightPurple, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Shout:return {EChatColor::Red, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::OOC:return {EChatColor::DarkGreen, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Tell:return {EChatColor::Purple, EMessageCategories::Chat, EMessageReach::External};
	case EGlobalMessageType::CombatDamageGiven:return {EChatColor::White, EMessageCategories::Combat, EMessageReach::Area};
	case EGlobalMessageType::CombatDamageReceived:return {EChatColor::Red, EMessageCategories::Combat, EMessageReach::Local};
	case EGlobalMessageType::CombatDamageOther:return {EChatColor::White, EMessageCategories::Combat, EMessageReach::Area};
	case EGlobalMessageType::SpellCast:return {EChatColor::Blue, EMessageCategories::Combat, EMessageReach::Area};
	case EGlobalMessageType::DotEffect:return {EChatColor::Yellow, EMessageCategories::Combat, EMessageReach::Local};
	case EGlobalMessageType::Consider:return {EChatColor::White, EMessageCategories::World, EMessageReach::Local};
	case EGlobalMessageType::ServerAnnouncement:return {EChatColor::Yellow, EMessageCategories::Server, EMessageReach::Server};
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatData(APlayerController* PC, EChatColor Color, EMessageCategories Category,
                                 const FString& Message)
{
	if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(PC))
		ChatInterface->Client_AddChatData(Color, Category, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatDataFromAvatar(ACharacter* Character, EChatColor Color, EMessageCategories Category,
                                           const FString& Message)
{
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		AddChatData(PC, Color, Category, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatDebugData(APlayerController* PC, EMessageCategories Category,
                                      const FString& Message)
{
	AddChatData(PC, EChatColor::Yellow, Category, "Debug - " + Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatDataType(APlayerController* PC, EGlobalMessageType Type, const FString& Message)
{
	FMessageMetaData Data = GetMessageMetaFromType(Type);
	AddChatData(PC, Data.Color, Data.Category, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatDataTypeFromAvatar(ACharacter* Character, EGlobalMessageType Type, const FString& Message)
{
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		AddChatDataType(PC, Type, Message);
}

