// Copyright 2022 Maximilien (Synock) Guislain


#include "UI/ChatBoxWidget.h"

#include "Interfaces/PlayerChatInterface.h"

void UChatBoxWidget::HandleTextEnter()
{
	FString InputString = TextBox->GetText().ToString();

	TextBox->SetText(FText::GetEmpty());

	if (InputString[0] == '/')
	{
		//figure out commandline
	}

	IPlayerChatInterface* PlayerChatInterface = Cast<IPlayerChatInterface>(GetOwningPlayer());

	if(const FString SelectedOption = ChatSelection->GetSelectedOption(); SelectedOption == "Say")
	{
		PlayerChatInterface->AreaSpeak(InputString, 1500.f);
	}
	else if(SelectedOption == "Group")
	{

	}
	else if(SelectedOption == "Shout")
	{
		PlayerChatInterface->ShoutSpeak(InputString);
	}
	else if(SelectedOption == "Raid")
	{

	}
	else if(SelectedOption == "OOC")
	{
		PlayerChatInterface->OOCSpeak(InputString);
	}
	else if(SelectedOption == "Tell")
	{

	}
}
