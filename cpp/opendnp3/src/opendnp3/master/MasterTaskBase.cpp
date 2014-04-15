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
#include "MasterTaskBase.h"

#include "AsyncTaskInterfaces.h"

#include <openpal/LogMacros.h>

#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

MasterTaskBase::MasterTaskBase(const openpal::Logger& logger_) : logger(logger_)
{}

bool MasterTaskBase::OnPartialResponse(const APDUResponseRecord& record)
{
	if(this->ValidateIIN(record.IIN)) return this->_OnPartialResponse(record);
	else return false;
}

TaskResult MasterTaskBase::OnFinalResponse(const APDUResponseRecord& record)
{
	if(this->ValidateIIN(record.IIN)) return this->_OnFinalResponse(record);
	else return TR_FAIL;
}

bool MasterTaskBase::ValidateIIN(const IINField& GetIIN) const
{
	return true;
}

SingleRspBase::SingleRspBase(openpal::Logger& arLogger) : MasterTaskBase(arLogger)
{}

bool SingleRspBase::_OnPartialResponse(const APDUResponseRecord& record)
{
	FORMAT_LOG_BLOCK(logger, flags::WARN, "Ignoring non-FIN response to task: %s", this->Name());	
	return false;
}

SimpleRspBase::SimpleRspBase(openpal::Logger& arLogger) : SingleRspBase(arLogger)
{}

TaskResult SimpleRspBase::_OnFinalResponse(const APDUResponseRecord& record)
{
	if(record.objects.Size() > 0)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Unexpected object data in response to task: %s", this->Name());
	}

	return TR_SUCCESS;
}

} //ens ns

