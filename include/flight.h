/**
 * @file flight.h
 * @brief 用户和航班数据结构定义
 * 
 * 定义用户数据结构、权限枚举及核心功能函数声明
 */
#ifndef __FLIGHT_H__
#define __FLIGHT_H__

#include "list.h" // 包含链表定义

#define U 20 ///< 用户名最大长度
#define P 20 ///< 密码最大长度

/**
 * @enum permission
 * @brief 用户权限级别枚举
 */
typedef enum permission {
    ADMIN = 0, ///< 管理员权限
    USER  = 1, ///< 普通用户权限
    SVIP  = 2  ///< 超级用户权限
} Permission;

/**
 * @struct usertype
 * @brief 用户信息数据结构
 */
typedef struct usertype {
    char username[U];           ///< 用户名
    char password[P];           ///< 密码
    Permission type;            ///< 权限级别
    double balance;             ///< 账户余额
    struct FlightNode* userorders; ///< 用户订单链表
} User;

// 函数声明
void get_password(char *password, int max_length); ///< 安全获取密码
int init();                      ///< 系统初始化
int log_on(char username[20], char password[16]); ///< 用户登录
int enroll(char username[20], char password[16]); ///< 用户注册
int display();                   ///< 显示主菜单
int sort_info(FlightNode* h);    ///< 航班信息排序
int exit_system();               ///< 安全退出系统

#endif // __FLIGHT_H__