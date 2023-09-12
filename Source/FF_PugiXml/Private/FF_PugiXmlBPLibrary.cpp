// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_PugiXmlBPLibrary.h"
#include "FF_PugiXml.h"

// UE Includes.
#include "Kismet/GameplayStatics.h"

THIRD_PARTY_INCLUDES_START
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

//#include "libdeflate.h"
THIRD_PARTY_INCLUDES_END

UFF_PugiXmlBPLibrary::UFF_PugiXmlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

/*
void UFF_PugiXmlBPLibrary::LibDeflateTest(FDelegateDeflate DelegateDeflate, TArray<uint8> In_Bytes)
{
#ifdef _WIN64

	AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [DelegateDeflate, In_Bytes]()
		{
			libdeflate_decompressor* DeCompressor = libdeflate_alloc_decompressor();

			if (!DeCompressor)
			{
				AsyncTask(ENamedThreads::GameThread, [DelegateDeflate]()
					{
						FDeflateUncompressed EmptyStruct;
						DelegateDeflate.ExecuteIfBound(false, EmptyStruct, "Deflate decompressor is not valid.");
					}
				);

				return;
			}

			int32 TargetMb = 1024;
			size_t UpplerLimit = static_cast<size_t>(TargetMb * 1024 * 1024);
			size_t ActualSize = 0;
			libdeflate_result Result = LIBDEFLATE_SUCCESS;

			Result = libdeflate_gzip_decompress(DeCompressor, In_Bytes.GetData(), In_Bytes.Num(), NULL, UpplerLimit, &ActualSize);
			if (Result != LIBDEFLATE_SUCCESS)
			{
				libdeflate_free_decompressor(DeCompressor);
				DeCompressor = nullptr;

				AsyncTask(ENamedThreads::GameThread, [DelegateDeflate, Result]()
					{
						FDeflateUncompressed EmptyStruct;
						DelegateDeflate.ExecuteIfBound(false, EmptyStruct, "Actual size calculation unsuccessful.");
						UE_LOG(LogTemp, Warning, TEXT("Libdeflate Error = %d"), (int)Result)
					}
				);

				return;
			}

			uint8* UncompressedBuffer = (uint8*)malloc(ActualSize);
			libdeflate_gzip_decompress(DeCompressor, In_Bytes.GetData(), In_Bytes.Num(), UncompressedBuffer, ActualSize, NULL);
			if (Result != LIBDEFLATE_SUCCESS)
			{
				libdeflate_free_decompressor(DeCompressor);
				DeCompressor = nullptr;

				AsyncTask(ENamedThreads::GameThread, [DelegateDeflate]()
					{
						FDeflateUncompressed EmptyStruct;
						DelegateDeflate.ExecuteIfBound(false, EmptyStruct, "Deflate decompression unsuccessful.");
					}
				);

				return;
			}

			FDeflateUncompressed Out_Uncompressed;
			Out_Uncompressed.UncompressedBuffer.SetNum(ActualSize);
			FMemory::Memcpy(Out_Uncompressed.UncompressedBuffer.GetData(), UncompressedBuffer, ActualSize);

			libdeflate_free_decompressor(DeCompressor);
			DeCompressor = nullptr;

			if (!Out_Uncompressed.UncompressedBuffer.GetData())
			{
				AsyncTask(ENamedThreads::GameThread, [DelegateDeflate]()
					{
						FDeflateUncompressed EmptyStruct;
						DelegateDeflate.ExecuteIfBound(false, EmptyStruct, "Decompressed buffer is not valid.");
					}
				);

				return;
			}

			AsyncTask(ENamedThreads::GameThread, [DelegateDeflate, Out_Uncompressed]()
				{
					DelegateDeflate.ExecuteIfBound(true, Out_Uncompressed, "Deflate decompression successful.");
				}
			);
		}
	);

#else

	FDeflateUncompressed EmptyStruct;
	DelegateDeflate.ExecuteIfBound(false, EmptyStruct, "This function is only for Windows");

#endif // _WIN64
}
*/

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

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Print(UFFPugiXml_Doc* In_Doc, FString& Out_String)
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

bool UFF_PugiXmlBPLibrary::PugiXml_Doc_Save(UFFPugiXml_Doc* In_Doc, FString In_Path)
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

void UFF_PugiXmlBPLibrary::PugiXml_Doc_Create(UFFPugiXml_Doc*& Out_Doc, FString RootName, FString DoctypeName, bool bIsStandalone, bool bAddDoctype)
{
	Out_Doc = NewObject<UFFPugiXml_Doc>();

	xml_node Declaration = Out_Doc->Document.prepend_child(node_declaration);
	Declaration.append_attribute("version") = "1.0";
	Declaration.append_attribute("encoding") = "utf-8";
	Declaration.append_attribute("standalone") = bIsStandalone ? "yes" : "no";

	if (bAddDoctype && !DoctypeName.IsEmpty())
	{
		// DTD delimiter comment.
		Out_Doc->Document.append_child(node_comment).set_value("--DTD--");
		
		xml_node Doctype = Out_Doc->Document.append_child(node_doctype);
		Doctype.set_value(TCHAR_TO_UTF8(*DoctypeName));
		
		// XML delimiter comment.
		Out_Doc->Document.append_child(node_comment).set_value("--XML--");
	}

	if (!RootName.IsEmpty())
	{
		Out_Doc->Root = Out_Doc->Document.append_child(node_element);
		Out_Doc->Root.set_name(TCHAR_TO_UTF8(*RootName));
		Out_Doc->Root.append_attribute("xmlns:xsd") = "http://www.w3.org/2001/XMLSchema";
		Out_Doc->Root.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
	}
}

bool UFF_PugiXmlBPLibrary::PugiXml_Add_Node_Element(UFFPugiXml_Node*& Out_Node, UFFPugiXml_Doc* In_Doc, UFFPugiXml_Node* Parent_Node, FString NodeName, FString NodeValue, TMap<FString, FString> Attributes)
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
		if (In_Doc->Root)
		{
			Out_Node->Node = In_Doc->Root.append_child(node_element);
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

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Children(TArray<UFFPugiXml_Node*>& Out_Children, UObject* Target_Object)
{
	if (!IsValid(Target_Object))
	{
		return false;
	}

	UFFPugiXml_Doc* Target_Document = Cast<UFFPugiXml_Doc>(Target_Object);
	if (Target_Document && Target_Document->Document)
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
	if (Target_Node && Target_Node->Node)
	{
		if (Target_Node->Node.type() == node_document || Target_Node->Node.type() == node_element)
		{
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

		else
		{
			return false;
		}
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
	}

	return true;
}

bool UFF_PugiXmlBPLibrary::PugiXml_Get_Value(FString& Out_Value, UFFPugiXml_Node* Target_Node)
{
	if (!IsValid(Target_Node))
	{
		return false;
	}

	xml_node_type TargetNodeType = Target_Node->Node.type();

	if (TargetNodeType == node_element)
	{
		if (!Target_Node->Node.first_child())
		{
			return false;
		}

		TargetNodeType = Target_Node->Node.first_child().type();
		
		if (TargetNodeType == node_pcdata || TargetNodeType == node_cdata || TargetNodeType == node_comment || TargetNodeType == node_pi || TargetNodeType == node_doctype)
		{	
			Out_Value = Target_Node->Node.first_child().value();
			return true;
		}
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