#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cson.h"
#if 0
typedef struct
{
    uint32_t year;
    uint32_t startWeek;
    uint32_t weekNum;
    // uint32_t weekList[53];
    uint32_t *weekList;
} cycle_list;

typedef struct
{
    uint16_t timeOn;
    uint16_t timeOff;
    uint32_t cycleNum;
    cycle_list *cycleList;
} cycle_date_info;

reflect_item_t wp_cycle_list_ref_tbl[] = {
    _property_int2(cycle_list, year, "Year"),
    _property_int2(cycle_list, startWeek, "StartWeek"),
    _property_int_ex(cycle_list, weekNum, _ex_args_all),
    _property_array_int2(cycle_list, weekList, "WeekList", int, "weekNum"),
    // _property_array_ints2(cycle_list, weekList, "WeekList", int, "weekNum"),
    _property_end(),
};

reflect_item_t wp_cycle_time_heat_date_info_ref_tbl[] = {
    _property_int2(cycle_date_info, timeOn, "TimeHeatOn"),
    _property_int2(cycle_date_info, timeOff, "TimeHeatOff"),
    _property_int_ex(cycle_date_info, cycleNum, _ex_args_all),
    // _property_array2(cycle_date_info, cycleList, "CycleList", wp_cycle_list_ref_tbl, cycle_list, "cycleNum"),
    _property_array_object2(cycle_date_info, cycleList, "CycleList", wp_cycle_list_ref_tbl, cycle_list, "cycleNum"),
    _property_end(),
};

// reflect_item_t wp_cycle_power_date_info_ref_tbl[] = {
//     _property_int2(cycle_date_info, timeOn, "SetPowerOnTime"),
//     _property_int2(cycle_date_info, timeOff, "SetPowerOffTime"),
//     _property_int_ex(cycle_date_info, cycleNum, _ex_args_all),
//     _property_array_objects2(
//         cycle_date_info, cycleList, "CycleList", wp_cycle_list_ref_tbl, cycle_date_info, "cycleNum"),
//     _property_end(),
// };

cycle_date_info gs_time_heat_date_info = {
    .timeOn = 0xffff,
    .timeOff = 0xffff,
};
static int parse_time_heat_cycle_date_info(char *msg)
{
    // 先释放内存
    csonFreePointer(&gs_time_heat_date_info, wp_cycle_time_heat_date_info_ref_tbl);

    gs_time_heat_date_info.timeOn = 0xffff;
    gs_time_heat_date_info.timeOff = 0xffff;
    int ret = csonJsonStr2Struct(msg, &gs_time_heat_date_info, wp_cycle_time_heat_date_info_ref_tbl);
    // FIKS_LOG_RET_VAL_IF_FAIL(ret == ERR_NONE, ret, "parse json failed, ret: %d", ret);
    if (ret != ERR_NONE) {
        printf("parse json failed, ret: %d\n", ret);
    }

    // csonFreePointer(&gs_time_heat_date_info, wp_cycle_time_heat_date_info_ref_tbl);

    printf("cycleNum: %d\n", gs_time_heat_date_info.cycleNum);
    for (int i = 0; i < gs_time_heat_date_info.cycleNum; i++) {
        printf("weekNum: %d\n", gs_time_heat_date_info.cycleList[i].weekNum);
        // for (int j = 0; j < gs_time_heat_date_info.cycleList[i].weekNum; j++) {
        //     printf("weekList: %d\n", gs_time_heat_date_info.cycleList[i].weekList[j]);
        // }
    }

    return ret;
}

void test4()
{
    char *msg = "{\"TimeHeatOn\": 1792, \"TimeHeatOff\": 5888, \"CycleList\": [{\"Year\": 2023, \"StartWeek\": 30, "
                "\"WeekList\": [60, 62, 62, 62, 62, 62, 62, 62, 62, 30, 192, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, "
                "62, 62]}]}";

    parse_time_heat_cycle_date_info(msg);
    printf("~~~~~~~~~\n");
    parse_time_heat_cycle_date_info(msg);
}
#endif