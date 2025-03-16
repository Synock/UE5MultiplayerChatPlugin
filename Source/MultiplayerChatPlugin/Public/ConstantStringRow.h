// Copyright 2025 Maximilien (Synock) Guislain
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConstantStringRow.generated.h"

USTRUCT(BlueprintType)
struct FConstantStringRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID  = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Text = FText();
};