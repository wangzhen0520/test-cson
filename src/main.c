#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

#include "cson.h"
#include "log.h"

/* Step1:定义与json相对应的数据结构 */
typedef struct
{
    uint32_t offset;     // payload偏移字节byte
    uint32_t offset_bit; // payload偏移位bit
    uint16_t len;        // 长度字节byte
    uint16_t len_bit;    // 长度位bit
    uint32_t bit_index;  // 在payloadBitFlag
} cmd_attr_t;

typedef struct
{
    uint32_t offset;
    uint32_t offset_bit;
    uint16_t len;
    uint16_t len_bit;
} sta_attr_t;

typedef struct
{
    char *name;
    cmd_attr_t cmd;
    sta_attr_t status;
} attri_list;

typedef struct
{
    uint32_t offset;
    uint16_t len;
    int default_;
} cfg_attr_t;

typedef struct
{
    cfg_attr_t head;
    cfg_attr_t type;
    cfg_attr_t len;
    cfg_attr_t cmd;
    cfg_attr_t stat_;
    cfg_attr_t flags;
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
    cmd_payload_cfg_t cmd;
    sta_payload_cfg_t status;
} header_info;

typedef struct
{
    char *version;
    uint8_t dataType;
    uint8_t uploadFilter;
    int attrNum;
    attri_list *attrList;
    header_info header;
    int cmd_size;
} fiks_schema_t;

/* Step2:定义数据结构的反射表 */
reflect_item_t cmd_attr_t_ref_tbl[] = {
    _property_int(cmd_attr_t, offset),
    _property_int(cmd_attr_t, len),
    _property_int(cmd_attr_t, bit_index),
    _property_end(),
};

reflect_item_t sta_attr_t_ref_tbl[] = {
    _property_int(sta_attr_t, offset),
    _property_int(sta_attr_t, len),
    _property_end(),
};

/* description for attri_list */
reflect_item_t attri_list_ref_tbl[] = {
    _property_string2(attri_list, name, "attr"),
    _property_obj(attri_list, status, sta_attr_t_ref_tbl),
    _property_obj(attri_list, cmd, cmd_attr_t_ref_tbl),
    _property_end(),
};

reflect_item_t cfg_attr_t_ref_tbl[] = {
    _property_int(cfg_attr_t, offset),
    _property_int(cfg_attr_t, len),
    _property_int2(cfg_attr_t, default_, "default"),
    _property_end(),
};

reflect_item_t cmd_payload_cfg_t_ref_tbl[] = {
    _property_obj(cmd_payload_cfg_t, head, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, type, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, len, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, cmd, cfg_attr_t_ref_tbl),
    _property_obj2(cmd_payload_cfg_t, stat_, "stat", cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, flags, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, payloadBitFlag, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, payloadData, cfg_attr_t_ref_tbl),
    _property_obj(cmd_payload_cfg_t, crc, cfg_attr_t_ref_tbl),
    _property_end(),
};

reflect_item_t sta_payload_cfg_t_ref_tbl[] = {
    _property_obj(sta_payload_cfg_t, payloadData, cfg_attr_t_ref_tbl),
    _property_end(),
};

/* description for header_info */
reflect_item_t header_info_ref_tbl[] = {
    _property_obj(header_info, cmd, cmd_payload_cfg_t_ref_tbl),
    _property_obj(header_info, status, sta_payload_cfg_t_ref_tbl),
    _property_end(),
};

/* description for fiks_schema_t */
reflect_item_t fiks_schema_t_ref_tbl[] = {
    _property_string(fiks_schema_t, version),
    _property_int(fiks_schema_t, dataType),
    _property_int(fiks_schema_t, uploadFilter),
    _property_int_ex(fiks_schema_t, attrNum, _ex_args_all),
    _property_array2(fiks_schema_t, attrList, "AttributeList", attri_list_ref_tbl, attri_list, "attrNum"),
    _property_obj(fiks_schema_t, header, header_info_ref_tbl),
    _property_end(),
};

void my_log(log_Event *ev)
{
    struct timeval tmval = {};
    gettimeofday(&tmval, NULL);

    char buf[5120];
    buf[strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf) - 1, ".%03d] %-5s %s::%d: ", (int)tmval.tv_usec / 1000,
        log_level_string(ev->level), ev->file, ev->line);
    vsnprintf(buf + strlen(buf), sizeof(buf) - strlen(buf) - 1, ev->fmt, ev->ap);
    printf("%s\n", buf);
}

int csonDemo()
{
    log_info("=========================================");
    log_info("\t\tRunning %s", __FUNCTION__);
    log_info("=========================================");

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

    // log_info("\nschema.json:\n%s", buf);

    clock_t begin = clock();
    fiks_schema_t gs_schema = {};
    csonFreePointer(&gs_schema, fiks_schema_t_ref_tbl);

    memset(&gs_schema, 0, sizeof(fiks_schema_t));

    int ret = csonJsonStr2Struct(buf, &gs_schema, fiks_schema_t_ref_tbl);
    clock_t end = clock();
    int cost = end - begin;
    log_debug("attrNum:%d", gs_schema.attrNum);
    for (int i = 0; i < gs_schema.attrNum; i++) {
        attri_list *attrList = gs_schema.attrList + i;
        // attri_list *attrList = &gs_schema.attrList[i];
        log_debug("[%d] %s cmd[%d, %d, %d] status[%d, %d]", i, attrList->name, attrList->cmd.offset, attrList->cmd.len,
            attrList->cmd.bit_index, attrList->status.offset, attrList->status.len);
    }

    cmd_payload_cfg_t *head_cmd = &gs_schema.header.cmd;
    log_debug("head_cmd head[%d, %d, %d] type[%d, %d, %d] payloadBitFlag[%d, %d] payloadData[%d, %d] crc[%d, %d]",
        head_cmd->head.offset, head_cmd->head.len, head_cmd->head.default_, head_cmd->type.offset, head_cmd->type.len,
        head_cmd->type.default_, head_cmd->payloadBitFlag.offset, head_cmd->payloadBitFlag.len,
        head_cmd->payloadData.offset, head_cmd->payloadData.len, head_cmd->crc.offset, head_cmd->crc.len);
    sta_payload_cfg_t *head_status = &gs_schema.header.status;
    log_debug("head_status payloadData[%d, %d]", head_status->payloadData.offset, head_status->payloadData.len);
    log_debug("decode ret:%d cost: %dms", ret, cost);

    begin = clock();
    char *jstrOutput = NULL;
    ret = csonStruct2JsonStr(&jstrOutput, &gs_schema, fiks_schema_t_ref_tbl);
    end = clock();
    cost = end - begin;
    log_debug("encode size[%d] json:\n%s", strlen(jstrOutput), jstrOutput);
    log_debug("encode ret: %d cost: %dms", ret, cost);

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

#define USING_STRINGS 1
typedef struct
{
#if USING_STRINGS
    char productId[128];
    char productKey[128];
    char securityKey[128];
    char xSource[128];
#else
    char *productId;
    char *productKey;
    char *securityKey;
    char *xSource;
#endif
    char *productName;
    int productSpecies;
    long long timestamp;
    ErrorInfo error;
} ProdcutInfo;

reflect_item_t ProdcutInfo_ref_tbl[] = {
#if USING_STRINGS
    _property_strings(ProdcutInfo, productId),
    _property_strings(ProdcutInfo, productKey),
    _property_strings(ProdcutInfo, securityKey),
    _property_strings(ProdcutInfo, xSource),
#else
    _property_string(ProdcutInfo, productId),
    _property_string(ProdcutInfo, productKey),
    _property_string(ProdcutInfo, securityKey),
    _property_string(ProdcutInfo, xSource),
#endif
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
    log_debug("ret: %d productId: %s size:%d xSource: %s productSpecies: %d timeStamp: %lld", ret, pdInfo.productId,
        (int)strlen(pdInfo.productId), pdInfo.xSource, pdInfo.productSpecies, pdInfo.timestamp);

    char *jstrOutput = NULL;
    ret = csonStruct2JsonStr(&jstrOutput, &pdInfo, ProdcutInfo_ref_tbl);
    log_debug("encode ret: %d", ret);
    log_debug("encode size[%d] jstrOutput: %s", strlen(jstrOutput), jstrOutput);

    if (jstrOutput != NULL) {
        free(jstrOutput);
        jstrOutput = NULL;
    }
    csonFreePointer(&pdInfo, ProdcutInfo_ref_tbl);
}

void DumpIndevSchema(fiks_schema_t *schema)
{
    for (int i = 0; i < schema->attrNum; i++) {
        attri_list *attr_info = &schema->attrList[i];
        log_debug("[%d] attr:%s cmd[of:%d of_bit:%d len:%d len_bit:%d bit_idx:%d] "
                  "status[of:%d of_bit:%d len:%d len_b:%d]",
            i, attr_info->name, attr_info->cmd.offset, attr_info->cmd.offset_bit, attr_info->cmd.len,
            attr_info->cmd.len_bit, attr_info->cmd.bit_index, attr_info->status.offset, attr_info->status.offset_bit,
            attr_info->status.len, attr_info->status.len_bit);
    }
}

void wireless_indoor_schema_convert(fiks_schema_t *schema)
{
    for (int i = 0; i < schema->attrNum - 1; i++) {
        for (int j = 0; j < schema->attrNum - 1 - i; j++) {
            attri_list *attr_info1 = &schema->attrList[j];
            attri_list *attr_info2 = &schema->attrList[j + 1];
            if (attr_info1->status.offset > attr_info2->status.offset) {
                attri_list schema_config_temp = {};
                memcpy(&schema_config_temp, attr_info1, sizeof(schema_config_temp));
                memcpy(attr_info1, attr_info2, sizeof(schema_config_temp));
                memcpy(attr_info2, &schema_config_temp, sizeof(schema_config_temp));
            }
        }
    }

    log_info("after convert schema_config from small to big ~~~~");

    DumpIndevSchema(schema);

    for (int i = 0; i < schema->attrNum - 1; i++) {
        attri_list *attr_info1 = &schema->attrList[i];
        attri_list *attr_info2 = &schema->attrList[i + 1];
        int report_bit_len1 = attr_info1->status.len * 8 + attr_info1->status.len_bit;
        if (report_bit_len1 >= 8) {
            attr_info2->status.offset = attr_info1->status.offset + (report_bit_len1 / 8);
        } else if (report_bit_len1 == 1) {
            int report_bit_len2 = attr_info2->status.len * 8 + attr_info2->status.len_bit;
            if (report_bit_len2 == 1) {
                attr_info2->status.offset = attr_info1->status.offset;
            } else if (report_bit_len2 == 8) {
                attr_info2->status.offset = attr_info1->status.offset + 1;
            }
        }
    }

    log_info("after remove the attr[workmode] index from schema_config ~~~~");

    DumpIndevSchema(schema);
}

void init_things_config(fiks_schema_t *schema)
{
    log_debug("attrNum: %d", schema->attrNum);
    for (int i = 0; i < schema->attrNum; i++) {
        attri_list *attr_info = schema->attrList + i;
        if (schema->dataType == 1) { // 如果是按位表示, 则转换位按字节表示
            attr_info->cmd.offset_bit = attr_info->cmd.offset % 8;
            attr_info->cmd.offset /= 8;
            attr_info->cmd.len_bit = attr_info->cmd.len % 8;
            attr_info->cmd.len /= 8;

            attr_info->status.offset_bit = attr_info->status.offset % 8;
            attr_info->status.offset /= 8;
            attr_info->status.len_bit = attr_info->status.len % 8;
            attr_info->status.len /= 8;
        }
        log_debug("[%d] attr:%s cmd[of:%d of_bit:%d len:%d len_bit:%d bit_idx:%d] "
                  "status[of:%d of_bit:%d len:%d len_b:%d]",
            i, attr_info->name, attr_info->cmd.offset, attr_info->cmd.offset_bit, attr_info->cmd.len,
            attr_info->cmd.len_bit, attr_info->cmd.bit_index, attr_info->status.offset, attr_info->status.offset_bit,
            attr_info->status.len, attr_info->status.len_bit);
    }

    cmd_payload_cfg_t *head_cmd = &schema->header.cmd;
    log_debug("head_cmd head[%d, %d, %d]", head_cmd->head.offset, head_cmd->head.len, head_cmd->head.default_);
    log_debug("head_cmd type[%d, %d, %d]", head_cmd->type.offset, head_cmd->type.len, head_cmd->type.default_);
    log_debug("head_cmd cmd[%d, %d, %d]", head_cmd->cmd.offset, head_cmd->cmd.len, head_cmd->cmd.default_);
    log_debug("head_cmd payloadBitFlag[%d, %d]", head_cmd->payloadBitFlag.offset, head_cmd->payloadBitFlag.len);
    log_debug("head_cmd payloadData[%d, %d]", head_cmd->payloadData.offset, head_cmd->payloadData.len);
    log_debug("head_cmd crc[%d, %d]", head_cmd->crc.offset, head_cmd->crc.len);

    schema->cmd_size = head_cmd->crc.offset + head_cmd->crc.len;

    sta_payload_cfg_t *head_status = &schema->header.status;
    log_debug("head_status payloadData[%d, %d]", head_status->payloadData.offset, head_status->payloadData.len);
}

void csonDemo3()
{
    char *buf =
        "{\"AttributeList\":[{\"attr\":\"InnerRoomNum\",\"status\":{\"offset\":40,\"len\":8}},{\"attr\":"
        "\"InnerFloorNum\",\"status\":{\"offset\":48,\"len\":8}},{\"attr\":\"PowerSwitchAll\",\"status\":{\"offset\":"
        "56,\"len\":8}},{\"attr\":\"ElectricValveAngle\",\"cmd\":{\"offset\":72,\"len\":8,\"bit_idx\":0},\"status\":{"
        "\"offset\":72,\"len\":8}},{\"attr\":\"FanGears\",\"status\":{\"offset\":80,\"len\":8}},{\"attr\":"
        "\"InnerFanPower\",\"status\":{\"offset\":88,\"len\":16}},{\"attr\":\"InnerFanCommError\",\"status\":{"
        "\"offset\":105,\"len\":1}},{\"attr\":\"InnerFanValveRunError\",\"status\":{\"offset\":108,\"len\":1}},{"
        "\"attr\":\"InnerFanDialError\",\"status\":{\"offset\":111,\"len\":1}},{\"attr\":\"EndAttrOffset\",\"status\":{"
        "\"offset\":112,\"len\":8}},{\"attr\":\"SignalStrength\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":"
        "\"RebootCause\",\"status\":{\"offset\":0,\"len\":0}},{\"attr\":\"ErrorMessageRecord\",\"status\":{\"offset\":"
        "0,\"len\":0}}],\"version\":\"V1.0.0_bit\",\"dataType\":1,\"uploadFilter\":1,\"header\":{\"cmd\":{\"head\":{"
        "\"offset\":0,\"len\":2,\"default\":62709},\"len\":{\"offset\":2,\"len\":1},\"type\":{\"offset\":4,\"len\":1,"
        "\"default\":195},\"cmd\":{\"offset\":3,\"len\":1,\"default\":2},\"stat\":{\"offset\":0,\"len\":0,\"default\":"
        "0},\"flags\":{\"offset\":0,\"len\":0,\"default\":0},\"payloadBitFlag\":{\"offset\":0,\"len\":0},"
        "\"payloadData\":{\"offset\":5,\"len\":16},\"crc\":{\"offset\":21,\"len\":2}},\"status\":{\"payloadData\":{"
        "\"offset\":5,\"len\":16}}}}";

    fiks_schema_t schema = {};
    memset(&schema, 0, sizeof(fiks_schema_t));
    int ret = csonJsonStr2Struct(buf, &schema, fiks_schema_t_ref_tbl);

    init_things_config(&schema);

    wireless_indoor_schema_convert(&schema);

    csonFreePointer(&schema, fiks_schema_t_ref_tbl);
}

extern void test1();
extern void test2();
extern void test3();
extern void test4();
extern void test5();
int main(int argc, char *argv[])
{
    log_info("=========================================");
    log_info("\t\tRunning %s", __FUNCTION__);
    log_info("=========================================");

    log_set_quiet(true);
    log_add_callback(my_log, NULL, LOG_TRACE);

    int cnt = 1;
    while (cnt-- > 0) {
        // test_week();
        // csonDemo();

        // csonDemo2();

        // csonDemo3();

        // test1();

        // test2();

        // test3();

        // test4();
        
        test5();

        usleep(100 * 1000);
    }

    return 0;
}