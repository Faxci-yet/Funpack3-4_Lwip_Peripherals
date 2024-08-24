/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "nt_drv_tsi_driver.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for tsi instances. */
TSI_Type *const g_tsiBase[] = TSI_BASE_PTRS;

/* Table to save TSI IRQ numbers defined in CMSIS files. */
#if (FSL_FEATURE_TSI_VERSION == 6)
const IRQn_Type g_tsiIrqId[FSL_FEATURE_SOC_TSI_COUNT] = {TSI_END_OF_SCAN_IRQn};
#elif (FSL_FEATURE_TSI_VERSION < 5 || defined(KE17Z9_SERIES) ||defined(KE17Z7_SERIES) || defined(KE13Z7_SERIES) || defined(KE12Z7_SERIES))
const IRQn_Type g_tsiIrqId[FSL_FEATURE_SOC_TSI_COUNT] = {TSI0_IRQn};
#else
const IRQn_Type g_tsiIrqId[FSL_FEATURE_SOC_TSI_COUNT] = {TSI_IRQn};
#endif

/* Pointer to tsi runtime state structure.*/
nt_tsi_state_t *g_tsiStatePtr[FSL_FEATURE_SOC_TSI_COUNT] = {NULL};

/*******************************************************************************
 * EOF
 ******************************************************************************/
