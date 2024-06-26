/**
 * @file tcp_ip_stack.h
 * @brief TCP/IP stack
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

#ifndef _TCP_IP_STACK_H
#define _TCP_IP_STACK_H

//Forward declaration of NetInterface structure
struct _NetInterface;
#define NetInterface struct _NetInterface

//Dependencies
#include "os_port.h"
#include "tcp_ip_stack_config.h"
#include "core/tcp_ip_stack_mem.h"
#include "endian.h"
#include "error.h"
#include "core/nic.h"
#include "core/ethernet.h"
#include "ipv4/ipv4.h"
#include "ipv4/ipv4_frag.h"
#include "ipv6/ipv6.h"
#include "ipv6/ipv6_frag.h"
#include "ipv4/arp.h"
#include "ipv6/ndp.h"
#include "ipv6/slaac.h"
#include "dns/dns_client.h"
#include "snmp/mib2_module.h"

//Number of network adapters
#ifndef NET_INTERFACE_COUNT
   #define NET_INTERFACE_COUNT 1
#elif (NET_INTERFACE_COUNT < 1)
   #error NET_INTERFACE_COUNT parameter is not valid
#endif

//Maximum number of callback functions that can be registered
//to monitor link changes
#ifndef TCP_IP_CALLBACK_TABLE_SIZE
   #define TCP_IP_CALLBACK_TABLE_SIZE 6
#elif (TCP_IP_CALLBACK_TABLE_SIZE < 1)
   #error TCP_IP_CALLBACK_TABLE_SIZE parameter is not valid
#endif

//Maximum length of interface name
#ifndef TCP_IP_MAX_IF_NAME_LEN
   #define TCP_IP_MAX_IF_NAME_LEN 8
#elif (TCP_IP_MAX_IF_NAME_LEN < 1)
   #error TCP_IP_MAX_IF_NAME_LEN parameter is not valid
#endif

//Maximum length of host name
#ifndef TCP_IP_MAX_HOSTNAME_LEN
   #define TCP_IP_MAX_HOSTNAME_LEN 16
#elif (TCP_IP_MAX_HOSTNAME_LEN < 1)
   #error TCP_IP_MAX_HOSTNAME_LEN parameter is not valid
#endif

//Maximum length of proxy server name
#ifndef TCP_IP_MAX_PROXY_NAME_LEN
   #define TCP_IP_MAX_PROXY_NAME_LEN 16
#elif (TCP_IP_MAX_PROXY_NAME_LEN < 1)
   #error TCP_IP_MAX_PROXY_NAME_LEN parameter is not valid
#endif

//OS resources are statically allocated at compile time
#ifndef TCP_IP_STATIC_OS_RESOURCES
   #define TCP_IP_STATIC_OS_RESOURCES DISABLED
#elif (TCP_IP_STATIC_OS_RESOURCES != ENABLED && TCP_IP_STATIC_OS_RESOURCES != DISABLED)
   #error TCP_IP_STATIC_OS_RESOURCES parameter is not valid
#endif

//Stack size required to run the TCP/IP tick task
#ifndef TCP_IP_TICK_STACK_SIZE
   #define TCP_IP_TICK_STACK_SIZE 550
#elif (TCP_IP_TICK_STACK_SIZE < 1)
   #error TCP_IP_TICK_STACK_SIZE parameter is not valid
#endif

//Priority at which the TCP/IP tick task should run
#ifndef TCP_IP_TICK_PRIORITY
   #define TCP_IP_TICK_PRIORITY 1
#elif (TCP_IP_TICK_PRIORITY < 0)
   #error TCP_IP_TICK_PRIORITY parameter is not valid
#endif

//TCP/IP stack tick interval
#ifndef TCP_IP_TICK_INTERVAL
   #define TCP_IP_TICK_INTERVAL 100
#elif (TCP_IP_TICK_INTERVAL < 100)
   #error TCP_IP_TICK_INTERVAL parameter is not valid
#endif

//Stack size required to run the TCP/IP RX task
#ifndef TCP_IP_RX_STACK_SIZE
   #define TCP_IP_RX_STACK_SIZE 650
#elif (TCP_IP_RX_STACK_SIZE < 1)
   #error TCP_IP_RX_STACK_SIZE parameter is not valid
#endif

//Priority at which the TCP/IP RX task should run
#ifndef TCP_IP_RX_PRIORITY
   #define TCP_IP_RX_PRIORITY 2
#elif (TCP_IP_RX_PRIORITY < 0)
   #error TCP_IP_RX_PRIORITY parameter is not valid
#endif


/**
 * @brief Structure describing a network interface
 **/

struct _NetInterface
{
   uint32_t id;                                         ///<A unique number identifying the interface
   char_t name[TCP_IP_MAX_IF_NAME_LEN];                 ///<A unique name identifying the interface
   char_t hostname[TCP_IP_MAX_HOSTNAME_LEN];            ///<Host name
   char_t mdnsHostname[TCP_IP_MAX_HOSTNAME_LEN + 6];    ///<mDNS host name
   MacAddr macAddr;                                     ///<Link-layer address
   char_t proxyName[TCP_IP_MAX_PROXY_NAME_LEN];         ///<Proxy server name
   uint16_t proxyPort;                                  ///<Proxy server port
   OsMutex macFilterMutex;                              ///<Mutex preventing simultaneous access to the MAC filter table
   MacFilterEntry macFilter[MAC_FILTER_MAX_SIZE];       ///<MAC filter table
   uint_t macFilterSize;                                ///<Number of entries in the MAC filter table
   uint8_t ethFrame[1534 /*ETH_MAX_FRAME_SIZE*/];       ///<Incoming Ethernet frame
   OsEvent nicTxEvent;                                  ///<Network controller TX event
   OsEvent nicRxEvent;                                  ///<Network controller RX event
   bool_t phyEvent;                                     ///<A PHY event is pending
   OsMutex nicDriverMutex;                              ///<Mutex preventing simultaneous access to the NIC driver
   const NicDriver *nicDriver;                          ///<NIC driver
   const PhyDriver *phyDriver;                          ///<PHY driver
   const SpiDriver *spiDriver;                          ///<Underlying SPI driver
   const ExtIntDriver *extIntDriver;                    ///<External interrupt line driver
   uint8_t nicContext[NIC_CONTEXT_SIZE];                ///<Driver specific context
   bool_t linkState;                                    ///<Link state
   bool_t speed100;                                     ///<Link speed
   bool_t speed1000;
   bool_t fullDuplex;                                   ///<Duplex mode
   bool_t configured;                                   ///<Configuration done

#if (IPV4_SUPPORT == ENABLED)
   Ipv4Config ipv4Config;                               ///<IPv4 configuration
   uint16_t ipv4Identification;                         ///<IPv4 fragment identification field
#if (IPV4_FRAG_SUPPORT == ENABLED)
   OsMutex ipv4FragQueueMutex;                          ///<Mutex preventing simultaneous access to reassembly queue
   Ipv4FragDesc ipv4FragQueue[IPV4_MAX_FRAG_DATAGRAMS]; ///<IPv4 fragment reassembly queue
#endif
   OsMutex arpCacheMutex;                               ///<Mutex preventing simultaneous access to ARP cache
   ArpCacheEntry arpCache[ARP_CACHE_SIZE];              ///<ARP cache
   OsMutex ipv4FilterMutex;                             ///<Mutex preventing simultaneous access to the IPv4 filter table
   Ipv4FilterEntry ipv4Filter[IPV4_FILTER_MAX_SIZE];    ///<IPv4 filter table
   uint_t ipv4FilterSize;                               ///<Number of entries in the IPv4 filter table
#if (IGMP_SUPPORT == ENABLED)
   systime_t igmpv1RouterPresentTimer;                  ///<IGMPv1 router present timer
   bool_t igmpv1RouterPresent;                          ///<An IGMPv1 query has been recently heard
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
   Ipv6Config ipv6Config;                               ///<IPv6 configuration
#if (IPV6_FRAG_SUPPORT == ENABLED)
   uint32_t ipv6Identification;                         ///<IPv6 Fragment identification field
   OsMutex ipv6FragQueueMutex;                          ///<Mutex preventing simultaneous access to reassembly queue
   Ipv6FragDesc ipv6FragQueue[IPV6_MAX_FRAG_DATAGRAMS]; ///<IPv6 fragment reassembly queue
#endif
   OsMutex ndpCacheMutex;                               ///<Mutex preventing simultaneous access to Neighbor cache
   NdpCacheEntry ndpCache[NDP_CACHE_SIZE];              ///<Neighbor cache
   OsMutex ipv6FilterMutex;                             ///<Mutex preventing simultaneous access to the IPv6 filter table
   Ipv6FilterEntry ipv6Filter[IPV6_FILTER_MAX_SIZE];    ///<IPv6 filter table
   uint_t ipv6FilterSize;                               ///<Number of entries in the IPv6 filter table
#if (SLAAC_SUPPORT == ENABLED)
   SlaacContext *slaacContext;                          ///<SLAAC context
#endif
#endif

#if (TCP_IP_STATIC_OS_RESOURCES == ENABLED)
   OsTask rxTask;                                       ///<Task that handles incoming frames
   uint_t rxTaskStack[TCP_IP_RX_STACK_SIZE];            ///<Stack size required to run RX task
#else
   OsTask *rxTask;                                      ///<Task that handles incoming frames
#endif

#if (MIB2_SUPPORT == ENABLED)
   Mib2IfEntry *mibIfEntry;
#endif
};


/**
 * @brief Link change callback
 **/

typedef void (*LinkChangeCallback)(NetInterface *interface, bool_t linkState, void *params);


/**
 * @brief Entry describing a user callback
 **/

typedef struct
{
   NetInterface *interface;
   LinkChangeCallback callback;
   void *params;
} LinkChangeCallbackDesc;


//Network interfaces
extern NetInterface netInterface[NET_INTERFACE_COUNT];

//TCP/IP stack related functions
error_t tcpIpStackInit(void);

error_t tcpIpStackSetInterfaceName(NetInterface *interface, const char_t *name);
error_t tcpIpStackSetHostname(NetInterface *interface, const char_t *name);
error_t tcpIpStackSetDriver(NetInterface *interface, const NicDriver *driver);
error_t tcpIpStackSetPhyDriver(NetInterface *interface, const PhyDriver *driver);
error_t tcpIpStackSetSpiDriver(NetInterface *interface, const SpiDriver *driver);
error_t tcpIpStackSetExtIntDriver(NetInterface *interface, const ExtIntDriver *driver);
error_t tcpIpStackSetMacAddr(NetInterface *interface, const MacAddr *macAddr);
error_t tcpIpStackSetProxy(NetInterface *interface, const char_t *name, uint16_t port);

error_t tcpIpStackConfigInterface(NetInterface *interface);

void tcpIpStackTickTask(void *param);
void tcpIpStackRxTask(void *param);

NetInterface *tcpIpStackGetDefaultInterface(void);

error_t tcpIpStackInitRand(uint32_t seed);
uint32_t tcpIpStackGetRand(void);
int32_t tcpIpStackGetRandRange(int32_t min, int32_t max);

error_t tcpIpStackAttachLinkChangeCallback(NetInterface *interface,
   LinkChangeCallback callback, void *params, uint_t *cookie);

error_t tcpIpStackDetachLinkChangeCallback(uint_t cookie);

void tcpIpStackInvokeLinkChangeCallback(NetInterface *interface, bool_t linkState);

#endif
