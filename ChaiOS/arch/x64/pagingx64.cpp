/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: CHAIKRNL
File: pagingx64.cpp
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\arch\x64\pagingx64.cpp
Created by Nathaniel on 13/12/2015 at 01:45

Description: Paging support code
**********************************************************/

#include <stdafx.h>
#include <pmmngr.h>
#include "paging.h"
#include "pagingdefs.h"

typedef struct _PAGE_STRUCTURE {
	union {
		struct {
			struct {
				char present : 1;
				char rw : 1;
				char us : 1;
				char writethrough : 1;
				char cachedisable : 1;
				char accessed : 1;
				char dirty : 1;
				char resv_0 : 1;		//PAT for a PT. PS for anything above PT.
			}attribs;
			union {
				struct {
					char global : 1;
					char swapped : 1;
					char freepages : 1;
					char unswappable : 1;
					char phybaselow : 4;
				};
				struct {
					char global : 1;
					char swapped : 1;
					char freepages : 1;
					char unswappable : 1;
					char pat : 1;		//PAT for anything above a PT
					char phybaselow2 : 3;
				}patabovepd;
			};
			uint16_t phybasemid;
			uint32_t phybasehigh : 20;
			uint32_t avl2 : 11;
			uint32_t nx : 1;
		};
		uint64_t entry;
	};
}PAGE_STRUCTURE, *PPAGE_STRUCTURE;

static_assert(sizeof(PAGE_STRUCTURE) == 8, "ERROR: PAGE STRUCTURE SIZE ERROR");

class CX64PagingStructure : public CPagingStructure {
public:
	CX64PagingStructure(void* pagetable)
		:m_pageent((PPAGE_STRUCTURE)pagetable)
	{
	}
	~CX64PagingStructure(){ ; }

	virtual void setAddress(physaddr addr)
	{
		size_t iaddr = (size_t)addr;
		m_pageent->phybaselow = (iaddr >> 12);
		m_pageent->phybasemid = (iaddr >> 16);
		m_pageent->phybasehigh = (iaddr >> 32);
	}

	void setUser(bool user = true)
	{
		user ? m_pageent->attribs.us = 1 : m_pageent->attribs.us = 0;
	}

	virtual void setPresent(bool present = true)
	{
		present ? m_pageent->attribs.present = 1 : m_pageent->attribs.present = 0;
	}

	virtual void setFreepages(bool freepages = true)
	{
		freepages ? m_pageent->freepages = 1 : m_pageent->freepages = 0;
	}

	virtual void setSwapped(bool swapped = true)
	{
		swapped ? m_pageent->swapped = 1 : m_pageent->swapped = 0;
	}

	virtual void setUnswappable(bool unswappable = true)
	{
		unswappable ? m_pageent->unswappable = 1 : m_pageent->unswappable = 0;
	}

	virtual void prepareTable()
	{
		m_pageent->entry = 0;
	}

	virtual bool getUser()
	{
		return m_pageent->attribs.us;
	}
	virtual bool getPresent()
	{
		return m_pageent->attribs.present;
	}
	virtual bool getFreepages()
	{
		return m_pageent->freepages;
	}
	virtual bool getSwapped()
	{
		return m_pageent->swapped;
	}
	virtual bool getUnswappable()
	{
		return m_pageent->unswappable;
	}
private:
	const PPAGE_STRUCTURE m_pageent;
};

static uint_fast16_t mmu_recursive_slot = 0;

#undef MMU_RECURSIVE_SLOT
#define MMU_RECURSIVE_SLOT mmu_recursive_slot

class CPagingTable
{
public:
	CPagingTable(const void* address, unsigned int level)
		:m_table((PPAGE_STRUCTURE)address_calcuator(address, level)), m_level(level)
	{
	}

	CX64PagingStructure operator [](size_t n)
	{
		if (n >= 4096/sizeof(PAGE_STRUCTURE))
			return CX64PagingStructure(NULL);
		else
			return CX64PagingStructure((void*)&m_table[n]);
	}

	size_t calculateIndex(const void* address)
	{
		return (((size_t)address)>>(m_level*9+3))&0x1FF;
	}
protected:
	static const void* address_calcuator(const void* addr, unsigned int level)
	{
		size_t address = CANONICAL_UNDO(addr);
		if (level > 4)
			return NULL;
		else if (level == 0)
			return addr;
		else
		{
			size_t PT_ADDR = (0xFFFF000000000000UL + ((size_t)mmu_recursive_slot << 39));
			for (unsigned int n = 1; n < level; n++)
			{
				PT_ADDR += (mmu_recursive_slot << (39 - n * 9));
			}
			size_t tableaddr = PT_ADDR + (((size_t)address)>>(level*9))&((1<<(48-level*9))-0x1000);
			return (void*)tableaddr;
		}
	}
private:
	const PPAGE_STRUCTURE m_table;
	const unsigned int m_level;
};

class CPagingInformation {
public:
	CPagingInformation(void* address)
		:m_address(address)
	{

	}

	CPagingTable getPageTableLevel(unsigned int level)
	{
		return CPagingTable(m_address, level);
	}

	CX64PagingStructure getPageEntryLevel(unsigned int level)
	{
		CPagingTable table = CPagingTable(m_address, level);
		return table[table.calculateIndex(m_address)];
	}

	CPagingTable getPML4()
	{
		return CPagingTable(m_address, 4);
	}

	CPagingTable getPDPT()
	{
		return CPagingTable(m_address, 3);
	}

	CPagingTable getPD()
	{
		return CPagingTable(m_address, 2);
	}

	CPagingTable getPT()
	{
		return CPagingTable(m_address, 1);
	}
protected:
	const void* m_address;
};

