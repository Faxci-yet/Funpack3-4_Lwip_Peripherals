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
#include "lwip/opt.h"
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
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif
#include "fsl_phy.h"

#include "fsl_enet.h"
#include "fsl_phylan8741.h"

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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
phy_lan8741_resource_t g_phy_resource;

static phy_handle_t phyHandle;
static struct netif netif;

/*******************************************************************************
 * Code
 ******************************************************************************/
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

static void client(void *thread_param)
{
    struct netconn *conn;
    int ret;
    ip4_addr_t ipaddr;
    uint8_t send_buf[]= "This is a TCP Client test...\n";

    for(;;)
    {
        conn = netconn_new(NETCONN_TCP);
        if(conn == NULL)
        {
            PRINTF("create conn failed!\n");
            vTaskDelay(10);
            continue;
        }
        IP4_ADDR(&ipaddr,192,168,2,1); // PC addr
        ret = netconn_connect(conn,&ipaddr,5001);
        if (ret == -1)
        {
            PRINTF("connect failed!\n");
            netconn_close(conn);
            vTaskDelay(10);
            continue;
        }
        PRINTF("\r\nConnect to iperf server successful!\r\n");
        while (1)
        {
            ret = netconn_write(conn,send_buf,sizeof(send_buf),0);
            PRINTF("\r\nThis is a TCP Client test...\r\n");
            vTaskDelay(1000);
        }
    }
}

void client_init(void)
{
    sys_thread_new("client", client, NULL, 512, 4);
}


/*!
 * @brief Initializes lwIP stack.
 */
static void stack_init(void *arg)
{
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

    vTaskDelete(NULL);
}


/*!
 * @brief Main function
 */
int main(void)
{
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(MUX_A(CM_ENETRMIICLKSEL, 0));
    CLOCK_EnableClock(kCLOCK_Enet);
    SYSCON0->PRESETCTRL2 = SYSCON_PRESETCTRL2_ENET_RST_MASK;
    SYSCON0->PRESETCTRL2 &= ~SYSCON_PRESETCTRL2_ENET_RST_MASK;

    MDIO_Init();

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    /* Initialize lwIP from thread */
    if (sys_thread_new("main", stack_init, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("main(): Task creation failed.", 0);

    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}
#endif
