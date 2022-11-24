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


	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatData(EChatColor Color, EMessageCategories Category, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat|Display")
	virtual void AddChatDataType(EGlobalMessageType Type, const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void AreaSpeak(const FString& Message, float Range = 500.);

	//this is the function that display the message of the speaker to itself.
	// You said "..." , You tell someone "..."
	UFUNCTION(BlueprintCallable, Category = "Chat|Talk")
	virtual void SelfHearingSpeak(EGlobalMessageType Type, const FString& Message);

	//UFUNCTION(Server, reliable, Category = "Chat|Talk")
	/*void AMainPlayerController::Server_AreaSpeak_Implementation(const FString& Message, float Range)
		{
			if (IGameModeChatInterface* GameModeChat = Cast<IGameModeChatInterface>(GetGameMode()))
				GameModeChat->AreaSpeak(this, Message, Range);
		}
	 **/
	virtual void Server_AreaSpeak(const FString& Message, float Range) = 0;


};
