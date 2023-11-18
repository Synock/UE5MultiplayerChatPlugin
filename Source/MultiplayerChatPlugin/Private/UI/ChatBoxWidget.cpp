// Copyright 2022 Maximilien (Synock) Guislain


#include "UI/ChatBoxWidget.h"

#include "Interfaces/PlayerChatInterface.h"

void UChatBoxWidget::HandleTextEnter()
{
	FString InputString = TextBox->GetText().ToString();

	if (InputString.IsEmpty())
	{
		ReturnFocus();
		return;
	}

	TextBox->SetText(FText::GetEmpty());

	IPlayerChatInterface* PlayerChatInterface = Cast<IPlayerChatInterface>(GetOwningPlayer());

	if (InputString[0] == ServerCommandTriggerCharacter)
	{
		FString Command;
		FString Arguments;
		InputString.Split(" ", &Command, &Arguments, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		Command = Command.IsEmpty() ? InputString.RightChop(1) : Command.RightChop(1);

		PlayerChatInterface->ProcessServerCommands(Command, Arguments);

		ReturnFocus();
		return;
	}

	if (InputString[0] == CommandTriggerCharacter)
	{
		FString Command;
		FString Arguments;
		InputString.Split(" ", &Command, &Arguments, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		Command = Command.IsEmpty() ? InputString.RightChop(1) : Command.RightChop(1);

		if (Command.Equals("Tell", ESearchCase::IgnoreCase) || Command.Equals("t", ESearchCase::IgnoreCase))
		{
			FString TargetName;
			FString Message;
			Arguments.Split(" ", &TargetName, &Message, ESearchCase::IgnoreCase, ESearchDir::FromStart);
			PlayerChatInterface->TellSpeak(TargetName, Message);
		}
		else if (Command.Equals("Say", ESearchCase::IgnoreCase) || Command.Equals("s", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->AreaSpeak(Arguments, 1500.f);
		}
		else if (Command.Equals("Group", ESearchCase::IgnoreCase) || Command.Equals("g", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->GroupSpeak(Arguments);
		}
		else if (Command.Equals("Shout", ESearchCase::IgnoreCase) )
		{
			PlayerChatInterface->ShoutSpeak(Arguments);
		}
		else if (Command.Equals("Raid", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->RaidSpeak(Arguments);
		}
		else if (Command.Equals("Guild", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->GuildSpeak(Arguments);
		}
		else if (Command.Equals("OOC", ESearchCase::IgnoreCase)|| Command.Equals("o", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->OOCSpeak(Arguments);
		}
		else if (Command.Equals("Auction", ESearchCase::IgnoreCase))
		{
			PlayerChatInterface->AuctionSpeak(Arguments);
		}
		else
		{
			PlayerChatInterface->ProcessCommands(Command, Arguments);
		}
		ReturnFocus();
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
	ReturnFocus();
}

void UChatBoxWidget::ReturnFocus()
{
	OnReleaseFocusRequired.Broadcast();
}

bool UChatBoxWidget::HasFocus() const
{
	return TextBox->HasUserFocus(GetOwningPlayer());
}

void UChatBoxWidget::SetChatFocus()
{
	TextBox->SetFocus();
}
