#include "../include/head.h"

/**
 * @brief 用户功能主菜单
 * 
 * 提供用户操作菜单：购买机票、查看订单、查看余额、修改密码、退出登录
 * 
 * @return int 操作状态码(SUCCESS/FAILURE)
 */
int user_function()
{
    // 显示欢迎信息
    printf("        <|欢迎您！%s用户|>\n\n",user->username);
    // 创建用户订单链表头节点
    user->userorders=createHead();
    while(1)
    {
        // 打印用户菜单
        printf("          <|请选择：|>\n"
               ">1.购买机票        >2.查看我的订单\n"
               ">3.查看余额        >4.修改密码\n"
               ">5.退出登陆\n");
        
        char c=getchar();
        while(getchar()!='\n'); // 清空输入缓冲区
        
        switch(c)
        {
            case '1': // 购买机票
                system("clear");
                buy_ticket();
                break;
            case '2': // 查看订单
                system("clear");
                if(SUCCESS==read_from_order()) // 从文件加载订单数据
                    view_my_orders();          // 显示订单界面
                break;
            case '3': // 查看余额
                system("clear");
                view_balance();
                break;
            case '4': // 修改密码
                system("clear");
                modify_personal_info();
                break;
            case '5': // 退出登录
                system("clear");
                free(user->userorders); // 释放订单内存
                printf("退出登陆！\n");
                break;
            default:
                printf("输入有误，请重新输入！\n");
        }
        if(c=='5') // 退出循环
            break;
    }
    return SUCCESS;
}

/**
 * @brief 购买机票功能
 * 
 * 流程：
 * 1. 输入出发地和目的地
 * 2. 查询符合条件的航班
 * 3. 选择航班并检查余额
 * 4. 扣款并更新订单
 * 
 * @return int 操作状态码(SUCCESS/ERR_NOT_FOUND/FAILURE)
 */
int buy_ticket()
{
    char start_port[20],arrival_port[20], f_n[10]; // 出发地/目的地/航班号
    
    // 输入出发地（带格式检查）
    while(1)
    {
        printf("请输入出发地：\n");
        if(1!=scanf("%19s",start_port)){
            printf("输入格式错误！请重新输入：\n");
            continue;
        }
        while(getchar()!='\n'); // 清空缓冲区
        break;
    }
    
    // 输入目的地（带格式检查）
    while(1)
    {
        printf("请输入目的地：\n");
        if(1!=scanf("%19s",arrival_port)){
            printf("输入格式错误！请重新输入：\n");
            continue;
        }
        while(getchar()!='\n');
        break;
    }
    
    system("clear");
    // 搜索符合条件的航班
    search_info(List,start_port,arrival_port);
    
    // 添加排序菜单循环
    while(1) {
        //read_from_order();
        // 显示搜索结果
        if(display_all(Searchlist))
        {   
            printf("暂无符合要求的航班！\n");
            printf("按任意键返回...");
            getchar();
            while (getchar() != '\n'); // 等待用户按键
            system("clear");
            return ERR_NOT_FOUND;
        }
        
        // 添加操作菜单
        printf("\n请选择操作：\n");
        printf(">1.选择航班      >2.按时间排序      >3.按价格排序      >4.重新搜索      >5.返回\n");
        char operation = getchar();
        while(getchar() != '\n');
        
        switch(operation) {
            case '1': 
                    // 选择航班
                    while(1)
                    {
                        printf("请输入要购买机票的航班号：\n");   
                        if(1!=scanf("%9s",f_n)){
                            printf("输入格式错误！\n");
                            continue;
                        }
                        while(getchar()!='\n');
                        // 验证航班是否存在
                        if(!get_pos(Searchlist,f_n)){
                            printf("没有查询到此航班！\n");
                            return FAILURE;
                        }
                        break;
                    }
                    
                    // 获取航班价格
                    double price = get_pos(Searchlist, f_n)->flight.price;
                    
                    // 检查余额
                    if (user->balance < price) 
                    {   
                        system("clear");
                        printf("余额不足，需要%.2f元\n", price);
                        printf("当前余额是：%.2f\n",user->balance);
                    } 
                    else 
                    {
                        // 扣款
                        user->balance -= price;
                        // 更新余额到文件
                        update_user_balance(user->balance);  
                        // 添加订单到链表
                        tail_insert(user->userorders,&get_pos(Searchlist,f_n)->flight);
                        // 更新订单文件
                        if(SUCCESS==update_user_order())
                        {
                            printf("购买成功！\n");
                            // 等待用户按键返回
                            printf("\n按任意键返回...");
                            getchar();
                            while(getchar()!='\n');
                            system("clear");
                        }
                    }
                    return SUCCESS;
                
            case '2': // 按时间排序
                system("clear");
                sort_list(&Searchlist, compare_by_departure_time);
                break;
                
            case '3': // 按价格排序
                system("clear");
                sort_list(&Searchlist, compare_by_price);
                break;
                
            case '4': // 重新搜索
                system("clear");
                return buy_ticket();
                
            case '5': // 返回
                system("clear");
                return SUCCESS;
                
            default:
                system("clear");
                printf("无效选项！\n");
                // 等待用户按键返回
                printf("\n按任意键返回...");
                getchar();
                while(getchar()!='\n');
                system("clear");
        }
    }
}

/**
 * @brief 查看用户订单
 * 
 * 显示所有订单并提供退票功能
 */
void view_my_orders() {
    read_from_order();//只能放在这，范进循环会覆盖排序功能
    while(1) {
        
        system("clear");
        
        // 显示订单
        if(display_all(user->userorders)) {
            printf("没有购票信息！\n");
        }
        
        // 添加操作菜单
        printf("\n请选择操作：\n");
        printf(">1.返回         >2.退票         >3.按时间排序         >4.按价格排序\n");
        char s = getchar();
        while(getchar() != '\n');
        
        switch(s) {
            case '1': // 返回
                system("clear");
                return;
                
            case '2': // 退票
                printf("请输入要退票的航班号：\n");
                char n[10];
                scanf("%9s", n);
                while(getchar() != '\n');
                
                if(delete_flight(user->userorders, n)) {
                    printf("退票失败！\n");
                    printf("\n按任意键返回...");
                    getchar();
                    while(getchar()!='\n');
                    system("clear");
                } else {
                    update_user_order();
                    system("clear");
                    printf("退票成功！\n");
                    printf("\n按任意键返回...");
                    getchar();
                    while(getchar()!='\n');
                    system("clear");
                }
                break;
                
            case '3': // 按时间排序
                system("clear");
                sort_list(&(user->userorders), compare_by_departure_time);
                break;
                
            case '4': // 按价格排序
                system("clear");
                sort_list(&(user->userorders), compare_by_price);
                break;
                
            default:
                printf("无效选项！\n");
        }
    }
}


/**
 * @brief 查看余额功能
 * 
 * 显示当前余额并提供充值选项
 * 
 * @return int 操作状态码(SUCCESS)
 */
int view_balance()
{
    printf("当前余额是：%.2f\n",user->balance);
    char s;
    printf("请选择：\n>1.返回    >2.充值\n");
    s=getchar();
    while(getchar()!='\n'); // 清空缓冲区
    
    switch(s)
    {
        case '1': // 返回
            system("clear");
            break;
        case '2': // 充值
            system("clear");
            recharge_balance();
            break;
    }
    return SUCCESS;
}


/**
 * @brief 余额充值功能
 * 
 * @return int 操作状态码(隐式)
 */
int recharge_balance()
{
    double amount;
    printf("请输入要充值的金额：\n");
    scanf("%lf", &amount);
    while (getchar() != '\n'); // 清空缓冲区
    
    // 更新内存余额
    user->balance+=amount;
    // 更新文件余额
    if(update_user_balance(user->balance))
        return FAILURE;
        // 等待用户按键返回
        printf("\n按任意键返回...");
        getchar();
        while(getchar()!='\n');
        system("clear");
}


/**
 * @brief 更新用户余额到文件
 * 
 * @param b 新的余额值
 * @return int 操作状态码(SUCCESS/FAILURE)
 */
int update_user_balance(double b)
{
    // 以读写模式打开用户文件
    FILE *fp = fopen("data/userinfo.txt", "r+b");
    if (fp == NULL) {
        perror("fopen");
        return FAILURE;
    }
    
    User tempuser;
    // 遍历用户文件
    while(fread(&tempuser,sizeof(User),1,fp))
    {
        // 匹配当前用户
        if(!strcmp(user->username,tempuser.username))
        {  
            // 回退文件指针
            fseek(fp,-sizeof(User),SEEK_CUR);
            // 写入更新后的用户数据
            if(1!=fwrite(user,sizeof(User),1,fp))
            {
                perror("fwrite");
            }
            fclose(fp);
            printf("当前余额是：%.2f\n", b);
            return SUCCESS;
        }
    }
    
    // 未找到用户
    free(user);
    fclose(fp);
    return FAILURE;
}  

/**
 * @brief 修改个人信息（密码）
 * 
 * @return int 操作状态码(SUCCESS/FAILURE)
 */
int modify_personal_info()
{
    char new_password[P];
    printf("请输入新密码：\n");
    get_password(new_password, sizeof(new_password)); // 安全获取密码
    printf("\n");
    
    // 更新内存中的密码
    memset(user->password,0,P*sizeof(char));
    strncpy(user->password,new_password,P-1); // 防止缓冲区溢出
    
    // 更新用户文件
    FILE *fp=fopen("data/userinfo.txt", "r+b");
    if (fp == NULL) {
        perror("fopen");
        return FAILURE;
    }

    User tempuser;
    // 遍历用户文件
    while(fread(&tempuser,sizeof(User),1,fp))
    {
        // 匹配当前用户
        if(!strcmp(user->username,tempuser.username))
        {
            // 回退文件指针
            fseek(fp,-sizeof(User),SEEK_CUR);
            // 写入更新后的用户数据
            if(1!=fwrite(user,sizeof(User),1,fp))
            {
                perror("fwrite");
            }
            fclose(fp);
            printf("密码修改成功！\n");
                // 等待用户按键返回
        printf("\n按任意键返回...");
        getchar();
        while(getchar()!='\n');
        system("clear");
            return SUCCESS;
        }
    }
    
    // 未找到用户
    free(user);
    fclose(fp);
    return FAILURE;
}