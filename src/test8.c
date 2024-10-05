#include "cson.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *path;
    char *thumb;
    char *bigPic;
    char *video;
    char *clockBgPic;
    char *hourPic;
    char *minutePic;
    char *secondPic;
    int mendian;
    int theme;
} BgDetailList;

typedef struct
{
    int index;
    char *name;
    char *coverPath;

    int listNum;
    BgDetailList *list;
} BgInfo;

typedef struct
{
    int lockTimeNum;
    int *lockTime;

    int bginfoNum;
    BgInfo *bginfo;
} BgData;

reflect_item_t bg_detail_list_ref_tbl[] = {
    _property_string(BgDetailList, path),
    _property_string(BgDetailList, thumb),
    _property_string(BgDetailList, bigPic),
    _property_string(BgDetailList, video),
    _property_string(BgDetailList, clockBgPic),
    _property_string(BgDetailList, hourPic),
    _property_string(BgDetailList, minutePic),
    _property_string(BgDetailList, secondPic),
    _property_int(BgDetailList, mendian),
    _property_int(BgDetailList, theme),
    _property_end(),
};

reflect_item_t bg_info_ref_tbl[] = {
    _property_int(BgInfo, index),
    _property_string(BgInfo, name),
    _property_string(BgInfo, coverPath),
    _property_int_ex(BgInfo, listNum, _ex_args_all),
    _property_array_object(BgInfo, list, bg_detail_list_ref_tbl, BgDetailList, listNum),
    _property_end(),
};

reflect_item_t bg_data_ref_tbl[] = {
    _property_int_ex(BgData, lockTimeNum, _ex_args_all),
    _property_array_int(BgData, lockTime, int, lockTimeNum),
    _property_int_ex(BgData, bginfoNum, _ex_args_all),
    _property_array_object(BgData, bginfo, bg_info_ref_tbl, BgInfo, bginfoNum),
    _property_end(),
};

const static char *jStr =
    "{\"lockTime\":[1,2,3,4,5,10,-1],\"bginfo\":[{\"index\":0,\"name\":\"动态壁纸\",\"coverPath\":\"png/lock/"
    "bg_setting1.png\",\"list\":[{\"path\":\"png/lock/"
    "\",\"thumb\":\"3D_thumb1.png\",\"bigPic\":\"3D_bg_big1.png\",\"video\":\"video/"
    "lock_bg3.mp4\",\"mendian\":0},{\"path\":\"png/lock/\",\"thumb\":\"3D_thumb4.png\",\"video\":\"video/"
    "lock_mendian.mp4\",\"mendian\":1}]},{\"index\":1,\"name\":\"套系壁纸\",\"coverPath\":\"png/lock/"
    "bg_setting2.png\",\"list\":[{\"path\":\"png/lock/"
    "\",\"thumb\":\"taoxi_thumb3.png\",\"bigPic\":\"taoxi_bg_big3.png\",\"mendian\":0},{\"path\":\"png/lock/"
    "\",\"thumb\":\"taoxi_thumb4.png\",\"bigPic\":\"taoxi_bg_big4.png\",\"mendian\":0,\"theme\":1},{\"path\":\"png/"
    "lock/\",\"thumb\":\"taoxi_thumb1.png\",\"bigPic\":\"taoxi_bg_big1.png\",\"video\":\"video/"
    "lock_bg1.mp4\",\"mendian\":0},{\"path\":\"png/lock/"
    "\",\"thumb\":\"taoxi_thumb5.png\",\"bigPic\":\"taoxi_bg_big5.png\",\"video\":\"video/"
    "lock_bg5.mp4\",\"mendian\":0,\"theme\":1},{\"path\":\"png/lock/"
    "\",\"thumb\":\"taoxi_thumb2.png\",\"bigPic\":\"taoxi_bg_big2.png\",\"mendian\":0}]},{\"index\":2,\"name\":"
    "\"时钟壁纸\",\"coverPath\":\"png/lock/bg_setting3.png\",\"list\":[{\"path\":\"png/lock/"
    "\",\"thumb\":\"clock_thumb1.png\",\"bigPic\":\"clock_bg_big1.png\",\"clockBgPic\":\"clock_bg1.png\",\"hourPic\":"
    "\"clock_hour1.png\",\"minutePic\":\"clock_minute1.png\",\"secondPic\":\"clock_second1.png\",\"mendian\":0},{"
    "\"path\":\"png/lock/"
    "\",\"thumb\":\"clock_thumb2.png\",\"bigPic\":\"clock_bg_big2.png\",\"clockBgPic\":\"clock_bg2.png\",\"hourPic\":"
    "\"clock_hour1.png\",\"minutePic\":\"clock_minute1.png\",\"secondPic\":\"clock_second1.png\",\"mendian\":0},{"
    "\"path\":\"png/lock/"
    "\",\"thumb\":\"clock_thumb3.png\",\"bigPic\":\"clock_bg_big3.png\",\"clockBgPic\":\"clock_bg3.png\",\"hourPic\":"
    "\"clock_hour2.png\",\"minutePic\":\"clock_minute2.png\",\"secondPic\":\"clock_second1.png\",\"mendian\":0},{"
    "\"path\":\"png/lock/"
    "\",\"thumb\":\"clock_thumb4.png\",\"bigPic\":\"clock_bg_big4.png\",\"clockBgPic\":\"clock_bg4.png\",\"hourPic\":"
    "\"clock_hour1.png\",\"minutePic\":\"clock_minute1.png\",\"secondPic\":\"clock_second1.png\",\"mendian\":0}]},{"
    "\"index\":3,\"name\":\"数字壁纸\",\"coverPath\":\"png/lock/bg_setting4.png\",\"list\":[{\"path\":\"png/lock/"
    "\",\"thumb\":\"digital_thumb1.png\",\"bigPic\":\"digital_bg_big1.png\",\"mendian\":0},{\"path\":\"png/lock/"
    "\",\"thumb\":\"digital_thumb2.png\",\"bigPic\":\"digital_bg_big2.png\",\"mendian\":0},{\"path\":\"png/lock/"
    "\",\"thumb\":\"digital_thumb3.png\",\"bigPic\":\"digital_bg_big3.png\",\"mendian\":0},{\"path\":\"png/lock/"
    "\",\"thumb\":\"digital_thumb4.png\",\"bigPic\":\"digital_bg_big4.png\",\"mendian\":0}]}]}";

void test8()
{
    printf("=========================================\n");
    printf("\t\tRunning %s\n", __FUNCTION__);
    printf("=========================================\n");
    BgData resp;
    memset(&resp, 0, sizeof(resp));

    /* string to struct */
    int ret = csonJsonStr2Struct(jStr, &resp, bg_data_ref_tbl);
    printf("decode ret=%d\n", ret);

    printf("decode lockTimeNum=%d\n", resp.lockTimeNum);
    printf("decode bginfoNum=%d\n", resp.bginfoNum);

    /* test print */
    csonPrintProperty(&resp, bg_data_ref_tbl);

    printf("Successed %s.\n", __FUNCTION__);
}