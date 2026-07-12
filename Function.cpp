#include "employee.h"

// ============================================================
//  控制台菜单输出
// ============================================================

void start_menu() {
    cout << endl;
    cout << "				+------------------------------------------+" << endl << endl;
    cout << "				|             员工管理系统                  |" << endl << endl;
    cout << "				|              [1] 管理员登录              |" << endl << endl;
    cout << "				|              [2] 员工登录                |" << endl << endl;
    cout << "				|              [0] 退出系统                |" << endl << endl;
    cout << "				+------------------------------------------+" << endl;
}

void admin_op_menu() {
    cout << endl;
    cout << "				+------------------------------------------+" << endl << endl;
    cout << "				|              操作清单                    |" << endl << endl;
    cout << "				|              [1]  添加员工               |" << endl << endl;
    cout << "				|              [2]  删除员工               |" << endl << endl;
    cout << "				|              [3]  查询所有员工           |" << endl << endl;
    cout << "				|              [4]  添加部门               |" << endl << endl;
    cout << "				|              [5]  删除部门               |" << endl << endl;
    cout << "				|              [6]  查询所有部门           |" << endl << endl;
    cout << "				|              [7]  按工号查询员工         |" << endl << endl;
    cout << "				|              [8]  按部门查询员工         |" << endl << endl;
    cout << "				|              [0]  退出                   |" << endl << endl;
    cout << "				+------------------------------------------+" << endl;
}

void emp_op_menu() {
    cout << endl;
    cout << "				+------------------------------------------+" << endl << endl;
    cout << "				|              操作清单                    |" << endl << endl;
    cout << "				|              [1]  查看个人信息           |" << endl << endl;
    cout << "				|              [2]  修改密码               |" << endl << endl;
    cout << "				|              [0]  退出                   |" << endl << endl;
    cout << "				+------------------------------------------+" << endl;
}

void input_wrong() {
    cout << "非法输入！" << endl;
}

// 过滤非法字符
int check(char* str, char c) {
    while (*str != '\0') {
        if (*str == c) return 1;
        str++;
    }
    return 0;
}

// ============================================================
//  工具函数
// ============================================================

// 获取 MD5 密文（调用 MySQL 的 MD5() 函数）
void getMD5(MYSQL* sock, const char* str) {
    char sqlbuf[200] = "SELECT MD5('";
    MYSQL_RES* result;
    MYSQL_ROW ROW(NULL);
    strcat_s(sqlbuf, str);
    strcat_s(sqlbuf, "')");
    mysql_query(sock, sqlbuf);
    result = mysql_store_result(sock);
    ROW = mysql_fetch_row(result);
    mysql_free_result(result);
    strcpy_s(global_str, (char*)ROW[0]);
}

// ============================================================
//  控制台下管理员登录
// ============================================================

void admin_login(MYSQL* sock) {
    while (1) {
        char id[70], pass[70], real_pass[70];
        char sqlbuf[200] = "SELECT admin_pass FROM admin WHERE admin_id = '";

        cout << "输入管理员账号: ";
        cin >> id;
        cout << "输入管理员密码: ";
        cin >> pass;

        getMD5(sock, pass);
        strcpy_s(pass, global_str);

        strcat_s(sqlbuf, id);
        strcat_s(sqlbuf, "'");

        if (mysql_query(sock, sqlbuf)) {
            printf("查询失败! %s\n", mysql_error(sock));
            return;
        }

        MYSQL_RES* result;
        MYSQL_ROW row;
        if (!(result = mysql_store_result(sock))) {
            printf("结果获取失败! %s\n", mysql_error(sock));
            return;
        }

        if (!mysql_num_rows(result)) {
            printf("该管理员账号不存在\n");
            mysql_free_result(result);
            return;
        }

        row = mysql_fetch_row(result);
        strcpy_s(real_pass, (char*)row[0]);
        mysql_free_result(result);

        if (!strcmp(real_pass, pass)) {
            cout << "管理员登录成功！" << endl;
            admin_operate(sock);
            break;
        }
        else {
            cout << "密码错误，请重试！" << endl;
            continue;
        }
    }
}

// ============================================================
//  控制台下员工登录
// ============================================================

void emp_login(MYSQL* sock) {
    while (1) {
        char id[70], pass[70], real_pass[70];
        char sqlbuf[200] = "SELECT emp_pass FROM emp_account WHERE emp_id = '";

        cout << "输入员工工号: ";
        cin >> id;
        cout << "输入密码: ";
        cin >> pass;

        getMD5(sock, pass);
        strcpy_s(pass, global_str);

        strcat_s(sqlbuf, id);
        strcat_s(sqlbuf, "'");

        if (mysql_query(sock, sqlbuf)) {
            printf("查询失败! %s\n", mysql_error(sock));
            return;
        }

        MYSQL_RES* result;
        MYSQL_ROW row;
        if (!(result = mysql_store_result(sock))) {
            printf("结果获取失败! %s\n", mysql_error(sock));
            return;
        }

        if (!mysql_num_rows(result)) {
            printf("该员工工号不存在\n");
            mysql_free_result(result);
            return;
        }

        row = mysql_fetch_row(result);
        strcpy_s(real_pass, (char*)row[0]);
        mysql_free_result(result);

        if (!strcmp(real_pass, pass)) {
            cout << "员工登录成功！" << endl;
            emp_operate(sock, id);
            break;
        }
        else {
            cout << "密码错误，请重试！" << endl;
            continue;
        }
    }
}

// ============================================================
//  控制台下管理员操作
// ============================================================

void admin_operate(MYSQL* sock) {
    int flag = -1;
    while (1) {
        admin_op_menu();
        cin >> flag;
        switch (flag) {
        case 0:  return;
        case 1:  add_emp(sock);       break;
        case 2:  del_emp(sock);       break;
        case 3:  show_emps(sock);     break;
        case 4:  add_dept(sock);      break;
        case 5:  del_dept(sock);      break;
        case 6:  show_depts(sock);    break;
        case 7:  select_emp_by_id(sock);    break;
        case 8:  select_emp_by_dept(sock);  break;
        default: input_wrong();       break;
        }
    }
}

// ============================================================
//  控制台下员工操作
// ============================================================

void emp_operate(MYSQL* sock, char id[]) {
    int flag = -1;
    while (1) {
        emp_op_menu();
        cin >> flag;
        if (flag == 0) return;
        else if (flag == 1) {
            // 查看个人信息
            char sqlbuf[200] = "SELECT e.emp_id, e.emp_name, d.dept_name, "
                               "e.emp_position, e.emp_phone, e.emp_salary "
                               "FROM employee e INNER JOIN department d ON e.emp_dept = d.dept_id "
                               "WHERE e.emp_id = '";
            strcat_s(sqlbuf, id);
            strcat_s(sqlbuf, "'");

            if (mysql_query(sock, sqlbuf)) {
                printf("查询失败! %s\n", mysql_error(sock));
                return;
            }

            MYSQL_RES* result = mysql_store_result(sock);
            if (!result) {
                printf("结果获取失败! %s\n", mysql_error(sock));
                return;
            }

            MYSQL_ROW row = mysql_fetch_row(result);
            if (row) {
                printf("\n========== 个人信息 ==========\n");
                printf("工号: %s\n", row[0]);
                printf("姓名: %s\n", row[1]);
                printf("部门: %s\n", row[2]);
                printf("职位: %s\n", row[3] ? row[3] : "未设置");
                printf("电话: %s\n", row[4] ? row[4] : "未设置");
                printf("薪资: %s\n", row[5] ? row[5] : "0.00");
                printf("==============================\n");
            }
            mysql_free_result(result);
        }
        else if (flag == 2) {
            update_pass(sock, id);
        }
        else {
            input_wrong();
        }
    }
}

// ============================================================
//  添加员工
// ============================================================

void add_emp(MYSQL* sock) {
    char sqlbuf[300] = "INSERT INTO employee(emp_id, emp_name, emp_dept, emp_position, emp_phone, emp_salary) VALUES(";
    char id[70], name[70], dept[70], pos[70], phone[70], salary_str[70];

    TCHAR Tid[70], Tname[70], Tdept[70], Tpos[70], Tphone[70];

    if (InputBox(Tid, 20, L"请输入员工工号", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(id, ConvertLPWSTRToLPSTR(Tid));

    if (InputBox(Tname, 20, L"请输入员工姓名", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(name, ConvertLPWSTRToLPSTR(Tname));

    if (InputBox(Tdept, 20, L"请输入部门编号", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(dept, ConvertLPWSTRToLPSTR(Tdept));

    if (InputBox(Tpos, 20, L"请输入职位", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(pos, ConvertLPWSTRToLPSTR(Tpos));

    if (InputBox(Tphone, 20, L"请输入电话", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(phone, ConvertLPWSTRToLPSTR(Tphone));

    // 构造 SQL: INSERT INTO employee VALUES('id','name','dept','pos','phone',0)
    sprintf_s(sqlbuf,
        "INSERT INTO employee(emp_id, emp_name, emp_dept, emp_position, emp_phone, emp_salary) "
        "VALUES('%s','%s','%s','%s','%s',0)",
        id, name, dept, pos, phone);

    if (mysql_query(sock, sqlbuf)) {
        printf("添加失败! %s\n", mysql_error(sock));
        HWND hWnd = GetHWnd();
        MessageBox(hWnd, L"添加员工失败！请检查部门是否存在。", L"提示", 0);
        return;
    }

    // 同时在 emp_account 中创建登录账号（默认密码 123456）
    char sqlbuf2[200];
    sprintf_s(sqlbuf2,
        "INSERT INTO emp_account(emp_id, emp_pass) VALUES('%s', MD5('123456'))",
        id);

    if (mysql_query(sock, sqlbuf2)) {
        printf("创建账号失败! %s\n", mysql_error(sock));
    }

    mysql_commit(sock);
    HWND hWnd = GetHWnd();
    MessageBox(hWnd, L"添加员工成功！默认密码: 123456", L"提示", 0);
}

// ============================================================
//  删除员工
// ============================================================

void del_emp(MYSQL* sock) {
    char id[70];
    TCHAR Tid[70];

    if (InputBox(Tid, 10, L"请输入要删除的员工工号", NULL, L"", 0, 0, false) == false)
        return;
    strcpy_s(id, ConvertLPWSTRToLPSTR(Tid));

    // 先删除登录账号
    char sqlbuf[200];
    sprintf_s(sqlbuf, "DELETE FROM emp_account WHERE emp_id = '%s'", id);
    mysql_query(sock, sqlbuf);

    // 再删除员工
    sprintf_s(sqlbuf, "DELETE FROM employee WHERE emp_id = '%s'", id);

    if (mysql_query(sock, sqlbuf)) {
        printf("删除失败! %s\n", mysql_error(sock));
        HWND hWnd = GetHWnd();
        MessageBox(hWnd, L"删除失败！", L"提示", 0);
        return;
    }

    mysql_commit(sock);
    HWND hWnd = GetHWnd();
    MessageBox(hWnd, L"删除成功！", L"提示", 0);
}

// ============================================================
//  添加部门
// ============================================================

void add_dept(MYSQL* sock) {
    char id[70], name[70];
    TCHAR Tid[70], Tname[70];

    if (InputBox(Tid, 10, L"请输入部门编号", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(id, ConvertLPWSTRToLPSTR(Tid));

    if (InputBox(Tname, 20, L"请输入部门名称", NULL, L"", 0, 0, false) == false) return;
    strcpy_s(name, ConvertLPWSTRToLPSTR(Tname));

    char sqlbuf[200];
    sprintf_s(sqlbuf, "INSERT INTO department(dept_id, dept_name) VALUES('%s','%s')", id, name);

    if (mysql_query(sock, sqlbuf)) {
        printf("添加失败! %s\n", mysql_error(sock));
        HWND hWnd = GetHWnd();
        MessageBox(hWnd, L"添加部门失败！可能编号已存在。", L"提示", 0);
        return;
    }

    mysql_commit(sock);
    HWND hWnd = GetHWnd();
    MessageBox(hWnd, L"添加部门成功！", L"提示", 0);
}

// ============================================================
//  删除部门
// ============================================================

void del_dept(MYSQL* sock) {
    char id[70];
    TCHAR Tid[70];

    if (InputBox(Tid, 10, L"请输入要删除的部门编号", NULL, L"", 0, 0, false) == false)
        return;
    strcpy_s(id, ConvertLPWSTRToLPSTR(Tid));

    char sqlbuf[200];
    sprintf_s(sqlbuf, "DELETE FROM department WHERE dept_id = '%s'", id);

    if (mysql_query(sock, sqlbuf)) {
        printf("删除失败! %s\n", mysql_error(sock));
        HWND hWnd = GetHWnd();
        MessageBox(hWnd, L"删除失败！请先删除该部门下的员工。", L"提示", 0);
        return;
    }

    mysql_commit(sock);
    HWND hWnd = GetHWnd();
    MessageBox(hWnd, L"删除部门成功！", L"提示", 0);
}

// ============================================================
//  带翻页功能的通用表格显示
// ============================================================

void show_paginated(MYSQL* sock, const char* sql, const wchar_t* title,
    const wchar_t* headers[], int col_widths[], int num_cols, int page_size) {

    struct RowData { wchar_t cols[6][70]; };
    RowData* data = nullptr;
    int total_rows = 0, total_cols = 0, total_pages = 1, current_page = 0;
    const int W = 1000, H = 600;

    // 查询加载数据
    auto load_data = [&]() -> bool {
        if (data) { delete[] data; data = nullptr; }
        if (mysql_query(sock, sql)) { printf("查询失败!\n"); return false; }
        MYSQL_RES* result = mysql_store_result(sock);
        if (!result) { printf("结果获取失败!\n"); return false; }
        total_rows = (int)mysql_num_rows(result);
        total_cols = (int)mysql_num_fields(result);
        total_pages = (total_rows + page_size - 1) / page_size;
        if (total_pages == 0) total_pages = 1;
        data = new RowData[total_rows];
        for (int i = 0; i < total_rows; i++) {
            MYSQL_ROW row = mysql_fetch_row(result);
            for (int j = 0; j < total_cols && j < 6; j++) {
                const char* val = row[j] ? row[j] : "";
                MultiByteToWideChar(CP_ACP, 0, val, -1, data[i].cols[j], 70);
            }
        }
        mysql_free_result(result);
        return true;
    };

    if (!load_data()) { delete[] data; return; }

    // 重绘一页
    auto draw_page = [&]() {
        initgraph(W, H);
        setbkcolor(RGB(240, 245, 255));
        cleardevice();
        setbkmode(TRANSPARENT);

        // 标题栏
        setfillcolor(RGB(30, 80, 160));
        setlinecolor(RGB(20, 60, 130));
        fillrectangle(0, 0, W, 50);
        settextcolor(WHITE);
        settextstyle(22, 0, L"微软雅黑");
        outtextxy(W / 2 - (int)wcslen(title) * 11, 14, title);

        // 表头
        settextcolor(RGB(30, 80, 160));
        settextstyle(18, 0, L"微软雅黑");
        int start_x = 50, x = start_x;
        for (int j = 0; j < num_cols; j++) {
            outtextxy(x + 15, 65, headers[j]);
            x += col_widths[j];
        }

        // 分隔线
        setlinecolor(RGB(200, 210, 230));
        setlinestyle(PS_SOLID, 1);
        line(30, 92, W - 30, 92);

        // 数据行
        settextcolor(RGB(50, 50, 50));
        settextstyle(17, 0, L"宋体");
        int start = current_page * page_size;
        int end = min(start + page_size, total_rows);
        for (int i = start; i < end; i++) {
            x = start_x;
            for (int j = 0; j < num_cols; j++) {
                outtextxy(x + 15, 105 + (i - start) * 33, data[i].cols[j]);
                x += col_widths[j];
            }
        }

        // 底部导航栏
        int nav_y = H - 60;
        setlinecolor(RGB(200, 210, 230));
        setlinestyle(PS_SOLID, 1);
        line(30, nav_y - 10, W - 30, nav_y - 10);

        // 页码
        wchar_t info[30];
        swprintf_s(info, 30, L"第 %d / %d 页  (共 %d 条)", current_page + 1, total_pages, total_rows);
        settextcolor(RGB(80, 100, 130));
        settextstyle(16, 0, L"微软雅黑");
        outtextxy(W / 2 - 100, nav_y + 8, info);

        // 上一页
        if (current_page > 0) {
            setfillcolor(RGB(60, 120, 220));
            setlinecolor(RGB(40, 90, 190));
            setlinestyle(PS_SOLID, 2);
            fillroundrect(150, nav_y, 280, nav_y + 40, 8, 8);
            settextcolor(WHITE);
            settextstyle(18, 0, L"微软雅黑");
            outtextxy(185, nav_y + 10, L"? 上一页");
        }
        // 下一页
        if (current_page < total_pages - 1) {
            setfillcolor(RGB(60, 120, 220));
            setlinecolor(RGB(40, 90, 190));
            setlinestyle(PS_SOLID, 2);
            fillroundrect(W - 280, nav_y, W - 150, nav_y + 40, 8, 8);
            settextcolor(WHITE);
            settextstyle(18, 0, L"微软雅黑");
            outtextxy(W - 265, nav_y + 10, L"下一页 ?");
        }
        // 返回
        setfillcolor(RGB(180, 180, 180));
        setlinecolor(RGB(140, 140, 140));
        setlinestyle(PS_SOLID, 2);
        fillroundrect(440, nav_y, 560, nav_y + 40, 8, 8);
        settextcolor(WHITE);
        settextstyle(18, 0, L"微软雅黑");
        outtextxy(465, nav_y + 10, L"返回");

        return nav_y;
    };

    int nav_y = draw_page();

    // ---- 鼠标事件循环（原地翻页，不关闭窗口） ----
    MOUSEMSG k;
    while (true) {
        k = GetMouseMsg();
        if (k.uMsg == WM_LBUTTONDOWN) {
            bool need_redraw = false;
            if (current_page > 0 && k.x >= 150 && k.x <= 280 && k.y >= nav_y && k.y <= nav_y + 40) {
                current_page--;
                need_redraw = true;
            }
            else if (current_page < total_pages - 1 && k.x >= W - 280 && k.x <= W - 150 && k.y >= nav_y && k.y <= nav_y + 40) {
                current_page++;
                need_redraw = true;
            }
            else if (k.x >= 440 && k.x <= 560 && k.y >= nav_y && k.y <= nav_y + 40) {
                break; // 返回
            }

            if (need_redraw) {
                // 只重绘内容区域，不关闭窗口
                cleardevice();

                // 重绘标题
                setfillcolor(RGB(30, 80, 160));
                setlinecolor(RGB(20, 60, 130));
                fillrectangle(0, 0, W, 50);
                settextcolor(WHITE);
                settextstyle(22, 0, L"微软雅黑");
                outtextxy(W / 2 - (int)wcslen(title) * 11, 14, title);

                // 重绘表头
                settextcolor(RGB(30, 80, 160));
                settextstyle(18, 0, L"微软雅黑");
                int start_x = 50, x = start_x;
                for (int j = 0; j < num_cols; j++) {
                    outtextxy(x + 15, 65, headers[j]);
                    x += col_widths[j];
                }

                // 分隔线
                setlinecolor(RGB(200, 210, 230));
                setlinestyle(PS_SOLID, 1);
                line(30, 92, W - 30, 92);

                // 数据行
                settextcolor(RGB(50, 50, 50));
                settextstyle(17, 0, L"宋体");
                int start = current_page * page_size;
                int end = min(start + page_size, total_rows);
                for (int i = start; i < end; i++) {
                    x = start_x;
                    for (int j = 0; j < num_cols; j++) {
                        outtextxy(x + 15, 105 + (i - start) * 33, data[i].cols[j]);
                        x += col_widths[j];
                    }
                }

                // 重绘导航
                setlinecolor(RGB(200, 210, 230));
                setlinestyle(PS_SOLID, 1);
                line(30, nav_y - 10, W - 30, nav_y - 10);

                wchar_t info[30];
                swprintf_s(info, 30, L"第 %d / %d 页  (共 %d 条)", current_page + 1, total_pages, total_rows);
                settextcolor(RGB(80, 100, 130));
                settextstyle(16, 0, L"微软雅黑");
                outtextxy(W / 2 - 100, nav_y + 8, info);

                if (current_page > 0) {
                    setfillcolor(RGB(60, 120, 220));
                    setlinecolor(RGB(40, 90, 190));
                    setlinestyle(PS_SOLID, 2);
                    fillroundrect(150, nav_y, 280, nav_y + 40, 8, 8);
                    settextcolor(WHITE);
                    settextstyle(18, 0, L"微软雅黑");
                    outtextxy(185, nav_y + 10, L"? 上一页");
                }
                if (current_page < total_pages - 1) {
                    setfillcolor(RGB(60, 120, 220));
                    setlinecolor(RGB(40, 90, 190));
                    setlinestyle(PS_SOLID, 2);
                    fillroundrect(W - 280, nav_y, W - 150, nav_y + 40, 8, 8);
                    settextcolor(WHITE);
                    settextstyle(18, 0, L"微软雅黑");
                    outtextxy(W - 265, nav_y + 10, L"下一页 ?");
                }
                setfillcolor(RGB(180, 180, 180));
                setlinecolor(RGB(140, 140, 140));
                setlinestyle(PS_SOLID, 2);
                fillroundrect(440, nav_y, 560, nav_y + 40, 8, 8);
                settextcolor(WHITE);
                settextstyle(18, 0, L"微软雅黑");
                outtextxy(465, nav_y + 10, L"返回");
            }
        }
    }

    closegraph();
    delete[] data;
}

// ============================================================
//  查询所有员工（带翻页）
// ============================================================

void show_emps(MYSQL* sock) {
    const wchar_t* headers[] = { L"工号", L"姓名", L"部门", L"职位", L"电话", L"薪资" };
    int col_widths[] = { 130, 130, 130, 160, 170, 130 };
    show_paginated(sock,
        "SELECT e.emp_id, e.emp_name, d.dept_name, e.emp_position, e.emp_phone, e.emp_salary "
        "FROM employee e INNER JOIN department d ON e.emp_dept = d.dept_id",
        L"员工列表", headers, col_widths, 6, 12);
}

// ============================================================
//  查询所有部门（部门数量少，无需翻页）
// ============================================================

// ============================================================
//  查询所有部门（GUI 图形界面展示）
// ============================================================

void show_depts(MYSQL* sock) {
    initgraph(700, 450);

    char sqlbuf[200] = "SELECT * FROM department";

    if (mysql_query(sock, sqlbuf)) {
        printf("查询失败! %s\n", mysql_error(sock));
        return;
    }

    MYSQL_RES* result = mysql_store_result(sock);
    if (!result) {
        printf("结果获取失败! %s\n", mysql_error(sock));
        return;
    }

    unsigned int Row_num = mysql_num_rows(result);

    setbkcolor(RGB(240, 245, 255));
    cleardevice();
    setbkmode(TRANSPARENT);

    // 顶部标题栏
    setfillcolor(RGB(30, 80, 160));
    setlinecolor(RGB(20, 60, 130));
    fillrectangle(0, 0, 700, 50);
    settextcolor(WHITE);
    settextstyle(20, 0, L"微软雅黑");
    outtextxy(280, 14, L"部门列表");

    settextcolor(RGB(30, 80, 160));
    settextstyle(18, 0, L"微软雅黑");
    outtextxy(180, 70, L"部门编号");
    outtextxy(380, 70, L"部门名称");

    // 分隔线
    setlinecolor(RGB(200, 210, 230));
    setlinestyle(PS_SOLID, 1);
    line(80, 97, 620, 97);

    settextcolor(RGB(50, 50, 50));
    settextstyle(18, 0, L"宋体");

    wchar_t temp[70];
    for (unsigned int i = 0; i < Row_num; i++) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row[0]) {
            MultiByteToWideChar(CP_ACP, 0, row[0], -1, temp, 70);
            outtextxy(180, 110 + i * 35, temp);
        }
        if (row[1]) {
            MultiByteToWideChar(CP_ACP, 0, row[1], -1, temp, 70);
            outtextxy(380, 110 + i * 35, temp);
        }
    }

    mysql_free_result(result);

    settextcolor(RGB(140, 150, 170));
    settextstyle(14, 0, L"宋体");
    outtextxy(280, 400, L"点击任意位置返回...");
    wait_for_key();
    closegraph();
}

// ============================================================
//  按工号/姓名查询（带翻页）
// ============================================================

void select_emp_by_id(MYSQL* sock) {
    char keyword[70];
    TCHAR Tkey[70];

    if (InputBox(Tkey, 20, L"请输入要查询的工号或姓名", NULL, L"", 0, 0, false) == false)
        return;
    strcpy_s(keyword, ConvertLPWSTRToLPSTR(Tkey));

    char sqlbuf[300];
    sprintf_s(sqlbuf,
        "SELECT e.emp_id, e.emp_name, d.dept_name, e.emp_position "
        "FROM employee e INNER JOIN department d ON e.emp_dept = d.dept_id "
        "WHERE e.emp_id LIKE '%%%s%%' OR e.emp_name LIKE '%%%s%%'",
        keyword, keyword);

    const wchar_t* headers[] = { L"工号", L"姓名", L"部门", L"职位" };
    int col_widths[] = { 170, 170, 200, 200 };
    show_paginated(sock, sqlbuf, L"查询结果", headers, col_widths, 4, 8);
}

// ============================================================
//  按部门查询员工（带翻页）
// ============================================================

void select_emp_by_dept(MYSQL* sock) {
    char dept[70];
    TCHAR Tdept[70];

    if (InputBox(Tdept, 20, L"请输入部门编号查询员工", NULL, L"", 0, 0, false) == false)
        return;
    strcpy_s(dept, ConvertLPWSTRToLPSTR(Tdept));

    char sqlbuf[300];
    sprintf_s(sqlbuf,
        "SELECT e.emp_id, e.emp_name, d.dept_name, e.emp_position "
        "FROM employee e INNER JOIN department d ON e.emp_dept = d.dept_id "
        "WHERE e.emp_dept = '%s'",
        dept);

    const wchar_t* headers[] = { L"工号", L"姓名", L"部门", L"职位" };
    int col_widths[] = { 170, 170, 200, 200 };
    show_paginated(sock, sqlbuf, L"查询结果", headers, col_widths, 4, 8);
}

// ============================================================
//  修改密码
// ============================================================

void update_pass(MYSQL* sock, char id[]) {
    char pass[70];
    char sqlbuf[200] = "UPDATE emp_account SET emp_pass = '";
    TCHAR Tpass[70];

    if (InputBox(Tpass, 20, L"请输入新密码", NULL, L"", 0, 0, false) == false)
        return;
    strcpy_s(pass, ConvertLPWSTRToLPSTR(Tpass));

    // MD5 加密
    getMD5(sock, pass);
    strcpy_s(pass, global_str);

    strcat_s(sqlbuf, pass);
    strcat_s(sqlbuf, "' WHERE emp_id = '");
    strcat_s(sqlbuf, id);
    strcat_s(sqlbuf, "'");

    if (mysql_query(sock, sqlbuf)) {
        printf("更新失败! %s\n", mysql_error(sock));
        HWND hWnd = GetHWnd();
        MessageBox(hWnd, L"密码更新失败！", L"提示", 0);
        return;
    }

    mysql_commit(sock);
    HWND hWnd = GetHWnd();
    MessageBox(hWnd, L"密码更新成功！", L"提示", 0);
}
