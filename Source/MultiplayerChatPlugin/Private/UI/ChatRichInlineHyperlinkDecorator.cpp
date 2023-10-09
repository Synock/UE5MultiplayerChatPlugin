// Copyright 2023 Maximilien (Synock) Guislain


#include "UI/ChatRichInlineHyperlinkDecorator.h"

#include "Framework/Text/SlateHyperlinkRun.h"
#include "Widgets/Input/SRichTextHyperlink.h"

class FChatRichInlineHyperlinkDecorator : public FRichTextDecorator
{
protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo,
													  const FTextBlockStyle& TextStyle) const override;
	FHyperlinkStyle LinkStyle;
	mutable FSimpleDelegate Delegate;
	UChatRichInlineHyperlinkDecorator* LocalDecorator;

public:
	FChatRichInlineHyperlinkDecorator(URichTextBlock* InOwner, class UChatRichInlineHyperlinkDecorator* Decorator);
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override;
};

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

UChatRichInlineHyperlinkDecorator::UChatRichInlineHyperlinkDecorator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}
//----------------------------------------------------------------------------------------------------------------------

FChatRichInlineHyperlinkDecorator::FChatRichInlineHyperlinkDecorator(URichTextBlock* InOwner,
                                                             UChatRichInlineHyperlinkDecorator* Decorator)
	: FRichTextDecorator(InOwner)
{
	LinkStyle = Decorator->Style;
	LocalDecorator = Decorator;
}

//----------------------------------------------------------------------------------------------------------------------

bool FChatRichInlineHyperlinkDecorator::Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const

{
	if (RunParseResult.Name == TEXT("ChatLink"))
	{
		if (RunParseResult.MetaData.Contains(TEXT("id")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
			FString IDString = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);
			Delegate.BindLambda([IDString,this]()
			{
				LocalDecorator->ClickFun(IDString);
			});
		}
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

TSharedPtr<SWidget> FChatRichInlineHyperlinkDecorator::CreateDecoratorWidget(
	const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const
{
	TSharedPtr<FSlateHyperlinkRun::FWidgetViewModel> Model = MakeShareable(new FSlateHyperlinkRun::FWidgetViewModel);
	TSharedPtr<SRichTextHyperlink> Link;

	/*if (DelegateMap.Contains(StuffToSearch))
	{
		Link = SNew(SRichTextHyperlink, Model.ToSharedRef())
		   .Text(RunInfo.Content)
		   .Style(&LinkStyle)
		   .OnNavigate(*DelegateMap.Find(StuffToSearch));
	}
	else */
		Link = SNew(SRichTextHyperlink, Model.ToSharedRef())
		  .Text(RunInfo.Content)
		  .Style(&LinkStyle)
		.OnNavigate(Delegate);


	return Link;
}

//----------------------------------------------------------------------------------------------------------------------

TSharedPtr<ITextDecorator> UChatRichInlineHyperlinkDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	TSharedPtr<ITextDecorator> Output = MakeShareable(new FChatRichInlineHyperlinkDecorator(InOwner, this));
	return Output;
}

//----------------------------------------------------------------------------------------------------------------------

void UChatRichInlineHyperlinkDecorator::ClickFun_Implementation(const FString& ID)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	BPHelperFunction(PlayerController,PlayerController->GetGameInstance(), ID);
}
