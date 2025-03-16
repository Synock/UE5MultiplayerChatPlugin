// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "UObject/Interface.h"

#include "GameModeChatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UGameModeChatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERCHATPLUGIN_API IGameModeChatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual void AreaSpeak(APlayerController* PlayerController, const FString& Message, float Range = 500.);

	UFUNCTION()
	virtual void ShoutSpeak(APlayerController* PlayerController, const FString& String);

	UFUNCTION()
	virtual void OOCSpeak(APlayerController* PlayerController, const FString& String);

	UFUNCTION()
	virtual void AuctionSpeak(APlayerController* MainPlayerController, const FString& String);

	UFUNCTION()
	virtual void GroupSpeak(APlayerController* PlayerController, const FString& String);

	/**
	 * @brief return the list of APlayerController that belong to the same Group as PlayerController
	 * by default it will return nothing
	 * @param PlayerController the APlayerController to get the group info from
	 * @return the list of APlayerController that belong to the same Group as PlayerController
	 */
	UFUNCTION()
	virtual TArray<APlayerController*> GetGroupMembers(APlayerController* PlayerController);

	/**
	 * @brief return the list of APlayerController that belong to the same Raid as PlayerController
	 * by default it will return nothing
	 * @param PlayerController the APlayerController to get the raid info from
	 * @return the list of APlayerController that belong to the same Raid as PlayerController
	 */
	UFUNCTION()
	virtual TArray<APlayerController*> GetRaidMembers(APlayerController* PlayerController);

	/**
	 * @brief return the list of APlayerController that belong to the same Guild as PlayerController
	 * by default it will return nothing
	 * @param PlayerController the APlayerController to get the Guild info from
	 * @return the list of APlayerController that belong to the same Guild as PlayerController
	 */
	UFUNCTION()
	virtual TArray<APlayerController*> GetGuildMembers(APlayerController* PlayerController);

	UFUNCTION()
	virtual void TellSpeak(APlayerController* MainPlayerController, const FString& Target, const FString& Message);

	UFUNCTION()
	virtual APlayerController* FindPlayerWithName(const FString& Name);

	UFUNCTION()
	virtual void SendMessageToOtherServer(const FString& Name, const FString& SenderName, const FString& Message);

	UFUNCTION()
	virtual void RaidSpeak(APlayerController* MainPlayerController, const FString& String);

	UFUNCTION()
	virtual void GuildSpeak(APlayerController* MainPlayerController, const FString& String);

	UFUNCTION()
	virtual void AreaLog(EGlobalMessageType MessageType, const FString& Message, const FVector& Origin, float Range = 500.);

	UFUNCTION()
	virtual void AreaLogAroundPlayer(APlayerController* MainPlayerController, EGlobalMessageType MessageType, const FString& Message, float Range = 500.);

	UFUNCTION()
	virtual void AreaConstantLogAroundPlayer(APlayerController* MainPlayerController, EChatColor Color, EMessageCategories Category, int32 MessageId, float Range = 500.);

	UFUNCTION()
	virtual void AreaConstantLog(EChatColor Color, EMessageCategories Category, int32 MessageId, const FVector& Origin, float Range = 500.);

	//Send message to everyone
	UFUNCTION()
	virtual void ServerBroadcast(EGlobalMessageType MessageType, const FString& Message);

};
