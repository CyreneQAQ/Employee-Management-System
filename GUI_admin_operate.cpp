#include "employee.h"

void GUI_menu(MYSQL* sock);

void GUI_admin_operate(MYSQL* sock) {
start:
    initgraph(1000, 600);

    setbkcolor(RGB(240, 245, 255));
    cleardevice();
    setbkmode(TRANSPARENT);

    // ---- 顶部横幅 ----
    setfillcolor(RGB(30, 80, 160));
    setlinecolor(RGB(20, 60, 130));
    fillrectangle(0, 0, 1000, 65);
    setfillcolor(RGB(25, 70, 145));
    fillrectangle(0, 60, 1000, 65);

    settextcolor(WHITE);
    settextstyle(28, 0, L"微软雅黑");
    outtextxy(390, 18, L"管理员操作面板");

    // ---- 列标签 ----
    settextcolor(RGB(80, 100, 130));
    settextstyle(16, 0, L"微软雅黑");
    outtextxy(130, 80, L"— 员工管理 —");
    outtextxy(450, 80, L"— 部门管理 —");
    outtextxy(760, 80, L"— 查询功能 —");

    // ---- 分隔线 ----
    setlinecolor(RGB(200, 210, 230));
    setlinestyle(PS_SOLID, 1);
    line(60, 95, 940, 95);

    // 按钮样式常量
    COLORREF btn_color = RGB(60, 120, 220);
    COLORREF btn_border = RGB(40, 90, 190);
    int bw = 240, bh = 55;
    int x1 = 60, x2 = 380, x3 = 700;
    int y_start = 110, y_gap = 85;

    // ---- 绘制按钮（文字自动居中） ----
    auto drawBtn = [&](int col, int row, const wchar_t* txt) {
        int x = (col == 1) ? x1 : (col == 2) ? x2 : x3;
        int y = y_start + row * y_gap;
        int cx = x + bw / 2;  // 按钮中心X
        int len = (int)wcslen(txt);
        int tx = cx - len * 11; // 20号字体中文字宽约22px，减半为11
        setfillcolor(btn_color);
        setlinecolor(btn_border);
        setlinestyle(PS_SOLID, 2);
        fillroundrect(x, y, x + bw, y + bh, 12, 12);
        settextcolor(WHITE);
        settextstyle(20, 0, L"微软雅黑");
        outtextxy(tx, y + 15, txt);
    };

    // 第1列
    drawBtn(1, 0, L"添加员工");
    drawBtn(1, 1, L"删除员工");
    drawBtn(1, 2, L"查询所有员工");

    // 第2列
    drawBtn(2, 0, L"添加部门");
    drawBtn(2, 1, L"删除部门");
    drawBtn(2, 2, L"查询所有部门");

    // 第3列
    drawBtn(3, 0, L"按工号查询");
    drawBtn(3, 1, L"按部门查询");
    drawBtn(3, 2, L"按姓名查询");

    // 退出按钮（文字居中）
    setfillcolor(RGB(200, 60, 60));
    setlinecolor(RGB(160, 40, 40));
    setlinestyle(PS_SOLID, 2);
    fillroundrect(380, 520, 620, 565, 12, 12);
    settextcolor(WHITE);
    settextstyle(20, 0, L"微软雅黑");
    outtextxy(474, 535, L"退出登录");

    // ---- 鼠标事件 ----
    MOUSEMSG k;
    while (true) {
        k = GetMouseMsg();

        if (k.uMsg == WM_LBUTTONDOWN) {
            // ---- 第1列 ----
            if (k.x >= x1 && k.x <= x1 + bw) {
                if (k.y >= y_start && k.y <= y_start + bh) {
                    add_emp(sock);
                    goto start;
                }
                if (k.y >= y_start + y_gap && k.y <= y_start + y_gap + bh) {
                    del_emp(sock);
                    goto start;
                }
                if (k.y >= y_start + 2 * y_gap && k.y <= y_start + 2 * y_gap + bh) {
                    show_emps(sock);
                    goto start;
                }
            }
            // ---- 第2列 ----
            if (k.x >= x2 && k.x <= x2 + bw) {
                if (k.y >= y_start && k.y <= y_start + bh) {
                    add_dept(sock);
                    goto start;
                }
                if (k.y >= y_start + y_gap && k.y <= y_start + y_gap + bh) {
                    del_dept(sock);
                    goto start;
                }
                if (k.y >= y_start + 2 * y_gap && k.y <= y_start + 2 * y_gap + bh) {
                    show_depts(sock);
                    goto start;
                }
            }
            // ---- 第3列 ----
            if (k.x >= x3 && k.x <= x3 + bw) {
                if (k.y >= y_start && k.y <= y_start + bh) {
                    select_emp_by_id(sock);
                    goto start;
                }
                if (k.y >= y_start + y_gap && k.y <= y_start + y_gap + bh) {
                    select_emp_by_dept(sock);
                    goto start;
                }
                if (k.y >= y_start + 2 * y_gap && k.y <= y_start + 2 * y_gap + bh) {
                    select_emp_by_id(sock);
                    goto start;
                }
            }
            // ---- 退出 ----
            if (k.x >= 380 && k.x <= 620 && k.y >= 520 && k.y <= 565) {
                closegraph();
                return;
            }
        }
    }
}
