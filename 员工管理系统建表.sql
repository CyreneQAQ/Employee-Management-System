-- ========================================
-- 员工管理系统 - 数据库建表脚本
-- 数据库名: employee_ms
-- ========================================

CREATE DATABASE IF NOT EXISTS employee_ms DEFAULT CHARACTER SET utf8mb4;
USE employee_ms;

-- ------------------------
-- 管理员表
-- ------------------------
CREATE TABLE IF NOT EXISTS admin (
    admin_id   VARCHAR(50) PRIMARY KEY,
    admin_pass VARCHAR(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ------------------------
-- 部门表
-- ------------------------
CREATE TABLE IF NOT EXISTS department (
    dept_id   VARCHAR(50) PRIMARY KEY,
    dept_name VARCHAR(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ------------------------
-- 员工表
-- ------------------------
CREATE TABLE IF NOT EXISTS employee (
    emp_id     VARCHAR(50)  PRIMARY KEY,
    emp_name   VARCHAR(100) NOT NULL,
    emp_dept   VARCHAR(50)  NOT NULL,
    emp_position VARCHAR(100) DEFAULT NULL,
    emp_phone  VARCHAR(20)  DEFAULT NULL,
    emp_salary DECIMAL(10,2) DEFAULT 0.00,
    FOREIGN KEY (emp_dept) REFERENCES department(dept_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ========================================
-- 性能优化：添加索引
-- ========================================

-- 员工姓名索引（加速 LIKE 模糊查询）
CREATE INDEX idx_employee_name ON employee(emp_name);

-- 部门外键索引（加速 JOIN 连接查询）
CREATE INDEX idx_employee_dept ON employee(emp_dept);

-- 部门名称索引（加速部门查询）
CREATE INDEX idx_department_name ON department(dept_name);

-- 管理员密码索引（加速登录验证）
CREATE INDEX idx_admin_pass ON admin(admin_pass);

-- ========================================
-- 插入初始测试数据
-- ========================================

-- 默认管理员: admin / 123456 (MD5加密后存入)
INSERT INTO admin(admin_id, admin_pass) VALUES('admin', MD5('123456'));

-- 默认部门
INSERT INTO department(dept_id, dept_name) VALUES('D001', '技术部');
INSERT INTO department(dept_id, dept_name) VALUES('D002', '人事部');
INSERT INTO department(dept_id, dept_name) VALUES('D003', '财务部');

-- 默认员工
INSERT INTO employee(emp_id, emp_name, emp_dept, emp_position, emp_phone, emp_salary)
VALUES('E001', '张三', 'D001', '高级工程师', '13800000001', 15000.00);
INSERT INTO employee(emp_id, emp_name, emp_dept, emp_position, emp_phone, emp_salary)
VALUES('E002', '李四', 'D002', '人事经理',   '13800000002', 12000.00);
INSERT INTO employee(emp_id, emp_name, emp_dept, emp_position, emp_phone, emp_salary)
VALUES('E003', '王五', 'D003', '财务主管',   '13800000003', 13000.00);

-- 员工默认密码为 123456
-- (这里再建一张简单的员工登录表，与图书管理系统的读者表类似)
-- 实际你可以将员工账号密码存在 admin 表或单独的表，这里仿照原项目：
CREATE TABLE IF NOT EXISTS emp_account (
    emp_id   VARCHAR(50) PRIMARY KEY,
    emp_pass VARCHAR(100) NOT NULL,
    FOREIGN KEY (emp_id) REFERENCES employee(emp_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO emp_account(emp_id, emp_pass) VALUES('E001', MD5('123456'));
INSERT INTO emp_account(emp_id, emp_pass) VALUES('E002', MD5('123456'));
INSERT INTO emp_account(emp_id, emp_pass) VALUES('E003', MD5('123456'));

SELECT * FROM admin;
SELECT * FROM department;
SELECT * FROM employee;
SELECT * FROM emp_account;
