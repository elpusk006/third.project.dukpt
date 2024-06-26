/**
 * @file rza1_eth.c
 * @brief Renesas RZ/A1 Ethernet MAC controller
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
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

//Switch to the appropriate trace level
#define TRACE_LEVEL NIC_TRACE_LEVEL

//Dependencies
#include "iodefine.h"
#include "dev_drv.h"
#include "devdrv_intc.h"
#include "core/tcp_ip_stack.h"
#include "drivers/rza1_eth.h"
#include "debug.h"

//IAR EWARM compiler?
#if defined(__ICCARM__)

//Transmit buffer
#pragma data_alignment = 32
static uint8_t txBuffer[RZA1_ETH_TX_BUFFER_COUNT][RZA1_ETH_TX_BUFFER_SIZE];
//Receive buffer
#pragma data_alignment = 32
static uint8_t rxBuffer[RZA1_ETH_RX_BUFFER_COUNT][RZA1_ETH_RX_BUFFER_SIZE];
//Transmit DMA descriptors
#pragma data_alignment = 32
static Rza1TxDmaDesc txDmaDesc[RZA1_ETH_TX_BUFFER_COUNT];
//Receive DMA descriptors
#pragma data_alignment = 32
static Rza1RxDmaDesc rxDmaDesc[RZA1_ETH_RX_BUFFER_COUNT];

//ARM or GCC compiler?
#else

//Transmit buffer
static uint8_t txBuffer[RZA1_ETH_TX_BUFFER_COUNT][RZA1_ETH_TX_BUFFER_SIZE]
   __attribute__((section(".BSS_DMAC_SAMPLE_INTERNAL_RAM"), aligned(32)));
//Receive buffer
static uint8_t rxBuffer[RZA1_ETH_RX_BUFFER_COUNT][RZA1_ETH_RX_BUFFER_SIZE]
   __attribute__((section(".BSS_DMAC_SAMPLE_INTERNAL_RAM"), aligned(32)));
//Transmit DMA descriptors
static Rza1TxDmaDesc txDmaDesc[RZA1_ETH_TX_BUFFER_COUNT]
   __attribute__((section(".BSS_DMAC_SAMPLE_INTERNAL_RAM"), aligned(32)));
//Receive DMA descriptors
static Rza1RxDmaDesc rxDmaDesc[RZA1_ETH_RX_BUFFER_COUNT]
   __attribute__((section(".BSS_DMAC_SAMPLE_INTERNAL_RAM"), aligned(32)));

#endif

//Current transmit descriptor
static uint_t txIndex;
//Current receive descriptor
static uint_t rxIndex;


/**
 * @brief RZ/A1 Ethernet MAC driver
 **/

const NicDriver rza1EthDriver =
{
   rza1EthInit,
   rza1EthTick,
   rza1EthEnableIrq,
   rza1EthDisableIrq,
   rza1EthEventHandler,
   rza1EthSetMacFilter,
   rza1EthSendPacket,
   rza1EthWritePhyReg,
   rza1EthReadPhyReg,
   TRUE,
   TRUE,
   TRUE
};


/**
 * @brief RZ/A1 Ethernet MAC initialization
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t rza1EthInit(NetInterface *interface)
{
   error_t error;

   //Debug message
   TRACE_INFO("Initializing RZ/A1 Ethernet MAC...\r\n");

   //Enable Ethernet peripheral clock
   CPG.STBCR7.BIT.MSTP74 = 0;

   //GPIO configuration
   rza1EthInitGpio(interface);

   //Perform software reset
   ETHER.ARSTR.BIT.ARST = 1;
   //Wait for the reset to complete
   sleep(10);

   //Start EDMAC transmitting unit
   EDMAC.EDSR0.BIT.ENT = 1;
   //Start EDMAC receiving unit
   EDMAC.EDSR0.BIT.ENR = 1;

   //To execute a software reset with this register, 1 must be
   //written to both the SWRT and SWRR bits simultaneously
   EDMAC.EDMR0.LONG = 3;
   //Wait for the reset to complete (transmit FIFO)
   while(EDMAC.EDMR0.BIT.SWRT);
   //Wait for the reset to complete (receive FIFO)
   while(EDMAC.EDMR0.BIT.SWRR);

   //PHY transceiver initialization
   error = interface->phyDriver->init(interface);
   //Failed to initialize PHY transceiver?
   if(error) return error;

   //Initialize DMA descriptor lists
   rza1EthInitDmaDesc(interface);

   //Set descriptor length (16 bytes)
   EDMAC.EDMR0.BIT.DL = 0;
   //Select little endian mode
   EDMAC.EDMR0.BIT.DE = 1;
   //Error masks
   EDMAC.TRSCER0.LONG = 0;
   //Use store and forward mode
   EDMAC.TFTR0 = 0;

   //Set transmit FIFO size (2048 bytes)
   EDMAC.FDR0.BIT.TFD = 7;
   //Set receive FIFO size (2048 bytes)
   EDMAC.FDR0.BIT.RFD = 7;

   //Enable continuous reception of multiple frames
   EDMAC.RMCR0.BIT.RNC = 1;
   //No padding insertion into receive data
   EDMAC.RPADIR0.LONG = 0;

   //Receive FIFO threshold (8 frames)
   EDMAC.FCFTR0.BIT.RFF = 7;
   //Receive FIFO threshold (2048-64 bytes)
   EDMAC.FCFTR0.BIT.RFD = 7;

   //Intelligent checksum operation mode
   EDMAC.CSMR.LONG = 0;

   //Enable multicast address filtering
   MAC.ECMR0.BIT.MCT = 1;

   //Set the upper 32 bits of the MAC address
   MAC.MAHR0 = (interface->macAddr.b[0] << 24) | (interface->macAddr.b[1] << 16) |
      (interface->macAddr.b[2] << 8) | interface->macAddr.b[3];

   //Set the lower 16 bits of the MAC address
   MAC.MALR0 = (interface->macAddr.b[4] << 8) | interface->macAddr.b[5];

   //Maximum frame length that can be accepted
   MAC.RFLR0 = 1518;
   //Automatic pause frame
   MAC.APR0 = 0;
   //Manual pause frame
   MAC.MPR0 = 0;
   //Automatic pause frame retransmit count
   MAC.TPAUSER0 = 0;

   //Disable all EMAC interrupts
   MAC.ECSIPR0.LONG = 0;

   //Disable all EDMAC interrupts
   EDMAC.EESIPR0.LONG = 0;
   //Enable only the desired EDMAC interrupts
   EDMAC.EESIPR0.BIT.TWBIP = 3;
   EDMAC.EESIPR0.BIT.FRIP = 1;

   //Register interrupt handler
   R_INTC_RegistIntFunc(INTC_ID_ETHERI, rza1EthIrqHandler);
   //Configure interrupt priority
   R_INTC_SetPriority(INTC_ID_ETHERI, RZA1_ETH_IRQ_PRIORITY);

   //Enable EDMAC transmission and reception
   MAC.ECMR0.BIT.TE = 1;
   MAC.ECMR0.BIT.RE = 1;

   //Instruct the DMA to poll the receive descriptor list
   EDMAC.EDRRR0.BIT.RR = 1;

   //Force the TCP/IP stack to check the link state
   osSetEvent(&interface->nicRxEvent);
   //RZ/A1 Ethernet MAC is now ready to send
   osSetEvent(&interface->nicTxEvent);

   //Successful initialization
   return __NO_ERROR;
}


//RSK RZ/A1 or ArchiTech Hachiko evaluation board?
#if defined(USE_RSK_RZA1) || defined(USE_ARCHITECH_HACHIKO)

/**
 * @brief GPIO configuration
 * @param[in] interface Underlying network interface
 **/

void rza1EthInitGpio(NetInterface *interface)
{
   //Configure ET_COL (P1_14)
   PORT1.PMCn.BIT.PMCn14 = 1;
   PORT1.PFCn.BIT.PFCn14 = 1;
   PORT1.PFCEn.BIT.PFCEn14 = 1;
   PORT1.PFCAEn.BIT.PFCAEn14 = 0;
   PORT1.PIPCn.BIT.PIPCn14 = 1;

   //Configure ET_TXCLK (P2_0)
   PORT2.PMCn.BIT.PMCn0 = 1;
   PORT2.PFCn.BIT.PFCn0 = 1;
   PORT2.PFCEn.BIT.PFCEn0 = 0;
   PORT2.PFCAEn.BIT.PFCAEn0 = 0;
   PORT2.PIPCn.BIT.PIPCn0 = 1;

   //Configure ET_TXER (P2_1)
   PORT2.PMCn.BIT.PMCn1 = 1;
   PORT2.PFCn.BIT.PFCn1 = 1;
   PORT2.PFCEn.BIT.PFCEn1 = 0;
   PORT2.PFCAEn.BIT.PFCAEn1 = 0;
   PORT2.PIPCn.BIT.PIPCn1 = 1;

   //Configure ET_TXEN (P2_2)
   PORT2.PMCn.BIT.PMCn2 = 1;
   PORT2.PFCn.BIT.PFCn2 = 1;
   PORT2.PFCEn.BIT.PFCEn2 = 0;
   PORT2.PFCAEn.BIT.PFCAEn2 = 0;
   PORT2.PIPCn.BIT.PIPCn2 = 1;

   //Configure ET_CRS (P2_3)
   PORT2.PMCn.BIT.PMCn3 = 1;
   PORT2.PFCn.BIT.PFCn3 = 1;
   PORT2.PFCEn.BIT.PFCEn3 = 0;
   PORT2.PFCAEn.BIT.PFCAEn3 = 0;
   PORT2.PIPCn.BIT.PIPCn3 = 1;

   //Configure ET_TXD0 (P2_4)
   PORT2.PMCn.BIT.PMCn4 = 1;
   PORT2.PFCn.BIT.PFCn4 = 1;
   PORT2.PFCEn.BIT.PFCEn4 = 0;
   PORT2.PFCAEn.BIT.PFCAEn4 = 0;
   PORT2.PIPCn.BIT.PIPCn4 = 1;

   //Configure ET_TXD1 (P2_5)
   PORT2.PMCn.BIT.PMCn5 = 1;
   PORT2.PFCn.BIT.PFCn5 = 1;
   PORT2.PFCEn.BIT.PFCEn5 = 0;
   PORT2.PFCAEn.BIT.PFCAEn5 = 0;
   PORT2.PIPCn.BIT.PIPCn5 = 1;

   //Configure ET_TXD2 (P2_6)
   PORT2.PMCn.BIT.PMCn6 = 1;
   PORT2.PFCn.BIT.PFCn6 = 1;
   PORT2.PFCEn.BIT.PFCEn6 = 0;
   PORT2.PFCAEn.BIT.PFCAEn6 = 0;
   PORT2.PIPCn.BIT.PIPCn6 = 1;

   //Configure ET_TXD3 (P2_7)
   PORT2.PMCn.BIT.PMCn7 = 1;
   PORT2.PFCn.BIT.PFCn7 = 1;
   PORT2.PFCEn.BIT.PFCEn7 = 0;
   PORT2.PFCAEn.BIT.PFCAEn7 = 0;
   PORT2.PIPCn.BIT.PIPCn7 = 1;

   //Configure ET_RXD0 (P2_8)
   PORT2.PMCn.BIT.PMCn8 = 1;
   PORT2.PFCn.BIT.PFCn8 = 1;
   PORT2.PFCEn.BIT.PFCEn8 = 0;
   PORT2.PFCAEn.BIT.PFCAEn8 = 0;
   PORT2.PIPCn.BIT.PIPCn8 = 1;

   //Configure ET_RXD1 (P2_9)
   PORT2.PMCn.BIT.PMCn9 = 1;
   PORT2.PFCn.BIT.PFCn9 = 1;
   PORT2.PFCEn.BIT.PFCEn9 = 0;
   PORT2.PFCAEn.BIT.PFCAEn9 = 0;
   PORT2.PIPCn.BIT.PIPCn9 = 1;

   //Configure ET_RXD2 (P2_10)
   PORT2.PMCn.BIT.PMCn10 = 1;
   PORT2.PFCn.BIT.PFCn10 = 1;
   PORT2.PFCEn.BIT.PFCEn10 = 0;
   PORT2.PFCAEn.BIT.PFCAEn10 = 0;
   PORT2.PIPCn.BIT.PIPCn10 = 1;

   //Configure ET_RXD3 (P2_11)
   PORT2.PMCn.BIT.PMCn11 = 1;
   PORT2.PFCn.BIT.PFCn11 = 1;
   PORT2.PFCEn.BIT.PFCEn11 = 0;
   PORT2.PFCAEn.BIT.PFCAEn11 = 0;
   PORT2.PIPCn.BIT.PIPCn11 = 1;

   //Configure ET_MDIO (P3_3)
   PORT3.PMCn.BIT.PMCn3 = 1;
   PORT3.PFCn.BIT.PFCn3 = 1;
   PORT3.PFCEn.BIT.PFCEn3 = 0;
   PORT3.PFCAEn.BIT.PFCAEn3 = 0;
   PORT3.PIPCn.BIT.PIPCn3 = 1;

   //Configure ET_RXCLK (P3_4)
   PORT3.PMCn.BIT.PMCn4 = 1;
   PORT3.PFCn.BIT.PFCn4 = 1;
   PORT3.PFCEn.BIT.PFCEn4 = 0;
   PORT3.PFCAEn.BIT.PFCAEn4 = 0;
   PORT3.PIPCn.BIT.PIPCn4 = 1;

   //Configure ET_RXER (P3_5)
   PORT3.PMCn.BIT.PMCn5 = 1;
   PORT3.PFCn.BIT.PFCn5 = 1;
   PORT3.PFCEn.BIT.PFCEn5 = 0;
   PORT3.PFCAEn.BIT.PFCAEn5 = 0;
   PORT3.PIPCn.BIT.PIPCn5 = 1;

   //Configure ET_RXDV (P3_6)
   PORT3.PMCn.BIT.PMCn6 = 1;
   PORT3.PFCn.BIT.PFCn6 = 1;
   PORT3.PFCEn.BIT.PFCEn6 = 0;
   PORT3.PFCAEn.BIT.PFCAEn6 = 0;
   PORT3.PIPCn.BIT.PIPCn6 = 1;

   //Configure ET_MDC (P5_9)
   PORT5.PMCn.BIT.PMCn9 = 1;
   PORT5.PFCn.BIT.PFCn9 = 1;
   PORT5.PFCEn.BIT.PFCEn9 = 0;
   PORT5.PFCAEn.BIT.PFCAEn9 = 0;
   PORT5.PIPCn.BIT.PIPCn9 = 1;
}

#endif


/**
 * @brief Initialize DMA descriptor lists
 * @param[in] interface Underlying network interface
 **/

void rza1EthInitDmaDesc(NetInterface *interface)
{
   uint_t i;

   //Initialize TX descriptors
   for(i = 0; i < RZA1_ETH_TX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the application
      txDmaDesc[i].td0 = 0;
      //Transmit buffer length
      txDmaDesc[i].td1 = 0;
      //Transmit buffer address
      txDmaDesc[i].td2 = (uint32_t) txBuffer[i];
      //Clear padding field
      txDmaDesc[i].padding = 0;
   }

   //Mark the last descriptor entry with the TDLE flag
   txDmaDesc[i - 1].td0 |= EDMAC_TD0_TDLE;
   //Initialize TX descriptor index
   txIndex = 0;

   //Initialize RX descriptors
   for(i = 0; i < RZA1_ETH_RX_BUFFER_COUNT; i++)
   {
      //The descriptor is initially owned by the DMA
      rxDmaDesc[i].rd0 = EDMAC_RD0_RACT;
      //Receive buffer length
      rxDmaDesc[i].rd1 = (RZA1_ETH_RX_BUFFER_SIZE << 16) & EDMAC_RD1_RBL;
      //Receive buffer address
      rxDmaDesc[i].rd2 = (uint32_t) rxBuffer[i];
      //Clear padding field
      rxDmaDesc[i].padding = 0;
   }

   //Mark the last descriptor entry with the RDLE flag
   rxDmaDesc[i - 1].rd0 |= EDMAC_RD0_RDLE;
   //Initialize RX descriptor index
   rxIndex = 0;

   //Address of the first TX descriptor
   EDMAC.TDLAR0 = &txDmaDesc[0];
   EDMAC.TDFAR0 = &txDmaDesc[0];
   //Address of the last TX descriptor
   EDMAC.TDFXR0 = &txDmaDesc[RZA1_ETH_TX_BUFFER_COUNT - 1];
   //Set TDLF flag
   EDMAC.TDFFR0.BIT.TDLF = 1;

   //Address of the first RX descriptor
   EDMAC.RDLAR0 = &rxDmaDesc[0];
   EDMAC.RDFAR0 = &rxDmaDesc[0];
   //Address of the last RX descriptor
   EDMAC.RDFXR0 = &rxDmaDesc[RZA1_ETH_RX_BUFFER_COUNT - 1];
   //Set RDLF flag
   EDMAC.RDFFR0.BIT.RDLF = 1;
}


/**
 * @brief RZ/A1 Ethernet MAC timer handler
 *
 * This routine is periodically called by the TCP/IP stack to
 * handle periodic operations such as polling the link state
 *
 * @param[in] interface Underlying network interface
 **/

void rza1EthTick(NetInterface *interface)
{
   //Handle periodic operations
   interface->phyDriver->tick(interface);
}


/**
 * @brief Enable interrupts
 * @param[in] interface Underlying network interface
 **/

void rza1EthEnableIrq(NetInterface *interface)
{
   //Enable Ethernet MAC interrupts
   R_INTC_Enable(INTC_ID_ETHERI);
   //Enable Ethernet PHY interrupts
   interface->phyDriver->enableIrq(interface);
}


/**
 * @brief Disable interrupts
 * @param[in] interface Underlying network interface
 **/

void rza1EthDisableIrq(NetInterface *interface)
{
   //Disable Ethernet MAC interrupts
   R_INTC_Disable(INTC_ID_ETHERI);
   //Disable Ethernet PHY interrupts
   interface->phyDriver->disableIrq(interface);
}


/**
 * @brief RZ/A1 Ethernet MAC interrupt service routine
 * @param intSense Unused parameter
 **/

void rza1EthIrqHandler(uint32_t intSense)
{
   bool_t flag;
   uint32_t status;
   NetInterface *interface;

   //Enter interrupt service routine
   osEnterIsr();

   //Point to the structure describing the network interface
   interface = &netInterface[0];
   //This flag will be set if a higher priority task must be woken
   flag = FALSE;

   //Read interrupt status register
   status = EDMAC.EESR0.LONG;

   //A packet has been transmitted?
   if(status & EDMAC_EESR0_TWB)
   {
      //Clear TWB interrupt flag
      EDMAC.EESR0.LONG = EDMAC_EESR0_TWB;

      //Check whether the TX buffer is available for writing
      if(!(txDmaDesc[txIndex].td0 & EDMAC_TD0_TACT))
      {
         //Notify the user that the transmitter is ready to send
         flag |= osSetEventFromIsr(&interface->nicTxEvent);
      }
   }
   //A packet has been received?
   if(status & EDMAC_EESR0_FR)
   {
      //Disable FR interrupts
      EDMAC.EESIPR0.BIT.FRIP = 0;

      //Notify the user that a packet has been received
      flag |= osSetEventFromIsr(&interface->nicRxEvent);
   }

   //Leave interrupt service routine
   osExitIsr(flag);
}


/**
 * @brief RZ/A1 Ethernet MAC event handler
 * @param[in] interface Underlying network interface
 **/

void rza1EthEventHandler(NetInterface *interface)
{
   error_t error;
   size_t length;
   bool_t linkStateChange;

   //PHY event is pending?
   if(interface->phyEvent)
   {
      //Acknowledge the event by clearing the flag
      interface->phyEvent = FALSE;
      //Handle PHY specific events
      linkStateChange = interface->phyDriver->eventHandler(interface);

      //Check whether the link state has changed?
      if(linkStateChange)
      {
         //Set duplex mode for proper operation
         if(interface->linkState)
         {
            //Half-duplex or full-duplex mode?
            if(interface->fullDuplex)
               MAC.ECMR0.BIT.DM = 1;
            else
               MAC.ECMR0.BIT.DM = 0;
         }

         //Process link state change event
         nicNotifyLinkChange(interface);
      }
   }

   //Packet received?
   if(EDMAC.EESR0.LONG & EDMAC_EESR0_FR)
   {
      //Clear FR interrupt flag
      EDMAC.EESR0.LONG = EDMAC_EESR0_FR;

      //Process all pending packets
      do
      {
         //Read incoming packet
         error = rza1EthReceivePacket(interface,
            interface->ethFrame, ETH_MAX_FRAME_SIZE, &length);

         //Check whether a valid packet has been received
         if(!error)
         {
            //Pass the packet to the upper layer
            nicProcessPacket(interface, interface->ethFrame, length);
         }

         //No more data in the receive buffer?
      } while(error != __ERROR_BUFFER_EMPTY);
   }

   //Re-enable EDMAC interrupts
   EDMAC.EESIPR0.BIT.TWBIP = 3;
   EDMAC.EESIPR0.BIT.FRIP = 1;
}


/**
 * @brief Configure multicast MAC address filtering
 * @param[in] interface Underlying network interface
 * @return Error code
 **/

error_t rza1EthSetMacFilter(NetInterface *interface)
{
   uint_t i;
   volatile uint32_t *addressHigh;
   volatile uint32_t *addressLow;

   //Debug message
   TRACE_INFO("Updating RZ/A1 CAM entry table...\r\n");

   //Disable all CAM entries
   TSU.TSU_TEN.LONG = 0;

   //The MAC filter table contains the multicast MAC addresses
   //to accept when receiving an Ethernet frame
   for(i = 0; i < interface->macFilterSize && i < 32; i++)
   {
      //Point to the current MAC address
      MacAddr *macAddr = &interface->macFilter[i].addr;

      //Debug message
      TRACE_INFO("  %s\r\n", macAddrToString(macAddr, NULL));

      //Point to the CAM entry registers
      addressHigh = &TSU.TSU_ADRH0 + 2 * i;
      addressLow = &TSU.TSU_ADRL0 + 2 * i;

      //The contents of the CAM entry table registers cannot be
      //modified while the ADSBSY flag is set
      while(TSU.TSU_ADSBSY.BIT.ADSBSY);

      //Set the upper 32 bits of the MAC address
      *addressHigh = (macAddr->b[0] << 24) | (macAddr->b[1] << 16) |
         (macAddr->b[2] << 8) | macAddr->b[3];

      //Wait for the ADSBSY flag to be cleared
      while(TSU.TSU_ADSBSY.BIT.ADSBSY);

      //Set the lower 16 bits of the MAC address
      *addressLow = (macAddr->b[4] << 8) | macAddr->b[5];

      //Enable the CAM entry
      TSU.TSU_TEN.LONG |= 1 << (31 - i);
   }

   //Successful processing
   return __NO_ERROR;
}


/**
 * @brief Send a packet
 * @param[in] interface Underlying network interface
 * @param[in] buffer Multi-part buffer containing the data to send
 * @param[in] offset Offset to the first data byte
 * @return Error code
 **/

error_t rza1EthSendPacket(NetInterface *interface,
   const ChunkedBuffer *buffer, size_t offset)
{
   //Retrieve the length of the packet
   size_t length = chunkedBufferGetLength(buffer) - offset;

   //Check the frame length
   if(length > RZA1_ETH_TX_BUFFER_SIZE)
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
      //Report an error
      return __ERROR_INVALID_LENGTH;
   }

   //Make sure the current buffer is available for writing
   if(txDmaDesc[txIndex].td0 & EDMAC_TD0_TACT)
      return __ERROR_FAILURE;

   //Copy user data to the transmit buffer
   chunkedBufferRead(txBuffer[txIndex], buffer, offset, length);

   //Write the number of bytes to send
   txDmaDesc[txIndex].td1 = (length << 16) & EDMAC_TD1_TDL;

   //Check current index
   if(txIndex < (RZA1_ETH_TX_BUFFER_COUNT - 1))
   {
      //Give the ownership of the descriptor to the DMA engine
      txDmaDesc[txIndex].td0 = EDMAC_TD0_TACT | EDMAC_TD0_TFP_SOF |
         EDMAC_TD0_TFP_EOF | EDMAC_TD0_TWBI;

      //Point to the next descriptor
      txIndex++;
   }
   else
   {
      //Give the ownership of the descriptor to the DMA engine
      txDmaDesc[txIndex].td0 = EDMAC_TD0_TACT | EDMAC_TD0_TDLE |
         EDMAC_TD0_TFP_SOF | EDMAC_TD0_TFP_EOF | EDMAC_TD0_TWBI;

      //Wrap around
      txIndex = 0;
   }

   //Instruct the DMA to poll the transmit descriptor list
   EDMAC.EDTRR0.BIT.TR = 3;

   //Check whether the next buffer is available for writing
   if(!(txDmaDesc[txIndex].td0 & EDMAC_TD0_TACT))
   {
      //The transmitter can accept another packet
      osSetEvent(&interface->nicTxEvent);
   }

   //Successful write operation
   return __NO_ERROR;
}


/**
 * @brief Receive a packet
 * @param[in] interface Underlying network interface
 * @param[out] buffer Buffer where to store the incoming data
 * @param[in] size Maximum number of bytes that can be received
 * @param[out] length Number of bytes that have been received
 * @return Error code
 **/

error_t rza1EthReceivePacket(NetInterface *interface,
   uint8_t *buffer, size_t size, size_t *length)
{
   error_t error;
   size_t n;

   //The current buffer is available for reading?
   if(!(rxDmaDesc[rxIndex].rd0 & EDMAC_RD0_RACT))
   {
      //SOF and EOF flags should be set
      if((rxDmaDesc[rxIndex].rd0 & EDMAC_RD0_RFP_SOF) &&
         (rxDmaDesc[rxIndex].rd0 & EDMAC_RD0_RFP_EOF))
      {
         //Make sure no error occurred
         if(!(rxDmaDesc[rxIndex].rd0 & (EDMAC_RD0_RFS_MASK & ~EDMAC_RD0_RFS_RMAF)))
         {
            //Retrieve the length of the frame
            n = rxDmaDesc[rxIndex].rd1 & EDMAC_RD1_RDL;
            //Limit the number of data to read
            n = MIN(n, size - 4);

            //Copy data from the receive buffer
            memcpy(buffer, rxBuffer[rxIndex], n);

            //CRC is not included in the transfer...
            buffer[n++] = 0xCC;
            buffer[n++] = 0xCC;
            buffer[n++] = 0xCC;
            buffer[n++] = 0xCC;

            //Total number of bytes that have been received
            *length = n;
            //Packet successfully received
            error = __NO_ERROR;
         }
         else
         {
            //The received packet contains an error
            error = __ERROR_INVALID_PACKET;
         }
      }
      else
      {
         //The packet is not valid
         error = __ERROR_INVALID_PACKET;
      }

      //Check current index
      if(rxIndex < (RZA1_ETH_RX_BUFFER_COUNT - 1))
      {
         //Give the ownership of the descriptor back to the DMA
         rxDmaDesc[rxIndex].rd0 = EDMAC_RD0_RACT;
         //Point to the next descriptor
         rxIndex++;
      }
      else
      {
         //Give the ownership of the descriptor back to the DMA
         rxDmaDesc[rxIndex].rd0 = EDMAC_RD0_RACT | EDMAC_RD0_RDLE;
         //Wrap around
         rxIndex = 0;
      }

      //Instruct the DMA to poll the receive descriptor list
      EDMAC.EDRRR0.BIT.RR = 1;
   }
   else
   {
      //No more data in the receive buffer
      error = __ERROR_BUFFER_EMPTY;
   }

   //Return status code
   return error;
}


/**
 * @brief Write PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @param[in] data Register value
 **/

void rza1EthWritePhyReg(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
   //Synchronization pattern
   rza1EthWriteSmi(SMI_SYNC, 32);
   //Start of frame
   rza1EthWriteSmi(SMI_START, 2);
   //Set up a write operation
   rza1EthWriteSmi(SMI_WRITE, 2);
   //Write PHY address
   rza1EthWriteSmi(phyAddr, 5);
   //Write register address
   rza1EthWriteSmi(regAddr, 5);
   //Turnaround
   rza1EthWriteSmi(SMI_TA, 2);
   //Write register value
   rza1EthWriteSmi(data, 16);
   //Release MDIO
   rza1EthReadSmi(1);
}


/**
 * @brief Read PHY register
 * @param[in] phyAddr PHY address
 * @param[in] regAddr Register address
 * @return Register value
 **/

uint16_t rza1EthReadPhyReg(uint8_t phyAddr, uint8_t regAddr)
{
   uint16_t data;

   //Synchronization pattern
   rza1EthWriteSmi(SMI_SYNC, 32);
   //Start of frame
   rza1EthWriteSmi(SMI_START, 2);
   //Set up a read operation
   rza1EthWriteSmi(SMI_READ, 2);
   //Write PHY address
   rza1EthWriteSmi(phyAddr, 5);
   //Write register address
   rza1EthWriteSmi(regAddr, 5);
   //Turnaround to avoid contention
   rza1EthReadSmi(1);
   //Read register value
   data = rza1EthReadSmi(16);
   //Force the PHY to release the MDIO pin
   rza1EthReadSmi(1);

   //Return PHY register contents
   return data;
}


/**
 * @brief SMI write operation
 * @param[in] data Raw data to be written
 * @param[in] length Number of bits to be written
 **/

void rza1EthWriteSmi(uint32_t data, uint_t length)
{
   //Skip the most significant bits since they are meaningless
   data <<= 32 - length;

   //Configure MDIO as an output
   MAC.PIR0.BIT.MMD = 1;

   //Write the specified number of bits
   while(length--)
   {
      //Write MDIO
      if(data & 0x80000000)
         MAC.PIR0.BIT.MDO = 1;
      else
         MAC.PIR0.BIT.MDO = 0;

      //Assert MDC
      usleep(1);
      MAC.PIR0.BIT.MDC = 1;
      //Deassert MDC
      usleep(1);
      MAC.PIR0.BIT.MDC = 0;

      //Rotate data
      data <<= 1;
   }
}


/**
 * @brief SMI read operation
 * @param[in] length Number of bits to be read
 * @return Data resulting from the MDIO read operation
 **/

uint32_t rza1EthReadSmi(uint_t length)
{
   uint32_t data = 0;

   //Configure MDIO as an input
   MAC.PIR0.BIT.MMD = 0;

   //Read the specified number of bits
   while(length--)
   {
      //Rotate data
      data <<= 1;

      //Assert MDC
      MAC.PIR0.BIT.MDC = 1;
      usleep(1);
      //Deassert MDC
      MAC.PIR0.BIT.MDC = 0;
      usleep(1);

      //Check MDIO state
      if(MAC.PIR0.BIT.MDI)
         data |= 0x00000001;
   }

   //Return the received data
   return data;
}
