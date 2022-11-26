// Copyright 2022 Maximilien (Synock) Guislain


#include "Interfaces/GameModeChatInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/PlayerChatInterface.h"
#include "Kismet/KismetSystemLibrary.h"


void IGameModeChatInterface::AreaSpeak(APlayerController* PlayerController, const FString& Message, float Range)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

	IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);
	if (!OriginalSpeaker)
		return;

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{{EObjectTypeQuery::ObjectTypeQuery3}};
	const TArray<AActor*> ActorsToIgnore{PlayerController->GetPawn()};
	TArray<AActor*> ActorArray;
	const bool OverlapStatus = UKismetSystemLibrary::SphereOverlapActors(
		GameMode->GetWorld(), PlayerController->GetPawn()->GetActorLocation(), Range,
		ObjectTypes, nullptr,
		ActorsToIgnore, ActorArray);

	UKismetSystemLibrary::DrawDebugSphere(GameMode->GetWorld(), PlayerController->GetPawn()->GetActorLocation(), Range,
	                                      8,
	                                      OverlapStatus ? FColor::Green : FColor::Red, 3.f, 0.f);

	const FString FinalMessage = OriginalSpeaker->GetChatName() + " says, '" + Message + "'";
	for (auto& Actor : ActorArray)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			AController* Controller = Character->GetController();
			IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(Controller);

			if (ChatInterface && Controller != PlayerController)
				ChatInterface->Client_AddChatData(EChatColor::White, EMessageCategories::Chat, FinalMessage);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::ShoutSpeak(APlayerController* PlayerController, const FString& String)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " shouts, " + FinalMessage;
	FConstPlayerControllerIterator EndIterator = GameMode->GetWorld()->GetPlayerControllerIterator();
	EndIterator.SetToEnd();

	for (FConstPlayerControllerIterator PCIterator = GameMode->GetWorld()->GetPlayerControllerIterator(); PCIterator !=
	     EndIterator; ++PCIterator)
	{
		IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(*PCIterator);

		if (ChatInterface && *PCIterator != PlayerController)
		{
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Shout, CompleteString);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::OOCSpeak(APlayerController* PlayerController, const FString& String)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " says out of character, " + FinalMessage;
	FConstPlayerControllerIterator EndIterator = GameMode->GetWorld()->GetPlayerControllerIterator();
	EndIterator.SetToEnd();

	for (FConstPlayerControllerIterator PCIterator = GameMode->GetWorld()->GetPlayerControllerIterator(); PCIterator !=
	     EndIterator; ++PCIterator)
	{
		IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(*PCIterator);

		if (ChatInterface && *PCIterator != PlayerController)
		{
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::OOC, CompleteString);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::GroupSpeak(APlayerController* PlayerController, const FString& String)
{
	const AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	const IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " tells the group, " + FinalMessage;

	for (const auto& GroupMember : GetGroupMembers(PlayerController))
	{
		if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(GroupMember); ChatInterface && GroupMember
			!= PlayerController)
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Group, CompleteString);
	}
}

//----------------------------------------------------------------------------------------------------------------------

TArray<APlayerController*> IGameModeChatInterface::GetGroupMembers(APlayerController* MainPlayerController)
{
	return {};
}
