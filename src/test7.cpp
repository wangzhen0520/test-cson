#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "elog.h"

using namespace std;

pthread_t startThread(void *(*run)(void *), void *arg) {
	pthread_t th;
	const pthread_attr_t *attr = NULL;
	void *(*func)(void *) = run;
	int result = pthread_create(&th, attr, func, arg);
	if (result != 0) {
		// 表示无法创建该线程
		printf("can't create thread in current application!");
		exit(-1);
	}

	// 表现为join操作，即线程必定阻塞主线程执行，&res表示将返回结果拿到
	//pthread_join(th, res);
    return th;
}

void* run1(void* arg) {
    char* thread_name = ((char**) arg)[0];

    int num = 0;
    do {
        log_i(thread_name);
        // char *buf = "{\"AttributeList\":[{\"attr\":\"IsEcoMode\",\"cmd\":{\"offset\":232,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":152,\"len\":8}},{\"attr\":\"TimeHeatOn\",\"cmd\":{\"offset\":240,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":160,\"len\":16}},{\"attr\":\"TimeHeatOff\",\"cmd\":{\"offset\":256,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":176,\"len\":16}},{\"attr\":\"SetPowerOnTime\",\"cmd\":{\"offset\":272,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":192,\"len\":16}},{\"attr\":\"SetPowerOffTime\",\"cmd\":{\"offset\":288,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":208,\"len\":16}},{\"attr\":\"IsLease\",\"cmd\":{\"offset\":304,\"len\":8,\"bit_index\":3},\"status\":{\"offset\":224,\"len\":8}},{\"attr\":\"LeaseRemainDay\",\"cmd\":{\"offset\":312,\"len\":16,\"bit_index\":3},\"status\":{\"offset\":232,\"len\":16}},{\"attr\":\"IntakeMode\",\"cmd\":{\"offset\":328,\"len\":8,\"bit_index\":4},\"status\":{\"offset\":248,\"len\":8}},{\"attr\":\"PowerSwitchAll\",\"cmd\":{\"offset\":360,\"len\":8,\"bit_index\":6},\"status\":{\"offset\":280,\"len\":8}},{\"attr\":\"WaterTempMode\",\"cmd\":{\"offset\":336,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":256,\"len\":8}},{\"attr\":\"NormalTemperature\",\"cmd\":{\"offset\":344,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":264,\"len\":8}},{\"attr\":\"SetBoilingPoint\",\"cmd\":{\"offset\":352,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":272,\"len\":8}},{\"attr\":\"Buzzer\",\"cmd\":{\"offset\":368,\"len\":8,\"bit_index\":7},\"status\":{\"offset\":288,\"len\":8}},{\"attr\":\"LockScreen\",\"cmd\":{\"offset\":376,\"len\":8,\"bit_index\":8},\"status\":{\"offset\":296,\"len\":8}},{\"attr\":\"OnekeyDrain\",\"cmd\":{\"offset\":0,\"len\":0,\"bit_index\":255},\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"WaterLevel\",\"status\":{\"offset\":320,\"len\":8}},{\"attr\":\"DrainState\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ShowTemperature\",\"status\":{\"offset\":328,\"len\":8}},{\"attr\":\"AmountWater4\",\"status\":{\"offset\":336,\"len\":8}},{\"attr\":\"AmountWater3\",\"status\":{\"offset\":344,\"len\":8}},{\"attr\":\"AmountWater2\",\"status\":{\"offset\":352,\"len\":8}},{\"attr\":\"AmountWater1\",\"status\":{\"offset\":360,\"len\":8}},{\"attr\":\"FilterElemenetFlux1\",\"status\":{\"offset\":368,\"len\":16}},{\"attr\":\"FilterElemenetFlux2\",\"status\":{\"offset\":384,\"len\":16}},{\"attr\":\"FilterElemenetFlux3\",\"status\":{\"offset\":400,\"len\":16}},{\"attr\":\"FilterElemenetFlux4\",\"status\":{\"offset\":416,\"len\":16}},{\"attr\":\"FilterElemenetFlux5\",\"status\":{\"offset\":432,\"len\":16}},{\"attr\":\"FilterElemenetFlux6\",\"status\":{\"offset\":448,\"len\":16}},{\"attr\":\"FilterElemenetFlux7\",\"status\":{\"offset\":464,\"len\":16}},{\"attr\":\"ErrorNum\",\"status\":{\"offset\":536,\"len\":8}},{\"attr\":\"Replace1\",\"status\":{\"offset\":480,\"len\":8}},{\"attr\":\"Replace2\",\"status\":{\"offset\":488,\"len\":8}},{\"attr\":\"Replace3\",\"status\":{\"offset\":496,\"len\":8}},{\"attr\":\"Replace4\",\"status\":{\"offset\":504,\"len\":8}},{\"attr\":\"Replace5\",\"status\":{\"offset\":512,\"len\":8}},{\"attr\":\"Replace6\",\"status\":{\"offset\":520,\"len\":8}},{\"attr\":\"Replace7\",\"status\":{\"offset\":528,\"len\":8}},{\"attr\":\"SignalStrength\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"FilterSoonExpire1\",\"cmd\":{\"offset\":393,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":545,\"len\":1}},{\"attr\":\"FilterSoonExpire2\",\"cmd\":{\"offset\":394,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":546,\"len\":1}},{\"attr\":\"FilterSoonExpire3\",\"cmd\":{\"offset\":395,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":547,\"len\":1}},{\"attr\":\"FilterSoonExpire4\",\"cmd\":{\"offset\":396,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":548,\"len\":1}},{\"attr\":\"FilterSoonExpire5\",\"cmd\":{\"offset\":397,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":549,\"len\":1}},{\"attr\":\"FilterSoonExpire6\",\"cmd\":{\"offset\":398,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":550,\"len\":1}},{\"attr\":\"FilterSoonExpire7\",\"cmd\":{\"offset\":399,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":551,\"len\":1}},{\"attr\":\"FilterExpire1\",\"cmd\":{\"offset\":401,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":553,\"len\":1}},{\"attr\":\"FilterExpire2\",\"cmd\":{\"offset\":402,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":554,\"len\":1}},{\"attr\":\"FilterExpire3\",\"cmd\":{\"offset\":403,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":555,\"len\":1}},{\"attr\":\"FilterExpire4\",\"cmd\":{\"offset\":404,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":556,\"len\":1}},{\"attr\":\"FilterExpire5\",\"cmd\":{\"offset\":405,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":557,\"len\":1}},{\"attr\":\"FilterExpire6\",\"cmd\":{\"offset\":406,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":558,\"len\":1}},{\"attr\":\"FilterExpire7\",\"cmd\":{\"offset\":407,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":559,\"len\":1}},{\"attr\":\"RebootCause\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ErrorMessageRecord\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"SpecialMode\",\"status\":{\"offset\":576,\"len\":8}},{\"attr\":\"WorkMode\",\"status\":{\"offset\":568,\"len\":8}},{\"attr\":\"PermeateMode\",\"cmd\":{\"offset\":416,\"len\":8,\"bit_index\":10},\"status\":{\"offset\":584,\"len\":8}},{\"attr\":\"EcoIdleTime\",\"cmd\":{\"offset\":408,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":560,\"len\":8}}],\"version\":\"V1.0\",\"dataType\":1,\"uploadFilter\":1,\"header\":{\"cmd\":{\"head\":{\"offset\":0,\"len\":2,\"default\":62709},\"len\":{\"offset\":2,\"len\":2},\"type\":{\"offset\":6,\"len\":1,\"default\":201},\"cmd\":{\"offset\":5,\"len\":1,\"default\":2},\"stat\":{\"offset\":0,\"len\":0,\"default\":0},\"flags\":{\"offset\":8,\"len\":0,\"default\":0},\"payloadBitFlag\":{\"offset\":27,\"len\":2},\"payloadData\":{\"offset\":29,\"len\":50},\"crc\":{\"offset\":79,\"len\":2}},\"status\":{\"payloadData\":{\"offset\":19,\"len\":80}}}}";
        const char *buf = "111111111111111111111111111111111111111111";
        log_i(buf);
        num++;

        usleep(100 * 1000);
    } while (num < 10);

    pthread_exit((void *)"num >5");
    
    return NULL;
}

void* run2(void* arg) {
    char* thread_name = ((char**) arg)[0];

    int num = 0;
    do {
        log_i(thread_name);
        // char *buf = "{\"AttributeList\":[{\"attr\":\"IsEcoMode\",\"cmd\":{\"offset\":232,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":152,\"len\":8}},{\"attr\":\"TimeHeatOn\",\"cmd\":{\"offset\":240,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":160,\"len\":16}},{\"attr\":\"TimeHeatOff\",\"cmd\":{\"offset\":256,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":176,\"len\":16}},{\"attr\":\"SetPowerOnTime\",\"cmd\":{\"offset\":272,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":192,\"len\":16}},{\"attr\":\"SetPowerOffTime\",\"cmd\":{\"offset\":288,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":208,\"len\":16}},{\"attr\":\"IsLease\",\"cmd\":{\"offset\":304,\"len\":8,\"bit_index\":3},\"status\":{\"offset\":224,\"len\":8}},{\"attr\":\"LeaseRemainDay\",\"cmd\":{\"offset\":312,\"len\":16,\"bit_index\":3},\"status\":{\"offset\":232,\"len\":16}},{\"attr\":\"IntakeMode\",\"cmd\":{\"offset\":328,\"len\":8,\"bit_index\":4},\"status\":{\"offset\":248,\"len\":8}},{\"attr\":\"PowerSwitchAll\",\"cmd\":{\"offset\":360,\"len\":8,\"bit_index\":6},\"status\":{\"offset\":280,\"len\":8}},{\"attr\":\"WaterTempMode\",\"cmd\":{\"offset\":336,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":256,\"len\":8}},{\"attr\":\"NormalTemperature\",\"cmd\":{\"offset\":344,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":264,\"len\":8}},{\"attr\":\"SetBoilingPoint\",\"cmd\":{\"offset\":352,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":272,\"len\":8}},{\"attr\":\"Buzzer\",\"cmd\":{\"offset\":368,\"len\":8,\"bit_index\":7},\"status\":{\"offset\":288,\"len\":8}},{\"attr\":\"LockScreen\",\"cmd\":{\"offset\":376,\"len\":8,\"bit_index\":8},\"status\":{\"offset\":296,\"len\":8}},{\"attr\":\"OnekeyDrain\",\"cmd\":{\"offset\":0,\"len\":0,\"bit_index\":255},\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"WaterLevel\",\"status\":{\"offset\":320,\"len\":8}},{\"attr\":\"DrainState\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ShowTemperature\",\"status\":{\"offset\":328,\"len\":8}},{\"attr\":\"AmountWater4\",\"status\":{\"offset\":336,\"len\":8}},{\"attr\":\"AmountWater3\",\"status\":{\"offset\":344,\"len\":8}},{\"attr\":\"AmountWater2\",\"status\":{\"offset\":352,\"len\":8}},{\"attr\":\"AmountWater1\",\"status\":{\"offset\":360,\"len\":8}},{\"attr\":\"FilterElemenetFlux1\",\"status\":{\"offset\":368,\"len\":16}},{\"attr\":\"FilterElemenetFlux2\",\"status\":{\"offset\":384,\"len\":16}},{\"attr\":\"FilterElemenetFlux3\",\"status\":{\"offset\":400,\"len\":16}},{\"attr\":\"FilterElemenetFlux4\",\"status\":{\"offset\":416,\"len\":16}},{\"attr\":\"FilterElemenetFlux5\",\"status\":{\"offset\":432,\"len\":16}},{\"attr\":\"FilterElemenetFlux6\",\"status\":{\"offset\":448,\"len\":16}},{\"attr\":\"FilterElemenetFlux7\",\"status\":{\"offset\":464,\"len\":16}},{\"attr\":\"ErrorNum\",\"status\":{\"offset\":536,\"len\":8}},{\"attr\":\"Replace1\",\"status\":{\"offset\":480,\"len\":8}},{\"attr\":\"Replace2\",\"status\":{\"offset\":488,\"len\":8}},{\"attr\":\"Replace3\",\"status\":{\"offset\":496,\"len\":8}},{\"attr\":\"Replace4\",\"status\":{\"offset\":504,\"len\":8}},{\"attr\":\"Replace5\",\"status\":{\"offset\":512,\"len\":8}},{\"attr\":\"Replace6\",\"status\":{\"offset\":520,\"len\":8}},{\"attr\":\"Replace7\",\"status\":{\"offset\":528,\"len\":8}},{\"attr\":\"SignalStrength\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"FilterSoonExpire1\",\"cmd\":{\"offset\":393,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":545,\"len\":1}},{\"attr\":\"FilterSoonExpire2\",\"cmd\":{\"offset\":394,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":546,\"len\":1}},{\"attr\":\"FilterSoonExpire3\",\"cmd\":{\"offset\":395,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":547,\"len\":1}},{\"attr\":\"FilterSoonExpire4\",\"cmd\":{\"offset\":396,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":548,\"len\":1}},{\"attr\":\"FilterSoonExpire5\",\"cmd\":{\"offset\":397,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":549,\"len\":1}},{\"attr\":\"FilterSoonExpire6\",\"cmd\":{\"offset\":398,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":550,\"len\":1}},{\"attr\":\"FilterSoonExpire7\",\"cmd\":{\"offset\":399,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":551,\"len\":1}},{\"attr\":\"FilterExpire1\",\"cmd\":{\"offset\":401,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":553,\"len\":1}},{\"attr\":\"FilterExpire2\",\"cmd\":{\"offset\":402,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":554,\"len\":1}},{\"attr\":\"FilterExpire3\",\"cmd\":{\"offset\":403,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":555,\"len\":1}},{\"attr\":\"FilterExpire4\",\"cmd\":{\"offset\":404,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":556,\"len\":1}},{\"attr\":\"FilterExpire5\",\"cmd\":{\"offset\":405,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":557,\"len\":1}},{\"attr\":\"FilterExpire6\",\"cmd\":{\"offset\":406,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":558,\"len\":1}},{\"attr\":\"FilterExpire7\",\"cmd\":{\"offset\":407,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":559,\"len\":1}},{\"attr\":\"RebootCause\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ErrorMessageRecord\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"SpecialMode\",\"status\":{\"offset\":576,\"len\":8}},{\"attr\":\"WorkMode\",\"status\":{\"offset\":568,\"len\":8}},{\"attr\":\"PermeateMode\",\"cmd\":{\"offset\":416,\"len\":8,\"bit_index\":10},\"status\":{\"offset\":584,\"len\":8}},{\"attr\":\"EcoIdleTime\",\"cmd\":{\"offset\":408,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":560,\"len\":8}}],\"version\":\"V1.0\",\"dataType\":1,\"uploadFilter\":1,\"header\":{\"cmd\":{\"head\":{\"offset\":0,\"len\":2,\"default\":62709},\"len\":{\"offset\":2,\"len\":2},\"type\":{\"offset\":6,\"len\":1,\"default\":201},\"cmd\":{\"offset\":5,\"len\":1,\"default\":2},\"stat\":{\"offset\":0,\"len\":0,\"default\":0},\"flags\":{\"offset\":8,\"len\":0,\"default\":0},\"payloadBitFlag\":{\"offset\":27,\"len\":2},\"payloadData\":{\"offset\":29,\"len\":50},\"crc\":{\"offset\":79,\"len\":2}},\"status\":{\"payloadData\":{\"offset\":19,\"len\":80}}}}";
        const char *buf = "2222222222222222222222222222222222222222222222222";
        log_i(buf);
        num++;

        usleep(100 * 1000);
    } while (num < 10);

    pthread_exit((void *)"num >5");

    return NULL;
}

int test_tree_main() {

    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~(ELOG_FMT_TAG));
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_TAG | ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_TAG | ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    /* start EasyLogger */
    elog_start();

    log_a("Hello EasyLogger!");
    log_e("Hello EasyLogger!");
    log_w("Hello EasyLogger!");
    log_i("Hello EasyLogger!");
    log_d("Hello EasyLogger!");
    log_v("Hello EasyLogger!");

    const char *buf = "{\"AttributeList\":[{\"attr\":\"IsEcoMode\",\"cmd\":{\"offset\":232,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":152,\"len\":8}},{\"attr\":\"TimeHeatOn\",\"cmd\":{\"offset\":240,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":160,\"len\":16}},{\"attr\":\"TimeHeatOff\",\"cmd\":{\"offset\":256,\"len\":16,\"bit_index\":1},\"status\":{\"offset\":176,\"len\":16}},{\"attr\":\"SetPowerOnTime\",\"cmd\":{\"offset\":272,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":192,\"len\":16}},{\"attr\":\"SetPowerOffTime\",\"cmd\":{\"offset\":288,\"len\":16,\"bit_index\":2},\"status\":{\"offset\":208,\"len\":16}},{\"attr\":\"IsLease\",\"cmd\":{\"offset\":304,\"len\":8,\"bit_index\":3},\"status\":{\"offset\":224,\"len\":8}},{\"attr\":\"LeaseRemainDay\",\"cmd\":{\"offset\":312,\"len\":16,\"bit_index\":3},\"status\":{\"offset\":232,\"len\":16}},{\"attr\":\"IntakeMode\",\"cmd\":{\"offset\":328,\"len\":8,\"bit_index\":4},\"status\":{\"offset\":248,\"len\":8}},{\"attr\":\"PowerSwitchAll\",\"cmd\":{\"offset\":360,\"len\":8,\"bit_index\":6},\"status\":{\"offset\":280,\"len\":8}},{\"attr\":\"WaterTempMode\",\"cmd\":{\"offset\":336,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":256,\"len\":8}},{\"attr\":\"NormalTemperature\",\"cmd\":{\"offset\":344,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":264,\"len\":8}},{\"attr\":\"SetBoilingPoint\",\"cmd\":{\"offset\":352,\"len\":8,\"bit_index\":5},\"status\":{\"offset\":272,\"len\":8}},{\"attr\":\"Buzzer\",\"cmd\":{\"offset\":368,\"len\":8,\"bit_index\":7},\"status\":{\"offset\":288,\"len\":8}},{\"attr\":\"LockScreen\",\"cmd\":{\"offset\":376,\"len\":8,\"bit_index\":8},\"status\":{\"offset\":296,\"len\":8}},{\"attr\":\"OnekeyDrain\",\"cmd\":{\"offset\":0,\"len\":0,\"bit_index\":255},\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"WaterLevel\",\"status\":{\"offset\":320,\"len\":8}},{\"attr\":\"DrainState\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ShowTemperature\",\"status\":{\"offset\":328,\"len\":8}},{\"attr\":\"AmountWater4\",\"status\":{\"offset\":336,\"len\":8}},{\"attr\":\"AmountWater3\",\"status\":{\"offset\":344,\"len\":8}},{\"attr\":\"AmountWater2\",\"status\":{\"offset\":352,\"len\":8}},{\"attr\":\"AmountWater1\",\"status\":{\"offset\":360,\"len\":8}},{\"attr\":\"FilterElemenetFlux1\",\"status\":{\"offset\":368,\"len\":16}},{\"attr\":\"FilterElemenetFlux2\",\"status\":{\"offset\":384,\"len\":16}},{\"attr\":\"FilterElemenetFlux3\",\"status\":{\"offset\":400,\"len\":16}},{\"attr\":\"FilterElemenetFlux4\",\"status\":{\"offset\":416,\"len\":16}},{\"attr\":\"FilterElemenetFlux5\",\"status\":{\"offset\":432,\"len\":16}},{\"attr\":\"FilterElemenetFlux6\",\"status\":{\"offset\":448,\"len\":16}},{\"attr\":\"FilterElemenetFlux7\",\"status\":{\"offset\":464,\"len\":16}},{\"attr\":\"ErrorNum\",\"status\":{\"offset\":536,\"len\":8}},{\"attr\":\"Replace1\",\"status\":{\"offset\":480,\"len\":8}},{\"attr\":\"Replace2\",\"status\":{\"offset\":488,\"len\":8}},{\"attr\":\"Replace3\",\"status\":{\"offset\":496,\"len\":8}},{\"attr\":\"Replace4\",\"status\":{\"offset\":504,\"len\":8}},{\"attr\":\"Replace5\",\"status\":{\"offset\":512,\"len\":8}},{\"attr\":\"Replace6\",\"status\":{\"offset\":520,\"len\":8}},{\"attr\":\"Replace7\",\"status\":{\"offset\":528,\"len\":8}},{\"attr\":\"SignalStrength\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"FilterSoonExpire1\",\"cmd\":{\"offset\":393,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":545,\"len\":1}},{\"attr\":\"FilterSoonExpire2\",\"cmd\":{\"offset\":394,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":546,\"len\":1}},{\"attr\":\"FilterSoonExpire3\",\"cmd\":{\"offset\":395,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":547,\"len\":1}},{\"attr\":\"FilterSoonExpire4\",\"cmd\":{\"offset\":396,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":548,\"len\":1}},{\"attr\":\"FilterSoonExpire5\",\"cmd\":{\"offset\":397,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":549,\"len\":1}},{\"attr\":\"FilterSoonExpire6\",\"cmd\":{\"offset\":398,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":550,\"len\":1}},{\"attr\":\"FilterSoonExpire7\",\"cmd\":{\"offset\":399,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":551,\"len\":1}},{\"attr\":\"FilterExpire1\",\"cmd\":{\"offset\":401,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":553,\"len\":1}},{\"attr\":\"FilterExpire2\",\"cmd\":{\"offset\":402,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":554,\"len\":1}},{\"attr\":\"FilterExpire3\",\"cmd\":{\"offset\":403,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":555,\"len\":1}},{\"attr\":\"FilterExpire4\",\"cmd\":{\"offset\":404,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":556,\"len\":1}},{\"attr\":\"FilterExpire5\",\"cmd\":{\"offset\":405,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":557,\"len\":1}},{\"attr\":\"FilterExpire6\",\"cmd\":{\"offset\":406,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":558,\"len\":1}},{\"attr\":\"FilterExpire7\",\"cmd\":{\"offset\":407,\"len\":1,\"bit_index\":255},\"status\":{\"offset\":559,\"len\":1}},{\"attr\":\"RebootCause\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ErrorMessageRecord\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"SpecialMode\",\"status\":{\"offset\":576,\"len\":8}},{\"attr\":\"WorkMode\",\"status\":{\"offset\":568,\"len\":8}},{\"attr\":\"PermeateMode\",\"cmd\":{\"offset\":416,\"len\":8,\"bit_index\":10},\"status\":{\"offset\":584,\"len\":8}},{\"attr\":\"EcoIdleTime\",\"cmd\":{\"offset\":408,\"len\":8,\"bit_index\":0},\"status\":{\"offset\":560,\"len\":8}}],\"version\":\"V1.0\",\"dataType\":1,\"uploadFilter\":1,\"header\":{\"cmd\":{\"head\":{\"offset\":0,\"len\":2,\"default\":62709},\"len\":{\"offset\":2,\"len\":2},\"type\":{\"offset\":6,\"len\":1,\"default\":201},\"cmd\":{\"offset\":5,\"len\":1,\"default\":2},\"stat\":{\"offset\":0,\"len\":0,\"default\":0},\"flags\":{\"offset\":8,\"len\":0,\"default\":0},\"payloadBitFlag\":{\"offset\":27,\"len\":2},\"payloadData\":{\"offset\":29,\"len\":50},\"crc\":{\"offset\":79,\"len\":2}},\"status\":{\"payloadData\":{\"offset\":19,\"len\":80}}}}";
    log_i(buf);

	void *(*func1)(void *) = run1;
	void *(*func2)(void *) = run2;
	const char* arg1[2] = { "thread-1", "" };
	void* args1 = arg1;
	const char* arg2[2] = { "thread-2", "" };
	void* args2 = arg2;

	pthread_t t1 = startThread(func1, args1);
	pthread_t t2 = startThread(func2, args2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}


extern "C" void test7(void)
{
#if 1
    test_tree_main();
#endif
}