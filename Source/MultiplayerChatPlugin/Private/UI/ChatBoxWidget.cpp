// Copyright 2022 Maximilien (Synock) Guislain


#include "UI/ChatBoxWidget.h"

#include "Interfaces/PlayerChatInterface.h"

void UChatBoxWidget::HandleTextEnter()
{
	FString InputString = TextBox->GetText().ToString();


	if (InputString[0] == '/')
	{
		//figure out commandline
	}

	IPlayerChatInterface* PlayerChatInterface = Cast<IPlayerChatInterface>(GetOwningPlayer());

	if(const FString SelectedOption = ChatSelection->GetSelectedOption(); SelectedOption == "Say")
	{

	}
	else if(SelectedOption == "Group")
	{

	}
	else if(SelectedOption == "Shout")
	{

	}
	else if(SelectedOption == "Raid")
	{

	}
	else if(SelectedOption == "OOC")
	{

	}
	else if(SelectedOption == "Tell")
	{

	}
}
