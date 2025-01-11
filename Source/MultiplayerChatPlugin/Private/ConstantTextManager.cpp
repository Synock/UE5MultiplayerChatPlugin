// Copyright 2025 Maximilien (Synock) Guislain
#include "ConstantTextManager.h"
#include "Engine/DataTable.h"
#include "ConstantStringRow.h"
#include "Engine/Engine.h"

TMap<int32, FText> ConstantTextManager::TextMap;

void ConstantTextManager::InitializeTextMap(const FString& EditorStringListFilePath)
{
	// Assuming you correctly set up a Data Table asset in your Content folder in EditorStringListFilePath
	// eg TEXT("/Game/ConstantStrings.ConstantStrings")
	static const FString ContextString(TEXT("MyTextTableContext"));
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *EditorStringListFilePath );

	if (DataTable)
	{
		TArray<FConstantStringRow*> Rows;
		DataTable->GetAllRows(ContextString, Rows);

		for (FConstantStringRow* Row : Rows)
		{
			if (Row)
			{
				TextMap.Add(Row->ID, Row->Text);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Data Table"));
	}
}

const FText& ConstantTextManager::GetTextByID(int32 ID)
{
	if (TextMap.Num() == 0)
	{
		InitializeTextMap();
	}

	if (const FText* FoundText = TextMap.Find(ID))
	{
		return *FoundText;
	}

	UE_LOG(LogTemp, Warning, TEXT("Cannot load string (%i)"), ID);
	static const FText EmptyText = FText::GetEmpty();
	return EmptyText;
}