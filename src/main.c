#include "../include/head.h"

/**
 * @brief 航空订票系统主函数
 * 
 * 程序入口，负责初始化系统、显示主界面并处理用户交互循环。
 * 
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return int 程序退出状态码 (0表示正常退出)
 */
int main(int argc, char const *argv[])
{   
    // 初始化航班数据链表
    list();
    
    // 主程序循环
    while(1)
    {   
        // 清空终端屏幕
        system("clear");
        
        // 系统初始化（登录/注册等）
        // 当返回值为退出系统指令时结束循环
        if(EXIT_SYSTEM == init()) break;
        
        // 显示系统主界面
        display();
    }
    
    // 系统退出前的清理工作（如数据保存等）
    exit_system();
    
    return 0;
}