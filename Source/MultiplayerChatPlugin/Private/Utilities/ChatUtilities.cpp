// Copyright 2022 Maximilien (Synock) Guislain


#include "Utilities/ChatUtilities.h"

#include "GameFramework/Character.h"
#include "Interfaces/GameModeChatInterface.h"
#include "Interfaces/PlayerChatInterface.h"


FMessageMetaData UChatUtilities::GetMessageMetaFromType(EGlobalMessageType Type)
{
	switch (Type)
	{
	default:
	case EGlobalMessageType::Unknown: return {};
	case EGlobalMessageType::Say: return {EChatColor::White, EMessageCategories::Chat, EMessageReach::Area};
	case EGlobalMessageType::Group: return {EChatColor::LightBlue, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Raid: return {EChatColor::LightPurple, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Shout: return {EChatColor::Red, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::OOC: return {EChatColor::DarkGreen, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::Tell: return {EChatColor::Purple, EMessageCategories::Chat, EMessageReach::External};
	case EGlobalMessageType::Guild: return {EChatColor::Green, EMessageCategories::Chat, EMessageReach::External};
	case EGlobalMessageType::Auction: return {EChatColor::Green, EMessageCategories::Chat, EMessageReach::Server};
	case EGlobalMessageType::CombatDamageGiven: return {
			EChatColor::White, EMessageCategories::Combat, EMessageReach::Area
		};
	case EGlobalMessageType::CombatDamageReceived: return {
			EChatColor::Red, EMessageCategories::Combat, EMessageReach::Local
		};
	case EGlobalMessageType::CombatDamageOther: return {
			EChatColor::White, EMessageCategories::Combat, EMessageReach::Area
		};
	case EGlobalMessageType::SpellCast: return {EChatColor::Blue, EMessageCategories::Combat, EMessageReach::Area};
	case EGlobalMessageType::DotEffect: return {EChatColor::Yellow, EMessageCategories::Combat, EMessageReach::Local};
	case EGlobalMessageType::Consider: return {EChatColor::White, EMessageCategories::World, EMessageReach::Local};
	case EGlobalMessageType::ServerAnnouncement: return {
			EChatColor::Yellow, EMessageCategories::Server, EMessageReach::Server
		};
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

void UChatUtilities::AddChatConstantData(APlayerController* PC, EChatColor Color, EMessageCategories Category,
	int32 MessageId)
{
	if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(PC))
		ChatInterface->Client_AddChatConstantData(Color, Category, MessageId);
}

void UChatUtilities::AddChatConstantWithVariablesData(APlayerController* PC, EChatColor Color,
	EMessageCategories Category, int32 MessageId, const TArray<FString>& Variables)
{
	if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(PC))
		ChatInterface->Client_AddChatConstantWithVariablesData(Color, Category, MessageId, Variables);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatDataFromAvatar(ACharacter* Character, EChatColor Color, EMessageCategories Category,
                                           const FString& Message)
{
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		AddChatData(PC, Color, Category, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddChatConstantDataFromAvatar(ACharacter* Character, EChatColor Color, EMessageCategories Category,
	int32 MessageId)
{
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
		AddChatConstantData(PC, Color, Category, MessageId);
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

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddLogInArea(AGameModeBase* GameMode, EGlobalMessageType Type, const FString& Message,
                                  const FVector& Origin, float Range)
{
	if (IGameModeChatInterface* ChatInterface = Cast<IGameModeChatInterface>(GameMode))
		ChatInterface->AreaLog(Type, Message, Origin, Range);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddLogAroundPlayer(APlayerController* PC, EGlobalMessageType Type, const FString& Message,
                                        float Range)
{
	if (IGameModeChatInterface* ChatInterface = Cast<IGameModeChatInterface>(PC->GetWorld()->GetAuthGameMode()))
		ChatInterface->AreaLogAroundPlayer(PC, Type, Message, Range);
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddLogAroundAvatar(AActor* Avatar, EGlobalMessageType Type, const FString& Message,
                                        float Range)
{
	if (IGameModeChatInterface* ChatInterface = Cast<IGameModeChatInterface>(Avatar->GetWorld()->GetAuthGameMode()))
	{
		ACharacter* Character = Cast<ACharacter>(Avatar);
		if(Character)
		{
			///It's a player avatar
			if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
				{
					ChatInterface->AreaLogAroundPlayer(PC, Type, Message, Range);
					return;
				}
		}
		ChatInterface->AreaLog(Type, Message, Character->GetActorLocation(), Range);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UChatUtilities::AddConstantLogAroundAvatar(AActor* Avatar, EChatColor Color, EMessageCategories Category, int32 MessageId,
	float Range)
{
	if (IGameModeChatInterface* ChatInterface = Cast<IGameModeChatInterface>(Avatar->GetWorld()->GetAuthGameMode()))
	{
		ACharacter* Character = Cast<ACharacter>(Avatar);
		if(Character)
		{
			///It's a player avatar
			if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
			{
				ChatInterface->AreaConstantLogAroundPlayer(PC, Color, Category, MessageId, Range);
				return;
			}
		}
		ChatInterface->AreaConstantLog(Color, Category,MessageId, Character->GetActorLocation(), Range);
	}
}
