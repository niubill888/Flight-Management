/**
 * @file list.h
 * @brief 航班链表操作接口
 * 
 * 定义航班数据结构及链表操作函数
 */
#ifndef __LIST_H__
#define __LIST_H__

/**
 * @struct flight_n
 * @brief 航班信息数据结构
 */
typedef struct flight_n {
    char number[10];           ///< 航班号
    char airline[20];          ///< 航空公司
    char departure_time[10];   ///< 出发时间
    char arrival_time[10];     ///< 到达时间
    char departure_airport[10];///< 出发机场
    char arrival_airport[10];  ///< 到达机场
    char status[10];           ///< 航班状态
    double price;              ///< 机票价格
} Flight_n;

/**
 * @struct FlightNode
 * @brief 航班链表节点
 */
typedef struct FlightNode {
    Flight_n flight;           ///< 航班数据
    struct FlightNode* prev;   ///< 前驱节点指针
    struct FlightNode* next;   ///< 后继节点指针
} FlightNode;

// 比较函数指针类型
typedef int (*CompareFunc)(const Flight_n*, const Flight_n*);

// 比较函数声明
int compare_by_departure_time(const Flight_n*, const Flight_n*); ///< 按出发时间比较
int compare_by_price(const Flight_n*, const Flight_n*);          ///< 按价格比较

// 链表操作函数声明
int load_flights_from_csv(const char* filename); ///< 从CSV加载航班数据
int load_flights_from_file();  ///< 从文件加载航班数据
int save_flights_to_file();    ///< 保存航班数据到文件
int list();                    ///< 链表初始化
FlightNode* createHead();      ///< 创建链表头节点
FlightNode* createNode(Flight_n*); ///< 创建新节点
int isnempty(FlightNode*);     ///< 检查链表是否为空
int tail_insert(FlightNode*, Flight_n*); ///< 尾插法插入节点
int display_all(FlightNode* h); ///< 显示所有航班信息
FlightNode* get_pos(FlightNode* h, char* number); ///< 按航班号查找节点
int delete_flight(FlightNode* h, char* number); ///< 删除航班节点
int change_node(FlightNode* h, char* number, char change_n, char* change_message); ///< 修改节点信息
int search_info(FlightNode* h, char* s, char* e); ///< 搜索航班信息
void sort_list(FlightNode** h, CompareFunc compare); ///< 链表排序
void paginated_display(FlightNode* );// 分页显示航班信息
int free_node(FlightNode** h); ///< 释放链表内存

#endif // __LIST_H__