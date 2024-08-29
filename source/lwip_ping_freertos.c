/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022-2023  NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/*System includes.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "lwip/opt.h"
#include "fsl_device_registers.h"
#include "fsl_adapter_gpio.h"
#include "fsl_debug_console.h"

// #if LWIP_IPV4 && !LWIP_RAW && !LWIP_SOCKET
#define LWIP_IPV4 1
#define LWIP_RAW 1
#define LWIP_SOCKET 1
// #endif


#if LWIP_IPV4 && LWIP_RAW && LWIP_SOCKET

#include "ping.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "ethernetif.h"


#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "main.h"
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif
#include "fsl_phy.h"
#include "fsl_pwm.h"
#include "fsl_enet.h"
#include "fsl_phylan8741.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT)
#include "fsl_port.h"
#endif
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_clock.h"

#include "fsl_p3t1755.h"
#include "fsl_i3c.h"

/* touch freemaster */
static void CTIMERInit(void);
static void init_freemaster_lpuart(void);
static void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index);

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position);

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position);

/* Call when the TSI counter overflows 65535 */
static void system_callback(uint32_t event, union nt_system_event_context *context);

#if defined(__ICCARM__)
uint8_t nt_memory_pool[3400]; /* IAR EWARM compiler */
#else
uint8_t nt_memory_pool[3400] __attribute__((aligned(4))); /* Keil, GCC compiler */
#endif

/*
 * This list describes all TSA tables that should be exported to the
 * FreeMASTER application.
 */
#ifndef FMSTR_PE_USED
FMSTR_TSA_TABLE_LIST_BEGIN();
FMSTR_TSA_TABLE(nt_frmstr_tsa_table);
FMSTR_TSA_TABLE_LIST_END();
#endif

#define nt_printf(...) /* do nothing - the debug lines are used by FreeMASTER */


//#include "client.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

/* IP address configuration. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 2
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 103
#endif

/* Netmask configuration. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 2
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 1
#endif

/* Ethernet configuration. */
extern phy_lan8741_resource_t g_phy_resource;
#define EXAMPLE_ENET_BASE    ENET0
#define EXAMPLE_PHY_ADDRESS  BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_PHY_OPS      &phylan8741_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   (50000000U)


#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*! @brief Selection of GPIO perihperal and its pin for the reception of PHY interrupts. */
#if ETH_LINK_POLLING_INTERVAL_MS == 0
#ifndef EXAMPLE_PHY_INT_PORT
#if (!defined(BOARD_NETWORK_USE_100M_ENET_PORT) || !BOARD_NETWORK_USE_100M_ENET_PORT) && \
    defined(BOARD_INITENET1GPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITENET1GPINS_PHY_INTR_PERIPHERAL
#elif defined(BOARD_INITENETPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITENETPINS_PHY_INTR_PERIPHERAL
#elif defined(BOARD_INITPINS_PHY_INTR_PERIPHERAL)
#define EXAMPLE_PHY_INT_PORT BOARD_INITPINS_PHY_INTR_PERIPHERAL
#else
#error "Interrupt-based link-state detection was enabled on an unsupported board."
#endif
#endif // #ifndef EXAMPLE_PHY_INT_PORT

#ifndef EXAMPLE_PHY_INT_PIN
#if (!defined(BOARD_NETWORK_USE_100M_ENET_PORT) || !BOARD_NETWORK_USE_100M_ENET_PORT) && \
    defined(BOARD_INITENET1GPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITENET1GPINS_PHY_INTR_CHANNEL
#elif defined(BOARD_INITENETPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITENETPINS_PHY_INTR_CHANNEL
#elif defined(BOARD_INITPINS_PHY_INTR_CHANNEL)
#define EXAMPLE_PHY_INT_PIN BOARD_INITPINS_PHY_INTR_CHANNEL
#else
#error "Interrupt-based link-state detection was enabled on an unsupported board."
#endif
#endif // #ifndef EXAMPLE_PHY_INT_PIN
#endif // #if ETH_LINK_POLLING_INTERVAL_MS == 0
#define EXAMPLE_MASTER             I3C1
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq(1)
#define SENSOR_SLAVE_ADDR          0x48U
#define I3C_TIME_OUT_INDEX 100000000U

#define SENSOR_ADDR 0x08U
#define CCC_RSTDAA  0x06U
#define CCC_SETDASA 0x87

#ifndef EXAMPLE_I2C_BAUDRATE
#define EXAMPLE_I2C_BAUDRATE 400000
#endif
#ifndef EXAMPLE_I3C_OD_BAUDRATE
#define EXAMPLE_I3C_OD_BAUDRATE 1500000
#endif
#ifndef EXAMPLE_I3C_PP_BAUDRATE
#define EXAMPLE_I3C_PP_BAUDRATE 4000000
#endif

#define BOARD_PWM_BASEADDR        PWM1
#define PWM_SRC_CLK_FREQ          CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_PWM_FAULT_LEVEL      true
#define APP_DEFAULT_PWM_FREQUENCE (10000UL)
/* Definition for default PWM frequence in hz. */
#ifndef APP_DEFAULT_PWM_FREQUENCE
#define APP_DEFAULT_PWM_FREQUENCE (1000UL)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void buttom_init(void);
static void SW_task(void *pvParameters);
static void p3t1755_task(void *pvParameters);
static void write_task_1(void *pvParameters);
static void write_task_2(void *pvParameters);
static void led_green_task(void *pvParameters);
static void rgb_led_task(void *pvParameters);
static void i3c_master_callback(I3C_Type *base, i3c_master_handle_t *handle, status_t status, void *userData);
void P3T1755_I3C_Init(void);
/* Logger API */
void log_add(char *log);
void log_init(uint32_t queue_length, uint32_t max_log_lenght);
static void log_task(void *pvParameters);

static void hello_task(void *pvParameters);
static void led_red_task(void *pvParameters);
static void led_blue_task(void *pvParameters);
static void touch_pad_task(void *pvParameters);
static TaskHandle_t hello_task_Handle = NULL;
static TaskHandle_t led_red_task_Handle = NULL;
static TaskHandle_t led_blue_task_Handle = NULL;
static TaskHandle_t touch_task_Handle = NULL;
static TaskHandle_t sw_task_Handle = NULL;
void PWM_Init_all(void);
static void pwm_task(void *pvParameters);
/*******************************************************************************
 * Globals
 ******************************************************************************/
/* Logger queue handle */
static QueueHandle_t log_queue = NULL;
status_t result = kStatus_Success;
double temperature = 0.0;
struct netconn *conn,*newconn;
int ret;
int pwmVal = 4;
char toward_char[2] = {'+','-'};
uint8_t toward_flag = 0;
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Variables
 ******************************************************************************/
phy_lan8741_resource_t g_phy_resource;

static phy_handle_t phyHandle;
static struct netif netif;
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;
volatile status_t g_completionStatus;
volatile bool g_masterCompletionFlag;
i3c_master_handle_t g_i3c_m_handle;
p3t1755_handle_t p3t1755Handle;
char data_uart[16] = "000 000 000 +000";

const i3c_master_transfer_callback_t masterCallback = {
    .slave2Master = NULL, .ibiCallback = NULL, .transferComplete = i3c_master_callback};
#define MAX_LOG_LENGTH 20
/*******************************************************************************
 * Code
 ******************************************************************************/
static void PWM_DRV_Init3PhPwm(void)
{
    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCE;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 50; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState0;
    pwmSignal[0].pwmchannelenable = true;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level      = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 50;
    pwmSignal[1].deadtimeValue    = deadTimeVal;
    pwmSignal[1].faultState       = kPWM_PwmFaultState0;
    pwmSignal[1].pwmchannelenable = true;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
#ifdef DEMO_PWM_CLOCK_DEVIDER
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz / (1 << DEMO_PWM_CLOCK_DEVIDER));
#else
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
#endif

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
#ifdef DEMO_PWM_CLOCK_DEVIDER
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz / (1 << DEMO_PWM_CLOCK_DEVIDER));
#else
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
#endif
}
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET_BASE)]);
    ENET_SetSMI(EXAMPLE_ENET_BASE, CLOCK_GetCoreSysClkFreq());
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET_BASE, phyAddr, regAddr, pData);
}

void substring(char *src, char *dest, int start, int length) {
    if (start < 0 || length <= 0 || start + length > strlen(src)) {
        printf("Invalid parameters!\n");
        return;
    }

    strncpy(dest, src + start, length);

    dest[length] = '\0';
}


static void client(void *thread_param)
{
//    struct netconn *conn;
//	struct netconn *newconn;
//    int ret;
    char temperature_str[20];
    err_t err;
    ip4_addr_t ipaddr;
    uint8_t send_buf[]= "Hello PC Client! \n";
//    char data_uart[15] = "";
    uint32_t ulReturn;

        conn = netconn_new(NETCONN_TCP);
        netconn_bind(conn, IP_ADDR_ANY, 5001);
        LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);
        netconn_listen(conn);
        while (1)
        {
        	err = netconn_accept(conn, &newconn);
        	if(err == ERR_OK)
        	{
        		PRINTF("\r\nnew connect\r\n");
        		netconn_write(newconn, send_buf, strlen(send_buf), 0);
        		struct netbuf *buf;
				void *data;

				u16_t len;
				while ((err = netconn_recv(newconn, &buf)) == ERR_OK)
				{
					do
					{
						netbuf_data(buf, &data, &len);
						err= netconn_write(newconn, data, len, NETCONN_COPY);
						strncpy(data_uart,data,16);
						data_uart[16] = '\0';
						PRINTF(data_uart);
//						PRINTF("\r\nstring len = %d\r\n",strlen(data_uart));
						if (err != ERR_OK)
						{
							PRINTF("tcpecho: netconn_write: error \"%s\"\n",lwip_strerr(err));

						}
					}while(netbuf_next(buf) >= 0);
					netbuf_delete(buf);
				}
				netconn_close(newconn);
				netconn_delete(newconn);
//				vTaskDelay(3);
        	}
        	else
        		continue;

//        	taskYIELD();
        }
//    }
}

void client_init(void)
{
    sys_thread_new("client", client, NULL, 1024, INIT_THREAD_PRIO);
}

static void i3c_master_callback(I3C_Type *base, i3c_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        g_masterCompletionFlag = true;
    }

    g_completionStatus = status;
}

status_t I3C_WriteSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI3C_Write;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI3C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I3C_MasterTransferNonBlocking(EXAMPLE_MASTER, &g_i3c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I3C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I3C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

status_t I3C_ReadSensor(uint8_t deviceAddress, uint32_t regAddress, uint8_t *regData, size_t dataSize)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint32_t timeout                 = 0U;

    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI3C_Read;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.subaddress     = regAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data           = regData;
    masterXfer.dataSize       = dataSize;
    masterXfer.flags          = kI3C_TransferDefaultFlag;

    g_masterCompletionFlag = false;
    g_completionStatus     = kStatus_Success;
    result                 = I3C_MasterTransferNonBlocking(EXAMPLE_MASTER, &g_i3c_m_handle, &masterXfer);
    if (kStatus_Success != result)
    {
        return result;
    }

    while (!g_masterCompletionFlag)
    {
        timeout++;
        if ((g_completionStatus != kStatus_Success) || (timeout > I3C_TIME_OUT_INDEX))
        {
            break;
        }
    }

    if (timeout == I3C_TIME_OUT_INDEX)
    {
        result = kStatus_Timeout;
    }
    result = g_completionStatus;

    return result;
}

status_t p3t1755_set_dynamic_address(void)
{
    status_t result                  = kStatus_Success;
    i3c_master_transfer_t masterXfer = {0};
    uint8_t g_master_txBuff[1];

    /* Reset dynamic address. */
    g_master_txBuff[0]      = CCC_RSTDAA;
    masterXfer.slaveAddress = 0x7E;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferDefaultFlag;
    result                  = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Assign dynmic address. */
    memset(&masterXfer, 0, sizeof(masterXfer));
    g_master_txBuff[0]      = CCC_SETDASA;
    masterXfer.slaveAddress = 0x7E;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferNoStopFlag;
    result                  = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (result != kStatus_Success)
    {
        return result;
    }

    memset(&masterXfer, 0, sizeof(masterXfer));
    g_master_txBuff[0]      = SENSOR_ADDR << 1;
    masterXfer.slaveAddress = SENSOR_SLAVE_ADDR;
    masterXfer.data         = g_master_txBuff;
    masterXfer.dataSize     = 1;
    masterXfer.direction    = kI3C_Write;
    masterXfer.busType      = kI3C_TypeI3CSdr;
    masterXfer.flags        = kI3C_TransferDefaultFlag;
    return I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
}

/*!
 * @brief Initializes lwIP stack.
 */
static void stack_init(void *arg)
{
	uint32_t ulReturn;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
        .srcClockHz  = EXAMPLE_CLOCK_FREQ,
#ifdef configMAC_ADDR
        .macAddress = configMAC_ADDR,
#endif
#if ETH_LINK_POLLING_INTERVAL_MS == 0
        .phyIntGpio    = EXAMPLE_PHY_INT_PORT,
        .phyIntGpioPin = EXAMPLE_PHY_INT_PIN
#endif
    };

//    ulReturn = taskENTER_CRITICAL_FROM_ISR();
    LWIP_UNUSED_ARG(arg);

    /* Set MAC address. */
#ifndef configMAC_ADDR
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    tcpip_init(NULL, NULL);

    HAL_GpioPreInit();

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" PING example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
           ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif_netmask)[0], ((u8_t *)&netif_netmask)[1],
           ((u8_t *)&netif_netmask)[2], ((u8_t *)&netif_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
           ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]);
    PRINTF("************************************************\r\n");

//    ping_init(&netif_gw);

    client_init();
//    taskEXIT_CRITICAL_FROM_ISR(ulReturn);
    if (xTaskCreate(hello_task, "Hello_TASK", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY, &hello_task_Handle) !=
           pdPASS)
       {
           PRINTF("Task creation failed!.\r\n");
           while (1)
               ;
       }
   //    if (xTaskCreate(write_task_2, "WRITE_TASK_2", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) !=
   //        pdPASS)
   //    {
   //        PRINTF("Task creation failed!.\r\n");
   //        while (1)
   //            ;
   //    }
       if (xTaskCreate(SW_task, "SW_task", configMINIMAL_STACK_SIZE + 100, NULL, INIT_THREAD_PRIO, &sw_task_Handle) !=
   		pdPASS)
   	{
   		PRINTF("Task creation failed!.\r\n");
   		while (1)
   			;
   	}
       if (xTaskCreate(p3t1755_task, "p3t1755_task", configMINIMAL_STACK_SIZE + 256, NULL, INIT_THREAD_PRIO, NULL) !=
   		pdPASS)
   	{
   		PRINTF("Task creation failed!.\r\n");
   		while (1)
   			;
   	}
    if (xTaskCreate(pwm_task, "pwm_task", configMINIMAL_STACK_SIZE + 256, NULL, INIT_THREAD_PRIO, NULL) !=
   		pdPASS)
   	{
   		PRINTF("Task creation failed!.\r\n");
   		while (1)
   			;
   	}
    vTaskDelete(NULL);
}


/*!
 * @brief Main function
 */
int main(void)
{
    int32_t result;
    bool one_key_only    = false; /* one key only valid is off */

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    // CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    LED_RED_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
    LED_BLUE_INIT(LOGIC_LED_OFF);
    P3T1755_I3C_Init();
    PWM_Init_all();
    NT_OSA_Init();

    if ((result = nt_init(&System_0, nt_memory_pool, sizeof(nt_memory_pool))) != NT_SUCCESS)
    {
        /* red colour signalizes the error, to solve is increase nt_memory_pool or debug it */
        LED_RED_ON();

        switch (result)
        {
            case NT_FAILURE:
                nt_printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
            case NT_OUT_OF_MEMORY:
                nt_printf("\nCannot initialize NXP Touch due to a lack of free memory.\n");
                printf("\nCannot initialize NXP Touch due to a non-specific error.\n");
                break;
        }
        while (1); /* add code to handle this error */
    }
    /* Get free memory size of the nt_memory_pool  */
    volatile uint32_t free_mem;
    free_mem = nt_mem_get_free_size();

    nt_printf("\nNXP Touch is successfully initialized.\n");
    nt_printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
              free_mem);
    printf("Unused memory: %d bytes, you can make the memory pool smaller without affecting the functionality.\n",
          (int)free_mem);

    /* Enable electrodes and controls */
    nt_enable();

/* Disable FRDM-TOUCH board electrodes and controls if FRDM-TOUCH board is not connected */
#if (NT_FRDM_TOUCH_SUPPORT) == 0
    nt_electrode_disable(&El_2);
    nt_electrode_disable(&El_3);
    nt_electrode_disable(&El_4);
    nt_electrode_disable(&El_5);

    nt_electrode_disable(&El_6);
    nt_electrode_disable(&El_7);
    nt_electrode_disable(&El_8);
    nt_electrode_disable(&El_9);
    nt_electrode_disable(&El_10);
    nt_electrode_disable(&El_11);
#endif
    /* Keypad electrodes*/
    nt_control_keypad_set_autorepeat_rate(&Keypad_1, 100, 1000);
    nt_control_keypad_register_callback(&Keypad_1, &keypad_callback);

    /* Slider electrodes */
    nt_control_aslider_register_callback(&ASlider_2, &aslider_callback);

    /* Rotary electrodes */
    nt_control_arotary_register_callback(&ARotary_3, &arotary_callback);

    if (one_key_only)
        nt_control_keypad_only_one_key_valid(&Keypad_1, true);


    /* System TSI overflow warning callback */
    nt_system_register_callback(&system_callback);
//
    CTIMERInit();

    CLOCK_AttachClk(MUX_A(CM_ENETRMIICLKSEL, 0));
    CLOCK_EnableClock(kCLOCK_Enet);
    SYSCON0->PRESETCTRL2 = SYSCON_PRESETCTRL2_ENET_RST_MASK;
    SYSCON0->PRESETCTRL2 &= ~SYSCON_PRESETCTRL2_ENET_RST_MASK;

    MDIO_Init();
    LED_RED_OFF();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

//    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, hello_task_PRIORITY) == NULL)
//            LWIP_ASSERT("main(): Task creation failed.", 0);
    if (xTaskCreate(stack_init, "ENET_TASK", INIT_THREAD_STACKSIZE, NULL, tskIDLE_PRIORITY + 2, NULL) !=
            pdPASS)
        {
            PRINTF("Task creation failed!.\r\n");
            while (1)
                ;
        }



    /* Initialize lwIP from thread */


    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}
#endif

/*******************************************************************************
 * Application functions
 ******************************************************************************/
/*!
 * @brief write_task_1 function
 */
static void write_task_1(void *pvParameters)
{
    char log[MAX_LOG_LENGTH + 1];
    uint32_t i = 0;
    for (i = 0; i < 5; i++)
    {
        sprintf(log, "Task1 Message %d", (int)i);
        log_add(log);
        taskYIELD();
    }
    vTaskSuspend(NULL);
}

/*!
 * @brief write_task_2 function
 */
static void write_task_2(void *pvParameters)
{
    char log[MAX_LOG_LENGTH + 1];
    uint32_t i = 0;
    for (i = 0; i < 5; i++)
    {
        sprintf(log, "Task2 Message %d", (int)i);
        log_add(log);
        taskYIELD();
    }
    vTaskSuspend(NULL);
}

/*******************************************************************************
 * Logger functions
 ******************************************************************************/
/*!
 * @brief log_add function
 */
void log_add(char *log)
{
    xQueueSend(log_queue, log, 0);
}

/*!
 * @brief log_init function
 */
void log_init(uint32_t queue_length, uint32_t max_log_lenght)
{
    log_queue = xQueueCreate(queue_length, max_log_lenght);
    /* Enable queue view in MCUX IDE FreeRTOS TAD plugin. */
    if (log_queue != NULL)
    {
        vQueueAddToRegistry(log_queue, "LogQ");
    }
    if (xTaskCreate(log_task, "log_task", configMINIMAL_STACK_SIZE + 512, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
}

/*!
 * @brief log_print_task function
 */
static void log_task(void *pvParameters)
{
    uint32_t counter = 0;
    char log[MAX_LOG_LENGTH + 1];
    while (1)
    {
        if (xQueueReceive(log_queue, log, portMAX_DELAY) != pdTRUE)
        {
            PRINTF("Failed to receive queue.\r\n");
        }
        PRINTF("Log %d: %s\r\n", counter, log);
        counter++;
    }
}

static void hello_task(void *pvParameters)
{
		uint32_t ulReturn;
//		ulReturn = taskENTER_CRITICAL_FROM_ISR();
        PRINTF("Hello world.\r\n");
        LED_RED_OFF();
        buttom_init();
        if (xTaskCreate(rgb_led_task, "rgb_task", configMINIMAL_STACK_SIZE, NULL, INIT_THREAD_PRIO, NULL) !=
            pdPASS)
        {
            PRINTF("Task creation failed!.\r\n");
            while (1)
                ;
        }

//        if (xTaskCreate(led_blue_task, "Led_Blue_task", configMINIMAL_STACK_SIZE, NULL, INIT_THREAD_PRIO, &led_blue_task_Handle) !=
//            pdPASS)
//        {
//            PRINTF("Task creation failed!.\r\n");
//            while (1)
//                ;
//        }

//        if (xTaskCreate(led_green_task, "Led_Green_task", configMINIMAL_STACK_SIZE, NULL, INIT_THREAD_PRIO-1, NULL) !=
//			pdPASS)
//		{
//			PRINTF("Task creation failed!.\r\n");
//			while (1)
//				;
//		}

        if (xTaskCreate(touch_pad_task, "Touch_Pad_task", configMINIMAL_STACK_SIZE + 256, NULL, INIT_THREAD_PRIO-1, &touch_task_Handle) !=
            pdPASS)
        {
            PRINTF("Task creation failed!.\r\n");
            while (1)
                ;
        }

        vTaskDelete(hello_task_Handle);
//		taskEXIT_CRITICAL_FROM_ISR( ulReturn );
        vTaskSuspend(NULL);

        for (;;)
           {

		}
}

static void rgb_led_task(void *pvParameters)
{
    char red_buff[3],green_buff[3],blue_buff[3];
    uint8_t red = 0,green = 0,blue = 0;
	for (;;)
    {
		strncpy(red_buff,data_uart,1);
		strncpy(green_buff,data_uart+4,1);
		strncpy(blue_buff,data_uart+8,1);
		red = atoi(red_buff);
		green = atoi(green_buff);
		blue = atoi(blue_buff);
		if(red == 1)
			LED_RED_ON();
		else if(red == 0)
			LED_RED_OFF();

		if(green == 1)
			LED_GREEN_ON();
		else if(green == 0)
			LED_GREEN_OFF();

		if(blue == 1)
			LED_BLUE_ON();
		else if(blue == 0)
			LED_BLUE_OFF();


    	vTaskDelay(50);

    }
}

static void led_red_task(void *pvParameters)
{
    for (;;)
    {
    	LED_RED_ON();
    	vTaskDelay(500);
    	LED_RED_OFF();
        vTaskDelay(500);

    }
}

static void led_blue_task(void *pvParameters)
{
    for (;;)
    {
    	LED_BLUE_OFF();
    	vTaskDelay(500);
    	LED_BLUE_ON();
        vTaskDelay(500);
    }
}

static void led_green_task(void *pvParameters)
{
    for (;;)
    {
    	LED_GREEN_TOGGLE();
    	vTaskDelay(100);
//    	LED_GREEN_ON();
//        vTaskDelay(500);
    }
}


static void touch_pad_task(void *pvParameters)
{
//	uint32_t ulReturn;

	for (;;)
    {
//		ulReturn = taskENTER_CRITICAL_FROM_ISR();
		nt_task();
//		taskEXIT_CRITICAL_FROM_ISR(ulReturn);
        vTaskDelay(10);
        /* The FreeMASTER poll call must be called in the main application loop
        to handle the communication interface and protocol.
           In the LONG_INTR FreeMASTER interrupt mode, all the processing is done
        during the communication interrupt routine and the FMSTR_Poll() is
        compiled empty. */
//        FMSTR_Poll();


    }
}

static void SW_task(void *pvParameters)
{
	char key_str[20];
    for(;;)
    {

//    	PRINTF("SW3 task!\r\n");
    	if (!GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
        {
            vTaskDelay(50);  // Debounce
            if (!GPIO_PinRead(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN))
            {
				sprintf(key_str,"key pressed");
				if (newconn != NULL)
					ret = netconn_write(newconn,key_str,strlen(key_str),0);
            	PRINTF("\r\nSW3 pressed!\r\n");
            }
        }

        vTaskDelay(200);
    }
}

static void p3t1755_task(void *pvParameters)
{

	for(;;)
    {
    	char temperature_str[23];
		uint32_t ulReturn;
		ulReturn = taskENTER_CRITICAL_FROM_ISR();
    	result = P3T1755_ReadTemperature(&p3t1755Handle, &temperature);
    	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
    	if (result != kStatus_Success)
        {
            PRINTF("\r\nP3T1755 read temperature failed.\r\n");
        }
    	sprintf(temperature_str,"Temperature:%.2f,PWM %d",temperature,pwmVal);
    	if(newconn != NULL)
    		ret = netconn_write(newconn,temperature_str,strlen(temperature_str),0);
    	sprintf(temperature_str,"\r\nTemperature:%.2f \r\n",temperature);
        PRINTF(temperature_str);
//        SDK_DelayAtLeastUs(1000000, CLOCK_GetCoreSysClkFreq());

        vTaskDelay(2000);
    }
}

static void pwm_task(void *pvParameters)
{
	pwm_config_t pwmConfig;
    pwm_fault_param_t faultConfig;

    char toward[1]="+";
    char pwm_val[4] = "+000";
    char buffer[5] = "+ 000";
    // PWM_Init_all();

    for (;;)
    {
//        strncpy(toward,data_uart+12,1);
        strncpy(pwm_val,data_uart+12,4);
        /* Delay at least 100 PWM periods. */
        // SDK_DelayAtLeastUs((1000000U / APP_DEFAULT_PWM_FREQUENCE) * 100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        pwmVal = atoi(pwm_val);

        /* Reset the duty cycle percentage */

        if(pwmVal>=0)
        {
            toward_flag = 0;
            if(pwmVal > 100)
            	pwmVal = 100;
            if(pwmVal < 4)
            	pwmVal = 4;
		/* Update duty cycles for all 3 PWM signals */
        	PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, abs(pwmVal)); // p2_4
        	PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, (0 >> 1)); // p2_6
        // PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, (pwmVal >> 2));
        }
        else
        {
            toward_flag = 1;
            if(pwmVal < -100)
            	pwmVal = -100;
            if(pwmVal > -4)
            	pwmVal = -4;
        	/* Update duty cycles for all 3 PWM signals */
			PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, 0); // p2_4
			PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, (abs(pwmVal) >> 1)); // p2_6
        }
        /* Set the load okay bit for all submodules to load registers from their buffer */
        PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);
        vTaskDelay(10);
    }
}


void CTIMER0_IRQHandler(void)
{
    /* Clear the interrupt flag.*/
    nt_trigger();

    /* Clear the match interrupt flag. */
    CTIMER0->IR |= CTIMER_IR_MR0INT(1U);

    /* Add empty instructions for correct interrupt flag clearing */
    __DSB();
    __ISB();
}

void TSI_END_OF_SCAN_DriverIRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}
void TSI_OUT_OF_SCAN_DriverIRQHandler(void)
{
    TSI_DRV_IRQHandler(0);
}

static void CTIMERInit(void)
{
    /* Use 96 MHz clock for some of the Ctimer0. */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);

    /* Enable Timer0 clock. */
    SYSCON->AHBCLKCTRLSET[1] |= SYSCON_AHBCLKCTRL1_TIMER0_MASK;

    /* Enable Timer0 clock reset. */
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Set bit. */
    while (0u == (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))   /* Wait until it reads 0b1 */
    {
    }

    /* Clear Timer0 clock reset. */
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Clear bit */
    while (SYSCON_PRESETCTRL1_TIMER0_RST_MASK ==                               /* Wait until it reads 0b0 */
          (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))
    {
    }

    /* Configure match control register. */
    CTIMER0->MCR |= CTIMER_MCR_MR0R(1U)  |   /* Enable reset of TC after it matches with MR0. */
                    CTIMER_MCR_MR0I(1U);     /* Enable interrupt generation after TC matches with MR0. */

    /* Configure match register. */
    CTIMER0->MR[0] = (nt_kernel_data.rom->time_period * CLOCK_GetFreq(kCLOCK_FroHf))  /* Get CTimer0 frequency for correct set Match register value. */
                     / 1000;                 /* Set slow control loop frequency in Hz. */

    /* Configure interrupt register. */
    CTIMER0->IR = CTIMER_IR_MR0INT_MASK;     /* Set interrupt flag for match channel 0. */
    NVIC_SetPriority(CTIMER0_IRQn, 1U);
    NVIC_EnableIRQ(CTIMER0_IRQn);            /* Enable LEVEL1 interrupt and update the call back function. */

    /* Configure timer control register. */
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Start the timer counter. */
}

static void keypad_callback(const struct nt_control *control, enum nt_control_keypad_event event, uint32_t index)
{
    char touch_str[20];
	switch (event)
    {
        case NT_KEYPAD_RELEASE:
            {
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();
            }
            break;
        case NT_KEYPAD_TOUCH:

            switch (index)
            {
                case 0:
                    /* WHILE on, full brightness */
                PRINTF("\r\n touch pad pressed \r\n");
                sprintf(touch_str,"touch pad pressed");
                if (newconn != NULL)
                	ret = netconn_write(newconn,touch_str,strlen(touch_str),0);
//				LED_RED_ON();
//                LED_GREEN_TOGGLE();
//                LED_BLUE_ON();
                break;
            case 1:
                /* WHILE on, full brightness */
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
                break;
            case 2:
                /* RED on, full brightness */
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
                break;
            case 3:
                /* BLUE on, full brightness */
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
                break;
            case 4:
                /* GREEN on, full brightness */
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
                break;
            default:
                    break;
            }
            break;

        case NT_KEYPAD_AUTOREPEAT:

            break;

        case NT_KEYPAD_MULTI_TOUCH:
            switch (index)
            {
            case 0:
                    /* BLUE + RED on, full brightness */
                    LED_RED_ON();
                    LED_GREEN_OFF();
                    LED_BLUE_ON();
                    break;
                case 1:
                    /* GREEN + BLUE on, full brightness */
                    LED_RED_OFF();
                    LED_GREEN_ON();
                    LED_BLUE_ON();
                    break;
                case 2:
                    /* GREEN + RED on, full brightness */
                    LED_RED_ON();
                    LED_GREEN_ON();
                    LED_BLUE_OFF();
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

static void aslider_callback(const struct nt_control *control, enum nt_control_aslider_event event, uint32_t position)
{
  switch (event)
    {
        case NT_ASLIDER_INITIAL_TOUCH:
            nt_printf("\n Touch: %d", position);
            if (position < 20)
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            break;
        case NT_ASLIDER_MOVEMENT:
            nt_printf("\n Movement: %d", position);
            if (position < 20)
            {
                LED_RED_TOGGLE();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 20) && (position < 40))
            {
                LED_RED_TOGGLE();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 40) && (position < 60))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_OFF();
            }
            else if ((position >= 60) && (position < 80))
            {
                LED_RED_OFF();
                LED_GREEN_TOGGLE();
                LED_BLUE_TOGGLE();
            }
            else if (position >= 80)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_TOGGLE();
            }
            break;
        case NT_ASLIDER_ALL_RELEASE:
            nt_printf("\n Release: %d", position);
            LED_RED_OFF();
            LED_GREEN_OFF();
            LED_BLUE_OFF();
            break;
    }
}

static void arotary_callback(const struct nt_control *control, enum nt_control_arotary_event event, uint32_t position)
{
  switch (event)
    {
        case NT_AROTARY_INITIAL_TOUCH:
            nt_printf("\n Touch: %d", position);
            if (position < 25)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 25) && (position < 50))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 50) && (position < 75))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 75) && (position < 100))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 100) && (position < 125))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if ((position >= 125) && (position < 150))
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if ((position >= 150) && (position < 175))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if (position >= 175)
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            break;
        case NT_AROTARY_MOVEMENT:
            nt_printf("\n Movement: %d", position);
            if (position < 25)
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 25) && (position < 50))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_OFF();
            }
            else if ((position >= 50) && (position < 75))
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 75) && (position < 100))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_OFF();
            }
            else if ((position >= 100) && (position < 125))
            {
                LED_RED_OFF();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            else if ((position >= 125) && (position < 150))
            {
                LED_RED_OFF();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if ((position >= 150) && (position < 175))
            {
                LED_RED_ON();
                LED_GREEN_OFF();
                LED_BLUE_ON();
            }
            else if (position >= 175)
            {
                LED_RED_ON();
                LED_GREEN_ON();
                LED_BLUE_ON();
            }
            break;
        case NT_AROTARY_ALL_RELEASE:
            nt_printf("\n Release: %d", position);
            break;
    }

    /* Recalculate and set RGB LED */
}

/* Call on the TSI CNTR overflow 16-bit range (65535) */
void system_callback(uint32_t event, union nt_system_event_context *context)
{
    switch (event)
    {
        case NT_SYSTEM_EVENT_OVERRUN:
        {
            /* red colour signalize the error, to solve it increase nt_kernel_data.rom->time_period  */
            LED_RED_ON();
            nt_printf("\n Overrun occurred increase nt_kernel_data.rom->time_period param \n");
            printf("\n Overrun occurred increase nt_kernel_data.rom->time_period param \n");

        }
        case NT_SYSTEM_EVENT_DATA_READY:
            // your code
        break;
        case NT_SYSTEM_EVENT_MODULE_DATA_READY:
            // your code
        break;
        case NT_SYSTEM_EVENT_DATA_OVERFLOW:
            // your code
        break;
    }
}

void buttom_init(void)
{
	 /* Define the init structure for the input switch pin */
	        gpio_pin_config_t sw_config = {
	            kGPIO_DigitalInput,
	            0,
	        };

	        /* Define the init structure for the output LED pin */
//	        gpio_pin_config_t led_config = {
//	            kGPIO_DigitalOutput,
//	            0,
//	        };
	/* Init input switch GPIO. */
	#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) || \
	    (!defined(FSL_FEATURE_SOC_PORT_COUNT))
	    GPIO_SetPinInterruptConfig(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, kGPIO_InterruptFallingEdge);
	#else
	    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
	#endif
//	    EnableIRQ(BOARD_SW3_IRQ);
	    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw_config);

	    /* Init output LED GPIO. */
//	    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &led_config);

}



void P3T1755_I3C_Init(void)
{
	status_t result = kStatus_Success;
	i3c_master_config_t masterConfig;
	p3t1755_config_t p3t1755Config;
	CLOCK_SetClkDiv(kCLOCK_DivI3c1FClk, 6U);
	CLOCK_AttachClk(kPLL0_to_I3C1FCLK);
	I3C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Hz.i2cBaud          = EXAMPLE_I2C_BAUDRATE;
	masterConfig.baudRate_Hz.i3cPushPullBaud  = EXAMPLE_I3C_PP_BAUDRATE;
	masterConfig.baudRate_Hz.i3cOpenDrainBaud = EXAMPLE_I3C_OD_BAUDRATE;
	masterConfig.enableOpenDrainStop          = false;
	masterConfig.disableTimeout               = true;
	I3C_MasterInit(EXAMPLE_MASTER, &masterConfig, I3C_MASTER_CLOCK_FREQUENCY);

	/* Create I3C handle. */
	I3C_MasterTransferCreateHandle(EXAMPLE_MASTER, &g_i3c_m_handle, &masterCallback, NULL);

	result = p3t1755_set_dynamic_address();
	if (result != kStatus_Success)
	{
		PRINTF("\r\nP3T1755 set dynamic address failed.\r\n");
	}
	else
		PRINTF("\r\nP3T1755 set dynamic address succeed.\r\n");
	p3t1755Config.writeTransfer = I3C_WriteSensor;
	p3t1755Config.readTransfer  = I3C_ReadSensor;
	p3t1755Config.sensorAddress = SENSOR_ADDR;

	P3T1755_Init(&p3t1755Handle, &p3t1755Config);


}

void PWM_Init_all(void)
{
    pwm_config_t pwmConfig;
    pwm_fault_param_t faultConfig;
    uint32_t pwmVal = 4;
    /* Board pin, clock, debug console init */
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable PWM1 SUB Clockn */
    SYSCON->PWM1SUBCTL |=
        (SYSCON_PWM1SUBCTL_CLK0_EN_MASK | SYSCON_PWM1SUBCTL_CLK1_EN_MASK | SYSCON_PWM1SUBCTL_CLK2_EN_MASK);

    PRINTF("FlexPWM driver example\n");

    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    PWM_GetDefaultConfig(&pwmConfig);

#ifdef DEMO_PWM_CLOCK_DEVIDER
    pwmConfig.prescale = DEMO_PWM_CLOCK_DEVIDER;
#endif

    /* Use full cycle reload */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation   = kPWM_ComplementaryPwmA;
    pwmConfig.enableDebugMode = true;

    /* Initialize submodule 0 */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        return 1;
    }

    /* Initialize submodule 1, make it use same counter clock as submodule 0. */
    pwmConfig.clockSource           = kPWM_Submodule0Clock;
    pwmConfig.prescale              = kPWM_Prescale_Divide_1;
    pwmConfig.initializationControl = kPWM_Initialize_MasterSync;
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        return 1;
    }

    /* Initialize submodule 2 the same way as submodule 1 */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_2, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
        return 1;
    }

    /*
     *   config->faultClearingMode = kPWM_Automatic;
     *   config->faultLevel = false;
     *   config->enableCombinationalPath = true;
     *   config->recoverMode = kPWM_NoRecovery;
     */
    PWM_FaultDefaultConfig(&faultConfig);

#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultLevel = DEMO_PWM_FAULT_LEVEL;
#endif

    /* Sets up the PWM fault protection */
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_0, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_1, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_2, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_3, &faultConfig);

    /* Set PWM fault disable mapping for submodule 0/1/2 */
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);

    /* Call the init function with demo configuration */
    PWM_DRV_Init3PhPwm();

    /* Set the load okay bit for all submodules to load registers from their buffer */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2);
}



