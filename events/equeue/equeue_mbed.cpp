/*
 * Implementation for the mbed library
 * https://github.com/mbedmicro/mbed
 *
 * Copyright (c) 2016 Christopher Haster
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "equeue_platform.h"
#include "em_core.h"
#if defined(EQUEUE_PLATFORM_MBED)
#include  <kernel/include/os.h>
#include <stdbool.h>
#include <string.h>
#include "platform/mbed_critical.h"
#include  <kernel/include/os_trace.h>
#include  <common/include/rtos_utils.h>

//using namespace mbed;

// Ticker operations
//#if MBED_CONF_RTOS_PRESENT

extern uint32_t readmsTicks(void);

unsigned equeue_tick()
{
    return readmsTicks();
}

// Mutex operations
int equeue_mutex_create(OS_MUTEX *m)
{
	RTOS_ERR  error;
	OSMutexCreate(m, "Enqueue Mutex", &error);
    return 0;
}
void equeue_mutex_destroy(OS_MUTEX *m)
{
	RTOS_ERR  error;
	OSMutexDel(m,OS_OPT_DEL_ALWAYS,&error);
}

void equeue_mutex_lock(OS_MUTEX *m)
{
	//core_util_critical_section_enter();
    RTOS_ERR  error;
    CPU_TS ts;
    OSMutexPend(m,0, OS_OPT_PEND_BLOCKING, &ts, &error);
}

void equeue_mutex_unlock(OS_MUTEX *m)
{
	//core_util_critical_section_exit();
    RTOS_ERR  error;
    CPU_TS ts;
    OSMutexPost(m, OS_OPT_POST_NONE, &error);
}

#endif


