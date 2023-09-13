/*
*	Copyright Epic Games, Inc. All Rights Reserved.
* 
*	PugiXml Binaries.
*	https://github.com/zeux/pugixml
*
*	PugiXml Documentations.
*	https://pugixml.org/docs/manual.html
*
*	Libdeflate Binaries.
*	https://github.com/ebiggers/libdeflate
* 
*	We disabled libdeflate because we thought it could help us to access .xlsx file contents and we were wrong.
*	But we don't want to delete it completely because it is a good referance for future projects.
*/

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
//#define PUGIXML_WCHAR_MODE
//#define pugi pugiw
#include "pugixml.hpp"
THIRD_PARTY_INCLUDES_END

#include "FF_PugiXmlBPLibrary.generated.h"

using namespace pugi;

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

	bool operator == (const UFFPugiXml_Node*& Other) const
	{
		return Node.hash_value() == Other->Node.hash_value();
	}

	bool operator != (const UFFPugiXml_Node*& Other) const
	{
		return !(*this == Other);
	}
};

FORCEINLINE uint32 GetTypeHash(const UFFPugiXml_Node& Key)
{
	uint32_t Hash_Node = GetTypeHash((uint32_t)Key.Node.hash_value());

	uint32_t GenericHash;
	FMemory::Memset(&GenericHash, 0, sizeof(uint32_t));
	GenericHash = HashCombine(GenericHash, Hash_Node);

	return GenericHash;
}

/*
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
*/

UCLASS()
class UFF_PugiXmlBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/*
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Libdeflate - Test", Keywords = "libdeflate, deflate, gzip, zlib, compression, compress"), Category = "FF_PugiXml")
	static void LibDeflateTest(FDelegateDeflate DelegateDeflate, TArray<uint8> In_Bytes);
	*/

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from File", Keywords = "pugixml, xml, document, open, file"), Category = "FF_PugiXml|Open")
	static bool PugiXml_Doc_Open_File(UFFPugiXml_Doc*& Out_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from Memory", Keywords = "pugixml, xml, document, open, memory"), Category = "FF_PugiXml|Open")
	static bool PugiXml_Doc_Open_Memory(UFFPugiXml_Doc*& Out_Doc, TArray<uint8> In_Buffer);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from String", Keywords = "pugixml, xml, document, open, string"), Category = "FF_PugiXml|Open")
	static bool PugiXml_Doc_Open_String(UFFPugiXml_Doc*& Out_Doc, FString In_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Print Document", Keywords = "pugixml, xml, document, print"), Category = "FF_PugiXml|Save")
	static bool PugiXml_Doc_Print(UFFPugiXml_Doc* In_Doc, FString& Out_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Save Document", Keywords = "pugixml, xml, document, save"), Category = "FF_PugiXml|Save")
	static bool PugiXml_Doc_Save(UFFPugiXml_Doc* In_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Create Document", Keywords = "pugixml, xml, document, create"), Category = "FF_PugiXml|Write")
	static void PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString RootName, FString DoctypeName, bool bIsStandalone, bool bAddDoctype);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Element", Keywords = "pugixml, xml, document, node, add, element"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Add_Element(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue, TMap<FString, FString> Attributes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Comment", Keywords = "pugixml, xml, document, node, add, comment"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Add_Comment(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Processing Instructions", Keywords = "pugixml, xml, document, node, add, pi, processing, instructions"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Add_Pi(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Plain Character", Keywords = "pugixml, xml, document, node, add, pcdata, plain, character"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Add_pcdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node CDATA", Keywords = "pugixml, xml, document, node, add,  cdata"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Add_cdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Remove Node", Keywords = "pugixml, xml, document, node, remove"), Category = "FF_PugiXml|Write")
	static bool PugiXml_Node_Remove(UObject* Source, UPARAM(ref)UFFPugiXml_Node*& Delete_Target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get Children", Keywords = "pugixml, xml, document, get, node, read, child, children, all"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Children(TArray<UFFPugiXml_Node*>& Out_Children, UObject* Target_Object);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get First Child", Keywords = "pugixml, xml, document, get, node, read, child, children, first"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Border_Children(UFFPugiXml_Node*& Out_Child, UObject* Target_Object, bool bIsLast);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get First Child", Keywords = "pugixml, xml, document, get, node, read, child, children, first"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Siblings(UFFPugiXml_Node*& Out_Sibling, UFFPugiXml_Node* Target_Node, bool bIsPrevious);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Child", Keywords = "pugixml, xml, document, get, node, read, child, children, find"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Find_Child(UFFPugiXml_Node*& Out_Sibling, UObject* Target_Object, FString Child_Name);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Child by Attribute", Keywords = "pugixml, xml, document, get, node, read, child, children, find, attribute"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Find_Child_By_Attribute(UFFPugiXml_Node*& Out_Sibling, UObject* Target_Object, FString Attribute_Name, FString Attribute_Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Parent", Keywords = "pugixml, xml, document, get, node, read, parent, find"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Find_Parent(UFFPugiXml_Node*& Found_Parent, bool& bIsParentXml, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Name", Keywords = "pugixml, xml, document, get, node, read, name"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Name(FString& Out_Name, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Value", Keywords = "pugixml, xml, document, get, node, read, value"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Value(FString& Out_Value, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Attributes", Keywords = "pugixml, xml, document, get, node, read, attribute, attributes"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Get_Attributes(TMap<FString, FString>& Out_Attributes, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Compare Nodes", Keywords = "pugixml, xml, document, get, node, compare"), Category = "FF_PugiXml|Parse")
	static bool PugiXml_Compare_Nodes(bool& bAreTheySame, UFFPugiXml_Node* First_Node, UFFPugiXml_Node* Second_Node);

};