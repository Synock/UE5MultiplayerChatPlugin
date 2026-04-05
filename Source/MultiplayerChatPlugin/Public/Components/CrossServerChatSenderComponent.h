// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "CrossServerChatSenderComponent.generated.h"

/**
 * Outbound component for sending cross-server direct messages (tells).
 *
 * POSTs JSON payloads to a gateway (e.g. NeverquestGate) that routes messages
 * to the target player's zone server.  Pair with UCrossServerChatBaseComponent
 * on the same GameMode to get full bidirectional cross-server tell support.
 *
 * Configuration (set via Blueprint on the owning actor):
 *  - GatewayBaseURL : Base URL of the message broker (e.g. "http://gateway:8080")
 *  - TellEndpoint   : Path appended to the base URL (default "/chat/tell")
 *  - OutboundAuthToken       : Shared secret sent in the outbound request header
 *  - OutboundAuthTokenHeader : Name of the HTTP header carrying the auth token
 */
UCLASS(ClassGroup = (Chat), Blueprintable, meta = (BlueprintSpawnableComponent))
class MULTIPLAYERCHATPLUGIN_API UCrossServerChatSenderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrossServerChatSenderComponent();

	//------------------------------------------------------------------------------------------------------------------
	// Configuration (designer-tunable)
	//------------------------------------------------------------------------------------------------------------------

	/**
	 * Base URL of the gateway/message broker that routes cross-server tells.
	 * Example: "http://gateway.neverquest.local:8080"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString GatewayBaseURL;

	/** Path appended to GatewayBaseURL for the tell endpoint. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString TellEndpoint = TEXT("/chat/tell");

	/** Shared secret sent in the outbound auth header. Leave empty to skip. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString OutboundAuthToken;

	/** HTTP header name for the outbound auth token. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString OutboundAuthTokenHeader = TEXT("x-chat-authtoken");

	//------------------------------------------------------------------------------------------------------------------
	// Public API
	//------------------------------------------------------------------------------------------------------------------

	/**
	 * Send a cross-server tell via the gateway.
	 * Fire-and-forget with failure logging; does not block the game thread.
	 *
	 * @param SenderName  Display name of the sending player.
	 * @param TargetName  Display name of the target player.
	 * @param Message     The message body.
	 */
	UFUNCTION(BlueprintCallable, Category = "Chat|CrossServer")
	void SendTell(const FString& SenderName, const FString& TargetName, const FString& Message);

private:
	void OnSendTellResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful,
	                        FString SenderName, FString TargetName);
};


