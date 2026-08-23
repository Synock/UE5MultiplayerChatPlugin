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
		InitializeTextMap(DataTable);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Data Table at '%s' — constant chat messages will be empty. "
			"Ensure the asset is cooked (add a UPROPERTY reference or DirectoriesToAlwaysCook)."),
			*EditorStringListFilePath);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void ConstantTextManager::InitializeTextMap(UDataTable* DataTable)
{
	static const FString ContextString(TEXT("MyTextTableContext"));
	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConstantTextManager::InitializeTextMap called with null DataTable"));
		return;
	}

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