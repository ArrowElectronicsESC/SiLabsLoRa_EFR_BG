/*
 * Copyright (c) 2015-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Declare __STDC_LIMIT_MACROS so stdint.h defines UINT32_MAX when using C++ */
#define __STDC_LIMIT_MACROS
#include "em_core.h"
#include "../trace.h"
#include "platform/mbed_critical.h"

static uint32_t critical_section_reentrancy_counter = 0;

void core_util_critical_section_enter(void)
{
	CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    // If the reentrancy counter overflows something has gone badly wrong.
    MBED_ASSERT(critical_section_reentrancy_counter < UINT32_MAX);

    ++critical_section_reentrancy_counter;
}

void core_util_critical_section_exit(void)
{

    // If critical_section_enter has not previously been called, do nothing
    if (critical_section_reentrancy_counter == 0) {
        return;
    }

    --critical_section_reentrancy_counter;

    if (critical_section_reentrancy_counter == 0) {
    	CORE_DECLARE_IRQ_STATE;
    	CORE_EXIT_CRITICAL();
    }
}
