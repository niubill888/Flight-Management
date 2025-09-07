#include "../include/head.h"

/**
 * @brief 从CSV文件加载航班数据到链表
 *
 * @param filename CSV文件路径
 * @return int 成功返回SUCCESS(0)，失败返回FAILURE(-1)
 */
int load_flights_from_csv(const char *filename)
{
    // 打开CSV文件
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("无法打开初始化文件");
        return FAILURE;
    }

    char line[256];
    // 跳过标题行
    fgets(line, sizeof(line), fp);

    // 创建链表头节点
    List = createHead();
    if (!List)
        return FAILURE;

    // 逐行读取CSV数据
    while (fgets(line, sizeof(line), fp))
    {
        Flight_n flight;
        memset(&flight, 0, sizeof(Flight_n)); // 初始化航班结构体

        // 使用strtok解析CSV行数据
        char *token = strtok(line, ",");
        if (!token)
            continue;
        strncpy(flight.number, token, sizeof(flight.number) - 1); // 航班号

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.airline, token, sizeof(flight.airline) - 1); // 航空公司

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.departure_time, token, sizeof(flight.departure_time) - 1); // 出发时间

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.arrival_time, token, sizeof(flight.arrival_time) - 1); // 到达时间

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.departure_airport, token, sizeof(flight.departure_airport) - 1); // 出发机场

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.arrival_airport, token, sizeof(flight.arrival_airport) - 1); // 到达机场

        token = strtok(NULL, ",");
        if (token)
            strncpy(flight.status, token, sizeof(flight.status) - 1); // 航班状态

        token = strtok(NULL, ",");
        if (token)
            flight.price = atof(token); // 机票价格

        // 将航班插入链表尾部
        tail_insert(List, &flight);
    }

    fclose(fp);
    return SUCCESS;
}

/**
 * @brief 从二进制文件加载航班数据到链表
 *
 * @return int 成功返回0，失败返回-1
 */
int load_flights_from_file()
{
    // 打开二进制文件
    FILE *fp = fopen("data/flights.txt", "rb");
    if (fp == NULL)
    {
        // 文件不存在不算错误（可能是首次运行）
        return FAILURE;
    }

    // 确保链表已初始化
    if (List == NULL)
    {
        List = createHead();
        if (List == NULL)
        {
            fclose(fp);
            return -1;
        }
    }

    // 读取文件内容
    Flight_n flight;
    int count = 0;

    // 逐条读取航班数据
    while (fread(&flight, sizeof(Flight_n), 1, fp) == 1)
    {
        // 将航班添加到链表尾部
        if (tail_insert(List, &flight) != SUCCESS)
        {
            fprintf(stderr, "添加航班数据到链表失败\n");
        }
        count++;
    }

    // 检查文件结束状态
    if (!feof(fp))
    {
        perror("读取航班数据文件时出错");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    // printf("成功加载 %d 条航班数据\n", count);
    return 0;
}

/**
 * @brief 将航班链表数据保存到二进制文件
 *
 * @return int 成功返回0，失败返回-1
 */
int save_flights_to_file()
{
    // 检查链表有效性
    if (List == NULL)
    {
        fprintf(stderr, "错误：航班链表未初始化\n");
        return -1;
    }

    // 打开二进制文件
    FILE *fp = fopen("data/flights.txt", "wb");
    if (fp == NULL)
    {
        perror("无法打开航班数据文件");
        return -1;
    }

    // 遍历链表并写入数据
    FlightNode *current = List->next; // 跳过头节点
    int count = 0;

    while (current != NULL)
    {
        // 写入当前航班数据
        size_t written = fwrite(&current->flight, sizeof(Flight_n), 1, fp);

        if (written != 1)
        {
            perror("写入航班数据失败");
            fclose(fp);
            return -1;
        }

        current = current->next;
        count++;
    }

    fclose(fp);
    printf("成功保存 %d 条航班数据到文件\n", count);
    return 0;
}

/**
 * @brief 初始化航班链表（优先从二进制文件加载，失败则从CSV初始化）
 *
 * @return int 成功返回SUCCESS(0)
 */
int list()
{
    // 尝试加载二进制文件
    if (load_flights_from_file() == SUCCESS)
    {
        return SUCCESS;
    }

    // 尝试从CSV文件初始化
    if (load_flights_from_csv("data/init_flights.csv") == SUCCESS)
    {
        // printf("从CSV文件初始化航班数据\n");
        save_flights_to_file(); // 保存为二进制格式
        return SUCCESS;
    }
    return SUCCESS;
}

/**
 * @brief 创建链表头节点
 *
 * @return FlightNode* 成功返回头节点指针，失败返回NULL
 */
FlightNode *createHead()
{
    FlightNode *head = (FlightNode *)malloc(sizeof(FlightNode));
    if (head == NULL)
    {
        perror("head malloc");
        return NULL;
    }
    memset(head, 0, sizeof(FlightNode)); // 初始化内存
    head->prev = head->next = NULL;      // 设置前后指针
    return head;
}

/**
 * @brief 创建新航班节点
 *
 * @param fn 航班数据指针
 * @return FlightNode* 成功返回节点指针，失败返回NULL
 */
FlightNode *createNode(Flight_n *fn)
{
    FlightNode *node = (FlightNode *)malloc(sizeof(FlightNode));
    if (node == NULL)
    {
        perror("node malloc");
        return NULL;
    }
    memcpy(&node->flight, fn, sizeof(Flight_n)); // 复制航班数据
    node->prev = node->next = NULL;              // 初始化指针
    return node;
}

/**
 * @brief 检查链表是否为空
 *
 * @param h 链表头节点
 * @return int 空链表返回ERR_EMPTY，无效链表返回FAILURE，成功返回SUCCESS
 */
int isnempty(FlightNode *h)
{
    if (h == NULL)
    {
        return FAILURE;
    }
    if (h->next == NULL)
    {
        return ERR_EMPTY;
    }
    return SUCCESS;
}

/**
 * @brief 尾插法插入航班节点
 *
 * @param h 链表头节点
 * @param fn 航班数据指针
 * @return int 成功返回SUCCESS，失败返回FAILURE
 */
int tail_insert(FlightNode *h, Flight_n *fn)
{
    if (h == NULL)
    {
        return FAILURE;
    }
    FlightNode *p = h;
    // 定位到链表尾部
    while (p->next)
    {
        p = p->next;
    }
    // 创建新节点并插入
    FlightNode *node = createNode(fn);
    p->next = node;
    node->prev = p;
    node->next = NULL;
    return SUCCESS;
}

/**
 * @brief 显示所有航班信息
 *
 * @param h 链表头节点
 * @return int 状态码（成功/失败）
 */
int display_all(FlightNode *h)
{
    if (isnempty(h) != SUCCESS)
        return isnempty(h);
    FlightNode *p = h->next;
    // 打印表头
    printf("航班号     航空公司      出发时间    到达时间    出发机场    到达机场    航班状态      机票价格\n");
    // 遍历打印所有航班
    while (p)
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
    }
    return SUCCESS;
}

/**
 * @brief 根据航班号获取节点位置
 *
 * @param h 链表头节点
 * @param number 航班号
 * @return FlightNode* 找到返回节点指针，未找到返回NULL
 */
FlightNode *get_pos(FlightNode *h, char *number)
{
    if (isnempty(h) != SUCCESS)
        return NULL;
    FlightNode *p = h->next;

    // 遍历查找航班号
    while (p)
    {
        if (!strcmp(p->flight.number, number))
        {
            break;
        }
        p = p->next;
    }
    return p;
}

/**
 * @brief 删除指定航班
 *
 * @param h 链表头节点
 * @param number 航班号
 * @return int 状态码
 */
int delete_flight(FlightNode *h, char *number)
{
    if (isnempty(h) != SUCCESS)
        return FAILURE;
    FlightNode *p = get_pos(h, number);
    if (p == NULL)
        return FAILURE;
    // 调整链表指针
    p->prev->next = p->next;
    if (p->next != NULL)
        p->next->prev = p->prev;
    free(p); // 释放节点内存
    p = NULL;
    return SUCCESS;
}

/**
 * @brief 修改航班信息
 *
 * @param h 链表头节点
 * @param number 航班号
 * @param change_n 修改项编号
 * @param change_message 修改内容
 * @return int 状态码
 */
int change_node(FlightNode *h, char *number, char change_n, char *change_message)
{
    if (isnempty(h) != SUCCESS)
        return FAILURE;

    FlightNode *p = get_pos(h, number);
    if (p == NULL)
        return ERR_NOT_FOUND;
    // 根据选项修改不同字段
    switch (change_n)
    {
    case '1': // 航空公司
        strcpy(p->flight.airline, change_message);
        break;
    case '2': // 出发时间
        strcpy(p->flight.departure_time, change_message);
        break;
    case '3': // 到达时间
        strcpy(p->flight.arrival_time, change_message);
        break;
    case '4': // 出发机场
        strcpy(p->flight.departure_airport, change_message);
        break;
    case '5': // 到达机场
        strcpy(p->flight.arrival_airport, change_message);
        break;
    case '6': // 航班状态
        strcpy(p->flight.status, change_message);
        break;
    case '7': // 机票价格
        if (sscanf(change_message, "%lf", &p->flight.price) != 1)
        {
            printf("价格格式错误！\n");
            return ERR_INVALID_INPUT;
        }
        break;
    default:
        printf("输入错误，请重新输入！\n");
    }
    return SUCCESS;
}

/**
 * @brief 根据起降机场搜索航班
 *
 * @param h 链表头节点
 * @param s 出发机场
 * @param e 到达机场
 * @return int 状态码
 */
int search_info(FlightNode *h, char *s, char *e)
{
    if (isnempty(h) != SUCCESS)
        return isnempty(h);
    FlightNode *p = h->next;
    // 创建搜索结果链表
    Searchlist = createHead();
    while (p)
    {
        // 匹配起降机场
        if (!strcmp(p->flight.departure_airport, s) && !strcmp(p->flight.arrival_airport, e))
            tail_insert(Searchlist, &p->flight); // 添加到结果链表
        p = p->next;
    }
    return SUCCESS;
}

/**
 * @brief 比较函数：按出发时间排序
 *
 * @param a 航班A
 * @param b 航班B
 * @return int a>b返回真
 */
int compare_by_departure_time(const Flight_n *a, const Flight_n *b)
{
    return strcmp(a->departure_time, b->departure_time) > 0;
}

/**
 * @brief 比较函数：按价格排序
 *
 * @param a 航班A
 * @param b 航班B
 * @return int a>b返回真
 */
int compare_by_price(const Flight_n *a, const Flight_n *b)
{
    return a->price > b->price;
}

/**
 * @brief 链表排序（冒泡排序）
 *
 * @param h 链表头节点指针的指针
 * @param compare 比较函数指针
 */
void sort_list(FlightNode **h, CompareFunc compare)
{
    if (!*h || !(*h)->next || !(*h)->next->next)
        return; // 空链表或单节点链表

    int swapped;
    FlightNode *p1, *p2, *end = NULL;
    do
    {
        swapped = 0;
        p1 = (*h)->next;

        while (p1->next != end)
        {
            p2 = p1->next;

            // 根据比较函数决定是否交换
            if (compare(&p1->flight, &p2->flight))
            {
                // 交换航班数据
                Flight_n temp = p1->flight;
                p1->flight = p2->flight;
                p2->flight = temp;
                swapped = 1;
            }
            p1 = p1->next;
        }
        end = p1; // 缩小排序范围
    } while (swapped);
}

/**
 * @brief 释放整个链表内存
 *
 * @param h 链表头节点指针的指针
 * @return int 状态码
 */
int free_node(FlightNode **h)
{
    FlightNode *p = (*h);
    while (p)
    {
        FlightNode *temp = p;
        p = p->next;
        free(temp); // 释放当前节点
    }
    *h = NULL; // 头指针置空
    return SUCCESS;
}