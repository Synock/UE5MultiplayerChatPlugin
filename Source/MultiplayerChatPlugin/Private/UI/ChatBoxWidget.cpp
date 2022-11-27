// Copyright 2022 Maximilien (Synock) Guislain


#include "UI/ChatBoxWidget.h"

#include "Interfaces/PlayerChatInterface.h"

void UChatBoxWidget::HandleTextEnter()
{
	FString InputString = TextBox->GetText().ToString();

	TextBox->SetText(FText::GetEmpty());

	IPlayerChatInterface* PlayerChatInterface = Cast<IPlayerChatInterface>(GetOwningPlayer());

	if (InputString[0] == '/')
	{
		FString Command;
		FString Arguments;
		InputString.Split(" ", &Command, &Arguments, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		Command = Command.IsEmpty() ? InputString.RightChop(1) : Command.RightChop(1);

		if (Command.Equals("Tell", ESearchCase::IgnoreCase))
		{
			FString TargetName;
			FString Message;
			Arguments.Split(" ", &TargetName, &Message, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			PlayerChatInterface->TellSpeak(TargetName, Message);
		}
		else
		{
			PlayerChatInterface->ProcessCommands(Command, Arguments);
		}

		return;
	}

	if (const FString SelectedOption = ChatSelection->GetSelectedOption(); SelectedOption == "Say")
	{
		PlayerChatInterface->AreaSpeak(InputString, 1500.f);
	}
	else if (SelectedOption == "Group")
	{
		PlayerChatInterface->GroupSpeak(InputString);
	}
	else if (SelectedOption == "Shout")
	{
		PlayerChatInterface->ShoutSpeak(InputString);
	}
	else if (SelectedOption == "Raid")
	{
		PlayerChatInterface->RaidSpeak(InputString);
	}
	else if (SelectedOption == "Guild")
	{
		PlayerChatInterface->GuildSpeak(InputString);
	}
	else if (SelectedOption == "OOC")
	{
		PlayerChatInterface->OOCSpeak(InputString);
	}
	else if (SelectedOption == "Auction")
	{
		PlayerChatInterface->AuctionSpeak(InputString);
	}
	/*else if(SelectedOption == "Tell")
	{
		PlayerChatInterface->TellSpeak(InputString);
	}*/
}
