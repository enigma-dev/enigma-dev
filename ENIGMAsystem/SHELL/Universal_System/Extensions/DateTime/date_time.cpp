/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Harijs Grînbergs                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
#include <time.h>
#include <string>
using std::string;

time_t date_current_datetime()
{
    return time(NULL);
}

time_t date_current_date()
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_sec = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_hour = 0;
    return mktime(timeinfo);
}

time_t date_current_time()
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = 70;
    timeinfo->tm_mon = 0;
    timeinfo->tm_mday = 1;
    return mktime(timeinfo);
}

time_t date_create_datetime(int year,int month,int day,int hour,int minute,int second)
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;
    return mktime(timeinfo);
}

time_t date_create_date(int year,int month,int day)
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    return mktime(timeinfo);
}

time_t date_create_time(int hour,int minute,int second)
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = 70;
    timeinfo->tm_mon = 0;
    timeinfo->tm_mday = 1;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;
    return mktime(timeinfo);
}

int date_get_year(time_t date)
{
    return localtime(&date)->tm_year+1900;
}

int date_get_month(time_t date)
{
    return localtime(&date)->tm_mon+1;
}

int date_get_day(time_t date)
{
    return localtime(&date)->tm_mday;
}

int date_get_hour(time_t date)
{
    return localtime(&date)->tm_hour;
}

int date_get_minute(time_t date)
{
    return localtime(&date)->tm_min;
}

int date_get_second(time_t date)
{
    return localtime(&date)->tm_sec;
}

int date_get_weekday(time_t date)
{
    return localtime(&date)->tm_wday+1;
}

int date_get_day_of_year(time_t date)
{
    return localtime(&date)->tm_yday+1;
}

int date_get_hour_of_year(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    return timeinfo->tm_yday*24+timeinfo->tm_hour;
}

int date_get_minute_of_year(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    return timeinfo->tm_yday*1440+timeinfo->tm_hour*60+timeinfo->tm_min;
}

int date_get_second_of_year(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    return timeinfo->tm_yday*86400+timeinfo->tm_hour*3600+timeinfo->tm_min*60+timeinfo->tm_sec;
}

double date_year_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/31556925.9936;
    else
        return difftime(date2,date1)/31556925.9936;
}

double date_month_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/2629800;
    else
        return difftime(date2,date1)/2629800;
}

double date_week_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/604800;
    else
        return difftime(date2,date1)/604800;
}

double date_day_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/86400;
    else
        return difftime(date2,date1)/86400;
}

double date_hour_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/3600;
    else
        return difftime(date2,date1)/3600;
}

double date_minute_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2)/60;
    else
        return difftime(date2,date1)/60;
}

double date_second_span(time_t date1,time_t date2)
{
    if (date1>date2)
        return difftime(date1,date2);
    else
      return difftime(date2,date1);
}

int date_compare_datetime(time_t date1,time_t date2)
{
    if (date1>date2) return 1;
    else if (date1==date2) return 0;
    else return -1;
}

int date_compare_date(time_t date1,time_t date2)
{
    struct tm * timeinfo1;
    timeinfo1 = localtime(&date1);
    timeinfo1->tm_hour = 0;
    timeinfo1->tm_min = 0;
    timeinfo1->tm_sec = 0;
    date1=mktime(timeinfo1);
    struct tm * timeinfo2;
    timeinfo2 = localtime(&date2);
    timeinfo2->tm_hour = 0;
    timeinfo2->tm_min = 0;
    timeinfo2->tm_sec = 0;
    date2=mktime(timeinfo2);
    if (date1>date2) return 1;
    else if (date1==date2) return 0;
    else return -1;
}

int date_compare_time(time_t date1,time_t date2)
{
    struct tm * timeinfo1;
    timeinfo1 = localtime(&date1);
    timeinfo1->tm_year = 70;
    timeinfo1->tm_mon = 0;
    timeinfo1->tm_mday = 1;
    date1=mktime(timeinfo1);
    struct tm * timeinfo2;
    timeinfo2 = localtime(&date2);
    timeinfo2->tm_year = 70;
    timeinfo2->tm_mon = 0;
    timeinfo2->tm_mday = 1;
    date2=mktime(timeinfo2);
    if (date1>date2) return 1;
    else if (date1==date2) return 0;
    else return -1;
}

time_t date_date_of(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    return mktime(timeinfo);
}

time_t date_time_of(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_year = 70;
    timeinfo->tm_mon = 0;
    timeinfo->tm_mday = 1;
    return mktime(timeinfo);
}

int date_days_in_month(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    int DaysPerMonth1[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int DaysPerMonth2[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((timeinfo->tm_year+1900)%400 == 0 || ((timeinfo->tm_year+1900)%100 != 0 && (timeinfo->tm_year+1900)%4 == 0))
        return DaysPerMonth1[timeinfo->tm_mon];
    else
        return DaysPerMonth2[timeinfo->tm_mon];
}

int date_days_in_year(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    if ((timeinfo->tm_year+1900)%400 == 0 || ((timeinfo->tm_year+1900)%100 != 0 && (timeinfo->tm_year+1900)%4 == 0))
        return 366;
    else
        return 365;
}

bool date_leap_year(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    if ((timeinfo->tm_year+1900)%400 == 0 || ((timeinfo->tm_year+1900)%100 != 0 && (timeinfo->tm_year+1900)%4 == 0))
        return true;
    else
        return false;
}

bool date_is_today(time_t date)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    int year = timeinfo->tm_year,
        month = timeinfo->tm_mon,
        day = timeinfo->tm_mday;
    time_t rawtime;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (timeinfo->tm_mday == day && timeinfo->tm_mon == month && timeinfo->tm_year == year)
        return true;
    else
        return false;
}

time_t date_inc_year(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_year += amount;
    return mktime(timeinfo);
}

time_t date_inc_month(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_mon += amount;
    return mktime(timeinfo);
}

time_t date_inc_day(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_mday += amount;
    return mktime(timeinfo);
}

time_t date_inc_hour(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_hour += amount;
    return mktime(timeinfo);
}

time_t date_inc_minute(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_min += amount;
    return mktime(timeinfo);
}

time_t date_inc_second(time_t date,int amount)
{
    struct tm * timeinfo;
    timeinfo = localtime(&date);
    timeinfo->tm_sec += amount;
    return mktime(timeinfo);
}

bool date_valid_datetime(int year,int month,int day,int hour,int minute,int second)
{
    if (hour>=0 && hour<24 && minute>=0 && minute<60 && second>=0 && second<60)
        return true;
    else
        return false;
    if (month>0 && month<=12){
        int daycount;
        int DaysPerMonth1[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int DaysPerMonth2[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((year+1900)%400 == 0 || ((year+1900)%100 != 0 && (year+1900)%4 == 0))
            daycount = DaysPerMonth1[month-1];
        else
            daycount = DaysPerMonth2[month-1];
        if (day>0 && day<=daycount)
            return true;
        else
            return false;
    }
    return false;
}

bool date_valid_date(int year,int month,int day)
{
    if (month>0 && month<=12){
        int daycount;
        int DaysPerMonth1[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int DaysPerMonth2[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((year+1900)%400 == 0 || ((year+1900)%100 != 0 && (year+1900)%4 == 0))
            daycount = DaysPerMonth1[month-1];
        else
            daycount = DaysPerMonth2[month-1];
        if (day>0 && day<=daycount)
            return true;
        else
            return false;
    }
    return false;
}

bool date_valid_time(int hour,int minute,int second)
{
    if (hour>=0 && hour<24 && minute>=0 && minute<60 && second>=0 && second<60)
        return true;
    else
        return false;
}

string date_datetime_string(time_t date)
{
    char buffer[80];
    strftime(buffer,80,"%Y.%m.%d. %H:%M:%S",localtime(&date));
    return string(buffer);
}

string date_date_string(time_t date)
{
    char buffer[80];
    strftime(buffer,80,"%Y.%m.%d.",localtime(&date));
    return string(buffer);
}

string date_time_string(time_t date)
{
    char buffer[80];
    strftime(buffer,80,"%H:%M:%S",localtime(&date));
    return string(buffer);
}

string date_datetime_stringf(time_t date,string format)
{
    char buffer[80];
    strftime(buffer,80,format.c_str(),localtime(&date));
    return string(buffer);
}
