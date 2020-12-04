#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H
#include "bsplib.h"

extern u8 uart_sendbuf[][300];
extern u8 uart_recvbuf[][300];
extern u8 eth_txbuf[1024];
extern u8 eth_rxbuf[1024];

extern void bsp_init(void);
extern void bsp_exec(void);

#endif

