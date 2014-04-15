/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __STATIC_SIZE_CONFIGURATION_H_
#define __STATIC_SIZE_CONFIGURATION_H_

#include <cstdint>

// Default configurations for statically allocated buffers int the stack
// They are liberally set by default, but can be reduced for embedded systems

#ifndef OPENDNP3_MAX_TX_APDU_SIZE
#define OPENDNP3_MAX_TX_APDU_SIZE 2048
#endif

#ifndef OPENDNP3_MAX_RX_APDU_SIZE
#define OPENDNP3_MAX_RX_APDU_SIZE 2048
#endif

#define MACRO_NUM_LINK_FRAMES(size) ((static_cast<uint32_t>(size) / 249) + ((static_cast<uint32_t>(size) % 249) ? 1 : 0))

namespace opendnp3
{
namespace sizes
{

// the maximum number of static read object/variation records that can be in any READ request
static const uint16_t MAX_READ_REQUESTS = 8;

// a safeguard parameter to project from DoS attacks, this really only applies to "empty strings"
static const uint32_t MAX_OBJECTS_PER_APDU = 32768;

// the maximum number of queued commands for a master
static const uint8_t MAX_COMMAND_QUEUE_SIZE = 16;

/// The miniumum allowed size for rx and tx APDUs
static const uint32_t MIN_APDU_SIZE = 20;

// the maximum size of a transmitted APDU
static const uint32_t MAX_TX_APDU_SIZE = OPENDNP3_MAX_TX_APDU_SIZE;

// the maximum size of a received APDU
static const uint32_t MAX_RX_APDU_SIZE = OPENDNP3_MAX_RX_APDU_SIZE;

// the default APDU buffer size
static const uint32_t DEFAULT_APDU_BUFFER_SIZE = MAX_TX_APDU_SIZE;

// The maximum number of stacks that can associate to a link layer router
static const uint16_t MAX_STACKS_PER_CHANNEL = 16;

// The number of bytes needed to hold an APDU fully encapsulated with TPDU/LPDU
static const uint32_t APDU_LPDU_BUFFER_SIZE = MACRO_NUM_LINK_FRAMES(MAX_TX_APDU_SIZE) * 292;

// default sizing is big enough to receive a full APDU with full LPDU's
static const uint32_t LINK_RECEIVER_BUFFER_SIZE = MACRO_NUM_LINK_FRAMES(MAX_RX_APDU_SIZE) * 292;

static_assert(MAX_TX_APDU_SIZE >= MIN_APDU_SIZE, "APDU tx buffer size must be at least the minimum size");
static_assert(MAX_RX_APDU_SIZE >= MIN_APDU_SIZE, "APDU rx buffer size must be at least the minimum size");
static_assert(DEFAULT_APDU_BUFFER_SIZE <= MAX_TX_APDU_SIZE, "default buffer size must be less than the maximum");
static_assert(MAX_STACKS_PER_CHANNEL > 0, "At least 1 stack is required per router");
static_assert(LINK_RECEIVER_BUFFER_SIZE >= 292, "Receiver must buffer at least 292 bytes");

}
}

#endif
