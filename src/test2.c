#include "cson.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "assert.h"
#include "math.h"

#define CHECK_STRING(a, b) assert(strcmp(a, b) == 0)
#define CHECK_NUMBER(a, b) assert(a == b)
#define CHECK_REAL(a, b) assert(fabs(a - b) <= 1e-6)

typedef struct
{
    char *name;
    char *jump_url;
} ClassInfoChild;

typedef struct
{
    int has_child;
    char *icon;
    int id;
    char *name;
    char childrenNum;
    ClassInfoChild *children;
} ClassInfo;

typedef struct
{
    long long timestamp;
    int infoNum;
    ClassInfo *info;
} Data;

typedef struct
{
    int status;
    Data data;
    int errcode;
} Response;

reflect_item_t ClassInfoChildTbl[] = {
    _property_string(ClassInfoChild, name),
    _property_string(ClassInfoChild, jump_url),
    _property_end(),
};

reflect_item_t ClassInfoTbl[] = {
    _property_int(ClassInfo, has_child),
    _property_string(ClassInfo, icon),
    _property_int(ClassInfo, id),
    _property_string(ClassInfo, name),
    _property_int_ex(ClassInfo, childrenNum, _ex_args_all),
    _property_array_object(ClassInfo, children, ClassInfoChildTbl, ClassInfoChild, childrenNum),
    _property_end(),
};

reflect_item_t DataTbl[] = {
    _property_int(Data, timestamp),
    _property_int_ex(Data, infoNum, _ex_args_all),
    _property_array_object(Data, info, ClassInfoTbl, ClassInfo, infoNum),
    _property_end(),
};

reflect_item_t ResponseTbl[] = {
    _property_int(Response, status),
    _property_obj(Response, data, DataTbl),
    _property_int(Response, errcode),
    _property_end(),
};

const static char *jStr =
    "{\"status\":1,\"error\":\"\",\"data\":{\"timestamp\":1579069151,\"info\":[{\"has_child\":1,\"icon\":\"http://"
    "imge.kugou.com/mcommon/{size}/20181130/20181130172444711866.png\",\"id\":153,\"children\":[{\"jump_url\":\"https:/"
    "/miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F1\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20190402/"
    "20190402165900910180.png\",\"id\":185,\"name\":\"DJ专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1hFCsBvW_1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150825/"
    "20150825145251513968.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F45\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20190711/"
    "20190711142859901220.png\",\"id\":1401,\"name\":\"抖音专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F3\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20190402/"
    "20190402165839822861.png\",\"id\":227,\"name\":\"儿童专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1xNKsBmb_1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":1,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150819/"
    "20150819142111579284.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F23\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110341142930.png\",\"id\":781,\"name\":\"网络专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F9\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110304370142.png\",\"id\":405,\"name\":\"车载专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1TJKsB_K_1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151020/"
    "20151020185634232826.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F11\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110531714649.png\",\"id\":483,\"name\":\"广场舞专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T13ebsBgC_1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151030/"
    "20151030100347338896.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F17\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110434661130.png\",\"id\":439,\"name\":\"轻音乐专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1jTJsBmZT1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151026/"
    "20151026102137944162.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F19\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110403270402.png\",\"id\":721,\"name\":\"情歌专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F7\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110649518903.png\",\"id\":363,\"name\":\"国风专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1XJYsBXWj1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":1,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151019/20151019195642566658.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20160119/"
    "20160119175107905652.png\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F27\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20190417/"
    "20190417104751432731.png\",\"id\":993,\"name\":\"电音专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F29\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191107/"
    "20191107150209613377.png\",\"id\":1047,\"name\":\"欧美专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F5\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101111143849486.png\",\"id\":381,\"name\":\"ACG专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1OdWsByYT1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151023/"
    "20151023175903317815.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F15\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181106/"
    "20181106162159569853.png\",\"id\":155,\"name\":\"影视专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1xzKsBvWT1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150916/"
    "20150916115630236448.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F25\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110659435688.png\",\"id\":803,\"name\":\"戏曲专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F13\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181106/"
    "20181106162128954559.png\",\"id\":625,\"name\":\"粤剧专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F31\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191115/"
    "20191115142149416417.png\",\"id\":1031,\"name\":\"韩流专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"}],\"name\":\"专区\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http:"
    "//imge.kugou.com/mcommon/{size}/20191104/"
    "20191104103209217784.png\",\"id\":2005,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":"
    "2131,\"name\":\"伤感专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191112/"
    "20191112110650706413.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2137,\"name\":\"安静专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112110813847387.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2143,\"name\":\"对抗抑郁专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112114053306456.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2149,\"name\":\"轻松专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112114115681652.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2155,\"name\":\"激情专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112114229679116.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2161,\"name\":\"甜蜜专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112114455522172.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2167,\"name\":\"开心专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112114657687089.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2173,\"name\":\"想念专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112141710521727.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2179,\"name\":\"疗伤专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191112/"
    "20191112141926500890.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"心情\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181130/"
    "20181130172455112096.png\",\"id\":1263,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":"
    "1409,\"name\":\"官方歌单\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190701/"
    "20190701141020364105.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F65\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2365,"
    "\"name\":\"重返2010专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"\",\"is_new\":1,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{\"jump_url\":"
    "\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":277,\"name\":\"佛乐专区\",\"special_tag_id\":0,\"bannerurl\":\"http:/"
    "/imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1eFVsBXZT1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150916/20150916115656566037.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191113/"
    "20191113230926826063.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2findex.html%23%2fmobile%2fhome%2f37\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181106/"
    "20181106162140754053.png\",\"id\":209,\"name\":\"综艺专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1zXKsBvA_1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150916/"
    "20150916115609586448.png\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F35\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181106/"
    "20181106162058103005.png\",\"id\":1243,\"name\":\"首发专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F33\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101111200228559.png\",\"id\":1091,\"name\":\"音乐人专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":789,\"name\":\"古典乐专区\","
    "\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://"
    "imge.kugou.com/mcommon/{size}/20190626/"
    "20190626152411750201.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F21\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181101/"
    "20181101110554351882.png\",\"id\":739,\"name\":\"HiFi专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1215,\"name\":\"英语学习专区\","
    "\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://"
    "imge.kugou.com/mcommon/{size}/20190626/"
    "20190626152421673705.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F69\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1269,"
    "\"name\":\"2019年终专区\",\"special_tag_id\":0,\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/"
    "{size}/387f9f19bffe870e3c722d2958661856.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181229/20181229140823201468.jpg\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191226/"
    "20191226172357900648.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F39\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20190328/"
    "20190328212216110068.png\",\"id\":1297,\"name\":\"有声电台\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\","
    "\"description\":\"\"},{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"http://imge.kugou.com/mcommon/{size}/"
    "20190812/"
    "20190812102122694905.png\",\"id\":1387,\"name\":\"Urban专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191118/"
    "20191118163905185498.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1415,\"name\":\"入驻厂牌专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20190819/"
    "20190819111049128125.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F71\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2389,"
    "\"name\":\"游戏专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{\"jump_url\":"
    "\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F63\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2395,"
    "\"name\":\"说唱专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"\",\"is_new\":1,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{\"jump_url\":"
    "\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2029,\"name\":\"冬天听什么专区\",\"special_tag_id\":0,\"bannerurl\":"
    "\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191107/"
    "20191107101634149569.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"特色\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191104/"
    "20191104103220446249.png\",\"id\":2003,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"http://"
    "imge.kugou.com/mcommon/{size}/20181101/"
    "20181101110630241664.png\",\"id\":279,\"name\":\"运动健身专区\",\"special_tag_id\":0,\"bannerurl\":\"http://"
    "imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1fFYsBX_j1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150916/20150916115717647762.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191202/"
    "20191202163552854924.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2013,\"name\":\"睡前专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151057972712.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2247,\"name\":\"店铺音乐专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151034810296.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2253,\"name\":\"打游戏专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151254643206.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2259,\"name\":\"K歌必唱专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151421773216.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2265,\"name\":\"开车专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151558972937.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2271,\"name\":\"咖啡馆专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122151837262100.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2277,\"name\":\"蹦迪专用专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122152028593609.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2289,\"name\":\"校园专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191122/"
    "20191122152405265191.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"场景\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191104/20191104103232420148.png\",\"id\":2007,\"children\":[{\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F51\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1555,"
    "\"name\":\"经典老歌专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191202/"
    "20191202141928720121.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F73\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2407,"
    "\"name\":\"春节专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{\"jump_url\":"
    "\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2055,\"name\":\"成名曲专区\",\"special_tag_id\":0,\"bannerurl\":\"\","
    "\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191111/"
    "20191111174249287596.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2061,\"name\":\"舞曲专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111174452186715.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2067,\"name\":\"儿歌专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111174906536922.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2073,\"name\":\"新歌推荐专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111175047676679.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2079,\"name\":\"情歌对唱专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111175209162382.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2085,\"name\":\"励志专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111175327714784.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2091,\"name\":\"军旅红歌专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111175550208859.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2097,\"name\":\"铃声专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111180946152637.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2103,\"name\":\"神曲专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111181117318818.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2109,\"name\":\"城市风情专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111182647106360.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2115,\"name\":\"会员专属专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111182803792312.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2121,\"name\":\"BGM专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191111/"
    "20191111183127748172.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"主题\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":0,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181130/"
    "20181130172345416784.png\",\"id\":32,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":33,"
    "\"name\":\"70后\",\"special_tag_id\":13,\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1O9bjBKDT1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150415/20150415164642347845.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191115/"
    "20191115170921589845.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":34,\"name\":\"80后\",\"special_tag_id\":14,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1kMCjB4dj1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150415/20150415164652136718.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191115/"
    "20191115170927143738.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":35,\"name\":\"90后\",\"special_tag_id\":76,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1gnCjBbKT1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20160215/20160215094154552929.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191115/"
    "20191115170935539112.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":96,\"name\":\"00后\",\"special_tag_id\":571,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1d7KQByhb1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150513/20150513180053392639.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191115/"
    "20191115170942262316.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"年代\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191202/"
    "20191202161746384633.png\",\"id\":2009,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":"
    "2195,\"name\":\"国语专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\","
    "\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120163611415408.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2201,\"name\":\"粤语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120163743863149.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2207,\"name\":\"英语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120163937144325.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2213,\"name\":\"韩语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120164217668731.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2219,\"name\":\"日语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120164440825780.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2225,\"name\":\"法语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120164814523281.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2231,\"name\":\"闽南语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120164945463716.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":2237,\"name\":\"客家语专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191120/"
    "20191120165207596701.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1323,\"name\":\"小语种专区\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191202/"
    "20191202162147271334.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"语言\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191104/20191104102344527371.png\",\"id\":1599,\"children\":[{\"jump_url\":\"https://h5.kugou.com/"
    "apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1601&subname=%E6%B5%81%E8%A1%8C%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":9,\"icon\":\"\",\"id\":1601,\"name\":\"流行专区\",\"special_tag_id\":9,\"bannerurl\":\"\",\"song_tag_"
    "id\":882,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120171432358307.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Pop\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1615&subname=%E7%94%B5%E5%AD%90%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1615,\"name\":\"电子专区\",\"special_tag_id\":33,\"bannerurl\":\"\",\"song_tag_"
    "id\":595,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120174623547423.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Electronic\",\"description\":"
    "\"\"},{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1611&subname=%E6%91%87%E6%BB%9A%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":27,\"icon\":\"\",\"id\":1611,\"name\":\"摇滚专区\",\"special_tag_id\":27,\"bannerurl\":\"\",\"song_tag_"
    "id\":883,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/20191101/"
    "20191101100648571107.jpg\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120181659501645.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Rock\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1613&subname=%E7%BA%AF%E9%9F%B3%E4%B9%90%E4%B8%93%E5%8C%BA\","
    "\"album_tag_id\":0,\"icon\":\"\",\"id\":1613,\"name\":\"纯音乐专区\",\"special_tag_id\":34,\"bannerurl\":\"\","
    "\"song_tag_id\":960,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120181707580363.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"PureMusic\",\"description\":"
    "\"\"},{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1707&subname=%E5%9B%BD%E9%A3%8E%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1707,\"name\":\"国风音乐专区\",\"special_tag_id\":574,"
    "\"bannerurl\":\"\",\"song_tag_id\":1621,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191120/"
    "20191120181744220526.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"GuoFeng\",\"description\":\"\"}"
    ",{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1715&subname=%E5%98%BB%E5%93%88%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":31,\"icon\":\"\",\"id\":1715,\"name\":\"嘻哈专区\",\"special_tag_id\":31,\"bannerurl\":\"\",\"song_tag_"
    "id\":588,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120183103443876.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"HipHop\",\"description\":\"\"},"
    "{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1743&subname=R&B/"
    "Soul%E4%B8%93%E5%8C%BA\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1743,\"name\":\"R&BSoul专区\",\"special_tag_id\":"
    "30,\"bannerurl\":\"\",\"song_tag_id\":30,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191120/"
    "20191120183438462155.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"R&BSoul\",\"description\":\"\"}"
    ",{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1761&subname=%E6%B0%91%E8%B0%A3%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1761,\"name\":\"民谣专区\",\"special_tag_id\":83,\"bannerurl\":\"\",\"song_tag_"
    "id\":590,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120182114164802.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Folk\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1775&subname=DJ%E6%AD%8C%E6%9B%B2%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1775,\"name\":\"DJ歌曲专区\",\"special_tag_id\":17,\"bannerurl\":\"\",\"song_tag_"
    "id\":961,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120182713572393.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"DJ\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1783&subname=%E5%8F%A4%E5%85%B8%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1783,\"name\":\"古典专区\",\"special_tag_id\":28,\"bannerurl\":\"\",\"song_tag_"
    "id\":28,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120184023344633.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Classical\",\"description\":"
    "\"\"},{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1807&subname=%E4%B8%96%E7%95%8C%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1807,\"name\":\"世界音乐专区\",\"special_tag_id\":0,\"bannerurl\":"
    "\"\",\"song_tag_id\":1370,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/"
    "20191115/"
    "20191115163459485041.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"WorldMusic\",\"description\":"
    "\"\"},{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1833&subname=%E4%B9%A1%E6%9D%91%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1833,\"name\":\"乡村专区\",\"special_tag_id\":15,\"bannerurl\":\"\",\"song_tag_"
    "id\":589,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120184345386442.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Country\",\"description\":\"\"}"
    ",{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1849&subname=%E7%88%B5%E5%A3%AB%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1849,\"name\":\"爵士专区\",\"special_tag_id\":32,\"bannerurl\":\"\",\"song_tag_"
    "id\":591,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120184430385134.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Jazz\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1883&subname=%E5%AE%9E%E9%AA%8C%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1883,\"name\":\"实验音乐专区\",\"special_tag_id\":0,\"bannerurl\":"
    "\"\",\"song_tag_id\":1508,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/"
    "20191120/"
    "20191120184446897622.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Experimental\",\"description\":"
    "\"\"},{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1895&subname=%E6%8B%89%E4%B8%81%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1895,\"name\":\"拉丁专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":636,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104102058253387.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Latin\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1901&subname=%E9%87%91%E5%B1%9E%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1901,\"name\":\"金属专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":964,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120185812621087.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Metal\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1915&subname=%E6%9C%8B%E5%85%8B%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1915,\"name\":\"朋克专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":981,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104101747389850.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Punk\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1917&subname=%E9%9B%B7%E9%AC%BC%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1917,\"name\":\"雷鬼专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":92,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104101740553832.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Reggae\",\"description\":\"\"},"
    "{\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1925&subname=%E5%B8%83%E9%B2%81%E6%96%AF%E4%B8%93%E5%8C%BA\","
    "\"album_tag_id\":0,\"icon\":\"\",\"id\":1925,\"name\":\"布鲁斯专区\",\"special_tag_id\":0,\"bannerurl\":\"\","
    "\"song_tag_id\":94,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120185852698739.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"Blues\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1921&subname=ACG%E6%AD%8C%E6%9B%B2%E4%B8%93%E5%8C%BA\",\"album_"
    "tag_id\":0,\"icon\":\"\",\"id\":1921,\"name\":\"ACG歌曲专区\",\"special_tag_id\":0,\"bannerurl\":\"\",\"song_tag_"
    "id\":77,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20191120/"
    "20191120190020600949.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"ACG\",\"description\":\"\"},{"
    "\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1923&subname=%E4%B8%AD%E5%9B%BD%E7%89%B9%E8%89%B2%E4%B8%93%E5%"
    "8C%BA\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1923,\"name\":\"中国特色专区\",\"special_tag_id\":0,\"bannerurl\":"
    "\"\",\"song_tag_id\":1539,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/"
    "20191120/"
    "20191120172611173213.jpg\",\"is_new\":0,\"has_child\":1,\"theme\":1,\"subtitle\":\"ChineseCharacteristic\","
    "\"description\":\"\"}],\"name\":\"风格\",\"subtitle\":\"\",\"description\":\"\"},{\"has_child\":0,\"icon\":\"http:"
    "//imge.kugou.com/mcommon/{size}/20181130/"
    "20181130172408696210.png\",\"id\":58,\"children\":[{\"jump_url\":\"\",\"album_tag_id\":727,\"icon\":\"\",\"id\":"
    "513,\"name\":\"古筝\",\"special_tag_id\":0,\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1VTKQBjKv1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20151030/20151030153435669683.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711161915866564.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":722,\"icon\":\"\",\"id\":85,\"name\":\"钢琴\",\"special_tag_id\":16,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1qeYQB5Cj1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525184305921626.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711161925785009.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":728,\"icon\":\"\",\"id\":87,\"name\":\"笛子\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1fwYQBjDj1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525184610645366.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711161934440425.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":729,\"icon\":\"\",\"id\":88,\"name\":\"二胡\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T13nbjB4KT1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525184645692027.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711161941249461.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":723,\"icon\":\"\",\"id\":89,\"name\":\"吉他\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T179KjB4L_1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525184751644907.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711161954746142.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":724,\"icon\":\"\",\"id\":90,\"name\":\"萨克斯\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1_nCjBgLg1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525185050912141.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711162001984006.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":726,\"icon\":\"\",\"id\":91,\"name\":\"八音盒\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T18eYQBXCQ1RCvBVdK.png\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525185012650817.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711162012632790.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":725,\"icon\":\"\",\"id\":92,\"name\":\"小提琴\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T1tg_jBsYT1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20150525/20150525184937467712.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711162040822512.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":854,\"icon\":\"\",\"id\":1097,\"name\":\"葫芦丝\",\"special_tag_id\":0,"
    "\"bannerurl\":\"http://imge.kugou.com/v2/mobile_class_banner/{size}/"
    "T158hkBmxv1RCvBVdK.jpg\",\"song_tag_id\":0,\"is_hot\":0,\"imgurl\":\"http://imge.kugou.com/mcommon/{size}/"
    "20170314/20170314160138927434.png\",\"banner_hd\":\"http://imge.kugou.com/mcommon/{size}/20190711/"
    "20190711162047231895.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"},{"
    "\"jump_url\":\"\",\"album_tag_id\":0,\"icon\":\"\",\"id\":1565,\"name\":\"唢呐\",\"special_tag_id\":0,"
    "\"bannerurl\":\"\",\"song_tag_id\":185,\"is_hot\":0,\"imgurl\":\"\",\"banner_hd\":\"http://imge.kugou.com/mcommon/"
    "{size}/20191014/"
    "20191014140609433424.jpg\",\"is_new\":0,\"has_child\":0,\"theme\":1,\"subtitle\":\"\",\"description\":\"\"}],"
    "\"name\":\"乐器\",\"subtitle\":\"\",\"description\":\"\"}]},\"errcode\":0}";

const char *encodeTest =
    "{\"status\":1,\"data\":{\"timestamp\":1579069151,\"info\":[{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20181130/"
    "20181130172444711866.png\",\"id\":153,\"name\":\"专区\",\"children\":[{\"name\":\"DJ专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F1\"},{\"name\":\"抖音专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F45\"},{\"name\":\"儿童专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F3\"},{\"name\":\"网络专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F23\"},{\"name\":\"车载专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F9\"},{\"name\":\"广场舞专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F11\"},{\"name\":\"轻音乐专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F17\"},{\"name\":\"情歌专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F19\"},{\"name\":\"国风专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F7\"},{\"name\":\"电音专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F27\"},{\"name\":\"欧美专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F29\"},{\"name\":\"ACG专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F5\"},{\"name\":\"影视专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F15\"},{\"name\":\"戏曲专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F25\"},{\"name\":\"粤剧专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F13\"},{\"name\":\"韩流专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F31\"}]},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/"
    "mcommon/{size}/20191104/"
    "20191104103209217784.png\",\"id\":2005,\"name\":\"心情\",\"children\":[{\"name\":\"伤感专区\",\"jump_url\":\"\"},{"
    "\"name\":\"安静专区\",\"jump_url\":\"\"},{\"name\":\"对抗抑郁专区\",\"jump_url\":\"\"},{\"name\":\"轻松专区\","
    "\"jump_url\":\"\"},{\"name\":\"激情专区\",\"jump_url\":\"\"},{\"name\":\"甜蜜专区\",\"jump_url\":\"\"},{\"name\":"
    "\"开心专区\",\"jump_url\":\"\"},{\"name\":\"想念专区\",\"jump_url\":\"\"},{\"name\":\"疗伤专区\",\"jump_url\":"
    "\"\"}]},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20181130/"
    "20181130172455112096.png\",\"id\":1263,\"name\":\"特色\",\"children\":[{\"name\":\"官方歌单\",\"jump_url\":\"\"},{"
    "\"name\":\"重返2010专区\",\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F65\"},{\"name\":\"佛乐专区\",\"jump_url\":\"\"},{\"name\":"
    "\"综艺专区\",\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2findex.html%23%2fmobile%2fhome%2f37\"},{\"name\":\"首发专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F35\"},{\"name\":\"音乐人专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F33\"},{\"name\":\"古典乐专区\",\"jump_url\":\"\"},{"
    "\"name\":\"HiFi专区\",\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F21\"},{\"name\":\"英语学习专区\",\"jump_url\":\"\"},{"
    "\"name\":\"2019年终专区\",\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F69\"},{\"name\":\"有声电台\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F39\"},{\"name\":\"Urban专区\",\"jump_url\":\"\"},{"
    "\"name\":\"入驻厂牌专区\",\"jump_url\":\"\"},{\"name\":\"游戏专区\",\"jump_url\":\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F71\"},{\"name\":\"说唱专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F63\"},{\"name\":\"冬天听什么专区\",\"jump_url\":\"\"}]},{"
    "\"has_child\":1,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104103220446249.png\",\"id\":2003,\"name\":\"场景\",\"children\":[{\"name\":\"运动健身专区\",\"jump_url\":"
    "\"\"},{\"name\":\"睡前专区\",\"jump_url\":\"\"},{\"name\":\"店铺音乐专区\",\"jump_url\":\"\"},{\"name\":"
    "\"打游戏专区\",\"jump_url\":\"\"},{\"name\":\"K歌必唱专区\",\"jump_url\":\"\"},{\"name\":\"开车专区\",\"jump_"
    "url\":\"\"},{\"name\":\"咖啡馆专区\",\"jump_url\":\"\"},{\"name\":\"蹦迪专用专区\",\"jump_url\":\"\"},{\"name\":"
    "\"校园专区\",\"jump_url\":\"\"}]},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104103232420148.png\",\"id\":2007,\"name\":\"主题\",\"children\":[{\"name\":\"经典老歌专区\",\"jump_url\":"
    "\"https://miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F51\"},{\"name\":\"春节专区\",\"jump_url\":\"https://"
    "miniapp.kugou.com/node/"
    "v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F73\"},{\"name\":\"成名曲专区\",\"jump_url\":\"\"},{"
    "\"name\":\"舞曲专区\",\"jump_url\":\"\"},{\"name\":\"儿歌专区\",\"jump_url\":\"\"},{\"name\":\"新歌推荐专区\","
    "\"jump_url\":\"\"},{\"name\":\"情歌对唱专区\",\"jump_url\":\"\"},{\"name\":\"励志专区\",\"jump_url\":\"\"},{"
    "\"name\":\"军旅红歌专区\",\"jump_url\":\"\"},{\"name\":\"铃声专区\",\"jump_url\":\"\"},{\"name\":\"神曲专区\","
    "\"jump_url\":\"\"},{\"name\":\"城市风情专区\",\"jump_url\":\"\"},{\"name\":\"会员专属专区\",\"jump_url\":\"\"},{"
    "\"name\":\"BGM专区\",\"jump_url\":\"\"}]},{\"has_child\":0,\"icon\":\"http://imge.kugou.com/mcommon/{size}/"
    "20181130/"
    "20181130172345416784.png\",\"id\":32,\"name\":\"年代\",\"children\":[{\"name\":\"70后\",\"jump_url\":\"\"},{"
    "\"name\":\"80后\",\"jump_url\":\"\"},{\"name\":\"90后\",\"jump_url\":\"\"},{\"name\":\"00后\",\"jump_url\":\"\"}]}"
    ",{\"has_child\":1,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20191202/"
    "20191202161746384633.png\",\"id\":2009,\"name\":\"语言\",\"children\":[{\"name\":\"国语专区\",\"jump_url\":\"\"},{"
    "\"name\":\"粤语专区\",\"jump_url\":\"\"},{\"name\":\"英语专区\",\"jump_url\":\"\"},{\"name\":\"韩语专区\",\"jump_"
    "url\":\"\"},{\"name\":\"日语专区\",\"jump_url\":\"\"},{\"name\":\"法语专区\",\"jump_url\":\"\"},{\"name\":"
    "\"闽南语专区\",\"jump_url\":\"\"},{\"name\":\"客家语专区\",\"jump_url\":\"\"},{\"name\":\"小语种专区\",\"jump_"
    "url\":\"\"}]},{\"has_child\":1,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20191104/"
    "20191104102344527371.png\",\"id\":1599,\"name\":\"风格\",\"children\":[{\"name\":\"流行专区\",\"jump_url\":"
    "\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1601&subname=%E6%B5%81%E8%A1%8C%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"电子专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1615&subname=%E7%94%B5%E5%AD%90%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"摇滚专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1611&subname=%E6%91%87%E6%BB%9A%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"纯音乐专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1613&subname=%E7%BA%AF%E9%9F%B3%E4%B9%90%E4%B8%93%E5%8C%BA\"},{"
    "\"name\":\"国风音乐专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1707&subname=%E5%9B%BD%E9%A3%8E%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\"},{\"name\":\"嘻哈专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1715&subname=%E5%98%BB%E5%93%88%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"R&BSoul专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1743&subname=R&B/"
    "Soul%E4%B8%93%E5%8C%BA\"},{\"name\":\"民谣专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1761&subname=%E6%B0%91%E8%B0%A3%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"DJ歌曲专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1775&subname=DJ%E6%AD%8C%E6%9B%B2%E4%B8%93%E5%8C%BA\"},{"
    "\"name\":\"古典专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1783&subname=%E5%8F%A4%E5%85%B8%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"世界音乐专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1807&subname=%E4%B8%96%E7%95%8C%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\"},{\"name\":\"乡村专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1833&subname=%E4%B9%A1%E6%9D%91%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"爵士专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1849&subname=%E7%88%B5%E5%A3%AB%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"实验音乐专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1883&subname=%E5%AE%9E%E9%AA%8C%E9%9F%B3%E4%B9%90%E4%B8%93%E5%"
    "8C%BA\"},{\"name\":\"拉丁专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1895&subname=%E6%8B%89%E4%B8%81%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"金属专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1901&subname=%E9%87%91%E5%B1%9E%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"朋克专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1915&subname=%E6%9C%8B%E5%85%8B%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"雷鬼专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1917&subname=%E9%9B%B7%E9%AC%BC%E4%B8%93%E5%8C%BA\"},{\"name\":"
    "\"布鲁斯专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1925&subname=%E5%B8%83%E9%B2%81%E6%96%AF%E4%B8%93%E5%8C%BA\"},{"
    "\"name\":\"ACG歌曲专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1921&subname=ACG%E6%AD%8C%E6%9B%B2%E4%B8%93%E5%8C%BA\"},{"
    "\"name\":\"中国特色专区\",\"jump_url\":\"https://h5.kugou.com/apps/style-category/dist/"
    "index.html?bid=1599&bname=%E9%A3%8E%E6%A0%BC&subid=1923&subname=%E4%B8%AD%E5%9B%BD%E7%89%B9%E8%89%B2%E4%B8%93%E5%"
    "8C%BA\"}]},{\"has_child\":0,\"icon\":\"http://imge.kugou.com/mcommon/{size}/20181130/"
    "20181130172408696210.png\",\"id\":58,\"name\":\"乐器\",\"children\":[{\"name\":\"古筝\",\"jump_url\":\"\"},{"
    "\"name\":\"钢琴\",\"jump_url\":\"\"},{\"name\":\"笛子\",\"jump_url\":\"\"},{\"name\":\"二胡\",\"jump_url\":\"\"},{"
    "\"name\":\"吉他\",\"jump_url\":\"\"},{\"name\":\"萨克斯\",\"jump_url\":\"\"},{\"name\":\"八音盒\",\"jump_url\":"
    "\"\"},{\"name\":\"小提琴\",\"jump_url\":\"\"},{\"name\":\"葫芦丝\",\"jump_url\":\"\"},{\"name\":\"唢呐\",\"jump_"
    "url\":\"\"}]}]},\"errcode\":0}";

static void checkResult(Response *resp, char *jstrOutput);

void test2()
{
    printf("=========================================\n");
    printf("\t\tRunning %s\n", __FUNCTION__);
    printf("=========================================\n");
    Response resp;
    memset(&resp, 0, sizeof(resp));

    /* string to struct */
    int ret = csonJsonStr2Struct(jStr, &resp, ResponseTbl);
    printf("decode ret=%d\n", ret);
    CHECK_NUMBER(ret, 0);

    /* test print */
    // csonPrintProperty(&resp, ResponseTbl);

    char *jstrOutput;
    ret = csonStruct2JsonStr(&jstrOutput, &resp, ResponseTbl);
    CHECK_NUMBER(ret, 0);
    printf("encode ret=%d\nJson:%s\n", ret, jstrOutput);

    /*assert check*/
    checkResult(&resp, jstrOutput);

    free(jstrOutput);
    csonFreePointer(&resp, ResponseTbl);

    printf("Successed %s.\n", __FUNCTION__);
}

void checkResult(Response *resp, char *jstrOutput)
{
    CHECK_NUMBER(resp->status, 1);
    CHECK_NUMBER(resp->data.timestamp, 1579069151);
    CHECK_NUMBER(resp->data.infoNum, 9);

    CHECK_NUMBER(resp->data.info[0].has_child, 1);
    CHECK_STRING(resp->data.info[0].icon, "http://imge.kugou.com/mcommon/{size}/20181130/20181130172444711866.png");
    CHECK_NUMBER(resp->data.info[0].id, 153);
    CHECK_STRING(resp->data.info[0].name, "专区");
    CHECK_NUMBER(resp->data.info[0].childrenNum, 16);
    CHECK_STRING(resp->data.info[0].children[0].name, "DJ专区");
    CHECK_STRING(resp->data.info[0].children[0].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F1");
    CHECK_STRING(resp->data.info[0].children[1].name, "抖音专区");
    CHECK_STRING(resp->data.info[0].children[1].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F45");
    CHECK_STRING(resp->data.info[0].children[2].name, "儿童专区");
    CHECK_STRING(resp->data.info[0].children[2].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F3");
    CHECK_STRING(resp->data.info[0].children[3].name, "网络专区");
    CHECK_STRING(resp->data.info[0].children[3].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F23");
    CHECK_STRING(resp->data.info[0].children[4].name, "车载专区");
    CHECK_STRING(resp->data.info[0].children[4].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F9");
    CHECK_STRING(resp->data.info[0].children[5].name, "广场舞专区");
    CHECK_STRING(resp->data.info[0].children[5].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F11");
    CHECK_STRING(resp->data.info[0].children[6].name, "轻音乐专区");
    CHECK_STRING(resp->data.info[0].children[6].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F17");
    CHECK_STRING(resp->data.info[0].children[7].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F19");
    CHECK_STRING(resp->data.info[0].children[7].name, "情歌专区");
    CHECK_STRING(resp->data.info[0].children[8].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F7");
    CHECK_STRING(resp->data.info[0].children[8].name, "国风专区");
    CHECK_STRING(resp->data.info[0].children[9].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F27");
    CHECK_STRING(resp->data.info[0].children[9].name, "电音专区");
    CHECK_STRING(resp->data.info[0].children[10].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F29");
    CHECK_STRING(resp->data.info[0].children[10].name, "欧美专区");
    CHECK_STRING(resp->data.info[0].children[11].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F5");
    CHECK_STRING(resp->data.info[0].children[11].name, "ACG专区");
    CHECK_STRING(resp->data.info[0].children[12].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F15");
    CHECK_STRING(resp->data.info[0].children[12].name, "影视专区");
    CHECK_STRING(resp->data.info[0].children[13].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F25");
    CHECK_STRING(resp->data.info[0].children[13].name, "戏曲专区");
    CHECK_STRING(resp->data.info[0].children[14].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F13");
    CHECK_STRING(resp->data.info[0].children[14].name, "粤剧专区");
    CHECK_STRING(resp->data.info[0].children[15].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F31");
    CHECK_STRING(resp->data.info[0].children[15].name, "韩流专区");

    CHECK_NUMBER(resp->data.info[1].has_child, 1);
    CHECK_STRING(resp->data.info[1].icon, "http://imge.kugou.com/mcommon/{size}/20191104/20191104103209217784.png");
    CHECK_NUMBER(resp->data.info[1].id, 2005);
    CHECK_STRING(resp->data.info[1].name, "心情");
    CHECK_NUMBER(resp->data.info[1].childrenNum, 9);
    CHECK_STRING(resp->data.info[1].children[0].name, "伤感专区");
    CHECK_STRING(resp->data.info[1].children[1].name, "安静专区");
    CHECK_STRING(resp->data.info[1].children[2].name, "对抗抑郁专区");
    CHECK_STRING(resp->data.info[1].children[3].name, "轻松专区");
    CHECK_STRING(resp->data.info[1].children[4].name, "激情专区");
    CHECK_STRING(resp->data.info[1].children[5].name, "甜蜜专区");
    CHECK_STRING(resp->data.info[1].children[6].name, "开心专区");
    CHECK_STRING(resp->data.info[1].children[7].name, "想念专区");
    CHECK_STRING(resp->data.info[1].children[8].name, "疗伤专区");

    CHECK_NUMBER(resp->data.info[2].has_child, 1);
    CHECK_STRING(resp->data.info[2].icon, "http://imge.kugou.com/mcommon/{size}/20181130/20181130172455112096.png");
    CHECK_NUMBER(resp->data.info[2].id, 1263);
    CHECK_STRING(resp->data.info[2].name, "特色");
    CHECK_NUMBER(resp->data.info[2].childrenNum, 16);
    CHECK_STRING(resp->data.info[2].children[0].name, "官方歌单");
    CHECK_STRING(resp->data.info[2].children[1].name, "重返2010专区");
    CHECK_STRING(resp->data.info[2].children[2].name, "佛乐专区");
    CHECK_STRING(resp->data.info[2].children[3].name, "综艺专区");
    CHECK_STRING(resp->data.info[2].children[4].name, "首发专区");
    CHECK_STRING(resp->data.info[2].children[5].name, "音乐人专区");
    CHECK_STRING(resp->data.info[2].children[6].name, "古典乐专区");
    CHECK_STRING(resp->data.info[2].children[7].name, "HiFi专区");
    CHECK_STRING(resp->data.info[2].children[8].name, "英语学习专区");
    CHECK_STRING(resp->data.info[2].children[9].name, "2019年终专区");
    CHECK_STRING(resp->data.info[2].children[10].name, "有声电台");
    CHECK_STRING(resp->data.info[2].children[11].name, "Urban专区");
    CHECK_STRING(resp->data.info[2].children[12].name, "入驻厂牌专区");
    CHECK_STRING(resp->data.info[2].children[13].name, "游戏专区");
    CHECK_STRING(resp->data.info[2].children[14].name, "说唱专区");
    CHECK_STRING(resp->data.info[2].children[15].name, "冬天听什么专区");
    CHECK_STRING(resp->data.info[2].children[1].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F65");
    CHECK_STRING(resp->data.info[2].children[3].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2findex.html%23%2fmobile%2fhome%2f37");
    CHECK_STRING(resp->data.info[2].children[4].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F35");
    CHECK_STRING(resp->data.info[2].children[5].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F33");
    CHECK_STRING(resp->data.info[2].children[7].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F21");
    CHECK_STRING(resp->data.info[2].children[9].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F69");
    CHECK_STRING(resp->data.info[2].children[10].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F39");
    CHECK_STRING(resp->data.info[2].children[13].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F71");
    CHECK_STRING(resp->data.info[2].children[14].jump_url,
        "https://miniapp.kugou.com/node/v2?type=1&id=74&path=%2Findex.html%23%2Fmobile%2Fhome%2F63");

    CHECK_NUMBER(resp->data.info[3].has_child, 1);
    CHECK_STRING(resp->data.info[3].icon, "http://imge.kugou.com/mcommon/{size}/20191104/20191104103220446249.png");
    CHECK_NUMBER(resp->data.info[3].id, 2003);
    CHECK_STRING(resp->data.info[3].name, "场景");
    CHECK_NUMBER(resp->data.info[3].childrenNum, 9);

    CHECK_NUMBER(resp->data.info[4].has_child, 1);
    CHECK_STRING(resp->data.info[4].icon, "http://imge.kugou.com/mcommon/{size}/20191104/20191104103232420148.png");
    CHECK_NUMBER(resp->data.info[4].id, 2007);
    CHECK_STRING(resp->data.info[4].name, "主题");
    CHECK_NUMBER(resp->data.info[4].childrenNum, 14);

    CHECK_NUMBER(resp->data.info[5].has_child, 0);
    CHECK_STRING(resp->data.info[5].icon, "http://imge.kugou.com/mcommon/{size}/20181130/20181130172345416784.png");
    CHECK_NUMBER(resp->data.info[5].id, 32);
    CHECK_STRING(resp->data.info[5].name, "年代");
    CHECK_NUMBER(resp->data.info[5].childrenNum, 4);

    CHECK_NUMBER(resp->data.info[6].has_child, 1);
    CHECK_STRING(resp->data.info[6].icon, "http://imge.kugou.com/mcommon/{size}/20191202/20191202161746384633.png");
    CHECK_NUMBER(resp->data.info[6].id, 2009);
    CHECK_STRING(resp->data.info[6].name, "语言");
    CHECK_NUMBER(resp->data.info[6].childrenNum, 9);

    CHECK_NUMBER(resp->data.info[7].has_child, 1);
    CHECK_STRING(resp->data.info[7].icon, "http://imge.kugou.com/mcommon/{size}/20191104/20191104102344527371.png");
    CHECK_NUMBER(resp->data.info[7].id, 1599);
    CHECK_STRING(resp->data.info[7].name, "风格");
    CHECK_NUMBER(resp->data.info[7].childrenNum, 21);

    CHECK_NUMBER(resp->data.info[8].has_child, 0);
    CHECK_STRING(resp->data.info[8].icon, "http://imge.kugou.com/mcommon/{size}/20181130/20181130172408696210.png");
    CHECK_NUMBER(resp->data.info[8].id, 58);
    CHECK_STRING(resp->data.info[8].name, "乐器");
    CHECK_NUMBER(resp->data.info[8].childrenNum, 10);

    CHECK_STRING(jstrOutput, encodeTest);
}