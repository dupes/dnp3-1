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
#ifndef __EVENT_METADATA_H_
#define __EVENT_METADATA_H_

#include <opendnp3/PointClass.h>

namespace opendnp3
{

// Base class for different types of event metadata
template <typename T>
class EventMetadata 
{
	public:
	EventMetadata(): clazz(PC_CLASS_0)
	{}

	inline bool HasEventClass() const { return clazz != PC_CLASS_0; }

	PointClass clazz;
	T lastEvent;
};

template <typename T>
struct SimpleEventMetadata : EventMetadata<T>
{
	public:	
	
	bool CheckForEvent(const T& aValue)
	{		
		if(aValue.IsEvent(lastEvent))
		{
			lastEvent = aValue;
			return true;
		}
		return false;
	}
};

/**
 * Structure for holding metadata information on points that have support deadbanding
 */
template <typename T>
struct DeadbandMetadata : EventMetadata<T>
{	
	DeadbandMetadata() : deadband(0) 
	{}

	bool CheckForEvent(const T& aValue)
	{		
		if(aValue.IsEvent(lastEvent, deadband))
		{
			lastEvent = aValue;
			return true;
		}
		return false;
	}
		
	typename T::DeadbandType deadband;			// deadband associated with measurement (optional)
};

} //end namespace

/* vim: set ts=4 sw=4: */

#endif