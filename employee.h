#pragma once
#ifndef _EMPLOYEE_H
#define _EMPLOYEE_H

// 必须在使用 Windows 头文件前定义 UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <cstdio>
#include <cstring>
#include <mysql.h>
#include <graphics.h>
#include <conio.h>
#include <windows.h>

using namespace std;

extern char global_str[70];

// ---- 控制台菜单 ----
void start_menu();
void admin_op_menu();
void emp_op_menu();
void input_wrong();
int  check(char* str, char c);

// ---- 工具函数 ----
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn);
void  getMD5(MYSQL* sock, const char* str);
void  wait_for_key();

// ---- 控制台功能 ----
void admin_login(MYSQL* sock);
void emp_login(MYSQL* sock);
void admin_operate(MYSQL* sock);
void emp_operate(MYSQL* sock, char id[]);

// ---- GUI 界面 ----
void GUI_menu(MYSQL* sock);
void GUI_admin_login(MYSQL* sock);
void GUI_emp_login(MYSQL* sock);
void GUI_admin_operate(MYSQL* sock);
void GUI_emp_operate(MYSQL* sock, char id[]);

// ---- CRUD 操作 ----
void add_emp(MYSQL* sock);
void del_emp(MYSQL* sock);
void add_dept(MYSQL* sock);
void del_dept(MYSQL* sock);
void show_emps(MYSQL* sock);
void show_depts(MYSQL* sock);
void select_emp_by_id(MYSQL* sock);
void select_emp_by_dept(MYSQL* sock);
void update_pass(MYSQL* sock, char id[]);

#endif
