#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "cson.h"
#include "log.h"

typedef unsigned char uint8_t;

/* Step1:定义与json相对应的数据结构 */
typedef struct
{
    uint8_t offset;
    uint8_t len;
    uint8_t bit_index;
} cmd_attr_t;

typedef struct
{
    uint8_t offset;
    uint8_t len;
} sta_attr_t;

typedef struct
{
    char *attr;
    cmd_attr_t cmd;
    sta_attr_t status;
} attri_list;

typedef struct
{
    uint8_t offset;
    uint8_t len;
    int default_;
} cfg_attr_t;

typedef struct
{
    cfg_attr_t head;
    cfg_attr_t type;
    cfg_attr_t payloadBitFlag;
    cfg_attr_t payloadData;
    cfg_attr_t crc;
} cmd_payload_cfg_t;

typedef struct
{
    cfg_attr_t payloadData;
} sta_payload_cfg_t;

typedef struct
{
    sta_payload_cfg_t status;
    cmd_payload_cfg_t cmd;
} header_info;

typedef struct
{
    char *version;
    uint8_t dataType;
    uint8_t uploadFilter;
    int attrNum;
    attri_list attributeList[30];
    header_info header;
} fiks_schema_t;

/* Step2:定义数据结构的反射表 */
reflect_item_t cmd_attr_t_ref_tbl[] = {
    _property_int2(cmd_attr_t, offset, "offset"),
    _property_int2(cmd_attr_t, len, "len"),
    _property_int(cmd_attr_t, bit_index),
    _property_end(),
};

reflect_item_t sta_attr_t_ref_tbl[] = {
    _property_int2(sta_attr_t, offset, "offset"),
    _property_int2(sta_attr_t, len, "len"),
    _property_end(),
};

/* description for attri_list */
reflect_item_t attri_list_ref_tbl[] = {
    _property_string2(attri_list, attr, "attr"),
    _property_obj2(attri_list, status, "status", sta_attr_t_ref_tbl),
    _property_obj2(attri_list, cmd, "cmd", cmd_attr_t_ref_tbl),
    _property_end(),
};

reflect_item_t cfg_attr_t_ref_tbl[] = {
    _property_int2(cfg_attr_t, offset, "offset"),
    _property_int2(cfg_attr_t, len, "len"),
    _property_int2(cfg_attr_t, default_, "default"),
    _property_end(),
};

reflect_item_t cmd_payload_cfg_t_ref_tbl[] = {
    _property_obj2(cmd_payload_cfg_t, head, "head", cfg_attr_t_ref_tbl),
    _property_obj2(cmd_payload_cfg_t, type, "type", cfg_attr_t_ref_tbl),
    _property_obj2(cmd_payload_cfg_t, payloadBitFlag, "payloadBitFlag", cfg_attr_t_ref_tbl),
    _property_obj2(cmd_payload_cfg_t, payloadData, "payloadData", cfg_attr_t_ref_tbl),
    _property_obj2(cmd_payload_cfg_t, crc, "crc", cfg_attr_t_ref_tbl),
    _property_end(),
};

reflect_item_t sta_payload_cfg_t_ref_tbl[] = {
    _property_obj2(sta_payload_cfg_t, payloadData, "payloadData", cfg_attr_t_ref_tbl),
    _property_end(),
};

/* description for header_info */
reflect_item_t header_info_ref_tbl[] = {
    _property_obj2(header_info, cmd, "cmd", cmd_payload_cfg_t_ref_tbl),
    _property_obj2(header_info, status, "status", sta_payload_cfg_t_ref_tbl),
    _property_end(),
};

/* description for fiks_schema_t */
reflect_item_t fiks_schema_t_ref_tbl[] = {
    _property_string2(fiks_schema_t, version, "version"),
    _property_int2(fiks_schema_t, dataType, "dataType"),
    _property_int2(fiks_schema_t, uploadFilter, "uploadFilter"),
    _property_int_ex2(fiks_schema_t, attrNum, "attrNum", _ex_args_all),
    _property_arrays2(fiks_schema_t, attributeList, "AttributeList", attri_list_ref_tbl, attri_list, "attrNum"),
    _property_obj2(fiks_schema_t, header, "header", header_info_ref_tbl),
    _property_end(),
};

void my_log(log_Event *ev)
{
    // struct timeb stTimeb;
    // ftime(&stTimeb);
    // char buf[1024];
    // buf[strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    // snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf) - 1, ".%03d] %-5s %s::%s:%d: ", stTimeb.millitm,
    //     log_level_string(ev->level), ev->file, ev->func, ev->line);
    // vsnprintf(buf + strlen(buf), sizeof(buf) - strlen(buf) - 1, ev->fmt, ev->ap);
    // printf("%s\n", buf);
}

int csonDemo()
{
    printf("=========================================\n");
    printf("\t\tRunning %s\n", __FUNCTION__);
    printf("=========================================\n");

    log_set_quiet(true);
    log_add_callback(my_log, NULL, LOG_TRACE);

    char file_name[] = "schema_all.json";
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL) {
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    int flen = ftell(fp);
    char *buf = (char *)malloc(flen + 1);
    if (buf == NULL) {
        fclose(fp);
        return -1;
    }
    fseek(fp, 0L, SEEK_SET);
    fread(buf, flen, 1, fp);
    buf[flen] = 0;

    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }

    // printf("\nschema.json:\n%s\n", buf);

    clock_t begin = clock();
    fiks_schema_t gs_schema = {};
    csonFreePointer(&gs_schema, fiks_schema_t_ref_tbl);

    memset(&gs_schema, 0, sizeof(fiks_schema_t));

    int ret = csonJsonStr2Struct(buf, &gs_schema, fiks_schema_t_ref_tbl);
    clock_t end = clock();
    int cost = end - begin;
    printf("attrNum:%d\n", gs_schema.attrNum);
    for (int i = 0; i < gs_schema.attrNum; i++) {
        // attri_list *attrList = gs_schema.attributeList + i;
        attri_list *attrList = &gs_schema.attributeList[i];
        printf("[%d] %s cmd[%d, %d, %d] status[%d, %d]\n", i, attrList->attr, attrList->cmd.offset, attrList->cmd.len,
            attrList->cmd.bit_index, attrList->status.offset, attrList->status.len);
    }

    cmd_payload_cfg_t *head_cmd = &gs_schema.header.cmd;
    printf("head_cmd head[%d, %d, %d] type[%d, %d, %d] payloadBitFlag[%d, %d] payloadData[%d, %d] crc[%d, %d]\n",
        head_cmd->head.offset, head_cmd->head.len, head_cmd->head.default_, head_cmd->type.offset, head_cmd->type.len,
        head_cmd->type.default_, head_cmd->payloadBitFlag.offset, head_cmd->payloadBitFlag.len,
        head_cmd->payloadData.offset, head_cmd->payloadData.len, head_cmd->crc.offset, head_cmd->crc.len);
    sta_payload_cfg_t *head_status = &gs_schema.header.status;
    printf("head_status payloadData[%d, %d]\n", head_status->payloadData.offset, head_status->payloadData.len);
    printf("decode ret:%d cost: %dms\n", ret, cost);

    begin = clock();
    char *jstrOutput = NULL;
    ret = csonStruct2JsonStr(&jstrOutput, &gs_schema, fiks_schema_t_ref_tbl);
    end = clock();
    cost = end - begin;
    printf("encode json:\n%s\n", jstrOutput);
    printf("encode ret: %d cost: %dms\n", ret, cost);

    csonFreePointer(&gs_schema, fiks_schema_t_ref_tbl);

    if (jstrOutput != NULL) {
        free(jstrOutput);
        jstrOutput = NULL;
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    return 0;
}

typedef struct
{
    int timeStamp;
} ErrorInfo;

reflect_item_t ErrorInfo_ref_tbl[] = {
    _property_int(ErrorInfo, timeStamp),
    _property_end(),
};

typedef struct
{
    char productId[128];
    char productKey[128];
    char securityKey[128];
    char xSource[128];
    char *productName;
    int productSpecies;
    long long timestamp;
    ErrorInfo error;
} ProdcutInfo;

reflect_item_t ProdcutInfo_ref_tbl[] = {
    _property_strings(ProdcutInfo, productId),
    _property_strings(ProdcutInfo, productKey),
    _property_strings(ProdcutInfo, securityKey),
    _property_strings(ProdcutInfo, xSource),
    _property_string(ProdcutInfo, productName),
    _property_int(ProdcutInfo, productSpecies),
    _property_int(ProdcutInfo, timestamp),
    _property_obj(ProdcutInfo, error, ErrorInfo_ref_tbl),
    _property_end(),
};

void csonDemo2()
{
    char *res_data = "{\"securityKey\":\"HqXpxKuXlrt1FIGu\",\"upgradeKey\":"
                     "\"95880288201091325707433253118984263478572987735494687588750185795377577721630844788736994473060"
                     "34466200616411960574122434059469100235892702736860872901247123456\",\"productId\":"
                     "\"e27d0ebc8fccd79f05c9ce604237cb20\",\"productName\":\"iMES2.0iMES-45-C1\",\"productKey\":"
                     "\"6e683d0bf8cc48a38ffc83d351be961f\",\"xSource\":\"DEVICE_iMES-45-C1\",\"productSpecies\":2,"
                     "\"timestamp\":1678185441827}";
    ProdcutInfo pdInfo = {
        .productId = "",
        .productSpecies = 1,
        .error.timeStamp = -1,
    };
    int ret = csonJsonStr2Struct(res_data, &pdInfo, ProdcutInfo_ref_tbl);
    printf("ret: %d productId: %s size:%d xSource: %s productSpecies: %d timeStamp: %lld\n", ret, 
        pdInfo.productId, (int)strlen(pdInfo.productId),
        pdInfo.xSource,
        pdInfo.productSpecies, pdInfo.timestamp);

    char *jstrOutput = NULL;
    ret = csonStruct2JsonStr(&jstrOutput, &pdInfo, ProdcutInfo_ref_tbl);
    printf("encode ret: %d jstrOutput: %s\n", ret, jstrOutput);

    if (jstrOutput != NULL) {
        free(jstrOutput);
        jstrOutput = NULL;
    }
    csonFreePointer(&pdInfo, ProdcutInfo_ref_tbl);
}

int main(int argc, char *argv[])
{
    printf("=========================================\n");
    printf("\t\tRunning %s\n", __FUNCTION__);
    printf("=========================================\n");

    // csonDemo();

    csonDemo2();

    return 0;
}