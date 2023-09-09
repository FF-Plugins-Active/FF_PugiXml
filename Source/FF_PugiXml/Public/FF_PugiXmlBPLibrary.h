// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
//#define PUGIXML_WCHAR_MODE
//#define pugi pugiw
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
	xml_node Root;
};

UCLASS(BlueprintType)
class FF_PUGIXML_API UFFPugiXml_Node : public UObject
{
	GENERATED_BODY()

public:

	xml_node Node;
};

USTRUCT(BlueprintType)
struct FDeflateUncompressed
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> UncompressedBuffer;

};

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDelegateDeflate, bool, bIsSuccessful, FDeflateUncompressed, Out_Bytes, FString, Out_Code);

UCLASS()
class UFF_PugiXmlBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Libdeflate - Test", Keywords = "libdeflate, deflate, gzip, zlib, compression, compress"), Category = "FF_PugiXml")
	static void LibDeflateTest(FDelegateDeflate DelegateDeflate, TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Create Document", Keywords = "pugixml, xml, document, create"), Category = "FF_PugiXml")
	static void PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString RootName, FString DoctypeName, bool bIsStandalone, bool bAddDoctype);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Print Document", Keywords = "pugixml, xml, document, print"), Category = "FF_PugiXml")
	static bool PugiXml_Doc_Print(UFFPugiXml_Doc* In_Doc, FString& Out_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Save Document", Keywords = "pugixml, xml, document, save"), Category = "FF_PugiXml")
	static bool PugiXml_Doc_Save(UFFPugiXml_Doc* In_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node", Keywords = "pugixml, xml, document, add, node"), Category = "FF_PugiXml")
	static bool PugiXml_Add_Node(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue, TMap<FString, FString> Attributes);

};