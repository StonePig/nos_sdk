/*
 * FreeRTOS Kernel V11.1.0
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef _GENERIC_TIMER_H_
#define _GENERIC_TIMER_H_

/*
CNTP_CTL, Counter-timer Physical Timer Control register fields
*/
#define ENABLE                              (1 << 0)
#define IMASK                               (1 << 1)
#define ISTATUS                             (1 << 2)

/* 
Interrupt ID for physical timer and virtual timer
Resource Information: (S32E2_interrupt_map.xlsx/Sheet INT_MAP_ARM/Line 19,22) was attacked in S32E27RM_Rev2DraftE.pdf
*/
#define VIRTUAL_TIMER_INT_ID                (27U)
#define PHYSICAL_TIMER_INT_ID               (30U)

#define ERROR                               (0)

#define TIMER_PRIO                          (1U)
#define ASMV_KEYWORD                        __asm__ volatile

/*
 * Convert from low number value means low prio to
 * low number value means high prio - the way NVIC works
 *
 * 0x00 means 0xFF in hw - this level will never generate an interrupt (>= in basepri)
 */
#define OSINTC_GIC_PRIO_BITS                (5u)
#define OSINTC_GIC_PRIO_SHIFT               (8u - OSINTC_GIC_PRIO_BITS)
#define OSINTC_GIC_MIN_PRIO                 0x00u
#define OSINTC_GIC_MAX_PRIO                 0xFFu
#define OSINTC_GIC_CONVERT_PRIO_SET(prio)   ((((prio) & 0x1Fu) ^ 0x1Fu) << OSINTC_GIC_PRIO_SHIFT)
#define OSINTC_GIC_CONVERT_PRIO_GET(prio)   ((((prio) & 0xF8u) ^ 0xF8u) >> OSINTC_GIC_PRIO_SHIFT)

/* RTU_GPR - Peripheral instance base addresses */
/** Peripheral RTU0__GPR base address */
#define IP_RTU0__GPR_BASE                   (0x76120000u)
/** Peripheral RTU1__GPR base address */
#define IP_RTU1__GPR_BASE                   (0x76920000u)
/** Peripheral RTUx__GPR_CFG_CNTDV base address */
#define CFG_CNTDV(cluster)                  ((cluster < 1) ? (IP_RTU0__GPR_BASE + 0x10u) : (IP_RTU1__GPR_BASE + 0x10u))

/*! @name MPIDR - Multiprocessor Affinity Register */
/*! @{ */
#define MPIDR_CFGMPIDRAFF1_SHIFT            (8U)
#define MPIDR_CFGMPIDRAFF1_CLUSTER1         (0x100U)
#define MPIDR_GET_CLUSTER(x)                (((uint32_t)(((uint32_t)(x)) & MPIDR_CFGMPIDRAFF1_CLUSTER1))>> MPIDR_CFGMPIDRAFF1_SHIFT )
/*! @} */

/*! @name CFG_CNTDV - Generic Timer Count Divider Control */
/*! @{ */
#define RTU_GPR_CFG_CNTDV_CNTDV_MASK        (0x7U)
#define RTU_GPR_CFG_CNTDV_CNTDV_SHIFT       (0U)
#define RTU_GPR_CFG_CNTDV_CNTDV_WIDTH       (3U)
#define RTU_GPR_CFG_CNTDV_CNTDV(x)          (((uint32_t)(((uint32_t)(x)) << RTU_GPR_CFG_CNTDV_CNTDV_SHIFT)) & RTU_GPR_CFG_CNTDV_CNTDV_MASK)

#define READ_CFG_CNTDV(cluster)             (*(volatile uint32_t *)CFG_CNTDV(cluster))
#define WRITE_CFG_CNTDV(cluster,x)          ((*(volatile uint32_t *)CFG_CNTDV(cluster)) = (RTU_GPR_CFG_CNTDV_CNTDV(x)))
/*! @} */

/*
 * @brief  Get Cluster ID
 *
 * This function get cluster
 *
 * @param[in] void
 * @return void
 */
uint32_t GET_CLUSTER_ID(void);

/*
 * @brief Update timer tick count
 *
 * This function Update timer tick count
 *
 * @param[in] void
 * @return void
 */
void vUpdateTimer(void);

/*
 * @brief Initialize Generic timer
 *
 * This function initializes Generic timer
 *
 * @param[in] void
 * @return void
 */
void prvSetupTimerInterrupt(void);

#endif /* _GENERIC_TIMER_H_ */