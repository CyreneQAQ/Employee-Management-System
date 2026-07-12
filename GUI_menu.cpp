#include "employee.h"

// LPWSTR -> LPSTR 转换
char* ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn) {
    LPSTR pszOut = NULL;
    if (lpwszStrIn != NULL) {
        int nInputStrLen = wcslen(lpwszStrIn);
        int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
        pszOut = new char[nOutputStrLen];
        if (pszOut != NULL) {
            memset(pszOut, 0x00, nOutputStrLen);
            WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
        }
    }
    return pszOut;
}

// 等待用户按键或点击
void wait_for_key() {
    flushmessage();
    MOUSEMSG m;
    while (true) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN || m.uMsg == WM_KEYDOWN)
            break;
    }
}

// 绘制圆角按钮（带外边框）
void draw_button(int x1, int y1, int x2, int y2, int radius, COLORREF fill, COLORREF border, const wchar_t* text, int tx, int ty) {
    setfillcolor(fill);
    setlinecolor(border);
    setlinestyle(PS_SOLID, 2);
    fillroundrect(x1, y1, x2, y2, radius, radius);
    settextcolor(WHITE);
    outtextxy(tx, ty, text);
}

// 检查鼠标是否在按钮区域内
bool is_hover(int x, int y, int x1, int y1, int x2, int y2) {
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

void GUI_menu(MYSQL* sock) {
start:
    initgraph(900, 520);

    setbkcolor(RGB(240, 245, 255));
    cleardevice();
    setbkmode(TRANSPARENT);

    // ---- 顶部横幅 ----
    setfillcolor(RGB(30, 80, 160));
    setlinecolor(RGB(20, 60, 130));
    fillrectangle(0, 0, 900, 80);
    setfillcolor(RGB(25, 70, 145));
    fillrectangle(0, 75, 900, 80);

    // 标题文字
    settextcolor(WHITE);
    settextstyle(36, 0, L"微软雅黑");
    settextcolor(RGB(255, 255, 255));
    outtextxy(320, 20, L"员工管理系统");

    // ---- 副标题装饰线 ----
    setlinecolor(RGB(180, 200, 230));
    setlinestyle(PS_DOT, 1);
    line(100, 100, 800, 100);

    // ---- 按钮区域 ----
    settextstyle(24, 0, L"微软雅黑");

    // 管理员登录按钮
    COLORREF btn_color = RGB(60, 120, 220);
    COLORREF btn_border = RGB(40, 90, 190);
    draw_button(250, 130, 650, 200, 20, btn_color, btn_border, L"管理员登录", 340, 155);

    // 员工登录按钮
    draw_button(250, 230, 650, 300, 20, btn_color, btn_border, L"员工登录", 355, 255);

    // 退出按钮
    draw_button(250, 330, 650, 400, 20, RGB(200, 60, 60), RGB(160, 40, 40), L"退出系统", 355, 355);

    // ---- 底部信息 ----
    settextcolor(RGB(150, 160, 180));
    settextstyle(14, 0, L"宋体");
    outtextxy(320, 460, L"数据库课程设计 · C++ + EasyX + MySQL");

    // ---- 鼠标事件响应（含悬停效果） ----
    MOUSEMSG k;
    int hover_btn = 0; // 0=none, 1=admin, 2=employee, 3=exit
    while (true) {
        k = GetMouseMsg();

        int new_hover = 0;
        if (is_hover(k.x, k.y, 250, 130, 650, 200)) new_hover = 1;
        else if (is_hover(k.x, k.y, 250, 230, 650, 300)) new_hover = 2;
        else if (is_hover(k.x, k.y, 250, 330, 650, 400)) new_hover = 3;

        // 悬停变色效果
        if (new_hover != hover_btn) {
            hover_btn = new_hover;
            // 重绘所有按钮
            btn_color = RGB(60, 120, 220);
            btn_border = RGB(40, 90, 190);

            // 管理员登录
            if (hover_btn == 1) draw_button(250, 130, 650, 200, 20, RGB(80, 150, 240), RGB(60, 120, 210), L"管理员登录", 340, 155);
            else draw_button(250, 130, 650, 200, 20, btn_color, btn_border, L"管理员登录", 340, 155);

            // 员工登录
            if (hover_btn == 2) draw_button(250, 230, 650, 300, 20, RGB(80, 150, 240), RGB(60, 120, 210), L"员工登录", 355, 255);
            else draw_button(250, 230, 650, 300, 20, btn_color, btn_border, L"员工登录", 355, 255);

            // 退出
            if (hover_btn == 3) draw_button(250, 330, 650, 400, 20, RGB(230, 80, 80), RGB(190, 55, 55), L"退出系统", 355, 355);
            else draw_button(250, 330, 650, 400, 20, RGB(200, 60, 60), RGB(160, 40, 40), L"退出系统", 355, 355);

            // 恢复底部信息
            settextcolor(RGB(150, 160, 180));
            settextstyle(14, 0, L"宋体");
            outtextxy(320, 460, L"数据库课程设计 · C++ + EasyX + MySQL");
        }

        // 点击事件
        if (k.uMsg == WM_LBUTTONDOWN) {
            if (is_hover(k.x, k.y, 250, 130, 650, 200)) {
                GUI_admin_login(sock);
                goto start;
            }
            if (is_hover(k.x, k.y, 250, 230, 650, 300)) {
                GUI_emp_login(sock);
                goto start;
            }
            if (is_hover(k.x, k.y, 250, 330, 650, 400)) {
                closegraph();
                return;
            }
        }
    }
}
