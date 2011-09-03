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

time_t date_current_datetime();
time_t date_current_date();
time_t date_current_time();
time_t date_create_datetime(int year,int month,int day,int hour,int minute,int second);
time_t date_create_date(int year,int month,int day);
time_t date_create_time(int hour,int minute,int second);
int date_get_year(time_t date);
int date_get_month(time_t date);
int date_get_week(time_t date);
int date_get_day(time_t date);
int date_get_hour(time_t date);
int date_get_minute(time_t date);
int date_get_second(time_t date);
int date_get_weekday(time_t date);
int date_get_day_of_year(time_t date);
int date_get_hour_of_year(time_t date);
int date_get_minute_of_year(time_t date);
int date_get_second_of_year(time_t date);

double date_year_span(time_t date1,time_t date2);
double date_month_span(time_t date1,time_t date2);
double date_week_span(time_t date1,time_t date2);
double date_day_span(time_t date1,time_t date2);
double date_hour_span(time_t date1,time_t date2);
double date_minute_span(time_t date1,time_t date2);
double date_second_span(time_t date1,time_t date2);

int date_compare_datetime(time_t date1,time_t date2);
int date_compare_date(time_t date1,time_t date2);
int date_compare_time(time_t date1,time_t date2);

time_t date_date_of(time_t date);
time_t date_time_of(time_t date);

int date_days_in_month(time_t date);
int date_days_in_year(time_t date);
bool date_leap_year(time_t date);
bool date_is_today(time_t date);

time_t date_inc_year(time_t date,int amount);
time_t date_inc_month(time_t date,int amount);
time_t date_inc_week(time_t date,int amount);
time_t date_inc_day(time_t date,int amount);
time_t date_inc_hour(time_t date,int amount);
time_t date_inc_minute(time_t date,int amount);
time_t date_inc_second(time_t date,int amount);

bool date_valid_datetime(int year,int month,int day,int hour,int minute,int second);
bool date_valid_date(int year,int month,int day);
bool date_valid_time(int hour,int minute,int second);

string date_datetime_string(time_t date);
string date_date_string(time_t date);
string date_time_string(time_t date);
string date_datetime_stringf(time_t date,string format);
