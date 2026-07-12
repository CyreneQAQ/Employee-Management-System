#include "employee.h"

char global_str[70];

int main() {
    MYSQL* sock, mysql;
    mysql_init(&mysql);

    // -------- 配置 MySQL 连接信息（请按你的实际情况修改）--------
    const char* host = "127.0.0.1";
    const char* user = "root";
    const char* pass = "123456";       // MySQL 密码
    const char* db   = "employee_ms";  // 数据库名
    unsigned int port = 3306;
    // ----------------------------------------------------------

    if (!(sock = mysql_real_connect(&mysql, host, user, pass, db, port, 0, 0))) {
        cout << "MySQL 连接失败！" << endl;
        cout << "错误: " << mysql_error(&mysql) << endl;
        system("pause");
        return -1;
    }
    else {
        cout << "MySQL 连接成功！" << endl;
    }

    // 设置编码为 GBK（EasyX 使用 GBK 编码显示中文）
    mysql_query(&mysql, "set names gbk");
    mysql_autocommit(sock, 0);

    // 启动 GUI 主界面
    GUI_menu(sock);

    // 关闭连接
    mysql_close(sock);
    return 0;
}
