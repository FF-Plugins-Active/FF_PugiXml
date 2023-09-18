#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPugiXmlDataTypes : uint8
{
	None			UMETA(DisplayName = "None"),
	Document		UMETA(DisplayName = "Document"),
	Decleration		UMETA(DisplayName = "Decleration"),
	Doctype			UMETA(DisplayName = "Doctype"),
	Element			UMETA(DisplayName = "Element"),
	Comment			UMETA(DisplayName = "Comment"),
	Pi				UMETA(DisplayName = "Process Instruction"),
	PCDATA			UMETA(DisplayName = "Plain Characters"),
	CDATA			UMETA(DisplayName = "CDATA"),
	Null			UMETA(DisplayName = "Null"),
};
ENUM_CLASS_FLAGS(EPugiXmlDataTypes)

UENUM(BlueprintType)
enum class EPugiXmlAddTypes : uint8
{
	Child_First		UMETA(DisplayName = "First Child"),
	Child_Last		UMETA(DisplayName = "Last Child"),
	Previous		UMETA(DisplayName = "Previous"),
	After			UMETA(DisplayName = "After"),
};
ENUM_CLASS_FLAGS(EPugiXmlAddTypes)