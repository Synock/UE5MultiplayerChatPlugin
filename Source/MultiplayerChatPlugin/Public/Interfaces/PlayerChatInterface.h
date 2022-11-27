// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "ChatPluginDefinitions.h"
#include "HUDChatInterface.h"
#include "UObject/Interface.h"
#include "PlayerChatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
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

	virtual void ProcessCommands(const FString& Command, const FString& Arguments);

	//this functions ensure it is run on the client, you'll have to redefine it at least by copying the following code
	//UFUNCTION(Client, unreliable, Category = "Chat|Display")
	/* virtual void Client_AddChatData_Implementation(EChatColor Color, EMessageCategories Category, const FString& Message)
	 * {
	 *		AddChatData(Color, Category, Message);
	 * }
	 */
	virtual void Client_AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message) = 0;

	//UFUNCTION(Client, unreliable, Category = "Chat|Display")
	/* virtual void Client_AddChatDataType_Implementation(EGlobalMessageType Type, const FString& Message)
	 * {
	 *		AddChatDataType(Type, Message);
	 * }
	 */
	virtual void Client_AddChatDataType(EGlobalMessageType Type, const FString& Message) = 0;

	//return the ingame chat name of a player
	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual FString GetChatName() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatDataType(EGlobalMessageType Type, const FString& Message);

	//this is the function that display the message of the speaker to itself.
	// You said "..." , You tell someone "..."
	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void SelfHearingSpeak(EGlobalMessageType Type, const FString& Message, const FString& Target = "");

	//Client callable version
	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void AreaSpeak(const FString& Message, float Range = 500.);
	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_AreaSpeak_Implementation(const FString& Message, float Range)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->AreaSpeak(this, Message, Range);
		}
	 **/
	virtual void Server_AreaSpeak(const FString& Message, float Range) = 0;

	//Client callable version
	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void ShoutSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_ShoutSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->ShoutSpeak(this, Message);
		}
	 **/
	virtual void Server_ShoutSpeak(const FString& Message) = 0;

	//Client callable version
	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void OOCSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_OOCSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->OOCSpeak(this, Message);
		}
	 **/
	virtual void Server_OOCSpeak(const FString& Message) = 0;

	void AuctionSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_AuctionSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->AuctionSpeak(this, Message);
		}
	 **/
	virtual void Server_AuctionSpeak(const FString& Message) = 0;


	UFUNCTION(BlueprintCallable, Category = "Chat")
	virtual bool IsInGroup() const;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	virtual bool IsInRaid() const;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	virtual bool IsInGuild() const;

	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void GroupSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_GroupSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->GroupSpeak(this, Message);
		}
	 **/
	virtual void Server_GroupSpeak(const FString& Message) = 0;

	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void TellSpeak(const FString& TargetName, const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_TellSpeak_Implementation(const FString& TargetName, const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->TellSpeak(this, TargetName, Message);
		}
	 **/
	virtual void Server_TellSpeak(const FString& TargetName, const FString& Message) = 0;


	void RaidSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_RaidSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->RaidSpeak(this, Message);
		}
	 **/
	virtual void Server_RaidSpeak(const FString& Message) = 0;


	void GuildSpeak(const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_GuildSpeak_Implementation(const FString& Message)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->GuildSpeak(this, Message);
		}
	 **/
	virtual void Server_GuildSpeak(const FString& Message) = 0;

};
