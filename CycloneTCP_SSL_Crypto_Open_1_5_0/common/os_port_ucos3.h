/**
 * @file os_port_ucos3.h
 * @brief RTOS abstraction layer (Micrium uC/OS-III)
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

#ifndef _OS_PORT_UCOS3_H
#define _OS_PORT_UCOS3_H

//Dependencies
#include "os.h"

//Maximum number of tasks that can be dynamically created
#ifndef OS_PORT_MAX_TASKS
   #define OS_PORT_MAX_TASKS 16
#elif (OS_PORT_MAX_TASKS < 1)
   #error OS_PORT_MAX_TASKS parameter is not valid
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Enter interrupt service routine
#define osEnterIsr() OSIntEnter()

//Leave interrupt service routine
#define osExitIsr(flag) OSIntExit()


/**
 * @brief Task object
 **/

typedef OS_TCB OsTask;


/**
 * @brief Event object
 **/

typedef OS_FLAG_GRP OsEvent;


/**
 * @brief Semaphore object
 **/

typedef OS_SEM OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef OS_MUTEX OsMutex;


/**
 * @brief Task routine
 **/

typedef void (*OsTaskCode)(void *params);


//Kernel management
void osInitKernel(void);
void osStartKernel(void);

//Task management
bool_t osCreateStaticTask(OsTask *task, const char_t *name, OsTaskCode taskCode,
   void *params, void *stack, size_t stackSize, int_t priority);

OsTask *osCreateTask(const char_t *name, OsTaskCode taskCode,
   void *params, size_t stackSize, int_t priority);

void osDeleteTask(OsTask *task);
void osDelayTask(systime_t delay);
void osSwitchTask(void);
void osSuspendAllTasks(void);
void osResumeAllTasks(void);

//Event management
bool_t osCreateEvent(OsEvent *event);
void osDeleteEvent(OsEvent *event);
void osSetEvent(OsEvent *event);
void osResetEvent(OsEvent *event);
bool_t osWaitForEvent(OsEvent *event, systime_t timeout);
bool_t osSetEventFromIsr(OsEvent *event);

//Semaphore management
bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count);
void osDeleteSemaphore(OsSemaphore *semaphore);
bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout);
void osReleaseSemaphore(OsSemaphore *semaphore);

//Mutex management
bool_t osCreateMutex(OsMutex *mutex);
void osDeleteMutex(OsMutex *mutex);
void osAcquireMutex(OsMutex *mutex);
void osReleaseMutex(OsMutex *mutex);

//System time
systime_t osGetSystemTime(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//Atomic operations
uint16_t osAtomicInc16(uint16_t *n);
uint32_t osAtomicInc32(uint32_t *n);

//Undefine conflicting definitions
#undef TRACE_LEVEL_OFF
#undef TRACE_LEVEL_INFO

#endif
