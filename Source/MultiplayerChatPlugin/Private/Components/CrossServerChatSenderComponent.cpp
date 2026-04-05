// Copyright 2022 Maximilien (Synock) Guislain

#include "Components/CrossServerChatSenderComponent.h"
#include "Components/CrossServerChatBaseComponent.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonWriter.h"

//----------------------------------------------------------------------------------------------------------------------

UCrossServerChatSenderComponent::UCrossServerChatSenderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatSenderComponent::SendTell(const FString& SenderName, const FString& TargetName,
                                               const FString& Message)
{
	if (GatewayBaseURL.IsEmpty())
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatSenderComponent: GatewayBaseURL is empty – cannot send cross-server tell"));
		return;
	}

	const FString URL = GatewayBaseURL + TellEndpoint;

	// Build JSON payload: { "sender": "...", "target": "...", "message": "..." }
	FString JsonPayload;
	{
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonPayload);
		Writer->WriteObjectStart();
		Writer->WriteValue(TEXT("sender"), SenderName);
		Writer->WriteValue(TEXT("target"), TargetName);
		Writer->WriteValue(TEXT("message"), Message);
		Writer->WriteObjectEnd();
		Writer->Close();
	}

	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	if (!OutboundAuthToken.IsEmpty())
	{
		Request->SetHeader(OutboundAuthTokenHeader, OutboundAuthToken);
	}

	Request->SetContentAsString(JsonPayload);

	Request->OnProcessRequestComplete().BindUObject(
		this, &UCrossServerChatSenderComponent::OnSendTellResponse, SenderName, TargetName);

	Request->ProcessRequest();

	UE_LOG(LogCrossServerChat, Verbose,
	       TEXT("CrossServerChatSenderComponent: POST %s  from='%s' to='%s'"), *URL, *SenderName, *TargetName);
}

//----------------------------------------------------------------------------------------------------------------------

void UCrossServerChatSenderComponent::OnSendTellResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                          bool bWasSuccessful,
                                                          FString SenderName, FString TargetName)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatSenderComponent: Tell from '%s' to '%s' failed – no response from gateway (%s)"),
		       *SenderName, *TargetName, *Request->GetURL());
		return;
	}

	const int32 StatusCode = Response->GetResponseCode();
	if (StatusCode >= 400)
	{
		UE_LOG(LogCrossServerChat, Warning,
		       TEXT("CrossServerChatSenderComponent: Tell from '%s' to '%s' rejected – HTTP %d: %s"),
		       *SenderName, *TargetName, StatusCode, *Response->GetContentAsString());
		return;
	}

	UE_LOG(LogCrossServerChat, Verbose,
	       TEXT("CrossServerChatSenderComponent: Tell from '%s' to '%s' accepted (HTTP %d)"),
	       *SenderName, *TargetName, StatusCode);
}

