#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"

// 路由表结构体
#define ROUTE_LIST_MAX (12)
#define WHITE_LIST_MAX (60)

typedef struct
{
    uint8_t connect_status;               // 连接状态
    uint32_t gateway_addr;                // 网关地址
    uint8_t floor;                        // 部署楼层
    uint8_t room;                         // 户号(烟道号)
    uint32_t dest_addr;                   // 终端设备地址
    uint8_t router_num;                   // 路由器数量
    uint32_t router_addr[ROUTE_LIST_MAX]; // 共 12 级路由器地址, 从上往下排列, 后续没有的填 0 地址
} __attribute__((packed)) ft_uart_payload_of_route_s;

typedef struct
{
    uint8_t route_list_num; // 路由个数
    ft_uart_payload_of_route_s route_info[WHITE_LIST_MAX];
} __attribute__((packed)) ft_uart_payload_of_route_list_s;

typedef struct
{
    uint32_t dest_addr; // 终端设备地址
    uint8_t floor;      // 部署楼层
    uint8_t room;       // 户号(烟道号)
} __attribute__((packed)) ft_uart_payload_of_destaddr_s;

typedef struct
{
    uint8_t dest_addr_num; // 终端个数
    ft_uart_payload_of_destaddr_s dest_addr_info[WHITE_LIST_MAX];
} __attribute__((packed)) ft_uart_payload_of_whitelist_s;

static ft_uart_payload_of_route_list_s gs_route_list = {};
void route_list_init()
{
    memset(&gs_route_list, 0, sizeof(ft_uart_payload_of_route_list_s));

    uint8_t route_list_num = 60;
    for (uint16_t i = 0; i < route_list_num; i++) {
        gs_route_list.route_info[i].connect_status = 0;
        gs_route_list.route_info[i].gateway_addr = 0x22320001;
        gs_route_list.route_info[i].floor = route_list_num - i;
        gs_route_list.route_info[i].room = 1;
        gs_route_list.route_info[i].dest_addr = 0x12320001 + route_list_num - 1 - i;

        uint16_t router_num = i / 5;
        for (uint16_t j = 0; j < router_num; j++) {
            gs_route_list.route_info[i].router_addr[j] = gs_route_list.route_info[j * 5 + 4].dest_addr;
        }
        gs_route_list.route_info[i].router_num = router_num;
    }
    gs_route_list.route_list_num = route_list_num;
}

int get_route_index_by_addr(uint32_t router_addr)
{
    for (uint16_t i = 0; i < gs_route_list.route_list_num; i++) {
        if (gs_route_list.route_info[i].dest_addr == router_addr) {
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

void format_route_list_to_json()
{
    char buf[16];

    cJSON *route_list = cJSON_CreateObject();
    cJSON *parent[WHITE_LIST_MAX] = {NULL};
    cJSON *child[WHITE_LIST_MAX] = {NULL};
    for (uint16_t i = 0; i < gs_route_list.route_list_num; i++) {
        uint8_t router_num = gs_route_list.route_info[i].router_num;
        if (parent[router_num] == NULL) {
            parent[router_num] = cJSON_CreateArray();
            int index = 0;
            if (router_num == 0) { // 网关
                // int ret_size = snprintf(
                //     buf, sizeof(buf), "%04x", util_ld_u32_be(gs_route_list.route_info[router_num].gateway_addr));
                int ret_size = snprintf(buf, sizeof(buf), "%04x", gs_route_list.route_info[router_num].gateway_addr);
                // FIKS_LOG_IF_FAIL(ret_size >= 0, "snprintf failed, ret_size: %d", ret_size);
                cJSON_AddStringToObject(route_list, "GwMac", buf);
                cJSON_AddItemToObject(route_list, "P", parent[router_num]);
            } else { // 其他节点
                index = get_route_index_by_addr(gs_route_list.route_info[i].router_addr[router_num - 1]);
                if (index >= 0 && child[index] == NULL) { // 如果没有找到则创建父节点
                    child[index] = cJSON_CreateObject();
                }
                cJSON_AddItemToObject(child[index], "P", parent[router_num]);
            }
        }
        if (child[i] == NULL) {
            child[i] = cJSON_CreateObject();
        }
        cJSON_AddItemToArray(parent[router_num], child[i]);
        int ret_size =
            snprintf(buf, sizeof(buf), "%u-%u", gs_route_list.route_info[i].room, gs_route_list.route_info[i].floor);
        // FIKS_LOG_IF_FAIL(ret_size >= 0, "snprintf failed, ret_size: %d", ret_size);
        cJSON_AddItemToObject(child[i], "F", cJSON_CreateString(buf));
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

void doubleForLoop(ft_uart_payload_of_whitelist_s *arr1, ft_uart_payload_of_whitelist_s *arr2)
{
    // ft_uart_payload_of_destaddr_s addr1[WHITE_LIST_MAX];
    // ft_uart_payload_of_destaddr_s route_info[WHITE_LIST_MAX];
    // for (uint16_t i = 0; i < arr1->dest_addr_num; i++) {
    //     for (uint16_t j = 0; j < arr2->dest_addr_num; j++) {
    //     }
    // }

    int i = 0, j = 0, k = 0;
    while (i < arr1->dest_addr_num && j < arr2->dest_addr_num) {
        if (arr1->dest_addr_info[i].room > arr2->dest_addr_info[j].room) {
            // remove add1
            printf("remove addr1:%u %u %u\n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
                arr1->dest_addr_info[i].dest_addr);

            printf("add addr2:%u %u %u\n", arr2->dest_addr_info[i].room, arr2->dest_addr_info[i].floor,
                arr2->dest_addr_info[i].dest_addr);

            j++;
        } else if (arr1->dest_addr_info[i].room < arr2->dest_addr_info[j].room) {
            // remove add1
            printf("remove addr1:%u %u %u\n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
                arr1->dest_addr_info[i].dest_addr);

            printf("add addr2:%u %u %u", arr2->dest_addr_info[i].room, arr2->dest_addr_info[i].floor,
                arr2->dest_addr_info[i].dest_addr);

            i++;
        } else {
            if (arr1->dest_addr_info[i].floor == arr2->dest_addr_info[j].floor) {
                if (arr1->dest_addr_info[i].dest_addr == arr2->dest_addr_info[j].dest_addr) {
                    // the same
                    printf("same addr1:%u %u %u addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
                        arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr, arr2->dest_addr_info[j].room,
                        arr2->dest_addr_info[j].floor, arr2->dest_addr_info[j].dest_addr);

                    i++;
                    j++;
                } else {
                    // replace
                    printf("replace addr1:%u %u %u addr2:%u %u %u\n", arr1->dest_addr_info[i].room,
                        arr1->dest_addr_info[i].floor, arr1->dest_addr_info[i].dest_addr, arr2->dest_addr_info[j].room,
                        arr2->dest_addr_info[j].floor, arr2->dest_addr_info[j].dest_addr);
                    i++;
                    j++;
                }
            } else if (arr1->dest_addr_info[i].floor > arr2->dest_addr_info[j].floor) {
                // remove
                printf("remove addr1:%u %u %u\n", arr1->dest_addr_info[i].room, arr1->dest_addr_info[i].floor,
                    arr1->dest_addr_info[i].dest_addr);
                i++;
            } else {
                // add
                printf("add addr2:%u %u %u\n", arr2->dest_addr_info[j].room, arr2->dest_addr_info[j].floor,
                    arr2->dest_addr_info[j].dest_addr);
                j++;
            }
        }
    }
}

void test3()
{
#if 0
    printf("%u\n", sizeof(ft_uart_payload_of_route_list_s));

    route_list_init();

    for (uint16_t i = 0; i < gs_route_list.route_list_num; i++) {
        // printf("%u %u ", gs_route_list.route_info[i].room, gs_route_list.route_info[i].floor);
        printf("%04x ", gs_route_list.route_info[i].dest_addr);
        for (uint16_t j = 0; j < gs_route_list.route_info[i].router_num; j++) {
            printf("%04x ", gs_route_list.route_info[i].router_addr[j]);
        }
        printf("\n");
    }

    format_route_list_to_json();

#endif

#if 0
    char buf[] = "F4F532326AAA";
    bool ret = string_start_with(buf, "F4F5");
    printf("ret %d\n", ret);

    uint32_t value = 0;
    if (ret) {
        value = strtol(buf + strlen("F4F5"), NULL, 16);
        printf("%d\n", value);
    }
    // char *subbuf = strstr(buf, "F4F5");
    // char *p = subbuf + strlen("F4F5");
    // printf("%d\n", strtol(p, NULL, 16));
#endif

#if 0
    // ft_uart_payload_of_whitelist_s orig = {};
    // orig.dest_addr_num = 8;
    // for (uint16_t i = 0; i < orig.dest_addr_num; i++) {
    //     orig.dest_addr_info[i].room = 1;
    //     orig.dest_addr_info[i].floor = orig.dest_addr_num - i;
    //     orig.dest_addr_info[i].dest_addr = 2300001 + i;

    //     printf("addr1[%u]: %u %u %u\n", i, orig.dest_addr_info[i].room, orig.dest_addr_info[i].floor,
    //         orig.dest_addr_info[i].dest_addr);
    // }

    // printf("\n");

    // ft_uart_payload_of_whitelist_s newer = {};
    // newer.dest_addr_num = 7;
    // for (uint16_t i = 0; i < newer.dest_addr_num; i++) {
    //     newer.dest_addr_info[i].room = 1;
    //     newer.dest_addr_info[i].floor = newer.dest_addr_num - i;
    //     newer.dest_addr_info[i].dest_addr = 2300001 + i;

    //     printf("addr2[%u]: %u %u %u\n", i, newer.dest_addr_info[i].room, newer.dest_addr_info[i].floor,
    //         newer.dest_addr_info[i].dest_addr);
    // }

    // doubleForLoop(&orig, &newer);
#endif
}

// public
// int[] intersect(int[] nums1, int[] nums2)
// {
//     // 这里我们先排个序，如果是有序数组，就不需要进行这个步骤
//     Arrays.sort(nums1);
//     Arrays.sort(nums2);
//     // 定义两个指针i和j，同时声明一个结果数组长度k
//     int i = 0, j = 0, k = 0;
//     while (i < nums1.length && j < nums2.length) {
//         if (nums1[i] < nums2[j]) {
//             // 如果nums1[i] 比 nums2[j]要小，说明
//             // 此时指针i所指的元素小于指针j所指向的元素，又是有序的，那么我们将i+1，进行下个比较
//             ++i;
//         } else if (nums1[i] > nums2[j]) {
//             // 如果nums1[i] 比 nums2[j]要大，说明
//             // 此时指针i所指的元素大于指针j所指向的元素，又是有序的，那么我们将j+1，进行下个比较
//             ++j;
//         } else {
//             // 当相等的时候，获得一个结果，直接放到nums1中，不浪费空间，同时三个指针+1，进行下个比较
//             nums1[k++] = nums1[i++];
//             ++j;
//         }
//     } // 比较结束后，我们将nums1从0到k的数组拿出来，便是我们的结果
//     return Arrays.copyOfRange(nums1, 0, k);
// }

// /***
//  * 双层for循环遍历，获取两个数组中相同的部分与不同的部分。
//  * @param arr1 数组1
//  * @param arr2 数组2
//  */
// private
// static void doubleForLoop(String[] arr1, String[] arr2)
// {
//     List<String> same = new ArrayList<>((arr1.length + arr2.length) / 2);
//     List<String> diff = new ArrayList<>((arr1.length + arr2.length) / 2);

//     for (int i = 0; i < arr1.length; i++) {
//         for (int j = 0; j < arr2.length; j++) {

//             /**
//              * 这么写是要以某个数组为副本，用这个副本来和另外一个数组进行比较。这里以arr2作为副本。
//              */
//             if (i == 0)
//                 diff.add(arr2[j]);

//             /**
//              * 相同时，首先要判断diff是否包含该值，包含则删除(因为在上一步中已经将arr2存进diff中)，再将该值存进same中(判断same是否包含该值)。
//              */
//             if (arr1[i].equals(arr2[j])) {
//                 if (diff.contains(arr1[i])) {
//                     diff.remove(arr1[i]);
//                 }
//                 if (!same.contains(arr1[i])) {
//                     same.add(arr1[i]);
//                 }
//                 break;
//             } else {
//                 /**
//                  * 如果same包含该值，则说明该值已遍历过并进行了一系类的处理。此时直接break，以免浪费性能。
//                  * 如果same不包含该值则说明该值不是2个数组共同拥有的值，则将该值存进diff中(判断diff是否包含该值)。
//                  */
//                 if (same.contains(arr1[i]))
//                     break;
//                 if (!diff.contains(arr1[i])) {
//                     diff.add(arr1[i]);
//                 }
//             }
//         }
//     }
//     System.out.println("双层for循环遍历，相同的部分：" + same);
//     System.out.println("双层for循环遍历，不同的部分：" + diff);
// }