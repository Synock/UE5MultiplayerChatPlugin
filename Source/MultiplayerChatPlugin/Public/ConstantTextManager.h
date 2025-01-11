// Copyright 2025 Maximilien (Synock) Guislain
#pragma once

#include "CoreMinimal.h"

class MULTIPLAYERCHATPLUGIN_API ConstantTextManager
{
public:
	static const FText& GetTextByID(int32 ID);

	static void InitializeTextMap(const FString& EditorStringListFilePath = {"/Game/ConstantStrings.ConstantStrings"});

private:
	static TMap<int32, FText> TextMap;
};