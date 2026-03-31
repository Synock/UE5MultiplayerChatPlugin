// Copyright 2022 Maximilien (Synock) Guislain

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HttpRouteHandle.h"
#include "IHttpRouter.h"
#include "CrossServerChatBaseComponent.generated.h"

struct FHttpServerRequest;
using FHttpResultCallback = TFunction<void(TUniquePtr<FHttpServerResponse>&&)>;

DECLARE_LOG_CATEGORY_EXTERN(LogCrossServerChat, Log, All);

/**
 * Base component for receiving cross-server direct messages (tells).
 *
 * Owns an FHttpServerModule listener that accepts POST /chat/tell from an
 * external message broker.  Subclass and override OnTellDelivered() to add
 * game-specific delivery behaviour; the default implementation forwards the
 * message through IGameModeChatInterface::ReceiveCrossServerTell on the owner.
 *
 * Configuration (set via Blueprint on the owning actor):
 *  - ChatListenPort  : explicit port, or 0 to auto-derive as (game port + 10000)
 *  - AuthToken       : shared secret validated in the incoming request header
 *  - AuthTokenHeader : name of the HTTP header carrying the auth token
 */
UCLASS(ClassGroup = (Chat), Blueprintable, meta = (BlueprintSpawnableComponent))
class MULTIPLAYERCHATPLUGIN_API UCrossServerChatBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrossServerChatBaseComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//------------------------------------------------------------------------------------------------------------------
	// Configuration (designer-tunable)
	//------------------------------------------------------------------------------------------------------------------

	/**
	 * HTTP port this server listens on for inbound cross-server tells.
	 * Set to 0 to auto-derive as (game port + 10000).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	int32 ChatListenPort = 0;

	/**
	 * Shared secret expected in the incoming auth token header.
	 * Must match the token registered with the message broker during server registration.
	 * Leave empty to disable token validation (development only).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString AuthToken;

	/**
	 * Name of the HTTP request header that carries the auth token.
	 * Must match whatever the broker sends. Stored in lowercase (HTTP convention).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat|CrossServer")
	FString AuthTokenHeader = TEXT("x-chat-authtoken");

	//------------------------------------------------------------------------------------------------------------------
	// Public accessors
	//------------------------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Chat|CrossServer")
	int32 GetResolvedListenPort() const { return ResolvedPort; }

	const FString& GetAuthToken() const { return AuthToken; }
	const FString& GetAuthTokenHeader() const { return AuthTokenHeader; }

protected:
	/**
	 * Called when a validated cross-server tell arrives on this server.
	 * Default: casts the owning actor to IGameModeChatInterface and calls ReceiveCrossServerTell.
	 * Override for game-specific logging, filtering, or alternate delivery.
	 */
	virtual void OnTellDelivered(const FString& Sender, const FString& Target, const FString& Message);

private:
	void StartListener();
	void StopListener();

	bool HandleInboundTell(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	static void ReplyError(const FHttpResultCallback& OnComplete, int32 Code, const FString& Message);
	static void ReplyOK(const FHttpResultCallback& OnComplete);

	TSharedPtr<IHttpRouter> HttpRouter;
	FHttpRouteHandle        RouteHandle;
	int32                   ResolvedPort = 0;
};

