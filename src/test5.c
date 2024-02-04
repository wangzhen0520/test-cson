#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <math.h>

int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int test_week()
{
    int year, month, day;
    printf("请输入日期（年 月 日）：");
    scanf("%d %d %d", &year, &month, &day);

    int days_in_month[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int yday = 0;
    for (int i = 1; i < month; i++) {
        yday += days_in_month[i - 1];
    }
    yday += day;

    int weekday = (yday + 10) % 7;
    int week = (yday + 10 - weekday) / 7;

    printf("当前是第%d周\n", week);

    return 0;
}

time_t time_difference = 8 * 3600;
time_t mymktime2(const struct tm *_Tm)
{
    int year = _Tm->tm_year + 1900;
    int mon = _Tm->tm_mon + 1;
    int day = _Tm->tm_mday;
    int hour = _Tm->tm_hour;
    int min = _Tm->tm_min;
    int sec = _Tm->tm_sec;

    if (0 >= (int)(mon -= 2)) { /* 1..12 -> 11,12,1..10 */
        mon += 12;              /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((unsigned long)(year / 4 - year / 100 + year / 400 + 367 * mon / 12 + day) + year * 365 - 719499) * 24 +
                hour /* now have hours */
                ) * 60 +
               min   /* now have minutes */
               ) *
               60 +
           sec - time_difference; /* finally seconds .time_difference是我加入的，刚好差了8小时 */
}

#define SECOND_OF_DAY (24 * 60 * 60)

void ft_time_to_date(uint64_t sec, struct tm *t)
{
    static const char DayOfMon[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    unsigned short i, j, iDay;
    unsigned long lDay;

    lDay = sec / SECOND_OF_DAY;
    sec = sec % SECOND_OF_DAY;

    i = 1970;
    while (lDay > 365) {
        if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0)) {
            lDay -= 366;
        } else {
            lDay -= 365;
        }
        i++;
    }
    if ((lDay == 365) && !(((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))) {
        lDay -= 365;
        i++;
    }
    t->tm_year = i - 1900;
    for (j = 0; j < 12; j++) {
        if ((j == 1) && (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))) {
            iDay = 29;
        } else {
            iDay = DayOfMon[j];
        }
        if (lDay >= iDay) {
            lDay -= iDay;
        } else {
            break;
        }
    }
    t->tm_mon = j;
    t->tm_mday = lDay + 1;
    t->tm_hour = ((sec / 3600)) % 24; // 这里注意，世界时间已经加上北京时间差8，
    t->tm_min = (sec % 3600) / 60;
    t->tm_sec = (sec % 3600) % 60;
}

uint8_t ft_time_to_weekday(struct tm *t)
{
    uint32_t u32WeekDay = 0;
    uint32_t u32Year = t->tm_year + 1900;
    uint8_t u8Month = t->tm_mon + 1;
    uint8_t u8Day = t->tm_mday;
    if (u8Month < 3U) {
        /*D = { [(23 x month) / 9] + day + 4 + year + [(year - 1) / 4] - [(year - 1) / 100] + [(year - 1) / 400] } mod
         * 7*/
        u32WeekDay = (((23U * u8Month) / 9U) + u8Day + 4U + u32Year + ((u32Year - 1U) / 4U) - ((u32Year - 1U) / 100U) +
                         ((u32Year - 1U) / 400U)) %
                     7U;
    } else {
        /*D = { [(23 x month) / 9] + day + 4 + year + [year / 4] - [year / 100] + [year / 400] - 2 } mod 7*/
        u32WeekDay = (((23U * u8Month) / 9U) + u8Day + 4U + u32Year + (u32Year / 4U) - (u32Year / 100U) +
                         (u32Year / 400U) - 2U) %
                     7U;
    }

    if (0U == u32WeekDay) {
        u32WeekDay = 7U;
    }

    return u32WeekDay % 7;
}

int get_days_of_year(struct tm *t)
{
    int year = t->tm_year + 1900;
    int mon = t->tm_mon + 1;
    int day = t->tm_mday;
    int leap = 0;

    static const int ppiYearDays[2][13] = {
        /* Normal year */
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        /* Leap year */
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335},
    };

    /* Check for leap year */
    if (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0))) {
        leap = 1;
    }
    /* Calculate the year days */
    return ppiYearDays[leap][mon] + day;
}

int get_weeks_of_year(const int year)
{
    struct tm t = {
        .tm_year = year - 1900,
        .tm_mon = 12 - 1,
        .tm_mday = 28,
    };
    int week_day = ft_time_to_weekday(&t);
    week_day = ((week_day + 6) % 7) + 1; // 转换到1~7
    int year_day = get_days_of_year(&t);
    int weeks = (year_day - week_day + 10) / 7;
    return weeks;
}

int ft_time_to_yearweek(struct tm *t, int weekday)
{
    int year = t->tm_year + 1900;
    int year_day = get_days_of_year(t);

    int week_num = (year_day - weekday + 10) / 7;
    if (week_num < 1) {
        week_num = get_weeks_of_year(year - 1);
        // t->tm_year = year - 1 - 1900;
    } else if (week_num > get_weeks_of_year(year)) {
        week_num = 1;
    }
    return week_num;
}

// return 1 on failure, 0 on success
int tm_YearWeek(const struct tm *tmptr, int *year, int *week)
{
    // work with local copy
    struct tm tm = *tmptr;
    // fully populate the yday and wday fields.
    time_t t = mktime(&tm);
    if (t == -1) {
        return 1;
    }

    // Find day-of-the-week: 0 to 6.
    // Week starts on Monday per ISO 8601
    // 0 <= DayOfTheWeek <= 6, Monday, Tuesday ... Sunday
    int DayOfTheWeek = (tm.tm_wday + (7 - 1)) % 7;

    // Offset the month day to the Monday of the week.
    tm.tm_mday -= DayOfTheWeek;
    // Offset the month day to the mid-week (Thursday) of the week, 3 days later.
    tm.tm_mday += 3;
    // Re-evaluate tm_year and tm_yday  (local time)
    t = mktime(&tm);
    if (t == -1) {
        return 1;
    }

    *year = tm.tm_year + 1900;
    // Convert yday to week of the year, stating with 1.
    *week = tm.tm_yday / 7 + 1;
    return 0;
}

// return 1 on failure, 0 on success
int my_tm_YearWeek(const struct tm *tmptr, int *year, int *week)
{
    // work with local copy
    struct tm tm = *tmptr;

    int week_day = ft_time_to_weekday(&tm);
    week_day = ((week_day + 6) % 7) + 1; // 转换到1~7

    // Convert yday to week of the year, stating with 1.
    int year_week = ft_time_to_yearweek(&tm, week_day);

    *year = tm.tm_year + 1900;
    *week = year_week;
    return 0;
}

void test5()
{
    for (size_t i = 1970; i < 2024; i++) {
        struct tm tm = {0};
        tm.tm_year = i - 1900;
        tm.tm_mon = 1 - 1;
        tm.tm_mday = 1;
        tm.tm_isdst = -1;
        int y = 0, w = 0;
        int err = tm_YearWeek(&tm, &y, &w);
        printf("Ori:%d  Year:%d  Week:%d %02d%02d\n", i, y, w, y % 100, w);
        int y2 = 0, w2 = 0;
        my_tm_YearWeek(&tm, &y2, &w2);
        printf("Ori:%d  Year:%d  Week:%d %02d%02d\n", i, y2, w2, y2 % 100, w2);

        if (w != w2) {
            printf("Week:%d != MyWeek:%d !!!!!!!!\n", w, w2);
        }
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    }
}