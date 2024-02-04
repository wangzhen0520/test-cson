#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cson.h"

#define FIKS_LOG_RET_VAL_IF_FAIL(exp, ret, ...) \
    do {                                        \
        if (!(exp)) {                           \
            printf(__VA_ARGS__);                \
            printf("\r\n");                     \
            return ret;                         \
        }                                       \
    } while (0);

typedef struct
{
    uint32_t year;
    uint32_t startWeek;
    uint32_t weekNum;
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
    _property_end(),
};

reflect_item_t wp_cycle_time_heat_date_info_ref_tbl[] = {
    _property_int2(cycle_date_info, timeOn, "TimeHeatOn"),
    _property_int2(cycle_date_info, timeOff, "TimeHeatOff"),
    _property_int_ex(cycle_date_info, cycleNum, _ex_args_all),
    _property_array_object2(cycle_date_info, cycleList, "CycleList", wp_cycle_list_ref_tbl, cycle_list, "cycleNum"),
    _property_end(),
};

reflect_item_t wp_cycle_power_date_info_ref_tbl[] = {
    _property_int2(cycle_date_info, timeOn, "SetPowerOnTime"),
    _property_int2(cycle_date_info, timeOff, "SetPowerOffTime"),
    _property_int_ex(cycle_date_info, cycleNum, _ex_args_all),
    _property_array_object2(cycle_date_info, cycleList, "CycleList", wp_cycle_list_ref_tbl, cycle_list, "cycleNum"),
    _property_end(),
};

static cycle_date_info gs_time_heat_date_info = {
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
    FIKS_LOG_RET_VAL_IF_FAIL(ret == ERR_NONE, ret, "parse json failed, ret: %d", ret);
    return ret;
}

static cycle_date_info gs_time_power_date_info = {
    .timeOn = 0xffff,
    .timeOff = 0xffff,
};
static int parse_time_power_cycle_date_info(char *msg)
{
    printf("parse Free 1111111~~~~~~~\n");
    // 先释放内存
    csonFreePointer(&gs_time_power_date_info, wp_cycle_power_date_info_ref_tbl);
    printf("parse Free done 1111111~~~~~~~\n");
    memset(&gs_time_power_date_info, 0, sizeof(gs_time_power_date_info));

    gs_time_power_date_info.timeOn = 0xffff;
    gs_time_power_date_info.timeOff = 0xffff;
    int ret = csonJsonStr2Struct(msg, &gs_time_power_date_info, wp_cycle_power_date_info_ref_tbl);
    printf("parse csonJson 1111111~~~~~~~\n");
    FIKS_LOG_RET_VAL_IF_FAIL(ret == ERR_NONE, ret, "parse json failed, ret: %d", ret);
    return ret;
}

void test5()
{
    char *timeswitch =
        "{\"SetPowerOnTime\":1792,\"SetPowerOffTime\":5888,\"CycleList\":[{\"Year\":2023,\"StartWeek\":35,"
        "\"WeekList\":[224,254]}]}";
    char *timeswitch_without_cyclist = "{\"SetPowerOnTime\":1792,\"SetPowerOffTime\":5888}";

    char *timeheat = "{\"TimeHeatOn\":1792,\"TimeHeatOff\":5888,\"CycleList\":[{\"Year\":2023,\"StartWeek\":35,"
                     "\"WeekList\":[224,254,254,254,254,254,254,254,254,6,0,0,0,0,0,0,0,0]},{\"Year\":2024,"
                     "\"StartWeek\":1,\"WeekList\":[0,0,0,0,0,0,0,0,224,254,254,254,254,254,254,254,254,6]}]}";
    char *timeheat_without_cyclist = "{\"TimeHeatOn\":1792,\"TimeHeatOff\":5888}";

    int cnt = 10;
    while (cnt-- > 0) {
        printf("parse 1111111~~~~~~~\n");
        parse_time_power_cycle_date_info(timeswitch);
        printf("parse 222222222~~~~~~~\n");
        parse_time_power_cycle_date_info(timeswitch_without_cyclist);
    }

    printf("done~~~~~~~\n");
}