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
#ifndef __STATIC_BUFFER_H_
#define __STATIC_BUFFER_H_

#include "StaticArray.h"
#include "BufferWrapper.h"
#include <cstdint>

namespace openpal
{

template <uint32_t N>
class StaticBuffer : public StaticArray<uint8_t, uint32_t, N>
{

public:

	StaticBuffer() : StaticArray<uint8_t, uint32_t, N>()
	{}

	ReadOnlyBuffer ToReadOnly() const
	{
		return ReadOnlyBuffer(this->buffer, this->size);
	}

	WriteBuffer GetWriteBuffer()
	{
		return WriteBuffer(this->buffer, this->Size());
	}

	WriteBuffer GetWriteBuffer(uint32_t maxSize)
	{		
		if (maxSize <= this->Size())
		{
			return WriteBuffer(this->buffer, maxSize);
		}
		else
		{
			return GetWriteBuffer();
		}
	}
};

}

#endif
