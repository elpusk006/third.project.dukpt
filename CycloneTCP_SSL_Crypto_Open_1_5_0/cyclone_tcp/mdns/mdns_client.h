/**
 * @file mdns_client.h
 * @brief mDNS client (Multicast DNS)
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

#ifndef _MDNS_CLIENT_H
#define _MDNS_CLIENT_H

//Dependencies
#include "core/tcp_ip_stack.h"
#include "core/socket.h"
#include "core/udp.h"
#include "dns/dns_cache.h"
#include "dns/dns_common.h"

//mDNS client support
#ifndef MDNS_CLIENT_SUPPORT
   #define MDNS_CLIENT_SUPPORT ENABLED
#elif (MDNS_CLIENT_SUPPORT != ENABLED && MDNS_CLIENT_SUPPORT != DISABLED)
   #error MDNS_CLIENT_SUPPORT parameter is not valid
#endif

//Maximum number of retransmissions of mDNS queries
#ifndef MDNS_CLIENT_MAX_RETRIES
   #define MDNS_CLIENT_MAX_RETRIES 3
#elif (MDNS_CLIENT_MAX_RETRIES < 1)
   #error MDNS_CLIENT_MAX_RETRIES parameter is not valid
#endif

//Initial retransmission timeout
#ifndef MDNS_CLIENT_INIT_TIMEOUT
   #define MDNS_CLIENT_INIT_TIMEOUT 1000
#elif (MDNS_CLIENT_INIT_TIMEOUT < 1000)
   #error MDNS_CLIENT_INIT_TIMEOUT parameter is not valid
#endif

//Maximum retransmission timeout
#ifndef MDNS_CLIENT_MAX_TIMEOUT
   #define MDNS_CLIENT_MAX_TIMEOUT 1000
#elif (MDNS_CLIENT_MAX_TIMEOUT < 1000)
   #error MDNS_CLIENT_MAX_TIMEOUT parameter is not valid
#endif

//Maximum cache lifetime for mDNS entries
#ifndef MDNS_MAX_LIFETIME
   #define MDNS_MAX_LIFETIME 60000
#elif (MDNS_MAX_LIFETIME < 1000)
   #error MDNS_MAX_LIFETIME parameter is not valid
#endif

//mDNS related functions
error_t mdnsResolve(NetInterface *interface,
   const char_t *name, HostType type, IpAddr *ipAddr);

error_t mdnsSendQuery(DnsCacheEntry *entry);

void mdnsProcessResponse(NetInterface *interface, const IpPseudoHeader *pseudoHeader,
   const UdpHeader *udpHeader, const DnsHeader *message, size_t length);

#endif
