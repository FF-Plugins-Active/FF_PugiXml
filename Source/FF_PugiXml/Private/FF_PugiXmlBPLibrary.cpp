// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_PugiXmlBPLibrary.h"
#include "FF_PugiXml.h"

// UE Includes.
#include "Kismet/GameplayStatics.h"

THIRD_PARTY_INCLUDES_START
#include <string>
#include <sstream>
#include <iostream>

#include "libdeflate.h"
THIRD_PARTY_INCLUDES_END

UFF_PugiXmlBPLibrary::UFF_PugiXmlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

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