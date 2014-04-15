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
#include <catch.hpp>


#include "NewOutstationTestObject.h"

#include <opendnp3/DNPErrorCodes.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "NewOutstationTestSuite - " name

TEST_CASE(SUITE("InitialState"))
{
	NewOutstationConfig config;
	NewOutstationTestObject test(config);
	
	test.outstation.OnLowerLayerDown();
	REQUIRE(flags::ERR == test.log.PopFilter());

	test.outstation.OnReceive(ReadOnlyBuffer());
	REQUIRE(flags::ERR == test.log.PopFilter());

	test.outstation.OnSendResult(true);
	REQUIRE(flags::ERR == test.log.PopFilter());
}


TEST_CASE(SUITE("UnsupportedFunction"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 10"); // func = initialize application (16)
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); // IIN = device restart + func not supported
}

TEST_CASE(SUITE("WriteIIN"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 50 01 00 07 07 00");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 00 00");
}

TEST_CASE(SUITE("WriteIINEnabled"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 50 01 00 07 07 01");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteIINWrongBit"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 50 01 00 04 04 01");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04");
}

TEST_CASE(SUITE("WriteNonWriteObject"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 01 02 00 07 07 00");
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01");
}

TEST_CASE(SUITE("DelayMeasure"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 17"); //delay measure
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00 34 02 07 01 00 00"); // response, Grp51Var2, count 1, value == 00 00
}

TEST_CASE(SUITE("DelayMeasureExtraData"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 17 DE AD BE EF"); //delay measure
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); // Func not supported
}

TEST_CASE(SUITE("WriteTimeDate"))
{
	NewOutstationConfig config;	
	NewOutstationTestObject t(config);	
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 32 01 07 01 D2 04 00 00 00 00"); // write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00");
	REQUIRE(t.timestamps.size() == 1);
	REQUIRE(t.timestamps.front().msSinceEpoch == 1234);

}

TEST_CASE(SUITE("WriteTimeDateNotAsking"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);	
	t.outstation.OnLowerLayerUp();

	t.timeHandler.isEnabled = false; // reject the time write from user code
	t.SendToOutstation("C0 02 32 01 07 01 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04"); // param error	
	REQUIRE(t.timestamps.size() == 0);
}

TEST_CASE(SUITE("WriteTimeDateMultipleObjects"))
{
	NewOutstationConfig cfg;	
	NewOutstationTestObject t(cfg, DatabaseTemplate());
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 02 32 01 07 02 D2 04 00 00 00 00 D2 04 00 00 00 00"); //write Grp50Var1, value = 1234 ms after epoch
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 04"); // param error +  need time still set	
	REQUIRE(t.timestamps.empty());
}

TEST_CASE(SUITE("BlankIntegrityPoll"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 00");
}

TEST_CASE(SUITE("ReadClass0MultiFragAnalog"))
{
	NewOutstationConfig config;
	config.params.maxTxFragSize = 20; // override to use a fragment length of 20	
	NewOutstationTestObject t(config, DatabaseTemplate::AnalogOnly(8));
	t.outstation.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		for (uint16_t i = 0; i < 8; i++)
		{
			t.db.Update(Analog(0, AQ_ONLINE), i);
		}
	}

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0

	// Response should be (30,1)x2 per fragment, quality ONLINE, value 0
	// 4 fragment response, first 3 fragments should be confirmed, last one shouldn't be
	REQUIRE(t.lower.PopWriteAsHex() == "A0 81 80 00 1E 01 00 00 01 01 00 00 00 00 01 00 00 00 00");
	t.SendToOutstation("C0 00");
	REQUIRE(t.lower.PopWriteAsHex() == "21 81 80 00 1E 01 00 02 03 01 00 00 00 00 01 00 00 00 00");
	t.SendToOutstation("C1 00");
	REQUIRE(t.lower.PopWriteAsHex() == "22 81 80 00 1E 01 00 04 05 01 00 00 00 00 01 00 00 00 00");
	t.SendToOutstation("C2 00");
	REQUIRE(t.lower.PopWriteAsHex() == "43 81 80 00 1E 01 00 06 07 01 00 00 00 00 01 00 00 00 00");	
}

TEST_CASE(SUITE("ReadFuncNotSupported"))
{
	NewOutstationConfig config;
	NewOutstationTestObject t(config);
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation("C0 01 0C 01 06"); //try to read 12/1 (control block)
	REQUIRE(t.lower.PopWriteAsHex() == "C0 81 80 01"); //restart/func not supported
}


void NewTestStaticRead(const std::string& request, const std::string& response)
{	
	NewOutstationConfig config;
	NewOutstationTestObject t(config, DatabaseTemplate::AllTypes(1));	
	t.outstation.OnLowerLayerUp();

	t.SendToOutstation(request);
	REQUIRE(t.lower.PopWriteAsHex() == response);
}

// ---- Static data reads ----- //

TEST_CASE(SUITE("ReadGrp1Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 01 00 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp1Var2ViaIntegrity"))
{
	NewTestStaticRead("C0 01 01 02 06", "C0 81 80 00 01 02 00 00 00 02"); // 1 byte start/stop, packed format
}

TEST_CASE(SUITE("ReadGrp10Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 0A 00 06", "C0 81 80 00 0A 02 00 00 00 02"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp20Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 00 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var1ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 01 06", "C0 81 80 00 14 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("RreadGrp20Var5ViaIntegrity"))
{
	NewTestStaticRead("C0 01 14 05 06", "C0 81 80 00 14 05 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 00 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var1ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 01 06", "C0 81 80 00 1E 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp30Var3ViaIntegrity"))
{
	NewTestStaticRead("C0 01 1E 03 06", "C0 81 80 00 1E 03 00 00 00 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadGrp40Var0ViaIntegrity"))
{
	NewTestStaticRead("C0 01 28 00 06", "C0 81 80 00 28 01 00 00 00 02 00 00 00 00"); // 1 byte start/stop, RESTART quality
}

TEST_CASE(SUITE("ReadByRangeHeader"))
{
	NewOutstationConfig config;	
	NewOutstationTestObject t(config, DatabaseTemplate::AnalogOnly(10));
	t.outstation.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(Analog(42, AQ_ONLINE), 5);
		t.db.Update(Analog(41, AQ_ONLINE), 6);
	}

	t.SendToOutstation("C2 01 1E 02 00 05 06"); // read 30 var 2, [05 : 06]
	REQUIRE(t.lower.PopWriteAsHex() == "C2 81 80 00 1E 02 00 05 06 01 2A 00 01 29 00");
}

template <class PointType>
void TestStaticType(const NewOutstationConfig& config, const DatabaseTemplate& tmp, PointType value, const std::string& rsp)
{
	NewOutstationTestObject t(config, tmp);

	t.outstation.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(PointType(value), 0);
	}

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
	REQUIRE(t.lower.PopWriteAsHex() == rsp);
}

template <class T>
void TestStaticCounter(StaticCounterResponse rspType, T aValue, const std::string& response)
{
	NewOutstationConfig cfg; cfg.defaultStaticResponses.counter = rspType;
	TestStaticType<Counter>(cfg, DatabaseTemplate::CounterOnly(1), aValue, response);
}

TEST_CASE(SUITE("ReadGrp20Var1"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var1, 5, "C0 81 80 00 14 01 00 00 00 01 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var2"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var2, 5, "C0 81 80 00 14 02 00 00 00 01 05 00");
}

TEST_CASE(SUITE("ReadGrp20Var5"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var5, 5, "C0 81 80 00 14 05 00 00 00 05 00 00 00");
}

TEST_CASE(SUITE("ReadGrp20Var6"))
{
	TestStaticCounter(StaticCounterResponse::Group20Var6, 5, "C0 81 80 00 14 06 00 00 00 05 00");
}

template <class T>
void TestStaticAnalog(StaticAnalogResponse aRsp, T aVal, const std::string& arRsp)
{
	NewOutstationConfig cfg; cfg.defaultStaticResponses.analog = aRsp;
	TestStaticType<Analog>(cfg, DatabaseTemplate::AnalogOnly(1), aVal, arRsp);
}

TEST_CASE(SUITE("ReadGrp30Var2"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var2, 100, "C0 81 80 00 1E 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var3"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var3, 65536, "C0 81 80 00 1E 03 00 00 00 00 00 01 00");
}

TEST_CASE(SUITE("ReadGrp30Var4"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var4, 100, "C0 81 80 00 1E 04 00 00 00 64 00");
}

TEST_CASE(SUITE("ReadGrp30Var5"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var5, 95.6, "C0 81 80 00 1E 05 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp30Var6"))
{
	TestStaticAnalog(StaticAnalogResponse::Group30Var6, -20, "C0 81 80 00 1E 06 00 00 00 01 00 00 00 00 00 00 34 C0");
}

template <class T>
void TestStaticBinaryOutputStatus(T aVal, const std::string& response)
{
	NewOutstationConfig cfg;
	NewOutstationTestObject t(cfg, DatabaseTemplate::BinaryOutputStatusOnly(1));
	t.outstation.OnLowerLayerUp();

	{
		Transaction tr(&t.db);
		t.db.Update(BinaryOutputStatus(aVal, TQ_ONLINE), 0);
	}

	t.SendToOutstation("C0 01 3C 01 06"); // Read class 0
	REQUIRE(t.lower.PopWriteAsHex() == response);
}

TEST_CASE(SUITE("ReadGrp10Var2"))
{
	TestStaticBinaryOutputStatus<bool>(true, "C0 81 80 00 0A 02 00 00 00 81");
}

template <class T>
void TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse aRsp, T aVal, const string& response)
{
	NewOutstationConfig cfg; cfg.defaultStaticResponses.analogOutputStatus = aRsp;
	TestStaticType<AnalogOutputStatus>(cfg, DatabaseTemplate::AnalogOutputStatusOnly(1), aVal, response);
}

TEST_CASE(SUITE("ReadGrp40Var1"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var1, 100, "C0 81 80 00 28 01 00 00 00 01 64 00 00 00");
}

TEST_CASE(SUITE("ReadGrp40Var2"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var2, 100, "C0 81 80 00 28 02 00 00 00 01 64 00");
}

TEST_CASE(SUITE("ReadGrp40Var3"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var3, 95.6, "C0 81 80 00 28 03 00 00 00 01 33 33 BF 42");
}

TEST_CASE(SUITE("ReadGrp40Var4"))
{
	TestStaticAnalogOutputStatus(StaticAnalogOutputStatusResponse::Group40Var4, -20.0, "C0 81 80 00 28 04 00 00 00 01 00 00 00 00 00 00 34 C0");
}

