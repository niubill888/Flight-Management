/**
 * @file user.h
 * @brief 用户功能接口
 * 
 * 定义普通用户相关操作函数声明
 */
#ifndef USER_H
#define USER_H

/**
 * @brief 用户功能主菜单
 * @return 操作状态码(SUCCESS/FAILURE)
 */
int user_function();

// 用户操作函数
int buy_ticket();               ///< 购买机票
void view_my_orders();          ///< 查看用户订单
int refund_ticket();            ///< 退票操作
int view_balance();             ///< 查看余额
int recharge_balance();         ///< 充值余额
int update_user_balance(double); ///< 更新用户余额
int modify_personal_info();     ///< 修改个人信息

#endif // USER_H