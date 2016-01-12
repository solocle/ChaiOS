/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: ChaiOS
File: liballoc.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\ChaiOS\liballoc.h
Created by Nathaniel on 24/12/2014 at 15:19

Description: liballoc public domain heap
**********************************************************/
#ifndef _LIBALLOC_H
#define _LIBALLOC_H

#include "stdafx.h"
#include "CHAIKRNL.h"

// If we are told to not define our own size_t, then we
// skip the define.

//CHAIOS:
#define _ALLOC_SKIP_DEFINE
#ifndef _ALLOC_SKIP_DEFINE

#ifndef _HAVE_SIZE_T
#define _HAVE_SIZE_T
typedef	unsigned int	size_t;
#endif


#ifndef	NULL
#define NULL		0
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif


/** This is a boundary tag which is prepended to the
 * page or section of a page which we have allocated. It is
 * used to identify valid memory blocks that the
 * application is trying to free.
 */
struct	boundary_tag
{
	unsigned int magic;			//&lt; It's a kind of ...
	size_t size; 			//&lt; Requested size.
	size_t real_size;		//&lt; Actual size.
	int index;					//&lt; Location in the page table.

	struct boundary_tag *split_left;	//&lt; Linked-list info for broken pages.	
	struct boundary_tag *split_right;	//&lt; The same.
	
	struct boundary_tag *next;	//&lt; Linked list info.
	struct boundary_tag *prev;	//&lt; Linked list info.
};


 

/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
int liballoc_lock();

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
int liballoc_unlock();

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
void* liballoc_alloc(int);

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void*,int);

       

CHAIOS_API_FUNC(void* DECL_CALL malloc(size_t));				//&lt; The standard function.
CHAIOS_API_FUNC(void* DECL_CALL realloc(void *, size_t));		//&lt; The standard function.
CHAIOS_API_FUNC(void* DECL_CALL calloc(size_t, size_t));		//&lt; The standard function.
CHAIOS_API_FUNC(void DECL_CALL free(void *));					//&lt; The standard function.

#ifdef __cplusplus
}
#endif

#endif

