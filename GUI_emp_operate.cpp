#include "employee.h"

void GUI_emp_operate(MYSQL* sock, char id[]) {
start:
    initgraph(800, 400);

    setbkcolor(RGB(240, 245, 255));
    cleardevice();
    setbkmode(TRANSPARENT);

    // ---- 顶部横幅 ----
    setfillcolor(RGB(30, 80, 160));
    setlinecolor(RGB(20, 60, 130));
    fillrectangle(0, 0, 800, 65);
    setfillcolor(RGB(25, 70, 145));
    fillrectangle(0, 60, 800, 65);

    settextcolor(WHITE);
    settextstyle(26, 0, L"微软雅黑");
    outtextxy(310, 18, L"员工操作面板");

    // ---- 分隔线 ----
    setlinecolor(RGB(200, 210, 230));
    setlinestyle(PS_SOLID, 1);
    line(50, 85, 750, 85);

    // ---- 按钮 ----
    settextcolor(WHITE);
    settextstyle(22, 0, L"微软雅黑");

    auto drawEmpBtn = [&](int y1, int y2, COLORREF fill, COLORREF border, const wchar_t* txt, int tx, int ty) {
        setfillcolor(fill);
        setlinecolor(border);
        setlinestyle(PS_SOLID, 2);
        fillroundrect(200, y1, 600, y2, 15, 15);
        settextcolor(WHITE);
        settextstyle(22, 0, L"微软雅黑");
        outtextxy(tx, ty, txt);
    };

    // 查看个人信息
    drawEmpBtn(110, 175, RGB(60, 120, 220), RGB(40, 90, 190), L"查看个人信息", 300, 135);
    // 修改密码
    drawEmpBtn(200, 265, RGB(60, 120, 220), RGB(40, 90, 190), L"修改密码", 320, 225);
    // 退出
    drawEmpBtn(300, 365, RGB(200, 60, 60), RGB(160, 40, 40), L"退出登录", 340, 325);

    // ---- 鼠标事件 ----
    MOUSEMSG k;
    while (true) {
        k = GetMouseMsg();
        if (k.uMsg == WM_LBUTTONDOWN) {
            // 查看个人信息
            if (k.x >= 200 && k.x <= 600 && k.y >= 110 && k.y <= 175) {
                initgraph(800, 300);
                setbkcolor(RGB(240, 245, 255));
                cleardevice();
                setbkmode(TRANSPARENT);

                // 顶部装饰
                setfillcolor(RGB(30, 80, 160));
                setlinecolor(RGB(20, 60, 130));
                fillrectangle(0, 0, 800, 55);
                settextcolor(WHITE);
                settextstyle(22, 0, L"微软雅黑");
                outtextxy(310, 15, L"个人信息");

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

                settextcolor(RGB(50, 50, 50));
                settextstyle(20, 0, L"微软雅黑");

                wchar_t temp[256];
                char info[256];

                if (row) {
                    sprintf_s(info, "工号: %s    姓名: %s    部门: %s",
                              row[0], row[1], row[2]);
                    MultiByteToWideChar(CP_ACP, 0, info, -1, temp, 256);
                    outtextxy(100, 85, temp);

                    sprintf_s(info, "职位: %s    电话: %s    薪资: %s 元",
                              row[3] ? row[3] : "未设置",
                              row[4] ? row[4] : "未设置",
                              row[5] ? row[5] : "0.00");
                    MultiByteToWideChar(CP_ACP, 0, info, -1, temp, 256);
                    outtextxy(100, 130, temp);
                }
                mysql_free_result(result);

                // 返回提示
                settextcolor(RGB(140, 150, 170));
                settextstyle(16, 0, L"宋体");
                outtextxy(320, 230, L"点击任意位置返回...");
                wait_for_key();
                closegraph();
                goto start;
            }
            // 修改密码
            if (k.x >= 200 && k.x <= 600 && k.y >= 200 && k.y <= 265) {
                update_pass(sock, id);
                goto start;
            }
            // 退出
            if (k.x >= 200 && k.x <= 600 && k.y >= 300 && k.y <= 365) {
                closegraph();
                return;
            }
        }
    }
}
