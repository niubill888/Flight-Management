/**
 * @file order.h
 * @brief 订单操作接口
 * 
 * 定义订单相关操作函数声明
 */
#ifndef ORDER_H
#define ORDER_H

/**
 * @brief 更新用户订单文件
 * @return 操作状态码(SUCCESS/FAILURE)
 */
int update_user_order();

/**
 * @brief 从文件读取用户订单
 * @return 操作状态码(SUCCESS/FAILURE)
 */
int read_from_order();

#endif // ORDER_H