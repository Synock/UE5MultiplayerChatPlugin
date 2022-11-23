// Copyright 2022 Maximilien (Synock) Guislain


#include "UI/ChatLineWidget.h"

FText UChatLineWidget::FormatMessage(EChatColor Color, const FString& Message)
{
	if(Color == EChatColor::Unknown)
		return FText::FromString(Message);

	FString BuiltString;
	switch (Color)
	{

	case EChatColor::Red: BuiltString+= "<Red>"; break;
	case EChatColor::White: BuiltString+= "<White>";break;
	case EChatColor::Blue: BuiltString+= "<Blue>";break;
	case EChatColor::LightBlue: BuiltString+= "<LightBlue>";break;
	case EChatColor::LightPurple: BuiltString+= "<LightPurple>";break;
	case EChatColor::Purple: BuiltString+= "<Purple>";break;
	case EChatColor::Yellow: BuiltString+= "<Yellow>";break;
	case EChatColor::Green: BuiltString+= "<Green>";break;
	case EChatColor::DarkGreen: BuiltString+= "<DarkGreen>";break;
	case EChatColor::Gray: BuiltString+= "<Gray>";break;
	default: ;
	case EChatColor::Unknown: break;
	}

	BuiltString+= Message;
	BuiltString+="</>";
	return FText::FromString(BuiltString);
}
