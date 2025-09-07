#include "../include/head.h"
/**
 * @brief 更新用户订单信息到文件
 * 
 * 将用户订单链表中的航班数据写入到对应用户名的订单文件中。
 * 文件路径格式：data/order/{用户名}.txt
 * 
 * @return int 执行结果：
 *             SUCCESS(0) - 更新成功 
 *             FAILURE(-1) - 文件操作失败
 */
int update_user_order()
{
    // 构建订单文件名
    char filename[50];
    sprintf(filename,"data/order/%s.txt",user->username);
    
    FILE* fp;
    fp=fopen(filename,"w");  // 以写入模式打开文件
    if(fp==NULL)
    {    
        perror("fopen");    // 输出文件打开错误信息
        printf("错误代码：%d\n",errno);
        fclose(fp);
        return FAILURE;
    }
    
    // 遍历用户订单链表
    FlightNode* p=user->userorders->next;
    while(p)
    {
        // 将航班数据写入文件
        if(1!=fwrite(&p->flight,sizeof(Flight_n),1,fp)){
            printf("fwrite error\n");
            fclose(fp);
            return FAILURE;
        }
        p=p->next;
    }
    
    fclose(fp);  // 关闭文件
    
    // 释放用户订单链表内存（注意：后续操作不可再访问该链表）
    //free(user->userorders);
    
    return SUCCESS;
}

/**
 * @brief 从文件读取用户订单信息
 * 
 * 从用户订单文件中读取航班数据，构建用户订单链表。
 * 文件路径格式：data/order/{用户名}.txt
 * 
 * @return int 执行结果：
 *             SUCCESS(0) - 读取成功
 *             FAILURE(-1) - 文件操作失败
 */
int read_from_order()
{
    // 创建空的订单链表头节点
    user->userorders=createHead();
    
    // 构建订单文件名
    char filename[50];
    sprintf(filename,"data/order/%s.txt",user->username);
    
    FILE* fp;
    fp=fopen(filename,"r");  // 以读取模式打开文件
    if(fp==NULL)
    {    
        // 处理文件不存在的情况（错误码2表示文件不存在）
        if(errno==2) return SUCCESS;  // 文件不存在视为空订单
        fclose(fp);
        return FAILURE;
    }
    
    Flight_n flight;  // 临时存储读取的航班数据
    
    // 循环读取文件中的航班数据
    while(1==fread(&flight,sizeof(Flight_n),1,fp))
    {
        // 将读取的航班插入链表尾部
        tail_insert(user->userorders,&flight);
    }
    
    // 检查是否正常到达文件末尾
    if(feof(fp)!=1)
    {
        perror("fread");  // 输出文件读取错误
        fclose(fp);
        return FAILURE;
    }
    
    fclose(fp);  // 关闭文件
    
    return SUCCESS;
}