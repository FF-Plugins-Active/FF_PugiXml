// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_PugiXmlBPLibrary.h"
#include "FF_PugiXml.h"

// UE Includes.
#include "Kismet/GameplayStatics.h"

THIRD_PARTY_INCLUDES_START
#include <string>
#include <sstream>
#include <iostream>
THIRD_PARTY_INCLUDES_END

UFF_PugiXmlBPLibrary::UFF_PugiXmlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UFF_PugiXmlBPLibrary::PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration)
{
	Out_Doc = NewObject<UFFPugiXml_Doc>();
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Print(UFFPugiXml_Doc* In_Doc, FString& Out_String)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}
	
	std::stringstream StringStream; 
	In_Doc->Document.print(StringStream);

	Out_String = UTF8_TO_TCHAR(std::string(StringStream.str()).c_str());

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Add_Node(UPARAM(ref)UFFPugiXml_Doc* In_Doc, FString NodeName, FString NodeValue)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	xml_node NewNode = In_Doc->Document.append_child(TCHAR_TO_UTF8(*NodeName));
	NewNode.append_attribute(TCHAR_TO_UTF8(*NodeValue));

	return true;
}