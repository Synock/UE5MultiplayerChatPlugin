// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.generated.h"

//This is the chat color scheme, this is probably straightforward
UENUM(BlueprintType)
enum struct EChatColor : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	Red UMETA(DisplayName = "Red"),
	White UMETA(DisplayName = "White"),
	Blue UMETA(DisplayName = "Blue"),
	LightBlue UMETA(DisplayName = "LightBlue"),
	LightPurple UMETA(DisplayName = "LightPurple"),
	Purple UMETA(DisplayName = "Purple"),
	Yellow UMETA(DisplayName = "Yellow"),
	Green UMETA(DisplayName = "Green"),
	DarkGreen UMETA(DisplayName = "DarkGreen"),
	Gray UMETA(DisplayName = "Gray"),
};

//This is the chat categories
UENUM(BlueprintType)
enum struct EMessageCategories : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	Chat UMETA(DisplayName = "Chat"), //Player (or npc) to player chat data
	Combat UMETA(DisplayName = "Combat"),//Combat related logging
	Misc UMETA(DisplayName = "Misc"),//everything else that need logging
	World UMETA(DisplayName = "World"),//World related logging
	Server UMETA(DisplayName = "Server")//Server related logging
};

//This how a message is handled
UENUM(BlueprintType)
enum struct EMessageReach : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),
	Local UMETA(DisplayName = "Local"),//Local is client only
	Area UMETA(DisplayName = "Area"),//Area is server limited in a radius
	Server UMETA(DisplayName = "Server"),//Server-wide
	External UMETA(DisplayName = "External")//this is external message handling, think server to server
};

USTRUCT(BlueprintType)
struct FMessageMetaData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	EChatColor Color = EChatColor::Unknown;
	UPROPERTY(BlueprintReadWrite)
	EMessageCategories Category = EMessageCategories::Unknown;
	UPROPERTY(BlueprintReadWrite)
	EMessageReach Reach = EMessageReach::Unknown;
};


//This is a shortcut combination of color, reach and category
UENUM(BlueprintType)
enum struct EGlobalMessageType : uint8
{
	Unknown UMETA(DisplayName = "Unknown"),

	// Below is Chat
	Say UMETA(DisplayName = "Say"),// Local Messaging, white
	Group UMETA(DisplayName = "Group"),// Group messaging,
	Raid UMETA(DisplayName = "Raid"), //Raid messaging
	Shout UMETA(DisplayName = "Shout"),// server wide messaging
	OOC UMETA(DisplayName = "OOC"),// server wide messaging Out Of Character
	Tell UMETA(DisplayName = "Tell"),//private messaging
	Guild UMETA(DisplayName = "Guild"),//guild wide messaging
	Auction UMETA(DisplayName = "Auction"),//server wide auction messaging

	// Below is Combat
	CombatDamageGiven UMETA(DisplayName = "CombatDamageGiven"),// when player do damage
	CombatDamageReceived UMETA(DisplayName = "CombatDamageReceived"), // when player receive damage
	CombatDamageOther UMETA(DisplayName = "CombatDamageOther"), // when something damage something else
	SpellCast UMETA(DisplayName = "SpellCast"), // when a spell is casted
	DotEffect UMETA(DisplayName = "DotEffect"), // when a recurrent damage is applied

	// Below is World
	Consider UMETA(DisplayName = "Consider"),

	// Below is Server
	ServerAnnouncement UMETA(DisplayName = "ServerAnnouncement")
};


