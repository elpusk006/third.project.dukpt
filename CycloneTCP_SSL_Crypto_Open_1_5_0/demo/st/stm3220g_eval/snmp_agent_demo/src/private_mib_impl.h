/**
 * @file private_mib_impl.h
 * @brief Private MIB module implementation
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

#ifndef _PRIVATE_MIB_IMPL_H
#define _PRIVATE_MIB_IMPL_H

//Dependencies
#include "snmp/mib_common.h"

//Private MIB related functions
error_t privateMibInit(void);
void privateMibLock(void);
void privateMibUnlock(void);

error_t privateMibGetCurrentTime(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibSetLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetNextLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

#endif
