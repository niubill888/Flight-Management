#include "../include/head.h"
#include "../include/admin.h"

/**
 * @brief 分页显示航班信息
 * @param list 航班链表头节点指针
 *
 * 该函数实现航班信息的分页显示功能，每页显示11条记录。
 * 用户可以通过输入'N'查看下一页，'P'查看上一页，'Q'退出分页浏览。
 */
void paginated_display(FlightNode *list)
{
    const int PAGE_SIZE = 11;   // 每页显示的记录数
    int page = 0, total = 0;    // 当前页码和总记录数
    FlightNode *p = list->next; // 遍历指针

    // 计算总航班数
    while (p)
    {
        total++;
        p = p->next;
    }

    // 分页显示循环
    while (1)
    {
        system("clear"); // 清屏

        // 显示当前页数据
        p = list->next;
        int start = page * PAGE_SIZE; // 当前页起始索引
        int count = 0;                // 当前页已显示记录数

        // 跳过前几页的数据
        for (int i = 0; i < start && p; i++, p = p->next)
            ;

        // 打印表头
        printf("航班号     航空公司      出发时间    到达时间    出发机场    到达机场    航班状态      机票价格\n");

        // 打印当前页的数据
        while (p && count < PAGE_SIZE)
        {
            printf("%-11s%-19s%-12s%-12s%-14s%-14s%-15s%-.2f\n",
                   p->flight.number,
                   p->flight.airline,
                   p->flight.departure_time,
                   p->flight.arrival_time,
                   p->flight.departure_airport,
                   p->flight.arrival_airport,
                   p->flight.status,
                   p->flight.price);
            p = p->next;
            count++;
        }

        // 显示分页信息
        printf("\n页码: %d/%d", page + 1, (total + PAGE_SIZE - 1) / PAGE_SIZE);
        printf("\n操作: (N)下一页 (P)上一页 (Q)退出\n");

        // 获取用户输入
        char input = getchar();
        while (getchar() != '\n')
            ; // 清空输入缓冲区

        // 处理用户操作
        switch (tolower(input))
        {
        case 'n':
            if ((page + 1) * PAGE_SIZE < total)
                page++;
            break; // 下一页
        case 'p':
            if (page > 0)
                page--;
            break; // 上一页
        case 'q':
            return; // 退出
        }
    }
}

/**
 * @brief 查看航班信息
 *
 * 该函数提供航班信息的查看功能，包括排序和分页显示。
 * 如果航班链表为空，则提示用户没有航班信息。
 */
void see_flight_info()
{
    system("clear");
    while (1)
    {
        // 检查航班链表是否为空
        if (display_all(List) == ERR_EMPTY)
        {
            printf("查询不到航班信息！\n按任意键后回车返回\n");
            getchar(); // 等待用户按键
            while (getchar() != '\n')
                ; // 清除输入缓冲区
            break;
        }

        // 排序航班信息
        if (sort_info(List))
            break;
        system("clear");

        // 分页显示航班信息
        paginated_display(List);
        system("clear");
    }
    return;
}

/**
 * @brief 添加航班信息
 * @return 操作结果（成功/失败）
 *
 * 该函数允许管理员添加新的航班信息。
 * 用户需要按指定格式输入航班信息，系统会验证输入的有效性和航班号是否已存在。
 */
int set_flight_info()
{
    system("clear");
    printf("请按以下顺序输入：\n航班号 航空公司 出发时间 到达时间 出发机场 到达机场 航班状态 机票价格\n");
    Flight_n *new_f_info = NULL; // 新航班信息指针
    char buffer[100];            // 输入缓冲区

    // 获取用户输入
    if (!fgets(buffer, sizeof(buffer), stdin))
    {
        perror("fgets");
        return FAILURE;
    }

    // 分配内存并解析输入
    new_f_info = (Flight_n *)malloc(sizeof(Flight_n));
    if (sscanf(buffer, "%s %s %s %s %s %s %s %lf",
               new_f_info->number,
               new_f_info->airline,
               new_f_info->departure_time,
               new_f_info->arrival_time,
               new_f_info->departure_airport,
               new_f_info->arrival_airport,
               new_f_info->status,
               &new_f_info->price) != 8)
    {
        free(new_f_info);
        printf("输入格式错误\n");
        printf("请选择：\n>1.返回\t   >2.重新添加\n");
        char n = getchar();
        while (getchar() != '\n')
            ;
        switch (n)
        {
        case '1':
            system("clear");
            break;
        case '2':
            system("clear");
            set_flight_info(); // 递归调用重新添加
            break;
        }
        return ERR_INVALID_INPUT;
    }

    // 检查航班号是否已存在
    if (get_pos(List, new_f_info->number) != NULL)
    {
        printf("航班号已存在！\n");
        free(new_f_info);
        printf("请选择：\n>1.返回\t  >2.继续添加\n");
        char n = getchar();
        while (getchar() != '\n')
            ;
        switch (n)
        {
        case '1':
            break;
        case '2':
            system("clear");
            set_flight_info(); // 递归调用重新添加
            break;
        }
        return ERR_EXISTS;
    }

    // 将新航班添加到链表尾部
    if (0 == tail_insert(List, new_f_info))
    {
        // 更新航班信息文件
        if (update_flight_info())
        {
            printf("更新航班信息到文件失败\n");
            return FAILURE;
        };

        printf("添加成功！\n");
        free(new_f_info); // 释放内存

        printf("请选择：\n>1.返回\t  >2.继续添加\n");
        char n = getchar();
        while (getchar() != '\n')
            ;
        switch (n)
        {
        case '1':
            break;
        case '2':
            system("clear");
            set_flight_info(); // 递归调用继续添加
            break;
        }
        return SUCCESS;
    }
    else
    {
        printf("添加失败\n");
        free(new_f_info); // 释放内存避免泄漏
        return FAILURE;
    }
}

/**
 * @brief 删除航班信息
 * @return 操作结果（成功/失败）
 *
 * 该函数允许管理员根据航班号删除航班信息。
 * 删除后会更新航班信息文件。
 */
int del_flight_info()
{
    system("clear");
    char n[10]; // 航班号缓冲区
    printf("请输入要删除的航班的航班号：\n");
    scanf("%s", n);
    while ((getchar()) != '\n')
        ; // 清空输入缓冲区

    // 检查航班是否存在并删除
    if (get_pos(List, n) == NULL || delete_flight(List, n))
        printf("删除失败！\n");
    else
    {
        // 更新航班信息文件
        if (update_flight_info())
        {
            printf("更新航班信息到文件失败\n");
            return FAILURE;
        };
        printf("删除成功！\n");
    }

    // 提供继续操作选项
    printf("请选择：\n>1.返回\t  >2.继续删除\n");
    char nn = getchar();
    while (getchar() != '\n')
        ;
    switch (nn)
    {
    case '1':
        system("clear");
        break;
    case '2':
        system("clear");
        del_flight_info(); // 递归调用继续删除
        break;
    }
    return SUCCESS;
}

/**
 * @brief 修改航班信息
 * @return 操作结果（成功/失败）
 *
 * 该函数允许管理员修改指定航班的特定信息。
 * 用户需要选择要修改的字段并输入新值。
 */
int change_flight_info()
{
    system("clear");
    char n[10];       // 航班号缓冲区
    char m;           // 菜单选项
    char message[20]; // 新值缓冲区
    paginated_display(List);
    printf(" 请输入要改变的航班号： ");
    scanf("%s", n);
    while (getchar() != '\n')
        ; // 清空输入缓冲区

    // 检查航班是否存在
    if (!get_pos(List, n))
    {
        return ERR_NOT_FOUND;
    }
    // 显示修改菜单
    printf(">1.航空公司      >2.出发时间\n"
           ">3.到达时间      >4.出发机场\n"
           ">5.到达机场      >6.航班状态\n"
           ">7.机票价格\n");
    printf(" 请输入要修改的选项： ");

    m = getchar();
    while (getchar() != '\n')
        ; // 清空输入缓冲区

    // 验证菜单选项有效性
    while (m != '1' && m != '2' && m != '3' && m != '4' && m != '5' && m != '6' && m != '7')
    {
        printf(" 没有此选项，请重新输入： ");
        m = getchar();
        while (getchar() != '\n')
            ;
    }

    printf(" 请输入修改的内容： ");
    scanf("%19s", message);
    while (getchar() != '\n')
        ; // 清空输入缓冲区

    // 执行修改操作
    if (SUCCESS != change_node(List, n, m, message))
    {
        printf("修改失败！\n");
        return FAILURE;
    }

    // 更新航班信息文件
    if (update_flight_info())
    {
        printf("更新航班信息到文件失败\n");
        return FAILURE;
    };
    printf("修改成功！\n");

    return SUCCESS;
}

/**
 * @brief 管理员主菜单
 * @return 操作结果（成功/失败）
 *
 * 该函数提供管理员操作的主菜单界面。
 * 管理员可以执行查看、添加、删除、修改航班等操作。
 */
int administrators()
{
    // 显示欢迎信息
    printf("             <|你好，%s管理员|>\n", user->username);

    while (1)
    {
        // 显示管理员菜单
        printf("\n"
               ">1.查看航班      >2.增加航班       >3.删除航班\n"
               ">4.修改航班信息  >5.航班报表       >6.订单报表\n"
               ">7.修改密码      >8.退出登陆\n\n"
               " 请选择： ");

        char c = getchar();
        while ((getchar()) != '\n')
            ; // 清空输入缓冲区

        // 处理用户选择
        switch (c)
        {
        case '1':
            see_flight_info();
            break; // 查看航班
        case '2':
            set_flight_info();
            break; // 添加航班
        case '3':
            del_flight_info();
            break; // 删除航班
        case '4':
            if (ERR_NOT_FOUND == change_flight_info())
            {
                printf("没有此航班！\n");
                // 等待用户按键返回
                printf("\n按任意键返回...");
                getchar();
                while (getchar() != '\n')
                    ;
                system("clear");
            }
            break; // 修改航班
        case '5':
            flight_report();
            break; // 航班报表
        case '6':
            order_report();
            break; // 订单报表
        case '7':
            modify_personal_info();
            break; // 修改密码
        case '8':
            system("clear");
            printf("退出登陆！\n");
            break; // 退出登录
        default:
            printf("输入有误，请重新输入！\n");
        }

        // 退出循环
        if (c == '8')
            break;
    }
    return SUCCESS;
}

/**
 * @brief 更新航班信息到文件
 * @return 操作结果（成功/失败）
 *
 * 该函数将航班链表中的数据写入文件保存。
 */
int update_flight_info()
{
    // 打开航班信息文件
    FILE *fp = fopen("data/flights.txt", "w");
    if (fp == NULL)
    {
        perror("fopen");
        return FAILURE;
    }

    // 检查链表是否为空
    if (isnempty(List) != SUCCESS)
        return isnempty(List);

    // 遍历链表并写入文件
    FlightNode *p = List->next;
    if (fwrite(&p->flight, sizeof(Flight_n), 1, fp) != 1)
    {
        perror("fwrite");
        fclose(fp);
        return FAILURE;
    }

    p = p->next;
    while (p != NULL && fwrite(&p->flight, sizeof(Flight_n), 1, fp))
    {
        p = p->next;
    }

    fclose(fp);
    return SUCCESS;
}

/**
 * @brief 生成航班报表
 * @return 操作结果（成功/失败）
 *
 * 该函数生成航班统计报表，包括航班状态分布和价格分析，
 * 并将报表保存到文件中。
 */
int flight_report()
{
    system("clear");
    printf("============ 航班报表 ============\n");

    // 航班统计变量
    int total_flights = 0;     // 总航班数
    int active_flights = 0;    // 正常航班数
    int delayed_flights = 0;   // 延误航班数
    int cancelled_flights = 0; // 取消航班数

    // 遍历航班链表进行统计
    FlightNode *p = List->next;
    while (p)
    {
        total_flights++;
        if (strcmp(p->flight.status, "准点") == 0)
            active_flights++;
        else if (strcmp(p->flight.status, "延误") == 0)
            delayed_flights++;
        else if (strcmp(p->flight.status, "取消") == 0)
            cancelled_flights++;
        p = p->next;
    }

    // 显示航班统计信息
    printf("\n航班统计:\n");
    printf("总航班数: %d\n", total_flights);
    printf("正常航班: %d (%.1f%%)\n", active_flights, total_flights ? (float)active_flights / total_flights * 100 : 0);
    printf("延误航班: %d (%.1f%%)\n", delayed_flights, total_flights ? (float)delayed_flights / total_flights * 100 : 0);
    printf("取消航班: %d (%.1f%%)\n", cancelled_flights, total_flights ? (float)cancelled_flights / total_flights * 100 : 0);

    // 价格分析
    double min_price = 99999, max_price = 0, avg_price = 0;
    p = List->next;
    while (p)
    {
        if (p->flight.price < min_price)
            min_price = p->flight.price;
        if (p->flight.price > max_price)
            max_price = p->flight.price;
        avg_price += p->flight.price;
        p = p->next;
    }

    if (total_flights)
        avg_price /= total_flights;

    // 显示价格分析
    printf("\n价格分析:\n");
    printf("最低票价: ¥%.2f\n", min_price);
    printf("最高票价: ¥%.2f\n", max_price);
    printf("平均票价: ¥%.2f\n", avg_price);

    // 生成报表文件名（含日期）
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char report_filename[100];
    sprintf(report_filename, "data/reports/flight_report_%04d%02d%02d.txt",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    // 写入报表文件
    FILE *report_fp = fopen(report_filename, "w");
    if (report_fp)
    {
        fprintf(report_fp, "航班报表 - %04d-%02d-%02d\n\n",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
        fprintf(report_fp, "总航班数: %d\n", total_flights);
        fprintf(report_fp, "正常航班: %d\n", active_flights);
        fprintf(report_fp, "延误航班: %d\n", delayed_flights);
        fprintf(report_fp, "取消航班: %d\n", cancelled_flights);
        fprintf(report_fp, "\n最低票价: %.2f\n", min_price);
        fprintf(report_fp, "最高票价: %.2f\n", max_price);
        fprintf(report_fp, "平均票价: %.2f\n", avg_price);
        fclose(report_fp);
        printf("\n报表已保存至: %s\n", report_filename);
    }
    else
    {
        perror("保存报表失败");
    }

    // 等待用户按键返回
    printf("\n按任意键返回...");
    getchar();
    while (getchar() != '\n')
        ;
    system("clear");
    return SUCCESS;
}

/**
 * @brief 生成订单报表
 * @return 操作结果（成功/失败）
 *
 * 该函数统计所有用户的订单信息，包括订单数和消费金额，
 * 并将报表保存到文件中。
 */
int order_report()
{
    system("clear");
    printf("============ 订单报表 ============\n");

    // 创建必要的目录
    system("mkdir -p data/order");
    system("mkdir -p data/reports");

    DIR *dir;                   // 目录指针
    struct dirent *entry;       // 目录条目
    int total_orders = 0;       // 总订单数
    double total_revenue = 0.0; // 总收入

    // 打开订单目录
    if ((dir = opendir("data/order")) != NULL)
    {
        printf("\n%-15s %-10s %-8s\n", "用户名", "订单数", "总消费");
        printf("--------------------------------\n");

        // 遍历目录中的文件
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            { // 只处理普通文件
                char filename[256];
                snprintf(filename, sizeof(filename), "data/order/%s", entry->d_name);

                // 打开用户订单文件
                FILE *fp = fopen(filename, "r");
                if (fp)
                {
                    // 从文件名提取用户名（移除.txt后缀）
                    char username[U];
                    strncpy(username, entry->d_name, strlen(entry->d_name) - 4);
                    username[strlen(entry->d_name) - 4] = '\0';

                    int user_orders = 0;       // 用户订单数
                    double user_revenue = 0.0; // 用户消费金额
                    Flight_n flight;

                    // 读取文件中的航班信息
                    while (fread(&flight, sizeof(Flight_n), 1, fp) == 1)
                    {
                        user_orders++;
                        user_revenue += flight.price;
                    }

                    // 累加到总计
                    total_orders += user_orders;
                    total_revenue += user_revenue;

                    // 显示用户订单信息
                    printf("%-15s %-10d ¥%-8.2f\n", username, user_orders, user_revenue);
                    fclose(fp);
                }
            }
        }
        closedir(dir);

        // 显示总计信息
        printf("--------------------------------\n");
        printf("%-15s %-10d ¥%-8.2f\n", "总计", total_orders, total_revenue);

        // 生成报表文件名（含日期）
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char report_filename[100];
        sprintf(report_filename, "data/reports/order_report_%04d%02d%02d.txt",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

        // 写入报表文件
        FILE *report_fp = fopen(report_filename, "w");
        if (report_fp)
        {
            fprintf(report_fp, "订单报表 - %04d-%02d-%02d\n\n",
                    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
            fprintf(report_fp, "总订单数: %d\n", total_orders);
            fprintf(report_fp, "总收入: ¥%.2f\n", total_revenue);
            fclose(report_fp);
            printf("\n报表已保存至: %s\n", report_filename);
        }
        else
        {
            perror("保存报表失败");
        }
    }
    else
    {
        perror("无法打开订单目录");
    }

    // 等待用户按键返回
    printf("\n按任意键返回...");
    getchar();
    while (getchar() != '\n')
        ;
    system("clear");
    return SUCCESS;
}