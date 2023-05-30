/*
 * Find_DLL_Dependencies
 * Copyright (C) 2023 Benjamin VERNOUX
 * MIT License (MIT)
*/

/* Visual Studio warnings removed */
#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define APP_NAME "Find_DLL_Dependencies"
#define VERSION "v0.1.0 29/05/2023 B.VERNOUX"

#define BANNER1 APP_NAME " " VERSION "\n"
#define USAGE "usage: " APP_NAME " <win32_64_exe_or_dll>\n"

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

/* Support only Windows x86 & x64 EXE/DLL */
#define IMAGE_FILE_MACHINE_I386 (0x014c) // x86
#define IMAGE_FILE_MACHINE_AMD64 (0x8664) // x64

#define DLL_NAME_MAX_SIZE (256)

/*
References:
https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
https://0xrick.github.io/win-internals/pe1/
https://github.com/0xRick/PE-Parser/tree/main
https://stofu.io/blog/view_post.php?id=10 PE Import Table Parser
*/
typedef struct _IMAGE_DOS_HEADER {
	uint16_t e_magic;
	uint16_t e_cblp;
	uint16_t e_cp;
	uint16_t e_crlc;
	uint16_t e_cparhdr;
	uint16_t e_minalloc;
	uint16_t e_maxalloc;
	uint16_t e_ss;
	uint16_t e_sp;
	uint16_t e_csum;
	uint16_t e_ip;
	uint16_t e_cs;
	uint16_t e_lfarlc;
	uint16_t e_ovno;
	uint16_t e_res[4];
	uint16_t e_oemid;
	uint16_t e_oeminfo;
	uint16_t e_res2[10];
	uint32_t e_lfanew;
} IMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	uint16_t  Machine;
	uint16_t  NumberOfSections;
	uint32_t  TimeDateStamp;
	uint32_t  PointerToSymbolTable;
	uint32_t  NumberOfSymbols;
	uint16_t  SizeOfOptionalHeader;
	uint16_t  Characteristics;
} IMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
  uint32_t VirtualAddress;
  uint32_t Size;
} IMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
typedef struct _IMAGE_OPTIONAL_HEADER32 {
	uint16_t  Magic;
	uint8_t   MajorLinkerVersion;
	uint8_t   MinorLinkerVersion;
	uint32_t  SizeOfCode;
	uint32_t  SizeOfInitializedData;
	uint32_t  SizeOfUninitializedData;
	uint32_t  AddressOfEntryPoint;
	uint32_t  BaseOfCode;
	uint32_t  BaseOfData;
	uint32_t  ImageBase;
	uint32_t  SectionAlignment;
	uint32_t  FileAlignment;
	uint16_t  MajorOperatingSystemVersion;
	uint16_t  MinorOperatingSystemVersion;
	uint16_t  MajorImageVersion;
	uint16_t  MinorImageVersion;
	uint16_t  MajorSubsystemVersion;
	uint16_t  MinorSubsystemVersion;
	uint32_t  Reserved1;
	uint32_t  SizeOfImage;
	uint32_t  SizeOfHeaders;
	uint32_t  CheckSum;
	uint16_t  Subsystem;
	uint16_t  DllCharacteristics;
	uint32_t  SizeOfStackReserve;
	uint32_t  SizeOfStackCommit;
	uint32_t  SizeOfHeapReserve;
	uint32_t  SizeOfHeapCommit;
	uint32_t  LoaderFlags;
	uint32_t  NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
	uint16_t  Magic;
	uint8_t   MajorLinkerVersion;
	uint8_t   MinorLinkerVersion;
	uint32_t  SizeOfCode;
	uint32_t  SizeOfInitializedData;
	uint32_t  SizeOfUninitializedData;
	uint32_t  AddressOfEntryPoint;
	uint32_t  BaseOfCode;
	uint64_t  ImageBase;
	uint32_t  SectionAlignment;
	uint32_t  FileAlignment;
	uint16_t  MajorOperatingSystemVersion;
	uint16_t  MinorOperatingSystemVersion;
	uint16_t  MajorImageVersion;
	uint16_t  MinorImageVersion;
	uint16_t  MajorSubsystemVersion;
	uint16_t  MinorSubsystemVersion;
	uint32_t  Reserved1;
	uint32_t  SizeOfImage;
	uint32_t  SizeOfHeaders;
	uint32_t  CheckSum;
	uint16_t  Subsystem;
	uint16_t  DllCharacteristics;
	uint64_t  SizeOfStackReserve;
	uint64_t  SizeOfStackCommit;
	uint64_t  SizeOfHeapReserve;
	uint64_t  SizeOfHeapCommit;
	uint32_t  LoaderFlags;
	uint32_t  NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS32 {
	uint32_t Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32;

typedef struct _IMAGE_NT_HEADERS64 {
	uint32_t Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64;

typedef struct _IMAGE_SECTION_HEADER {
	char Name[8];
	uint32_t VirtualSize;
	uint32_t VirtualAddress;
	uint32_t SizeOfRawData;
	uint32_t PointerToRawData;
	uint32_t PointerToRelocations;
	uint32_t PointerToLinenumbers;
	uint16_t NumberOfRelocations;
	uint16_t NumberOfLinenumbers;
	uint32_t Characteristics;
} IMAGE_SECTION_HEADER;

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
	union {
		uint32_t Characteristics;
		uint32_t OriginalFirstThunk;
	} DUMMYUNIONNAME;
	uint32_t TimeDateStamp;
	uint32_t ForwarderChain;
	uint32_t Name;
	uint32_t FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

#define Find_DLL_Dependencies_file_error_exit(file_handle) \
    do { \
        fclose(file_handle); \
        printf("File error exit:\nFile: %s\nLine: %d\nFunction: %s\n", __FILE__, __LINE__, __func__); \
        exit(-1); \
    } while (0)

/* Return number of DLL dependencies found >0 or error code */
int Find_DLL_Dependencies(const char* filename, char* dependencies[], size_t num_dependencies) {
	IMAGE_DOS_HEADER dos_header;
	IMAGE_NT_HEADERS32 nt_headers32;
	IMAGE_NT_HEADERS64 nt_headers64;
	IMAGE_SECTION_HEADER section_header;
	int nb_dependencies = 0;
	size_t fread_nb;

	if (dependencies == NULL)
		return -1; // Error dependencies is NULL
	if (num_dependencies < 1)
		return -2; // Error num_dependencies invalid size

	FILE* file = fopen(filename, "rb");
	if (!file) {
		return -3; // Error to open file
	}

	fread_nb = fread(&dos_header, 1, sizeof(IMAGE_DOS_HEADER), file);
	if(fread_nb != sizeof(IMAGE_DOS_HEADER)) {
		Find_DLL_Dependencies_file_error_exit(file);
	}
	if(fseek(file, dos_header.e_lfanew, SEEK_SET) != 0) {
		Find_DLL_Dependencies_file_error_exit(file);
	}
	fread_nb = fread(&nt_headers32.Signature, 1, sizeof(uint32_t), file);
	if(fread_nb != sizeof(uint32_t)) {
		Find_DLL_Dependencies_file_error_exit(file);
	}
	memcpy(&nt_headers64.Signature, &nt_headers32.Signature, sizeof(uint32_t));

	// Signature "PE\0\0"
	if (nt_headers32.Signature != 0x00004550) {
		fclose(file);
		return -4; // Error nt_headers32.Signature != "PE\0\0"
	}
	fread_nb = fread(&nt_headers32.FileHeader, 1, sizeof(IMAGE_FILE_HEADER), file);
	if(fread_nb != sizeof(IMAGE_FILE_HEADER)) {
		Find_DLL_Dependencies_file_error_exit(file);
	}
	memcpy(&nt_headers64.FileHeader, &nt_headers32.FileHeader, sizeof(IMAGE_FILE_HEADER));

	IMAGE_DATA_DIRECTORY import_directory;
	uint16_t FileHeader_NumberOfSections = 0;
	switch(nt_headers32.FileHeader.Machine)
	{
		case IMAGE_FILE_MACHINE_I386: // 32-bit executable / DLL
		{
			fread_nb = fread(&nt_headers32.OptionalHeader, 1, sizeof(IMAGE_OPTIONAL_HEADER32), file);
			if(fread_nb != sizeof(IMAGE_OPTIONAL_HEADER32)) {
				Find_DLL_Dependencies_file_error_exit(file);
			}
			import_directory = nt_headers32.OptionalHeader.DataDirectory[1]; // Set to Import directory
			FileHeader_NumberOfSections = nt_headers32.FileHeader.NumberOfSections;
			break;
		}
		case IMAGE_FILE_MACHINE_AMD64: // 64-bit executable / DLL
		{
			fread_nb = fread(&nt_headers64.OptionalHeader, 1, sizeof(IMAGE_OPTIONAL_HEADER64), file);
			if(fread_nb != sizeof(IMAGE_OPTIONAL_HEADER64)) {
				Find_DLL_Dependencies_file_error_exit(file);
			}
			import_directory = nt_headers64.OptionalHeader.DataDirectory[1]; // Set to Import directory
			FileHeader_NumberOfSections = nt_headers64.FileHeader.NumberOfSections;
			break;
		}
		default:
		{
			fclose(file);
			return -5; // Image file machine not supported
		}
	}

	// Find the section containing the import directory
	int i;
	for (i = 0; i < FileHeader_NumberOfSections; i++) {
		fread_nb = fread(&section_header, 1, sizeof(IMAGE_SECTION_HEADER), file);
		if(fread_nb != sizeof(IMAGE_SECTION_HEADER)) {
			Find_DLL_Dependencies_file_error_exit(file);
		}
		if (section_header.VirtualAddress <= import_directory.VirtualAddress &&
			import_directory.VirtualAddress < (section_header.VirtualAddress + section_header.VirtualSize)) {
			break;
		}
	}
	if (i == FileHeader_NumberOfSections) {
		fclose(file);
		return 0; /* No dependencies found / nt_headers32 or nt_headers64 => FileHeader.NumberOfSections */
	}

	if (import_directory.VirtualAddress == 0 || import_directory.Size == 0) {
		fclose(file);
		return 0; /* No dependencies found / Error import_directory invalid data */
	}

	long fileoffset_import_descriptor = section_header.PointerToRawData + (import_directory.VirtualAddress - section_header.VirtualAddress);
	while (1) {
		IMAGE_IMPORT_DESCRIPTOR import_descriptor;
		if(fseek(file, fileoffset_import_descriptor, SEEK_SET) != 0) {
			Find_DLL_Dependencies_file_error_exit(file);
		}
		fread_nb = fread(&import_descriptor, 1, sizeof(IMAGE_IMPORT_DESCRIPTOR), file);
		if(fread_nb != sizeof(IMAGE_IMPORT_DESCRIPTOR)) {
			Find_DLL_Dependencies_file_error_exit(file);
		}
		fileoffset_import_descriptor += sizeof(IMAGE_IMPORT_DESCRIPTOR);

		if (import_descriptor.Name == 0 && import_descriptor.FirstThunk == 0)
			break;

		if (nb_dependencies < num_dependencies) {
			if(fseek(file, section_header.PointerToRawData + (import_descriptor.Name - section_header.VirtualAddress), SEEK_SET) != 0) {
				Find_DLL_Dependencies_file_error_exit(file);
			}
			char dll_name[DLL_NAME_MAX_SIZE];
			fread_nb = fread(dll_name, 1, DLL_NAME_MAX_SIZE, file);
			if(fread_nb != DLL_NAME_MAX_SIZE) {
				Find_DLL_Dependencies_file_error_exit(file);
			}
			size_t j;
			for (j = 0; j < DLL_NAME_MAX_SIZE; j++) {
				if (dll_name[j] == '\0') {
					break;
				}
			}
			dependencies[nb_dependencies++] = strdup(dll_name);
		} else {
			fclose(file);
			return nb_dependencies;
		}
	}
	fclose(file);
	return nb_dependencies;
}

void Free_DLL_Dependencies(char* dependencies[], size_t num_dependencies)
{
	for (size_t i = 0; i < num_dependencies; i++)
	{
		free(dependencies[i]);
	}
}

/*
int main(int argc, char* argv[]) {

	#define MAX_DEPS 10000
	printf(BANNER1);

	if (argc < 2) {
		printf(USAGE);
		return -1;
	}

	const char* filename = argv[1];
	char* dependencies[MAX_DEPS]; // Adjust the array size as per your requirements
	size_t num_dependencies = ARRAY_LENGTH(dependencies);

	int num_deps = Find_DLL_Dependencies(filename, dependencies, num_dependencies);
	if (num_deps == 0)
	{
		printf("No dependencies found.\n");
	}else if (num_deps > 0) {
		printf("Dependencies(%d):\n", num_deps);
		for (size_t i = 0; i < num_deps; i++) {
			if (dependencies[i] != NULL) {
				printf("%s\n", dependencies[i]);
			}
		}
		Free_DLL_Dependencies(dependencies, num_deps);
	} else {
		printf("Find_DLL_Dependencies error %d.\n", num_deps);
	}
	return 0;
}
*/

