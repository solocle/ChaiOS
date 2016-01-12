/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: PortableExecutable.cpp
Path: c:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\PortableExecutable.cpp
Created by Nathaniel on 24/12/2014 at 13:47

Description: Portable Executable Loader
**********************************************************/
#include "PortableExecutable.h"
#include "memory.h"
#include "terminal_basic.h"

namespace PortableExecutable {
#pragma pack(push, 1)
	typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
		uint16_t e_magic;		// must contain "MZ"
		uint16_t e_cblp;		// number of bytes on the last page of the file
		uint16_t e_cp;		// number of pages in file
		uint16_t e_crlc;		// relocations
		uint16_t e_cparhdr;		// size of the header in paragraphs
		uint16_t e_minalloc;	// minimum and maximum paragraphs to allocate
		uint16_t e_maxalloc;
		uint16_t e_ss;		// initial SS:SP to set by Loader
		uint16_t e_sp;
		uint16_t e_csum;		// checksum
		uint16_t e_ip;		// initial CS:IP
		uint16_t e_cs;
		uint16_t e_lfarlc;		// address of relocation table
		uint16_t e_ovno;		// overlay number
		uint16_t e_res[4];		// resevered
		uint16_t e_oemid;		// OEM id
		uint16_t e_oeminfo;		// OEM info
		uint16_t e_res2[10];	// reserved
		uint32_t   e_lfanew;	// address of new EXE header
	} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

	typedef struct _IMAGE_FILE_HEADER {
		uint16_t  Machine;
		uint16_t  NumberOfSections;			// Number of sections in section table
		uint32_t   TimeDateStamp;			// Date and time of program link
		uint32_t   PointerToSymbolTable;		// RVA of symbol table
		uint32_t   NumberOfSymbols;			// Number of symbols in table
		uint16_t  SizeOfOptionalHeader;		// Size of IMAGE_OPTIONAL_HEADER in bytes
		uint16_t  Characteristics;
	} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

	enum DataDirectories {
		 DATA_DIRECTORY_EXPORT = 0,
		 DATA_DIRECTORY_IMPORT = 1,
		 DATA_DIRECTORY_RESOURCE = 2,
		 DATA_DIRECTORY_EXCEPTION = 3,
		 DATA_DIRECTORY_CERTIFICATE = 4,
		 DATA_DIRECTORY_RELOCATION = 5,
		 DATA_DIRECTORY_DEBUG = 6,
		 DATA_DIRECTORY_ARCH = 7,
		 DATA_DIRECTORY_GLOBALPTR = 8,
		 DATA_DIRECTORY_TLS = 9,
		 DATA_DIRECTORY_LOADCFG = 10,
		 DATA_DIRECTORY_BOUND_IMPORT = 11,
		 DATA_DIRECTORY_IAT = 12,
		 DATA_DIRECTORY_DELAYIMPORT = 13,
		 DATA_DIRECTORY_CLR = 14,
		 DATA_DIRECTORY_RESERVED = 15
	};

	typedef struct _IMAGE_DATA_DIRECTORY {
		uint32_t VirtualAddress;		// RVA of table
		uint32_t Size;			// size of table
	} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

	typedef struct _IMAGE_IMPORT_DESCRIPTOR {
		union {
			uint32_t   Characteristics;		// 0 for terminating null import descriptor
			uint32_t   OriginalFirstThunk;		// RVA to INT
		};
		uint32_t   TimeDateStamp;			// Time/Date of module, or other properties (see below)
		uint32_t   ForwarderChain;			// Forwarder chain ID
		uint32_t   Name;				// Module name
		uint32_t   FirstThunk;			// RVA to IAT (if bound this IAT has actual addresses)
	} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

	typedef struct _IMAGE_IMPORT_BY_NAME {
		uint16_t  Hint;		// Possible ordinal number to use
		char   Name[1];	// Name of function, null terminated
	} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

	typedef struct _IMAGE_THUNK_DATA {
		uint32_t data : 31;
		uint32_t Ordinal : 1;		//Is this import by ordinal?
	} IMAGE_THUNK_DATA, *PIMAGE_THUNK_DATA;

	typedef struct _IMAGE_THUNK_DATA64 {
		uint64_t data : 63;
		uint64_t Ordinal : 1;		//Is this import by ordinal?
	} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

	typedef struct _IMAGE_EXPORT_DIRECTORY {
		uint32_t Characteristics;
		uint32_t TimeDateStamp;
		uint16_t MajorVersion;
		uint16_t MinorVersion;
		uint32_t Name;
		uint32_t Base;
		uint32_t NumberOfFunctions;
		uint32_t NumberOfNames;
		uint32_t AddressOfFunctions;
		uint32_t AddressOfNames;
		uint32_t AddressOfNameOrdinal;
	}IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

	typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
		union {
			struct {
				uint32_t NameOffset : 31;
				uint32_t NameIsString : 1;
			}strucName;
			uint32_t   Name;
			uint16_t    Id;
		};
		union {
			uint32_t   OffsetToData;
			struct {
				uint32_t   OffsetToDirectory : 31;
				uint32_t   DataIsDirectory : 1;
			}strucOffset;
		};
	} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

	typedef struct _IMAGE_RESOURCE_DIRECTORY {
		uint32_t                       Characteristics;
		uint32_t                       TimeDateStamp;
		uint16_t                       MajorVersion;
		uint16_t                       MinorVersion;
		uint16_t                       NumberOfNamedEntries;
		uint16_t                       NumberOfIdEntries;
		IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[1];
	} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

	typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
		uint32_t   OffsetToData;
		uint32_t   Size;
		uint32_t   CodePage;
		uint32_t   Reserved;
	} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

	typedef struct _IMAGE_OPTIONAL_HEADER {

		uint16_t  Magic;				// not-so-magical number
		uint8_t   MajorLinkerVersion;			// linker version
		uint8_t   MinorLinkerVersion;
		uint32_t   SizeOfCode;				// size of .text in bytes
		uint32_t   SizeOfInitializedData;		// size of .bss (and others) in bytes
		uint32_t   SizeOfUninitializedData;		// size of .data,.sdata etc in bytes
		uint32_t   AddressOfEntryPoint;		// RVA of entry point
		uint32_t   BaseOfCode;				// base of .text
		uint32_t   BaseOfData;				// base of .data
		uint32_t   ImageBase;				// image base VA
		uint32_t   SectionAlignment;			// file section alignment
		uint32_t   FileAlignment;			// file alignment
		uint16_t  MajorOperatingSystemVersion;	// Windows specific. OS version required to run image
		uint16_t  MinorOperatingSystemVersion;
		uint16_t  MajorImageVersion;			// version of program
		uint16_t  MinorImageVersion;
		uint16_t  MajorSubsystemVersion;		// Windows specific. Version of SubSystem
		uint16_t  MinorSubsystemVersion;
		uint32_t   Reserved1;
		uint32_t   SizeOfImage;			// size of image in bytes
		uint32_t   SizeOfHeaders;			// size of headers (and stub program) in bytes
		uint32_t   CheckSum;				// checksum
		uint16_t  Subsystem;				// Windows specific. subsystem type
		uint16_t  DllCharacteristics;			// DLL properties
		uint32_t   SizeOfStackReserve;			// size of stack, in bytes
		uint32_t   SizeOfStackCommit;			// size of stack to commit
		uint32_t   SizeOfHeapReserve;			// size of heap, in bytes
		uint32_t   SizeOfHeapCommit;			// size of heap to commit
		uint32_t   LoaderFlags;			// no longer used
		uint32_t   NumberOfRvaAndSizes;		// number of DataDirectory entries
		IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

	typedef struct _IMAGE_OPTIONAL_HEADER_64 {

		uint16_t  Magic;				// not-so-magical number
		uint8_t   MajorLinkerVersion;			// linker version
		uint8_t   MinorLinkerVersion;
		uint32_t   SizeOfCode;				// size of .text in bytes
		uint32_t   SizeOfInitializedData;		// size of .bss (and others) in bytes
		uint32_t   SizeOfUninitializedData;		// size of .data,.sdata etc in bytes
		uint32_t   AddressOfEntryPoint;		// RVA of entry point
		uint32_t   BaseOfCode;				// base of .text

		uint64_t   ImageBase;				// image base VA
		uint32_t   SectionAlignment;			// file section alignment
		uint32_t   FileAlignment;			// file alignment
		uint16_t  MajorOperatingSystemVersion;	// Windows specific. OS version required to run image
		uint16_t  MinorOperatingSystemVersion;
		uint16_t  MajorImageVersion;			// version of program
		uint16_t  MinorImageVersion;
		uint16_t  MajorSubsystemVersion;		// Windows specific. Version of SubSystem
		uint16_t  MinorSubsystemVersion;
		uint32_t   Reserved1;
		uint32_t   SizeOfImage;			// size of image in bytes
		uint32_t   SizeOfHeaders;			// size of headers (and stub program) in bytes
		uint32_t   CheckSum;				// checksum
		uint16_t  Subsystem;				// Windows specific. subsystem type
		uint16_t  DllCharacteristics;			// DLL properties
		uint64_t   SizeOfStackReserve;			// size of stack, in bytes
		uint64_t   SizeOfStackCommit;			// size of stack to commit
		uint64_t   SizeOfHeapReserve;			// size of heap, in bytes
		uint64_t   SizeOfHeapCommit;			// size of heap to commit
		uint32_t   LoaderFlags;			// no longer used
		uint32_t   NumberOfRvaAndSizes;		// number of DataDirectory entries
		IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

#define IMAGE_SIZEOF_SHORT_NAME 8
	typedef struct _IMAGE_SECTION_HEADER {
		char  Name[IMAGE_SIZEOF_SHORT_NAME];
		union {
			uint32_t PhysicalAddress;
			uint32_t VirtualSize;
		} Misc;
		uint32_t VirtualAddress;
		uint32_t SizeOfRawData;
		uint32_t PointerToRawData;
		uint32_t PointerToRelocations;
		uint32_t PointerToLinenumbers;
		uint16_t  NumberOfRelocations;
		uint16_t  NumberOfLinenumbers;
		uint32_t Characteristics;
	} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

	typedef struct _IMAGE_NT_HEADERS {
		uint32_t                 Signature;
		IMAGE_FILE_HEADER     FileHeader;
		IMAGE_OPTIONAL_HEADER OptionalHeader;
	} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;
	typedef struct _IMAGE_NT_HEADERS64 {
		uint32_t                 Signature;
		IMAGE_FILE_HEADER     FileHeader;
		IMAGE_OPTIONAL_HEADER64 OptionalHeader;
	} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;
	typedef struct _RELOC_TABLE_FIXUP_BLOCK_ETNRY {
		unsigned short offset : 12;		//bitfields grow LSB->MSB
		unsigned char type : 4;			//high 4 bits are type
	}RELOC_TABLE_FIXUP_BLOCK_ETNRY, *PRELOC_TABLE_FIXUP_BLOCK_ETNRY;
	typedef struct _IMAGE_RELOCATION_TABLE_ENTRY {
		uint32_t PageRVA;		//Each entry represents one page
		uint32_t BlockSize;	//Size of Block, including all fields
		RELOC_TABLE_FIXUP_BLOCK_ETNRY entries[1024];	//entries in this fixup block	
	}IMAGE_RELOCATION_TABLE_ENTRY, *PIMAGE_RELOCATION_TABLE_ENTRY;
#pragma pack(pop)
}

using namespace PortableExecutable;

CPortableExecutable::CPortableExecutable(void* address, char* name)
:m_pPE(address)
{
	int n = 0;
	for (; name[n] && n < 63; n++)
	{
		m_name[n] = name[n];
	}
	m_name[n] = 0;
}


CPortableExecutable::~CPortableExecutable()
{
}


// Gets the base address of the PE
size_t CPortableExecutable::getBaseAddress()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	if (nt_hdr->OptionalHeader.Magic == 0x010B)
		return nt_hdr->OptionalHeader.ImageBase;
	else if (nt_hdr->OptionalHeader.Magic == 0x020B)
		return ((PIMAGE_NT_HEADERS64)nt_hdr)->OptionalHeader.ImageBase;
	return NULL;
}

void CPortableExecutable::CallEntry()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	if (nt_hdr->OptionalHeader.Magic == 0x010B)
		return CallEntry32();
	else if (nt_hdr->OptionalHeader.Magic == 0x020B)
		return CallEntry64();
	return;
}

void CPortableExecutable::CallEntry32()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	void(*entry)() = (void(*)())((size_t)m_pPE + nt_hdr->OptionalHeader.AddressOfEntryPoint);
	if (!nt_hdr->OptionalHeader.AddressOfEntryPoint)
		return;
	return entry();
}

void CPortableExecutable::CallEntry64()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS64 nt_hdr = (PIMAGE_NT_HEADERS64)((size_t)m_pPE + dos_hdr->e_lfanew);
	void(*entry)() = (void(*)())((size_t)m_pPE + nt_hdr->OptionalHeader.AddressOfEntryPoint);
	if (!nt_hdr->OptionalHeader.AddressOfEntryPoint)
		return;
	return entry();
}

void(*CPortableExecutable::GetProcAddress(char* FuncName))()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	if (nt_hdr->OptionalHeader.Magic == 0x010B)
		return GetProcAddress32(FuncName);
	else if (nt_hdr->OptionalHeader.Magic == 0x020B)
		return GetProcAddress64(FuncName);
	return NULL;
}

CPortableExecutable::MACHINE_ID CPortableExecutable::getMachineID()
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	return (MACHINE_ID)nt_hdr->FileHeader.Machine;
}

static char toUpper(char c)
{
	if ('a' <= c && c <= 'z')
		return (c - 'a') + 'A';
	else
		return c;
}

static int strcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2))
		s1++, s2++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

static int strcasecmp(const char* s1, const char* s2)
{
	while (*s1 && (toUpper(*s1) == toUpper(*s2)))
		s1++, s2++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void(*CPortableExecutable::GetProcAddress32(char* FuncName))()
{
	PIMAGE_DOS_HEADER dos_hdr_dll = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr_dll = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr_dll->e_lfanew);
	size_t dllactualBase = (size_t)m_pPE;
	if (nt_hdr_dll->OptionalHeader.DataDirectory[0].Size == 0)
		return 0;
	size_t addr = dllactualBase + nt_hdr_dll->OptionalHeader.DataDirectory[0].VirtualAddress;
	//resource directory
	PIMAGE_EXPORT_DIRECTORY expDir = (PIMAGE_EXPORT_DIRECTORY)addr;
	uint32_t* FunctionNameAddressArray = (uint32_t*)((uint32_t)expDir->AddressOfNames + dllactualBase);
	uint16_t* FunctionOrdinalAddressArray = (uint16_t*)((uint32_t)expDir->AddressOfNameOrdinal + dllactualBase);
	uint32_t* FunctionAddressArray = (uint32_t*)((uint32_t)expDir->AddressOfFunctions + dllactualBase);
	for (unsigned int n = 0; n < expDir->NumberOfFunctions; n++)
	{
		char* FunctionName = (char*)(FunctionNameAddressArray[n] + dllactualBase);
		if (strcmp(FunctionName, FuncName) == 0) {
			uint16_t Ordinal = FunctionOrdinalAddressArray[n] + expDir->Base;		//Ordinals are biased by the ordinal base
			uint32_t FunctionAddress = FunctionAddressArray[Ordinal - expDir->Base];
#if 0
			kputs_basic(FunctionName);
			kputs_basic(" -> ");
			kputhex_basic((FunctionAddress + dllactualBase));
			kputs_basic("\n");
			kputhex_basic(Ordinal);
			kputs_basic(" -> ");
			kputhex_basic(n);
			kputs_basic("\n");
#endif
			return (void(*)())(FunctionAddress + dllactualBase);
		}
	}
	return NULL;
}

void(*CPortableExecutable::GetProcAddress64(char* FuncName))()
{
	PIMAGE_DOS_HEADER dos_hdr_dll = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS64 nt_hdr_dll = (PIMAGE_NT_HEADERS64)((size_t)m_pPE + dos_hdr_dll->e_lfanew);
	size_t dllactualBase = (size_t)m_pPE;
	if (nt_hdr_dll->OptionalHeader.DataDirectory[0].Size == 0)
		return 0;
	size_t addr = dllactualBase + nt_hdr_dll->OptionalHeader.DataDirectory[0].VirtualAddress;
	//resource directory
	PIMAGE_EXPORT_DIRECTORY expDir = (PIMAGE_EXPORT_DIRECTORY)addr;
	uint32_t* FunctionNameAddressArray = (uint32_t*)((uint64_t)expDir->AddressOfNames + dllactualBase);
	uint16_t* FunctionOrdinalAddressArray = (uint16_t*)((uint64_t)expDir->AddressOfNameOrdinal + dllactualBase);
	uint32_t* FunctionAddressArray = (uint32_t*)((uint64_t)expDir->AddressOfFunctions + dllactualBase);
	for (unsigned int n = 0; n < expDir->NumberOfFunctions; n++)
	{
		char* FunctionName = (char*)(FunctionNameAddressArray[n] + dllactualBase);
		if (strcmp(FunctionName, FuncName) == 0) {
			uint16_t Ordinal = FunctionOrdinalAddressArray[n] + expDir->Base;
			uint32_t FunctionAddress = FunctionAddressArray[Ordinal - expDir->Base];
			return (void(*)())(FunctionAddress + dllactualBase);
		}
	}
	//Something went wrong
	return NULL;
}

bool CPortableExecutable::linkPESymbols(void* pDLL, char* DLLName)
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	if (nt_hdr->OptionalHeader.Magic == 0x010B)
		return linkPESymbols32(pDLL, DLLName);
	else if (nt_hdr->OptionalHeader.Magic == 0x020B)
		return linkPESymbols64(pDLL, DLLName);
	return false;
}

bool CPortableExecutable::linkPESymbols32(void* pDLL, char* DLLName)
{
	PIMAGE_DOS_HEADER dos_hdr_exe = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr_exe = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr_exe->e_lfanew);
	CPortableExecutable dll(pDLL, DLLName);
	if (dll.getMachineID() != getMachineID())
		return false;
	size_t exeactualBase = (size_t)m_pPE;
	if (nt_hdr_exe->OptionalHeader.DataDirectory[1].Size == 0)
		return true;
	//resource directory
	size_t addr = exeactualBase + nt_hdr_exe->OptionalHeader.DataDirectory[1].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR impDir = (PIMAGE_IMPORT_DESCRIPTOR)addr;
	for (int n = 0; impDir[n].Characteristics; n++)
	{
		if (strcasecmp((char*)(impDir[n].Name + exeactualBase), DLLName) == 0)
		{
			PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)(impDir[n].FirstThunk + exeactualBase);
			PIMAGE_THUNK_DATA origthunk = (PIMAGE_THUNK_DATA)(impDir[n].OriginalFirstThunk + exeactualBase);
			for (unsigned int x = 0; *(size_t*)origthunk; x++)
			{
				if (thunk->Ordinal)
				{
					//TODO: Support Ordinals
					return false;
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME imp = (PIMAGE_IMPORT_BY_NAME)(thunk->data + exeactualBase);
					//Now find the export
					void(*exported)();
					exported = dll.GetProcAddress32((char*)imp->Name);
					if (!exported)
					{
						return false;
					}
					*(size_t*)thunk = (size_t)exported;
				}
				thunk++;
				origthunk++;
			}
		}
	}
	return true;
}

bool CPortableExecutable::linkPESymbols64(void* pDLL, char* DLLName)
{
	PIMAGE_DOS_HEADER dos_hdr_exe = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS64 nt_hdr_exe = (PIMAGE_NT_HEADERS64)((size_t)m_pPE + dos_hdr_exe->e_lfanew);
	CPortableExecutable dll(pDLL, DLLName);
	if (dll.getMachineID() != getMachineID())
		return false;
	size_t exeactualBase = (size_t)m_pPE;
	//resource directory
	size_t addr = exeactualBase + nt_hdr_exe->OptionalHeader.DataDirectory[1].VirtualAddress;
	if (nt_hdr_exe->OptionalHeader.DataDirectory[1].Size == 0)
		return true;
	PIMAGE_IMPORT_DESCRIPTOR impDir = (PIMAGE_IMPORT_DESCRIPTOR)addr;
	for (unsigned int n = 0; impDir[n].Characteristics; n++)
	{
		if (strcasecmp((char*)(impDir[n].Name + exeactualBase), DLLName) == 0)
		{
			PIMAGE_THUNK_DATA64 thunk = (PIMAGE_THUNK_DATA64)(impDir[n].FirstThunk + exeactualBase);
			for (int x = 0; *(size_t*)thunk; x++)
			{
				if (thunk->Ordinal)
				{
					//TODO: Support Ordinals
					kpanic_basic("Ordinals Unsupported", 0x999);
					return false;
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME imp = (PIMAGE_IMPORT_BY_NAME)(thunk->data + exeactualBase);
					//kputs_basic((char*)imp->Name);
					//Now find the export
					void(*exported)();
					exported = dll.GetProcAddress64((char*)imp->Name);
					if (!exported)
					{
						kpanic_basic((char*)imp->Name, 0x999);
						return false;
					}
					*(size_t*)thunk = (size_t)exported;
				}
				thunk++;
			}
		}
	}
	return true;
}

CPortableExecutable* CPortableExecutable::loadIntoMemory(const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t fileLen)
{
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	if (nt_hdr->OptionalHeader.Magic == 0x010B)
		return loadIntoMemory32(attr, fileLen);
	else if (nt_hdr->OptionalHeader.Magic == 0x020B)
		return loadIntoMemory64(attr, fileLen);
	return NULL;

}

CPortableExecutable* CPortableExecutable::loadIntoMemory32(const CVMemMngr::PAGING_ATTRIBUTES& attr,size_t fileLen)
{
#if 0
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	size_t prefferedBase = getBaseAddress();
	size_t sizeInMem = nt_hdr->OptionalHeader.SizeOfImage;
	virtaddr newExe = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)prefferedBase, sizeInMem, attr);
	if (prefferedBase != (size_t)newExe)
	{
		//OK, we can't load it at the preferred load address
		//We should relocate the exe
		kputs_basic("Error loading module: Cannot load at preffered load address\n");
		kputhex_basic(prefferedBase);
		kputs_basic("\n");
		return NULL;
	}
	//OK, we have the new EXE address
	//Copy across headers
	memcpy(newExe, m_pPE, nt_hdr->OptionalHeader.SizeOfHeaders);
	PIMAGE_DOS_HEADER dos_hdr_new = (PIMAGE_DOS_HEADER)newExe;
	PIMAGE_NT_HEADERS nt_hdr_new = (PIMAGE_NT_HEADERS)((size_t)newExe + dos_hdr_new->e_lfanew);
	//Load sections
	//Section header after optional header
	PIMAGE_SECTION_HEADER sectionHeaders = (PIMAGE_SECTION_HEADER)((size_t)nt_hdr_new + nt_hdr_new->FileHeader.SizeOfOptionalHeader);
	void* offset = (void*)(((nt_hdr->OptionalHeader.SizeOfHeaders - 1) / nt_hdr->OptionalHeader.SectionAlignment + 1)*nt_hdr->OptionalHeader.SectionAlignment);
	for (int n = 0; n < nt_hdr->FileHeader.NumberOfSections; n++)
	{
		void* section = (void*)((size_t)m_pPE + sectionHeaders[n].PointerToRawData);
		sectionHeaders[n].VirtualAddress = (size_t)offset;
		memcpy(offset, section, sectionHeaders[n].SizeOfRawData);
		sectionHeaders[n].Misc.VirtualSize = ((sectionHeaders[n].SizeOfRawData - 1) / nt_hdr->OptionalHeader.SectionAlignment + 1)*nt_hdr->OptionalHeader.SectionAlignment;
		offset = (void*)((size_t)offset + sectionHeaders[n].Misc.VirtualSize);
	}
	//Update everything...
#else
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	size_t prefferedBase = getBaseAddress();
	size_t sizeInMem = nt_hdr->OptionalHeader.SizeOfImage;
	virtaddr newExe = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)prefferedBase, sizeInMem, attr);
	if (prefferedBase != (size_t)newExe)
	{
		//OK, we can't load it at the preferred load address
		//We should relocate the exe
		kputs_basic("Error loading module: Cannot load at preffered load address\n");
		kputhex_basic(prefferedBase);
		kputs_basic("\n");
		return NULL;
	}
	//OK, we have the new EXE address
	memcpy(newExe, m_pPE, fileLen);
#endif
	CPortableExecutable* loaded = new CPortableExecutable(newExe, m_name);
	return loaded;
}

CPortableExecutable* CPortableExecutable::loadIntoMemory64(const CVMemMngr::PAGING_ATTRIBUTES& attr, size_t fileLen)
{
#if 0
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)((size_t)m_pPE + dos_hdr->e_lfanew);
	size_t prefferedBase = getBaseAddress();
	size_t sizeInMem = nt_hdr->OptionalHeader.SizeOfImage;
	virtaddr newExe = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)prefferedBase, sizeInMem, attr);
	if (prefferedBase != (size_t)newExe)
	{
		//OK, we can't load it at the preferred load address
		//We should relocate the exe
		kputs_basic("Error loading module: Cannot load at preffered load address\n");
		kputhex_basic(prefferedBase);
		kputs_basic("\n");
		return NULL;
	}
	//OK, we have the new EXE address
	//Copy across headers
	memcpy(newExe, m_pPE, nt_hdr->OptionalHeader.SizeOfHeaders);
	PIMAGE_DOS_HEADER dos_hdr_new = (PIMAGE_DOS_HEADER)newExe;
	PIMAGE_NT_HEADERS nt_hdr_new = (PIMAGE_NT_HEADERS)((size_t)newExe + dos_hdr_new->e_lfanew);
	//Load sections
	//Section header after optional header
	PIMAGE_SECTION_HEADER sectionHeaders = (PIMAGE_SECTION_HEADER)((size_t)nt_hdr_new + nt_hdr_new->FileHeader.SizeOfOptionalHeader);
	void* offset = (void*)(((nt_hdr->OptionalHeader.SizeOfHeaders - 1) / nt_hdr->OptionalHeader.SectionAlignment + 1)*nt_hdr->OptionalHeader.SectionAlignment);
	for (int n = 0; n < nt_hdr->FileHeader.NumberOfSections; n++)
	{
		void* section = (void*)((size_t)m_pPE + sectionHeaders[n].PointerToRawData);
		sectionHeaders[n].VirtualAddress = (size_t)offset;
		memcpy(offset, section, sectionHeaders[n].SizeOfRawData);
		sectionHeaders[n].Misc.VirtualSize = ((sectionHeaders[n].SizeOfRawData - 1) / nt_hdr->OptionalHeader.SectionAlignment + 1)*nt_hdr->OptionalHeader.SectionAlignment;
		offset = (void*)((size_t)offset + sectionHeaders[n].Misc.VirtualSize);
	}
	//Update everything...
#else
	PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)m_pPE;
	PIMAGE_NT_HEADERS64 nt_hdr = (PIMAGE_NT_HEADERS64)((size_t)m_pPE + dos_hdr->e_lfanew);
	size_t prefferedBase = getBaseAddress();
	size_t sizeInMem = nt_hdr->OptionalHeader.SizeOfImage;
	virtaddr newExe = getMemoryManager()->getVMemMngr()->allocateAt((virtaddr)prefferedBase, sizeInMem, attr);
	if (prefferedBase != (size_t)newExe)
	{
		//OK, we can't load it at the preferred load address
		//We should relocate the exe
		kputs_basic("Error loading module: Cannot load at preffered load address\n");
		kputhex_basic(prefferedBase);
		kputs_basic("\n");
		return NULL;
	}
	//OK, we have the new EXE address
	memcpy(newExe, m_pPE, fileLen);
#endif
	CPortableExecutable* loaded = new CPortableExecutable(newExe, m_name);
	return loaded;
}
