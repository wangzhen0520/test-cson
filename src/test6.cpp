#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>

#include "cJSON.h"

using namespace std;

// 路由表结构体
#define WHITE_LIST_MAX (60)
#define ROUTE_LIST_MAX (12)

typedef struct
{
    uint8_t floor;                        // 部署楼层
    uint8_t room;                         // 户号(烟道号)
    uint32_t dest_addr;                   // 终端设备地址
    uint8_t router_num;                   // 路由器数量
    uint32_t router_addr[ROUTE_LIST_MAX]; // 共 12 级路由器地址, 从上往下排列, 后续没有的填 0 地址
} __attribute__((packed)) ft_uart_payload_of_route_s;

typedef struct
{
    uint32_t gateway_addr;  // 网关地址
    uint8_t route_list_num; // 路由个数
    ft_uart_payload_of_route_s route_info[WHITE_LIST_MAX];
} __attribute__((packed)) ft_uart_payload_of_route_list_s;

void route_list_init(ft_uart_payload_of_route_list_s *p)
{
    memset(p, 0, sizeof(ft_uart_payload_of_route_list_s));

    uint8_t route_list_num = 60;
    for (uint16_t i = 0; i < route_list_num; i++) {
        p->route_info[i].floor = route_list_num - i;
        p->route_info[i].room = 1;
        p->route_info[i].dest_addr = 0x12320001 + route_list_num - 1 - i;

        uint16_t router_num = i / 5;
        for (uint16_t j = 0; j < router_num; j++) {
            p->route_info[i].router_addr[j] = p->route_info[j * 5 + 4].dest_addr;
        }
        p->route_info[i].router_num = router_num;
    }
    p->gateway_addr = 0x22320001;
    p->route_list_num = route_list_num;
}

int get_route_index_by_addr(ft_uart_payload_of_route_list_s *p, uint32_t router_addr)
{
    for (uint16_t i = 0; i < p->route_list_num; i++) {
        if (p->route_info[i].dest_addr == router_addr) {
            return i;
        }
    }

    return -1;
}

// 字符串originString以字符串prefix开头
bool string_start_with(const char *oriStr, char *prefix)
{
    // 参数校验
    // FIKS_LOG_RET_IF_FAIL(oriStr != NULL && prefix != NULL, false, "param null");
    // FIKS_LOG_RET_IF_FAIL(strlen(prefix) <= strlen(oriStr), false, "prefix len over origin");
    if (oriStr == NULL || prefix == NULL || strlen(prefix) > strlen(oriStr)) {
        printf("param null");
        return false;
    }

    int n = strlen(prefix);
    for (int i = 0; i < n; i++) {
        if (oriStr[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

typedef struct
{
    cJSON *obj;
    uint32_t addr;
    bool visit;
} parent_obj_detail_s;

typedef struct
{
    int parent_num;
    parent_obj_detail_s parent[ROUTE_LIST_MAX + 1];
    cJSON *child[WHITE_LIST_MAX];
} parent_obj_s;

parent_obj_detail_s *get_parent_obj(parent_obj_s *p, uint32_t addr)
{
    for (int i = 0; i < p->parent_num; i++) {
        if (p->parent[i].addr == addr) {
            return &p->parent[i];
        }
    }

    return NULL;
}

void format_route_list_to_json(ft_uart_payload_of_route_list_s *route)
{
    cJSON *route_list = cJSON_CreateObject();

    parent_obj_s parent_m = {};
    // 首先初始化所有的路由节点
    for (uint16_t i = 0; i < route->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &route->route_info[i];

        uint8_t router_num = p->router_num;

        parent_obj_detail_s *obj_detail = get_parent_obj(&parent_m, route->gateway_addr);
        if (obj_detail == NULL && router_num == 0) {
            if (parent_m.parent_num >= ROUTE_LIST_MAX) {
                break;
            }
            obj_detail = &parent_m.parent[parent_m.parent_num++];
            obj_detail->addr = route->gateway_addr;
            obj_detail->obj = cJSON_CreateArray();
            obj_detail->visit = false;
        }

        for (uint8_t j = 0; j < router_num; j++) {
            uint32_t addr = p->router_addr[j];
            obj_detail = get_parent_obj(&parent_m, addr);
            if (obj_detail == NULL) {
                if (parent_m.parent_num >= ROUTE_LIST_MAX) {
                    break;
                }
                obj_detail = &parent_m.parent[parent_m.parent_num++];
                obj_detail->addr = addr;
                obj_detail->obj = cJSON_CreateArray();
                obj_detail->visit = false;
            }
        }
    }

    for (uint16_t i = 0; i < route->route_list_num; i++) {
        // ft_uart_payload_of_route_s *p = &route->route_info[i];
        // parent_obj_detail_s *obj_detail = get_parent_obj(&parent_m, p->dest_addr);
        // if (obj_detail == NULL) {
        if (parent_m.child[i] == NULL) {
            parent_m.child[i] = cJSON_CreateObject();
        }
        // }
    }

    for (uint16_t i = 0; i < parent_m.parent_num; i++) {
        parent_obj_detail_s *obj_detail = &parent_m.parent[i];
        printf("[%u] %8X %p\n", i, obj_detail->addr, obj_detail->obj);
    }

    for (uint16_t i = 0; i < route->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &route->route_info[i];
        if (parent_m.child[i] != NULL) {
            printf("[%u] %8X %p\n", i, p->dest_addr, parent_m.child[i]);
        }
    }

    char buf[16];
    for (uint16_t i = 0; i < route->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &route->route_info[i];

        uint8_t router_num = p->router_num;
        parent_obj_detail_s *obj_detail = NULL;
        if (router_num == 0) {
            obj_detail = get_parent_obj(&parent_m, route->gateway_addr);
            if (obj_detail != NULL && !obj_detail->visit) {
                int ret_size = snprintf(buf, sizeof(buf), "%04x", obj_detail->addr);
                cJSON_AddStringToObject(route_list, "GwMac", buf);
                cJSON_AddItemToObject(route_list, "P", obj_detail->obj);
                obj_detail->visit = true;
            }

        } else {
            obj_detail = get_parent_obj(&parent_m, p->router_addr[router_num - 1]);
            if (obj_detail != NULL) {
                if (!obj_detail->visit) {
                    int index = get_route_index_by_addr(route, obj_detail->addr);
                    cJSON_AddItemToObject(parent_m.child[index], "P", obj_detail->obj);
                    obj_detail->visit = true;
                }
            }
        }
        int ret_size = snprintf(buf, sizeof(buf), "%u-%u", p->room, p->floor);
        cJSON_AddItemToObject(parent_m.child[i], "F", cJSON_CreateString(buf));
        cJSON_AddItemToArray(obj_detail->obj, parent_m.child[i]);
    }

    cJSON *report = cJSON_CreateObject();
    char *out = cJSON_PrintUnformatted(route_list);
    if (out) {
        cJSON_AddStringToObject(report, "RouterList", out);
        free(out);
    }
    cJSON_Delete(route_list);

    char *out2 = cJSON_PrintUnformatted(report);
    if (out2) {
        printf("%s\n", out2);
        free(out2);
    }

    cJSON_Delete(report);
}

bool get_room_floor_info(ft_uart_payload_of_route_list_s *route, uint32_t addr, uint8_t *room, uint8_t *floor)
{
    for (int i = 0; i < route->route_list_num; i++) {
        if (route->route_info[i].dest_addr == addr) {
            *room = route->route_info[i].room;
            *floor = route->route_info[i].floor;
            return true;
        }
    }
    return false;
}

void format_route_list_to_json2(ft_uart_payload_of_route_list_s *route)
{
    cJSON *route_list = cJSON_CreateObject();

    char buf[16];
    int ret_size = snprintf(buf, sizeof(buf), "%04x", route->gateway_addr);
    cJSON_AddStringToObject(route_list, "GwMac", buf);

    cJSON *route_p = cJSON_CreateArray();
    cJSON_AddItemToObject(route_list, "P", route_p);

    for (int i = 0; i < route->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &route->route_info[i];
        ret_size = snprintf(buf, sizeof(buf), "%u-%u", p->room, p->floor);
        cJSON *route_f = cJSON_CreateArray();
        cJSON_AddItemToArray(route_f, cJSON_CreateString(buf));
        for (int j = 0; j < p->router_num; j++) {
            uint8_t room = 0;
            uint8_t floor = 0;
            if (get_room_floor_info(route, p->router_addr[p->router_num - j - 1], &room, &floor)) {
                ret_size = snprintf(buf, sizeof(buf), "%u-%u", room, floor);
                cJSON_AddItemToArray(route_f, cJSON_CreateString(buf));
            }
        }
        cJSON_AddItemToArray(route_p, route_f);
    }

    cJSON *report = cJSON_CreateObject();
    char *out = cJSON_PrintUnformatted(route_list);
    if (out) {
        cJSON_AddStringToObject(report, "RouterList", out);
        free(out);
    }
    cJSON_Delete(route_list);

    char *out2 = cJSON_PrintUnformatted(report);
    if (out2) {
        printf("%s\n", out2);
        free(out2);
    }

    cJSON_Delete(report);
}

void get_route_list_from_file(const char *filename, ft_uart_payload_of_route_list_s *data)
{
    ifstream readFile;
    readFile.open(filename, ios::in);

    int index = 0;
    if (readFile.is_open()) {
        cout << "Sucess open!" << endl;
        string str;
        while (getline(readFile, str)) {
            // cout << str << endl;
            const char *delim = " ";
            char buf[512];
            snprintf(buf, sizeof(buf), "%s", str.c_str());
            char *p = strtok(buf, delim);
            int item = 0;
            int sub_item_num = 0;
            while (p) {
                if (item == 0) {
                    data->route_info[index].room = atoi(p);
                } else if (item == 1) {
                    data->route_info[index].floor = atoi(p);
                } else if (item == 2) {
                    sscanf(p, "%X", &data->route_info[index].dest_addr);
                } else {
                    sscanf(p, "%X", &data->route_info[index].router_addr[sub_item_num]);
                    sub_item_num++;
                }
                p = strtok(NULL, delim);
                item++;
            }
            data->route_info[index].router_num = sub_item_num;
            index++;
        }
    } else {
        cout << "Open Failure!" << endl;
    }
    readFile.close();
    data->route_list_num = index;
}

void show_route_info(ft_uart_payload_of_route_list_s *data)
{
    for (uint16_t i = 0; i < data->route_list_num; i++) {
        ft_uart_payload_of_route_s *p = &data->route_info[i];
        printf("%2u %2u %04X ", p->room, p->floor, p->dest_addr);
        for (uint16_t j = 0; j < p->router_num; j++) {
            printf("%04X ", p->router_addr[j]);
        }
        printf("\n");
    }
}

extern "C" void test6(void)
{
#if 0
    ft_uart_payload_of_route_list_s orig = {};
    get_route_list_from_file("../router.txt", &orig);
    orig.gateway_addr = 0x22320001;
    show_route_info(&orig);

    format_route_list_to_json2(&orig);
#endif
}