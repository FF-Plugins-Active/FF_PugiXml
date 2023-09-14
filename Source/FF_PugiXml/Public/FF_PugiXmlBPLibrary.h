/*
*	
*	PugiXml Binaries.
*	https://github.com/zeux/pugixml
*
*	PugiXml Documentations.
*	https://pugixml.org/docs/manual.html
*
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

USTRUCT()
struct FF_PUGIXML_API FPugiXmlDoctypeElements
{
	GENERATED_BODY()

public:

	FString Element_Name = "";

	TArray<FString> Element_Contents;

};

USTRUCT()
struct FF_PUGIXML_API FPugiXmlDoctypeAttributes
{
	GENERATED_BODY()

public:

	FString Element_Name = "";
	
	FString Attribute_Name = "";

	FString Attribute_Value_Default;

	TArray<FString> Attribute_Value_List;

};

UCLASS(BlueprintType)
class FF_PUGIXML_API UFFPugiXml_Node : public UObject
{
	GENERATED_BODY()

public:

	xml_node Node;

};

UCLASS(BlueprintType)
class FF_PUGIXML_API UFFPugiXml_Doc : public UObject
{
	GENERATED_BODY()

public:

	xml_document Document;
	
	UPROPERTY(BlueprintReadWrite)
	UFFPugiXml_Node* Root = nullptr;

	TMap<FString, FPugiXmlDoctypeElements> Doctype_Elements;

	TMap<FString, FPugiXmlDoctypeAttributes> Doctype_Attributes;
};

UCLASS()
class UFF_PugiXmlBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from File", Keywords = "pugixml, xml, document, open, file"), Category = "FF_PugiXml|Open")
	static FF_PUGIXML_API bool PugiXml_Doc_Open_File(UFFPugiXml_Doc*& Out_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from Memory", Keywords = "pugixml, xml, document, open, memory"), Category = "FF_PugiXml|Open")
	static FF_PUGIXML_API bool PugiXml_Doc_Open_Memory(UFFPugiXml_Doc*& Out_Doc, TArray<uint8> In_Buffer);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Open Document from String", Keywords = "pugixml, xml, document, open, string"), Category = "FF_PugiXml|Open")
	static FF_PUGIXML_API bool PugiXml_Doc_Open_String(UFFPugiXml_Doc*& Out_Doc, FString In_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Print Document", Keywords = "pugixml, xml, document, print"), Category = "FF_PugiXml|Save")
	static FF_PUGIXML_API bool PugiXml_Doc_Print(UFFPugiXml_Doc* In_Doc, FString& Out_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Save Document", Tooltip = "", Keywords = "pugixml, xml, document, save"), Category = "FF_PugiXml|Save")
	static FF_PUGIXML_API bool PugiXml_Doc_Save(UFFPugiXml_Doc* In_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Create Document", Tooltip = "", Keywords = "pugixml, xml, document, create"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API void PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString RootName, bool bIsStandalone);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Clear XML", Tooltip = "", Keywords = "pugixml, xml, document, empty, delete, remove, clear"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Doc_Clear(UPARAM(ref)UFFPugiXml_Doc*& In_Doc);

	/**
	* ADVANCED !
	* DOCTYPE has to be only one in an XML and it's parent has to be directly document's itself.
	* Function automatically check if there is a DOCTYPE as third child in XML's root. (1st. Declaration, 2nd. DTD delimiter comment, 3rd. DOCTYPE, 4. XML delimiter comment)
	* @param Out_Nodes This array contains three nodes. DTD delimiter, DOCTYPE, XML delimiter.
	* @param Elements You have to write all node "names" in here. You don't have to write values or informations about only value based nodes.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Custom Doctype", Keywords = "pugixml, xml, document, node, add, doctype, custom"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_Doctype_Custom(TArray<UFFPugiXml_Node*>& Out_Nodes, UFFPugiXml_Doc* In_Doc, FString DoctypeName, TMap<FString, FString> In_Elements);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Custom Auto", Keywords = "pugixml, xml, document, node, add, doctype, auto"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_Doctype_Auto(TArray<UFFPugiXml_Node*>& Out_Nodes, UFFPugiXml_Doc* In_Doc, FString DoctypeName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Element", Tooltip = "", Keywords = "pugixml, xml, document, node, add, element"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_Element(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue, TMap<FString, FString> Attributes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Comment", Tooltip = "", Keywords = "pugixml, xml, document, node, add, comment"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_Comment(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Processing Instructions", Tooltip = "", Keywords = "pugixml, xml, document, node, add, pi, processing, instructions"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_Pi(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node Plain Character", Tooltip = "", Keywords = "pugixml, xml, document, node, add, pcdata, plain, character"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_pcdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Add Node CDATA", Tooltip = "", Keywords = "pugixml, xml, document, node, add,  cdata"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Add_cdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Remove Node 1", Tooltip = "It has to be direct child/parent not grand-children.", Keywords = "pugixml, xml, document, node, remove"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Remove_1(UPARAM(ref)UObject*& Source, UPARAM(ref)UFFPugiXml_Node*& Delete_Target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Remove Node 2", Tooltip = "It automatically finds parent.", Keywords = "pugixml, xml, document, node, remove"), Category = "FF_PugiXml|Write")
	static FF_PUGIXML_API bool PugiXml_Node_Remove_2(UPARAM(ref)UFFPugiXml_Node*& Delete_Target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get Children", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, child, children, all"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Children(TArray<UFFPugiXml_Node*>& Out_Children, UObject* Target_Object);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get First Child", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, child, children, first"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Border_Children(UFFPugiXml_Node*& Out_Child, UObject* Target_Object, bool bIsLast);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Get First Child", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, child, children, first"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Siblings(UFFPugiXml_Node*& Out_Sibling, UFFPugiXml_Node* Target_Node, bool bIsPrevious);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Child", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, child, children, find"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Find_Child(UFFPugiXml_Node*& Out_Sibling, UObject* Target_Object, FString Child_Name);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Child by Attribute", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, child, children, find, attribute"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Find_Child_By_Attribute(UFFPugiXml_Node*& Out_Sibling, UObject* Target_Object, FString Attribute_Name, FString Attribute_Value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pugixml - Find Parent", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, parent, find"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Find_Parent(UFFPugiXml_Node*& Found_Parent, bool& bIsParentXml, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Name", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, name"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Name(FString& Out_Name, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Value", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, value"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Value(FString& Out_Value, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Get Attributes", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, attribute, attributes"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Get_Attributes(TMap<FString, FString>& Out_Attributes, UFFPugiXml_Node* Target_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Check If Empty", Tooltip = "", Keywords = "pugixml, xml, document, get, node, read, check, empty"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Check_Empty(bool bIsEmpty, UObject* Target_Object);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Compare Nodes", Tooltip = "", Keywords = "pugixml, xml, document, get, node, compare"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Compare_Nodes(bool& bAreTheySame, UFFPugiXml_Node* First_Node, UFFPugiXml_Node* Second_Node);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Pugixml - Compare Documents", Tooltip = "", Keywords = "pugixml, xml, document, get compare"), Category = "FF_PugiXml|Parse")
	static FF_PUGIXML_API bool PugiXml_Compare_XML(bool& bAreTheySame, UFFPugiXml_Doc* First_Document, UFFPugiXml_Doc* Second_Document);

};