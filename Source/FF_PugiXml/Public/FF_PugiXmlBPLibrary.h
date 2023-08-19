// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
#include "pugixml.hpp"
THIRD_PARTY_INCLUDES_END

#include "FF_PugiXmlBPLibrary.generated.h"

using namespace pugi;

/*
*	PugiXml Binaries
*	https://github.com/zeux/pugixml
*/

UCLASS(BlueprintType)
class FF_PUGIXML_API UFFPugiXml_Doc : public UObject
{
	GENERATED_BODY()

public:

	xml_document Document;

};

UCLASS()
class UFF_PugiXmlBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PugiXml - Create Document", Keywords = "pugixml, xml, document, create"), Category = "FF_PugiXml")
	static void PugiXml_Create_Doc(UFFPugiXml_Doc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration = true);

};