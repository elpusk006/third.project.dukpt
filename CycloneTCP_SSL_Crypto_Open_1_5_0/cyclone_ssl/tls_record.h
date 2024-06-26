/**
 * @file tls_record.h
 * @brief TLS record protocol
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This file is part of CycloneSSL Open.
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

#ifndef _TLS_RECORD_H
#define _TLS_RECORD_H

//Dependencies
#include "tls.h"

//TLS related functions
error_t tlsWriteProtocolData(TlsContext *context,
   size_t length, TlsContentType contentType);

error_t tlsReadProtocolData(TlsContext *context,
   void **data, size_t *length, TlsContentType *contentType);

error_t tlsWriteRecord(TlsContext *context,
   size_t length, TlsContentType contentType);

error_t tlsReadRecord(TlsContext *context, uint8_t *data,
   size_t size, size_t *length, TlsContentType *contentType);

void tlsIncSequenceNumber(TlsSequenceNumber seqNum);

#endif
