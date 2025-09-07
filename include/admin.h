/**
 * @file admin.h
 * @brief 管理员功能接口声明
 * 
 * 包含管理员相关操作函数的声明，如航班管理、订单查看等
 */
#ifndef ADMIN_H
#define ADMIN_H

#include "../include/head.h" 


// 分页显示航班信息
void paginated_display(FlightNode* );

// 管理员功能函数
void see_flight_info();      ///< 查看航班信息
int set_flight_info();       ///< 添加航班信息
int del_flight_info();       ///< 删除航班信息
int change_flight_info();    ///< 修改航班信息
int administrators();        ///< 管理员主菜单
int update_flight_info();    ///< 更新航班信息文件
int flight_report();         ///< 航班统计报表
int order_report();          ///< 订单统计报表

#endif