// Copyright 2022 Maximilien (Synock) Guislain

#include "Components/CrossServerChatBaseComponent.h"

#include "HttpServerModule.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "HttpResultCallback.h"
#include "IHttpRouter.h"
#include "HttpPath.h"
#include "Interfaces/GameModeChatInterface.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY(LogCrossServerChat);

//----------------------------------------------------------------------------------------------------------------------

UCrossServerChatBaseComponent::UCrossServerChatBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only the server starts the listener.
	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	if (ChatListenPort <= 0)
	{
		const UWorld* World = GetWorld();
		const int32 GamePort = World ? World->URL.Port : 7777;
		ResolvedPort = GamePort + 10000;
	}
	else
	{
		ResolvedPort = ChatListenPort;
	}

	StartListener();
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopListener();
	Super::EndPlay(EndPlayReason);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::StartListener()
{
	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	HttpRouter = HttpServerModule.GetHttpRouter(ResolvedPort);

	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogCrossServerChat, Error,
		       TEXT("CrossServerChatBaseComponent: Failed to acquire HTTP router on port %d"), ResolvedPort);
		return;
	}

	RouteHandle = HttpRouter->BindRoute(
		FHttpPath(TEXT("/chat/tell")),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateUObject(this, &UCrossServerChatBaseComponent::HandleInboundTell));

	HttpServerModule.StartAllListeners();

	UE_LOG(LogCrossServerChat, Log,
	       TEXT("CrossServerChatBaseComponent: Listening for cross-server tells on port %d"), ResolvedPort);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::StopListener()
{
	if (HttpRouter.IsValid() && RouteHandle.IsValid())
	{
		HttpRouter->UnbindRoute(RouteHandle);
		RouteHandle.Reset();
	}
	HttpRouter.Reset();
}

//----------------------------------------------------------------------------------------------------------------------

bool UCrossServerChatBaseComponent::HandleInboundTell(const FHttpServerRequest& Request,
                                                       const FHttpResultCallback& OnComplete)
{
	// --- Token validation ---
	if (!AuthToken.IsEmpty())
	{
		// Headers is TMap<FString, TArray<FString>>; header names are lowercased by the HTTP server.
		const TArray<FString>* TokenValues = Request.Headers.Find(AuthTokenHeader);
		if (!TokenValues || TokenValues->IsEmpty() || (*TokenValues)[0] != AuthToken)
		{
			UE_LOG(LogCrossServerChat, Warning,
			       TEXT("CrossServerChatBaseComponent: Rejected tell - bad or missing auth token"));
			ReplyError(OnComplete, 403, TEXT("Forbidden"));
			return true;
		}
	}

	// --- Parse JSON body ---
	// Body is TArray<uint8>; convert to FString via UTF-8 decode.
	const FString BodyStr = FString(
		UTF8_TO_TCHAR(reinterpret_cast<const char*>(Request.Body.GetData())));

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(BodyStr);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatBaseComponent: Rejected tell - invalid JSON body"));
		ReplyError(OnComplete, 400, TEXT("Invalid JSON"));
		return true;
	}

	FString Sender, Target, Message;
	if (!JsonObject->TryGetStringField(TEXT("sender"), Sender) ||
		!JsonObject->TryGetStringField(TEXT("target"), Target) ||
		!JsonObject->TryGetStringField(TEXT("message"), Message))
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatBaseComponent: Rejected tell - missing required fields"));
		ReplyError(OnComplete, 400, TEXT("Missing required fields: sender, target, message"));
		return true;
	}

	UE_LOG(LogCrossServerChat, Log,
	       TEXT("CrossServerChatBaseComponent: Inbound tell from '%s' to '%s'"), *Sender, *Target);

	// Deliver on the game thread via a captured weak-pointer to stay crash-safe.
	TWeakObjectPtr<UCrossServerChatBaseComponent> WeakSelf(this);
	AsyncTask(ENamedThreads::GameThread, [WeakSelf, Sender, Target, Message]()
	{
		if (UCrossServerChatBaseComponent* Self = WeakSelf.Get())
			Self->OnTellDelivered(Sender, Target, Message);
	});

	ReplyOK(OnComplete);
	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::OnTellDelivered(const FString& Sender, const FString& Target,
                                                    const FString& Message)
{
	AActor* Owner = GetOwner();
	if (!Owner)
		return;

	IGameModeChatInterface* ChatInterface = Cast<IGameModeChatInterface>(Owner);
	if (!ChatInterface)
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatBaseComponent: Owner does not implement IGameModeChatInterface"));
		return;
	}

	ChatInterface->ReceiveCrossServerTell(Sender, Target, Message);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::ReplyError(const FHttpResultCallback& OnComplete,
                                               int32 Code, const FString& Message)
{
	const FString Body = FString::Printf(TEXT("{\"error\":\"%s\"}"), *Message);
	TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(Body, TEXT("application/json"));
	Response->Code = static_cast<EHttpServerResponseCodes>(Code);
	OnComplete(MoveTemp(Response));
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatBaseComponent::ReplyOK(const FHttpResultCallback& OnComplete)
{
	TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(
		TEXT("{\"status\":\"ok\"}"), TEXT("application/json"));
	Response->Code = EHttpServerResponseCodes::Ok;
	OnComplete(MoveTemp(Response));
}


