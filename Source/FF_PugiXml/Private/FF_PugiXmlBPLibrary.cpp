// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_PugiXmlBPLibrary.h"
#include "FF_PugiXml.h"

// UE Includes.
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

THIRD_PARTY_INCLUDES_START
#include <sstream>
#include <fstream>
THIRD_PARTY_INCLUDES_END

UFF_PugiXmlBPLibrary::UFF_PugiXmlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void Callback_Children(xml_node Target_Node, UPARAM(ref)TArray<UFFPugiXml_Node*>& Pool_Children)
{
	if (!Target_Node)
	{
		return;
	}

	for (xml_node Each_Node : Target_Node.children())
	{
		if (!Each_Node)
		{
			continue;
		}

		UFFPugiXml_Node* EachNodeObject = NewObject<UFFPugiXml_Node>();
		EachNodeObject->Node = Each_Node;

		Pool_Children.Add(EachNodeObject);

		if (Each_Node.first_child())
		{
			Callback_Children(Each_Node, Pool_Children);
		}
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Open_File(UFFPugiXml_Doc*& Out_Doc, FString In_Path)
{
	if (In_Path.IsEmpty())
	{
		return false;
	}

	FPaths::MakeStandardFilename(In_Path);
	if (!FPaths::FileExists(In_Path))
	{
		return false;
	}

	Out_Doc = NewObject<UFFPugiXml_Doc>();
	xml_parse_result Result = Out_Doc->Document.load_file(TCHAR_TO_UTF8(*In_Path));

	if (Result.status == xml_parse_status::status_ok)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Open_Memory(UFFPugiXml_Doc*& Out_Doc, TArray<uint8> In_Buffer)
{
	if (In_Buffer.Num() == 0)
	{
		return false;
	}

	Out_Doc = NewObject<UFFPugiXml_Doc>();
	xml_parse_result Result = Out_Doc->Document.load_buffer(In_Buffer.GetData(), In_Buffer.Num());

	if (Result.status == xml_parse_status::status_ok)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Open_String(UFFPugiXml_Doc*& Out_Doc, FString In_String)
{
	if (In_String.IsEmpty())
	{
		return false;
	}

	Out_Doc = NewObject<UFFPugiXml_Doc>();
	xml_parse_result Result = Out_Doc->Document.load_string(TCHAR_TO_UTF8(*In_String));

	if (Result.status == xml_parse_status::status_ok)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Print(FString& Out_String, UFFPugiXml_Doc* In_Doc)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	std::stringstream StringStream;
	In_Doc->Document.print(StringStream, "  ");

	Out_String = UTF8_TO_TCHAR(std::string(StringStream.str()).c_str());

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Save_Memory(TArray<uint8>& Out_Bytes, UFFPugiXml_Doc* In_Doc)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	std::stringstream StringStream;
	In_Doc->Document.print(StringStream, "  ");

	FTCHARToUTF8 Source = FTCHARToUTF8((FString)StringStream.str().c_str());
	Out_Bytes.Append((uint8*)Source.Get(), Source.Length());

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Save_File(UFFPugiXml_Doc* In_Doc, FString In_Path)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	if (In_Path.IsEmpty())
	{
		return false;
	}

	std::stringstream StringStream;
	In_Doc->Document.print(StringStream, "  ");

	std::ofstream OutFile(TCHAR_TO_UTF8(*In_Path));
	OutFile << StringStream.str();
	OutFile.close();

	return true;
}

void UFF_PugiXmlBPLibrary::PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString RootName, bool bIsStandalone)
{
	Out_Doc = NewObject<UFFPugiXml_Doc>();

	xml_node Declaration = Out_Doc->Document.prepend_child(node_declaration);
	Declaration.append_attribute("version") = "1.0";
	Declaration.append_attribute("encoding") = "utf-8";
	Declaration.append_attribute("standalone") = bIsStandalone ? "yes" : "no";

	if (!RootName.IsEmpty())
	{
		UFFPugiXml_Node* Root = NewObject<UFFPugiXml_Node>();
		Root->Node = Out_Doc->Document.append_child(node_element);
		Root->Node.set_name(TCHAR_TO_UTF8(*RootName));
		Root->Node.append_attribute("xmlns:xsd") = "http://www.w3.org/2001/XMLSchema";
		Root->Node.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";

		Out_Doc->SchemeElement = Root;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Clear(UPARAM(ref)UFFPugiXml_Doc*& In_Doc)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	In_Doc->Document.reset();
	In_Doc->SchemeElement = nullptr;
	In_Doc = nullptr;

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_Doctype_Custom(TArray<UFFPugiXml_Node*>& Out_Nodes, UFFPugiXml_Doc* In_Doc, FString DoctypeName, FString CustomDefinition)
{
	if (!IsValid(In_Doc) || DoctypeName.IsEmpty() || CustomDefinition.IsEmpty())
	{
		return false;
	}

	xml_node Decleration = In_Doc->Document.first_child();
	if (!Decleration)
	{
		return false;
	}

	// If there is a DOCTYPE in XML, return false.
	for (xml_node Each_Node : In_Doc->Document.children())
	{
		if (Each_Node.type() == node_doctype)
		{
			return false;
		}
	}

	// DTD delimiter comment.
	UFFPugiXml_Node* Node_Delimiter_DTD = NewObject<UFFPugiXml_Node>();
	Node_Delimiter_DTD->Node = In_Doc->Document.insert_child_after(node_comment, Decleration);
	bool Result_Delimiter_DTD = Node_Delimiter_DTD->Node.set_value("DTD");
	
	// DOCTYPE definition.
	FString DTD_Value = "";
	DTD_Value += DoctypeName + "\n";
	DTD_Value += "[\n";
	DTD_Value += CustomDefinition + "\n";
	DTD_Value += "]";

	// Doctype Node.
	UFFPugiXml_Node* Node_Doctype = NewObject<UFFPugiXml_Node>();
	Node_Doctype->Node = In_Doc->Document.insert_child_after(node_doctype, Node_Delimiter_DTD->Node);
	bool Result_Doctype = Node_Doctype->Node.set_value(TCHAR_TO_UTF8(*DTD_Value));

	// XML delimiter comment.
	UFFPugiXml_Node* Node_Delimiter_XML = NewObject<UFFPugiXml_Node>();
	Node_Delimiter_XML->Node = In_Doc->Document.insert_child_after(node_comment, Node_Doctype->Node);
	bool Result_Delimiter_XML = Node_Delimiter_XML->Node.set_value("XML");

	if (Result_Delimiter_DTD && Result_Doctype && Result_Delimiter_XML)
	{
		Out_Nodes.Add(Node_Delimiter_DTD);
		Out_Nodes.Add(Node_Doctype);
		Out_Nodes.Add(Node_Delimiter_XML);

		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_Doctype_Auto(TArray<UFFPugiXml_Node*>& Out_Nodes, UFFPugiXml_Doc* In_Doc, FString DoctypeName, bool bAddAttributes)
{
	if (!IsValid(In_Doc) || DoctypeName.IsEmpty())
	{
		return false;
	}

	xml_node Decleration = In_Doc->Document.first_child();
	if (!Decleration)
	{
		return false;
	}

	// If there is a DOCTYPE in XML, return false.
	for (xml_node Each_Node : In_Doc->Document.children())
	{
		if (Each_Node.type() == node_doctype)
		{
			return false;
		}
	}

	// DTD delimiter comment.
	UFFPugiXml_Node* Node_Delimiter_DTD = NewObject<UFFPugiXml_Node>();
	Node_Delimiter_DTD->Node = In_Doc->Document.insert_child_after(node_comment, Decleration);
	bool Result_Delimiter_DTD = Node_Delimiter_DTD->Node.set_value("DTD");

	// Calculate ELEMENTS and ATTLIST

	TArray<UFFPugiXml_Node*> Pool;
	Callback_Children(In_Doc->Document.root(), Pool);

	TMap<FString, FString> MAP_Elements;
	TMap<FString, FString> MAP_Attribute_Options;
	TArray<FPugiXmlDoctypeAttributes> Array_Attributes;

	for (int32 Index_Elements = 0; Index_Elements < Pool.Num(); Index_Elements++)
	{
		xml_node EachNode = Pool[Index_Elements]->Node;
		xml_node Parent = EachNode.parent();
		
		if (Parent && Parent.type() == node_element)
		{
			FString EachNodeName = "";
			FString ParentName = Parent.name();

			if (EachNode.type() == node_element || EachNode.type() == node_pi)
			{
				EachNodeName = EachNode.name();
			}

			else if (EachNode.type() == node_pcdata)
			{
				EachNodeName = "#PCDATA";
			}

			else if (EachNode.type() == node_cdata)
			{
				EachNodeName = "#CDATA";
			}
			
			if (MAP_Elements.Contains(ParentName))
			{
				FString CurrentContents = *MAP_Elements.Find(ParentName);
				
				if (EachNode.first_child() && EachNode.first_child().type() == node_element)
				{
					CurrentContents += "," + EachNodeName + "+";
				}

				else
				{
					CurrentContents += "," + EachNodeName;
				}

				MAP_Elements.Add(ParentName, CurrentContents);
			}

			else
			{
				MAP_Elements.Add(ParentName, EachNodeName);
			}
		}

		if (EachNode.type() == node_element && bAddAttributes)
		{
			for (xml_attribute Each_Attribute : EachNode.attributes())
			{
				if ((FString)Each_Attribute.name() != "xmlns:xsd" && (FString)Each_Attribute.name() != "xmlns:xsi")
				{
					FPugiXmlDoctypeAttributes EachAttribute;
					EachAttribute.Element_Name = EachNode.name();
					EachAttribute.Attribute_Name = Each_Attribute.name();
					Array_Attributes.Add(EachAttribute);

					if (MAP_Attribute_Options.Contains(Each_Attribute.name()))
					{
						FString CurrentContents = *MAP_Attribute_Options.Find(Each_Attribute.name());
						CurrentContents += "|" + (FString)Each_Attribute.value();
						MAP_Attribute_Options.Add(Each_Attribute.name(), CurrentContents);
					}

					else
					{
						MAP_Attribute_Options.Add(Each_Attribute.name(), Each_Attribute.value());
					}
				}
			}
		}
	}

	FString String_Elements;
	TArray<FString> Keys_Elements;
	MAP_Elements.GenerateKeyArray(Keys_Elements);
	for (int32 Index_Elements = 0; Index_Elements < Keys_Elements.Num(); Index_Elements++)
	{
		FString EachElement = "<!ELEMENT " + Keys_Elements[Index_Elements] + " (" + *MAP_Elements.Find(Keys_Elements[Index_Elements]) + ")>\n";
		String_Elements += EachElement;
	}

	FString String_Attributes;
	for (int32 Index_Attributes = 0; Index_Attributes < Array_Attributes.Num(); Index_Attributes++)
	{
		FPugiXmlDoctypeAttributes EachAttribute = Array_Attributes[Index_Attributes];
		
		FString AttributeOptions = *MAP_Attribute_Options.Find(EachAttribute.Attribute_Name);

		TArray<FString> Temp_Options = UKismetStringLibrary::ParseIntoArray(AttributeOptions, "|");
		FString EachAttributeString = "<!ATTLIST " + EachAttribute.Element_Name + " " + EachAttribute.Attribute_Name + " (" + AttributeOptions + ") \"" + Temp_Options[0] + "\">\n";
		
		String_Attributes += EachAttributeString;
	}

	// SET DTD VALUES
	FString DTD_Value = "";
	DTD_Value += DoctypeName + "\n";
	DTD_Value += "[\n";
	DTD_Value += String_Elements;		// It already has line breaking at the end.
	DTD_Value += String_Attributes;		// It already has line breaking at the end.
	DTD_Value += "]";

	// DOCTYPE node.
	UFFPugiXml_Node* Node_Doctype = NewObject<UFFPugiXml_Node>();
	Node_Doctype->Node = In_Doc->Document.insert_child_after(node_doctype, Node_Delimiter_DTD->Node);
	bool Result_Doctype = Node_Doctype->Node.set_value(TCHAR_TO_UTF8(*DTD_Value));

	// XML delimiter comment.
	UFFPugiXml_Node* Node_Delimiter_XML = NewObject<UFFPugiXml_Node>();
	Node_Delimiter_XML->Node = In_Doc->Document.insert_child_after(node_comment, Node_Doctype->Node);
	bool Result_Delimiter_XML = Node_Delimiter_XML->Node.set_value("XML");

	if (Result_Delimiter_DTD && Result_Doctype && Result_Delimiter_XML)
	{
		Out_Nodes.Add(Node_Delimiter_DTD);
		Out_Nodes.Add(Node_Doctype);
		Out_Nodes.Add(Node_Delimiter_XML);

		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_Element(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue, TMap<FString, FString> Attributes)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	Out_Node = NewObject<UFFPugiXml_Node>();

	if (IsValid(Parent_Node))
	{
		xml_node_type TargetNodeType = Parent_Node->Node.type();

		if (TargetNodeType == node_document || TargetNodeType == node_element)
		{
			Out_Node->Node = Parent_Node->Node.append_child(node_element);
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (In_Doc->SchemeElement)
		{
			Out_Node->Node = In_Doc->SchemeElement->Node.append_child(node_element);
		}

		else
		{
			Out_Node->Node = In_Doc->Document.append_child(node_element);
		}
	}

	if (!NodeName.IsEmpty())
	{
		Out_Node->Node.set_name(TCHAR_TO_UTF8(*NodeName));
	}

	if (!NodeValue.IsEmpty())
	{
		Out_Node->Node.append_child(node_pcdata).set_value(TCHAR_TO_UTF8(*NodeValue));
	}

	TArray<FString> Attribute_Names;
	Attributes.GenerateKeyArray(Attribute_Names);

	for (int32 Index_Attributes = 0; Index_Attributes < Attribute_Names.Num(); Index_Attributes++)
	{
		FString AttributeName = Attribute_Names[Index_Attributes];
		FString AttributeValue = *Attributes.Find(AttributeName);
		Out_Node->Node.append_attribute(TCHAR_TO_UTF8(*AttributeName)) = TCHAR_TO_UTF8(*AttributeValue);
	}

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_Comment(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Comment)
{
	if (!IsValid(In_Doc) || Comment.IsEmpty())
	{
		return false;
	}

	Out_Node = NewObject<UFFPugiXml_Node>();

	if (IsValid(Parent_Node))
	{
		xml_node_type TargetNodeType = Parent_Node->Node.type();

		if (TargetNodeType == node_document || TargetNodeType == node_element)
		{
			Out_Node->Node = Parent_Node->Node.append_child(node_comment);
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (In_Doc->SchemeElement)
		{
			Out_Node->Node = In_Doc->SchemeElement->Node.append_child(node_comment);
		}

		else
		{
			Out_Node->Node = In_Doc->Document.append_child(node_comment);
		}
	}

	return Out_Node->Node.set_value(TCHAR_TO_UTF8(*Comment));
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_Pi(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	Out_Node = NewObject<UFFPugiXml_Node>();

	if (IsValid(Parent_Node))
	{
		xml_node_type TargetNodeType = Parent_Node->Node.type();

		if (TargetNodeType == node_document || TargetNodeType == node_element)
		{
			Out_Node->Node = Parent_Node->Node.append_child(node_pi);
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (In_Doc->SchemeElement)
		{
			Out_Node->Node = In_Doc->SchemeElement->Node.append_child(node_pi);
		}

		else
		{
			Out_Node->Node = In_Doc->Document.append_child(node_pi);
		}
	}

	bool ResultName = Out_Node->Node.set_name(TCHAR_TO_UTF8(*NodeName));
	bool ResultValue = Out_Node->Node.set_value(TCHAR_TO_UTF8(*NodeValue));

	if (ResultName && ResultValue)
	{
		return true;
	}
	
	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_pcdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Value)
{
	if (!IsValid(In_Doc) || Value.IsEmpty())
	{
		return false;
	}

	Out_Node = NewObject<UFFPugiXml_Node>();

	if (IsValid(Parent_Node))
	{
		xml_node_type TargetNodeType = Parent_Node->Node.type();

		if (TargetNodeType == node_document || TargetNodeType == node_element)
		{
			Out_Node->Node = Parent_Node->Node.append_child(node_pcdata);
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (In_Doc->SchemeElement)
		{
			Out_Node->Node = In_Doc->SchemeElement->Node.append_child(node_pcdata);
		}

		else
		{
			Out_Node->Node = In_Doc->Document.append_child(node_pcdata);
		}
	}

	return Out_Node->Node.set_value(TCHAR_TO_UTF8(*Value));
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Add_cdata(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString Value)
{
	if (!IsValid(In_Doc))
	{
		return false;
	}

	Out_Node = NewObject<UFFPugiXml_Node>();

	if (IsValid(Parent_Node))
	{
		xml_node_type TargetNodeType = Parent_Node->Node.type();

		if (TargetNodeType == node_document || TargetNodeType == node_element)
		{
			Out_Node->Node = Parent_Node->Node.append_child(node_cdata);
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (In_Doc->SchemeElement)
		{
			Out_Node->Node = In_Doc->SchemeElement->Node.append_child(node_cdata);
		}

		else
		{
			Out_Node->Node = In_Doc->Document.append_child(node_cdata);
		}
	}

	return Out_Node->Node.set_value(TCHAR_TO_UTF8(*Value));
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Remove_1(UPARAM(ref)UObject * &Source, UPARAM(ref)UFFPugiXml_Node * &Delete_Target)
{
	if (!IsValid(Source) && !IsValid(Delete_Target))
	{
		return false;
	}

	UFFPugiXml_Doc* Source_Document = Cast<UFFPugiXml_Doc>(Source);
	if (IsValid(Source_Document) && Source_Document->Document)
	{
		if (Source_Document->Document != Delete_Target->Node.parent())
		{
			UE_LOG(LogTemp, Warning, TEXT("Target node's parent is not document !"));
			return false;
		}

		bool bAreNodesSame = false;
		PugiXml_Compare_Nodes(bAreNodesSame, Source_Document->SchemeElement, Delete_Target);

		bool ResultRemoveChild = Source_Document->Document.remove_child(Delete_Target->Node);

		if (bAreNodesSame)
		{
			Source_Document->SchemeElement = nullptr;
		}

		return ResultRemoveChild;
	}

	UFFPugiXml_Node* Source_Node = Cast<UFFPugiXml_Node>(Source);
	if (IsValid(Source_Node) && Source_Node->Node && Delete_Target->Node)
	{
		xml_node_type SourceNodeType = Source_Node->Node.type();
		if (SourceNodeType == node_document || SourceNodeType == node_element)
		{
			return Source_Node->Node.remove_child(Delete_Target->Node);
		}

		return false;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Node_Remove_2(UPARAM(ref)UFFPugiXml_Node*& Delete_Target)
{
	if (!IsValid(Delete_Target))
	{
		return false;
	}

	xml_node Parent = Delete_Target->Node.parent();
	bool RemoveResult = Parent.remove_child(Delete_Target->Node);

	if (RemoveResult)
	{
		Delete_Target = nullptr;
		return true;
	}
	
	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Children_Direct(TArray<UFFPugiXml_Node*>& Out_Children, UObject* Target_Object)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		for (xml_node Each_Node : Target_Document->Document.children())
		{
			if (Each_Node)
			{
				UFFPugiXml_Node* Each_Node_Object = NewObject<UFFPugiXml_Node>();
				Each_Node_Object->Node = Each_Node;

				Out_Children.Add(Each_Node_Object);
			}
		}

		return true;
	}

	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		xml_node_type TargetNodeType = Target_Node->Node.type();
		if (TargetNodeType != node_document && TargetNodeType != node_element)
		{
			return false;
		}

		for (xml_node Each_Node : Target_Node->Node.children())
		{
			if (Each_Node)
			{
				UFFPugiXml_Node* Each_Node_Object = NewObject<UFFPugiXml_Node>();
				Each_Node_Object->Node = Each_Node;

				Out_Children.Add(Each_Node_Object);
			}
		}

		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Children_Recursive(TArray<UFFPugiXml_Node*>& Out_Children, UObject* Target_Object)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		Callback_Children(Target_Document->Document.root(), Out_Children);
	}

	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		Callback_Children(Target_Node->Node, Out_Children);
	}

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Children_Border(UFFPugiXml_Node*& Out_Child, UObject* Target_Object, bool bIsLast)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		xml_node Child = bIsLast ? Target_Document->Document.last_child() : Target_Document->Document.first_child();

		if (!Child)
		{
			return false;
		}

		Out_Child = NewObject<UFFPugiXml_Node>();
		Out_Child->Node = Child;
		return true;
	}

	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		xml_node_type TargetNodeType = Target_Node->Node.type();
		if (TargetNodeType != node_document && TargetNodeType != node_element)
		{
			return false;
		}

		xml_node Child = bIsLast ? Target_Node->Node.last_child() : Target_Node->Node.first_child();
		if (!Child)
		{
			return false;
		}

		Out_Child = NewObject<UFFPugiXml_Node>();
		Out_Child->Node = Child;
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Siblings(UFFPugiXml_Node*& Out_Sibling, UFFPugiXml_Node* Target_Node, bool bIsPrevious)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node Sibling = bIsPrevious ? Target_Node->Node.previous_sibling() : Target_Node->Node.next_sibling();
	if (!Sibling)
	{
		return false;
	}

	Out_Sibling = NewObject<UFFPugiXml_Node>();
	Out_Sibling->Node = Sibling;
	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Find_Child_By_Name(UFFPugiXml_Node*& Out_Sibling, UObject* Target_Object, FString Child_Name)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		xml_node Found_Node = Target_Document->Document.child(TCHAR_TO_UTF8(*Child_Name));
		if (!Found_Node)
		{
			return false;
		}

		Out_Sibling = NewObject<UFFPugiXml_Node>();
		Out_Sibling->Node = Found_Node;
		return true;
	}

	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		xml_node_type TargetNodeType = Target_Node->Node.type();
		if (TargetNodeType != node_document && TargetNodeType != node_element)
		{
			return false;
		}

		xml_node Found_Node = Target_Node->Node.child(TCHAR_TO_UTF8(*Child_Name));
		if (!Found_Node)
		{
			return false;
		}

		Out_Sibling = NewObject<UFFPugiXml_Node>();
		Out_Sibling->Node = Found_Node;
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Find_Child_By_Attribute(UFFPugiXml_Node*& Out_Child, UObject* Target_Object, FString Attribute_Name, FString Attribute_Value)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		xml_node Found_Node = Target_Document->Document.find_child_by_attribute(TCHAR_TO_UTF8(*Attribute_Name), TCHAR_TO_UTF8(*Attribute_Value));
		if (!Found_Node)
		{
			return false;
		}

		Out_Child = NewObject<UFFPugiXml_Node>();
		Out_Child->Node = Found_Node;
		return true;
	}
	
	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		xml_node_type TargetNodeType = Target_Node->Node.type();
		if (TargetNodeType != node_document && TargetNodeType != node_element)
		{
			return false;
		}

		xml_node Found_Node = Target_Node->Node.find_child_by_attribute(TCHAR_TO_UTF8(*Attribute_Name), TCHAR_TO_UTF8(*Attribute_Value));
		if (!Found_Node)
		{
			return false;
		}

		Out_Child = NewObject<UFFPugiXml_Node>();
		Out_Child->Node = Found_Node;
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Find_Parent(UFFPugiXml_Node*& Found_Parent, bool& bIsParentXml, UFFPugiXml_Node* Target_Node)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node Parent = Target_Node->Node.parent();
	if (!Parent)
	{
		return false;
	}

	xml_node_type ParentNodeType = Parent.type();

	if (ParentNodeType == node_document)
	{
		bIsParentXml = true;
		return true;
	}

	else if (ParentNodeType == node_element)
	{
		Found_Parent = NewObject<UFFPugiXml_Node>();
		Found_Parent->Node = Parent;
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Name(FString& Out_Name, UFFPugiXml_Node* Target_Node)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node_type TargetNodeType = Target_Node->Node.type();

	if (TargetNodeType == node_element || TargetNodeType == node_pi || TargetNodeType == node_declaration)
	{
		Out_Name = Target_Node->Node.name();
		return true;
	}

	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Value(FString& Out_Value, UFFPugiXml_Node* Target_Node)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node_type TargetNodeType = Target_Node->Node.type();

	if (TargetNodeType == node_element && Target_Node->Node.first_child())
	{
		Out_Value = Target_Node->Node.child_value();
		return true;
	}

	else if (TargetNodeType == node_pcdata || TargetNodeType == node_cdata || TargetNodeType == node_comment || TargetNodeType == node_pi || TargetNodeType == node_doctype)
	{
		Out_Value = Target_Node->Node.value();
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Attributes(TMap<FString, FString>& Out_Attributes, UFFPugiXml_Node* Target_Node)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node_type TargetNodeType = Target_Node->Node.type();

	if (TargetNodeType == node_element || TargetNodeType == node_declaration)
	{
		for (xml_attribute Each_Attribute : Target_Node->Node.attributes())
		{
			if (Each_Attribute)
			{
				FString Attribute_Name = Each_Attribute.name();
				FString Attribute_Value = Each_Attribute.value();

				Out_Attributes.Add(Attribute_Name, Attribute_Value);
			}
		}

		return true;
	}
	
	else
	{
		return false;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Check_Empty(bool bIsEmpty, UObject* Target_Object)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (IsValid(Target_Document) && Target_Document->Document)
	{
		bIsEmpty = Target_Document->Document.empty();
		return true;
	}

	UFFPugiXml_Node* Target_Node = Cast<UFFPugiXml_Node>(Target_Object);
	if (IsValid(Target_Node) && Target_Node->Node)
	{
		bIsEmpty = Target_Node->Node.empty();
		return true;
	}

	return false;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Compare_Nodes(bool& bAreTheySame, UFFPugiXml_Node* First_Node, UFFPugiXml_Node* Second_Node)
{
	if (!IsValid(First_Node) && !IsValid(Second_Node))
	{
		return false;
	}

	if (First_Node->Node.hash_value() == Second_Node->Node.hash_value())
	{
		bAreTheySame = true;
		return true;
	}

	else
	{
		bAreTheySame = false;
		return true;
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Compare_XML(bool& bAreTheySame, UFFPugiXml_Doc* First_Document, UFFPugiXml_Doc* Second_Document)
{
	if (!IsValid(First_Document) && !IsValid(Second_Document))
	{
		return false;
	}

	if (First_Document->Document.hash_value() == Second_Document->Document.hash_value())
	{
		bAreTheySame = true;
		return true;
	}

	else
	{
		bAreTheySame = false;
		return true;
	}
}