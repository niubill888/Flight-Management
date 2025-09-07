/**
 * @file head.h
 * @brief 项目主头文件
 * 
 * 包含系统所需的所有标准库头文件和项目自定义头文件，
 * 定义全局常量、错误码和全局变量
 */
#ifndef __HEAD_H__
#define __HEAD_H__

// 标准库头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <time.h> 
#include <dirent.h>
//#include <ctype.h>

// 项目自定义头文件
#include "flight.h"  ///< 航班数据结构
#include "user.h"    ///< 用户功能接口
#include "admin.h"   ///< 管理员功能接口
#include "list.h"    ///< 链表操作接口
#include "order.h"   ///< 订单操作接口

// 系统状态码
#define SUCCESS 0          ///< 操作成功
#define FAILURE -1         ///< 操作失败
#define ERR_INVALID_INPUT -10 ///< 无效输入错误
#define ERR_NOT_FOUND -11  ///< 未找到错误
#define ERR_EXISTS -12     ///< 已存在错误
#define ERR_EMPTY -13      ///< 空数据错误
#define BACK 1             ///< 返回操作
#define EXIT_SYSTEM 2      ///< 退出系统


// 全局变量声明
 User* user;           ///< 当前登录用户
 FlightNode* userorders; ///< 用户订单链表
 FlightNode* List;     ///< 航班主链表
 FlightNode* Searchlist; ///< 搜索结果链表

// 文件路径常量
#define LOG_FILE "log/error.log"       ///< 错误日志文件路径
#define flightS_FILE "data/flights.txt" ///< 航班数据文件路径

#endif // __HEAD_H__