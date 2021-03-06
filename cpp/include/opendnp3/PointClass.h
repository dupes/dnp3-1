
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __POINT_CLASS_H_
#define __POINT_CLASS_H_

namespace opendnp3
{

/// Bitmasks for Class1/2/3 events
enum PointClass {
	PC_CLASS_0 = 0x01,
	PC_CLASS_1 = 0x02,
	PC_CLASS_2 = 0x04,
	PC_CLASS_3 = 0x08,
	PC_ALL_EVENTS = PC_CLASS_1 | PC_CLASS_2 | PC_CLASS_3,
	PC_INVALID = 0x10
};

///converts an integer to a PointClass enumeration
PointClass IntToPointClass(int aClass);

}

#endif

