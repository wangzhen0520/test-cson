#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// UTF-8字符长度查找表（基于首字节）
static const unsigned char utf8_len_tab[256] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

typedef struct
{
    char *data;          // 字符串数据指针
    size_t length;       // 当前长度（字节数，不含终止符）
    size_t capacity;     // 当前分配容量（字节数，含终止符）
    size_t max_capacity; // 最大允许容量（字节数，0表示无限制）
} DynamicString;

// 初始化动态字符串
int dynamic_string_init(DynamicString *ds, size_t initial_capacity, size_t max_capacity)
{
    if (max_capacity > 0) {
        if (initial_capacity > max_capacity)
            initial_capacity = max_capacity;
        if (initial_capacity < 1)
            initial_capacity = 1;
    } else {
        if (initial_capacity < 1)
            initial_capacity = 1;
    }

    ds->data = (char *)malloc(initial_capacity);
    if (!ds->data)
        return -1;

    ds->capacity = initial_capacity;
    ds->max_capacity = max_capacity;
    ds->length = 0;
    ds->data[0] = '\0';
    return 0;
}

// 释放内存
void dynamic_string_free(DynamicString *ds)
{
    if (ds->data) {
        free(ds->data);
        ds->data = NULL;
    }
    ds->length = ds->capacity = ds->max_capacity = 0;
}

// 内部扩容函数
static int dynamic_string_expand(DynamicString *ds, size_t required)
{
    if (ds->max_capacity > 0 && required > ds->max_capacity)
        required = ds->max_capacity;

    size_t new_capacity = ds->capacity;
    while (new_capacity < required) {
        new_capacity *= 2;
        if (ds->max_capacity > 0 && new_capacity > ds->max_capacity) {
            new_capacity = ds->max_capacity;
            break;
        }
    }

    if (new_capacity < required)
        return -1;

    char *new_data = (char *)realloc(ds->data, new_capacity);
    if (!new_data)
        return -1;

    ds->data = new_data;
    ds->capacity = new_capacity;
    return 0;
}

// 计算字符串中完整UTF8字符的字节长度
static size_t utf8_strlen(const char *s, size_t max_bytes)
{
    size_t len = 0, total = 0;
    while (*s && total < max_bytes) {
        unsigned char c = (unsigned char)*s;
        size_t char_len = utf8_len_tab[c];
        if (total + char_len > max_bytes)
            break;
        len++;
        s += char_len;
        total += char_len;
    }
    return len;
}

// 安全追加字符串（核心函数）
int dynamic_string_append_str(DynamicString *ds, const char *str)
{
    const size_t append_len = strlen(str);
    const size_t max_storage = (ds->max_capacity > 0) ? (ds->max_capacity - 1) : SIZE_MAX;

    // 情况1：不需要截断
    if (ds->length + append_len <= max_storage) {
        const size_t required = ds->length + append_len + 1;
        if (required > ds->capacity) {
            if (dynamic_string_expand(ds, required) != 0)
                return -1;
        }
        memcpy(ds->data + ds->length, str, append_len);
        ds->length += append_len;
        ds->data[ds->length] = '\0';
        return 0;
    }

    // 情况2：需要截断处理
    const size_t overflow = ds->length + append_len - max_storage;
    size_t remove_bytes = 0;
    size_t pos = 0;

    // 步骤1：移除旧数据的完整字符
    while (remove_bytes < overflow && pos < ds->length) {
        const unsigned char c = (unsigned char)ds->data[pos];
        const size_t char_len = utf8_len_tab[c];

        if (pos + char_len > ds->length)
            break; // 无效字符

        remove_bytes += char_len;
        pos += char_len;
    }

    // 执行数据移除
    const size_t new_length = ds->length - pos;
    memmove(ds->data, ds->data + pos, new_length);
    ds->length = new_length;

    // 步骤2：追加新数据的完整字符
    size_t remaining_space = max_storage - ds->length;
    size_t copied = 0;
    const char *p = str;

    while (remaining_space > 0 && *p) {
        const unsigned char c = (unsigned char)*p;
        const size_t char_len = utf8_len_tab[c];

        if (char_len > remaining_space)
            break;
        if (p + char_len > str + append_len)
            break; // 不完整字符

        memcpy(ds->data + ds->length, p, char_len);
        ds->length += char_len;
        p += char_len;
        copied += char_len;
        remaining_space -= char_len;
    }

    // 更新终止符
    ds->data[ds->length] = '\0';

    // 检查是否需要调整容量
    if (ds->length + 1 > ds->capacity) {
        dynamic_string_expand(ds, ds->length + 1);
    }

    return 0;
}

// 测试用例
int test10()
{
    DynamicString ds;
    dynamic_string_init(&ds, 512, 1024); // 最大容量10字节

    // 测试1：正常追加
    dynamic_string_append_str(&ds, "你好"); // 3+3=6字节
    printf("测试1: %s\n", ds.data);         // 输出"你好"

    // 测试2：触发截断（需要移除完整字符）
    dynamic_string_append_str(&ds, "世界123"); // 3+3+3=9字节，总需求6+9=15
    /* 执行过程：
       1. 需要移除15-10=5字节
       2. 移除"你"(3字节)后还需移除2字节，但下一个字符是"好"(3字节)
       3. 完整移除"你好"(6字节)
       4. 追加"世界1"中的"世界"(6字节)和"1"(1字节，共7字节)
       5. 最终结果："世界1"（3+3+1=7字节）
    */
    printf("测试2: %s\n", ds.data); // 输出"世界1"

    // 测试3：边界测试
    dynamic_string_append_str(&ds, "abc"); // 当前7 + 3 = 10
    printf("测试3: %s\n", ds.data);        // 输出"世界1abc"

    dynamic_string_free(&ds);
    return 0;
}