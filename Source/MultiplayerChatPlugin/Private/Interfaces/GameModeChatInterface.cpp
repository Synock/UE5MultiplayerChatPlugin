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

void IGameModeChatInterface::AuctionSpeak(APlayerController* PlayerController, const FString& String)
{
	const AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " auctions, " + FinalMessage;
	FConstPlayerControllerIterator EndIterator = GameMode->GetWorld()->GetPlayerControllerIterator();
	EndIterator.SetToEnd();

	for (FConstPlayerControllerIterator PCIterator = GameMode->GetWorld()->GetPlayerControllerIterator(); PCIterator !=
	     EndIterator; ++PCIterator)
	{
		if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(*PCIterator); ChatInterface && *PCIterator
			!= PlayerController)
		{
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Auction, CompleteString);
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

//----------------------------------------------------------------------------------------------------------------------

TArray<APlayerController*> IGameModeChatInterface::GetRaidMembers(APlayerController* PlayerController)
{
	return {};
}

//----------------------------------------------------------------------------------------------------------------------

TArray<APlayerController*> IGameModeChatInterface::GetGuildMembers(APlayerController* PlayerController)
{
	return {};
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::TellSpeak(APlayerController* MainPlayerController, const FString& Target,
                                       const FString& Message)
{
	const IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(MainPlayerController);
	const FString FinalMessage = "'" + Message + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " tells you, " + FinalMessage;

	if (APlayerController* TargetPC = FindPlayerWithName(Target))
	{
		if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(TargetPC); ChatInterface)
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Tell, CompleteString);
	}
	else //we haven't find the player locally. maybe try another server
	{
		SendMessageToOtherServer(Target, OriginalSpeaker->GetChatName(), Message);
	}
}

//----------------------------------------------------------------------------------------------------------------------

APlayerController* IGameModeChatInterface::FindPlayerWithName(const FString& Name)
{
	//this is a really bad way to do it, better use a lookup table
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	FConstPlayerControllerIterator EndIterator = GameMode->GetWorld()->GetPlayerControllerIterator();
	EndIterator.SetToEnd();

	for (FConstPlayerControllerIterator PCIterator = GameMode->GetWorld()->GetPlayerControllerIterator(); PCIterator !=
	     EndIterator; ++PCIterator)
	{
		IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(*PCIterator);

		if (ChatInterface && ChatInterface->GetChatName() == Name)
		{
			return PCIterator->Get();
		}
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::SendMessageToOtherServer(const FString& Name, const FString& SenderName,
                                                      const FString& Message)
{
	//override this to send the message to other servers with a Rest API or something
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::RaidSpeak(APlayerController* PlayerController, const FString& String)
{
	const AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	const IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " tells the raid, " + FinalMessage;

	for (const auto& GroupMember : GetRaidMembers(PlayerController))
	{
		if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(GroupMember); ChatInterface && GroupMember
			!= PlayerController)
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Raid, CompleteString);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::GuildSpeak(APlayerController* PlayerController, const FString& String)
{
	const AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	const IPlayerChatInterface* OriginalSpeaker = Cast<IPlayerChatInterface>(PlayerController);

	check(GameMode)

	if (!OriginalSpeaker)
		return;

	const FString FinalMessage = "'" + String + "'";
	const FString CompleteString = OriginalSpeaker->GetChatName() + " tells the guild, " + FinalMessage;

	for (const auto& GroupMember : GetGuildMembers(PlayerController))
	{
		if (IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(GroupMember); ChatInterface && GroupMember
			!= PlayerController)
			ChatInterface->Client_AddChatDataType(EGlobalMessageType::Guild, CompleteString);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::AreaLog(EGlobalMessageType MessageType, const FString& Message, const FVector& Origin,
                                     float Range)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{{EObjectTypeQuery::ObjectTypeQuery3}};
	const TArray<AActor*> ActorsToIgnore{};
	TArray<AActor*> ActorArray;
	const bool OverlapStatus = UKismetSystemLibrary::SphereOverlapActors(
		GameMode->GetWorld(), Origin, Range,
		ObjectTypes, nullptr,
		ActorsToIgnore, ActorArray);

	UKismetSystemLibrary::DrawDebugSphere(GameMode->GetWorld(), Origin, Range,
	                                      8,
	                                      OverlapStatus ? FColor::Green : FColor::Red, 3.f, 0.f);

	for (auto& Actor : ActorArray)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			AController* Controller = Character->GetController();
			IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(Controller);

			if (ChatInterface)
				ChatInterface->Client_AddChatDataType(MessageType, Message);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::AreaLogAroundPlayer(APlayerController* PlayerController,
	EGlobalMessageType MessageType, const FString& Message, float Range)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

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

	for (auto& Actor : ActorArray)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			AController* Controller = Character->GetController();
			IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(Controller);

			if (ChatInterface && Controller != PlayerController)
				ChatInterface->Client_AddChatDataType(MessageType, Message);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::AreaConstantLogAroundPlayer(APlayerController* PlayerController,
	EChatColor Color, EMessageCategories Category, int32 MessageId, float Range)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

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

	for (auto& Actor : ActorArray)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			AController* Controller = Character->GetController();
			IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(Controller);

			if (ChatInterface && Controller != PlayerController)
				ChatInterface->Client_AddChatConstantData(Color, Category, MessageId);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::AreaConstantLog(EChatColor Color, EMessageCategories Category, int32 MessageId,
	const FVector& Origin, float Range)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{{EObjectTypeQuery::ObjectTypeQuery3}};
	const TArray<AActor*> ActorsToIgnore{};
	TArray<AActor*> ActorArray;
	const bool OverlapStatus = UKismetSystemLibrary::SphereOverlapActors(
		GameMode->GetWorld(), Origin, Range,
		ObjectTypes, nullptr,
		ActorsToIgnore, ActorArray);

	UKismetSystemLibrary::DrawDebugSphere(GameMode->GetWorld(), Origin, Range,
										  8,
										  OverlapStatus ? FColor::Green : FColor::Red, 3.f, 0.f);

	for (auto& Actor : ActorArray)
	{
		if (ACharacter* Character = Cast<ACharacter>(Actor))
		{
			AController* Controller = Character->GetController();
			IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(Controller);

			if (ChatInterface)
				ChatInterface->Client_AddChatConstantData(Color,Category,MessageId);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void IGameModeChatInterface::ServerBroadcast(EGlobalMessageType MessageType, const FString& Message)
{
	AGameModeBase* GameMode = Cast<AGameModeBase>(this);
	check(GameMode)

	FConstPlayerControllerIterator EndIterator = GameMode->GetWorld()->GetPlayerControllerIterator();
	EndIterator.SetToEnd();

	for (FConstPlayerControllerIterator PCIterator = GameMode->GetWorld()->GetPlayerControllerIterator(); PCIterator !=
		 EndIterator; ++PCIterator)
	{
		IPlayerChatInterface* ChatInterface = Cast<IPlayerChatInterface>(*PCIterator);

		if (ChatInterface)
			ChatInterface->Client_AddChatDataType(MessageType,Message);

	}
}