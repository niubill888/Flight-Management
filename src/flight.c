#include "../include/head.h" // 包含自定义头文件

/**
 * @brief 安全获取密码输入（隐藏输入字符）
 * @param password 用于存储密码的字符数组指针
 * @param max_length 密码最大长度限制
 */
void get_password(char *password, int max_length)
{
    struct termios old, new; // 终端属性结构体
    int i = 0;               // 密码字符索引
    char ch;                 // 当前输入的字符

    // 获取当前终端属性并修改
    tcgetattr(STDIN_FILENO, &old); // 获取当前设置
    new = old;
    new.c_lflag &= ~ECHO;                   // 关闭回显功能
    tcsetattr(STDIN_FILENO, TCSANOW, &new); // 立即应用新设置

    // 逐字符读取直到回车
    while ((ch = getchar()) != '\n')
    {
        if (ch == '\b' && i > 0)
        { // 处理退格键
            i--;
        }
        else if (i < max_length - 1)
        {                       // 检查长度限制
            password[i++] = ch; // 存储合法字符
        }
    }
    password[i] = '\0'; // 添加字符串结束符

    // 恢复原始终端属性
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

/**
 * @brief 系统初始化函数（创建初始用户/处理登录注册）
 * @return 系统状态码：EXIT_SYSTEM/SUCCESS/FAILURE
 */
int init()
{
    FILE *fp;

    // 检查用户数据文件是否存在，不存在则创建
    fp = fopen("data/userinfo.txt", "r");
    if (fp == NULL)
    {
        // 创建文件并初始化管理员账户
        fp = fopen("data/userinfo.txt", "w+");
        User *admin = (User *)malloc(sizeof(User));
        if (admin == NULL)
        {
            perror("malloc");
            fclose(fp);
            return FAILURE;
        }
        strcpy(admin->username, "admin"); // 默认管理员账号
        strcpy(admin->password, "123");   // 默认密码
        admin->type = 0;                  // 管理员类型标识
        if (1 != fwrite(admin, sizeof(User), 1, fp))
            perror("fwrite");
        fclose(fp);
    }

    int r = 1; // 循环控制标志
    while (r)
    {
        // 系统欢迎界面
        printf("<|Welcome to Flight Management System|>\n\n");
        printf("              <|请选择|>\n"
               "      >1.注册             >2.登录\n\n"
               "              0.退出系统\n");

        char in = getchar(); // 获取用户选择
        // 输入验证：只接受1/2/0
        while (in != '1' && in != '2' && in != '0')
        {
            printf("请按提示输入正确的选项：\n");
            in = getchar();
            while (getchar() != '\n')
                ; // 清空输入缓冲区
        }
        system("clear"); // 清屏

        // 退出系统处理
        if (in == '0')
        {
            if (update_flight_info())
            { // 保存航班数据
                printf("更新航班信息到文件失败\n");
                return FAILURE;
            };
            return EXIT_SYSTEM; // 返回退出系统状态码
        }

        // 获取用户名
        printf("请输入用户名：\n");
        char username[U] = {0}; // U为用户名最大长度常量
        scanf("%s", username);
        while (getchar() != '\n')
            ; // 清空输入缓冲区

        // 获取密码（隐藏输入）
        printf("请输入密码：\n");
        char password[P] = {0}; // P为密码最大长度常量
        get_password(password, sizeof(password));
        printf("\n"); // 密码输入后换行

        // 注册功能
        if (in == '1')
        {
            if (0 == enroll(username, password)) // 调用注册函数
            {
                printf("注册成功！\n");
                printf("按任意键返回...");
                getchar();
                while (getchar() != '\n')
                    ; // 清空缓冲区
                system("clear");
                continue;
            }
            else
            {
                printf("用户名已存在！\n");
                printf("按任意键返回...");
                getchar();
                while (getchar() != '\n')
                    ;
                system("clear");
                continue;
            }
        }
        // 登录功能
        else if (in == '2')
        {
            if (0 == log_on(username, password)) // 调用登录函数
            {
                printf("登陆成功\n");
                r = 0; // 退出循环
            }
            else
            {
                printf("用户名或密码错误！\n");
                // 等待用户按键返回
                printf("\n按任意键返回...");
                getchar();
                while (getchar() != '\n')
                    ;
                system("clear");
            }
        }
    }
    return SUCCESS; // 初始化成功
}

/**
 * @brief 用户登录验证
 * @param un 用户名指针
 * @param pd 密码指针
 * @return 验证结果：SUCCESS/FAILURE/ERR_NOT_FOUND
 */
int log_on(char *un, char *pd)
{
    User *newuser = (User *)malloc(sizeof(User));
    if (newuser == NULL)
    {
        perror("User malloc");
        return FAILURE;
    }
    memset(newuser, 0, sizeof(User));

    FILE *fp = fopen("data/userinfo.txt", "r");
    if (fp == NULL)
    {
        perror("fopen");
        free(newuser);
        return FAILURE;
    }

    // 遍历用户文件查找匹配项
    while (fread(newuser, sizeof(User), 1, fp))
    {
        if (!strcmp(newuser->username, un) && !strcmp(newuser->password, pd))
        {
            // 分配全局用户结构体
            user = (User *)malloc(sizeof(User));
            if (user == NULL)
            {
                perror("global user malloc");
                free(newuser);
                fclose(fp);
                return FAILURE;
            }
            memcpy(user, newuser, sizeof(User)); // 复制用户数据
            free(newuser);
            fclose(fp);
            return SUCCESS; // 登录成功
        }
    }
    free(newuser);
    fclose(fp);
    return ERR_NOT_FOUND; // 用户不存在
}

/**
 * @brief 用户注册功能
 * @param un 用户名指针
 * @param pd 密码指针
 * @return 注册结果：SUCCESS/FAILURE/ERR_NOT_FOUND
 */
int enroll(char *un, char *pd)
{
    User *user = (User *)malloc(sizeof(User));
    if (user == NULL)
    {
        perror("User malloc");
        return FAILURE;
    }
    memset(user, 0, sizeof(User));

    // 以读写方式打开文件
    FILE *fp = fopen("data/userinfo.txt", "r+");
    if (fp == NULL)
    {
        perror("fopen");
        free(user);
        return FAILURE;
    }

    // 检查用户名是否已存在
    while (fread(user, sizeof(User), 1, fp))
    {
        if (!strcmp(user->username, un))
        {
            free(user);
            fclose(fp);
            return ERR_NOT_FOUND; // 用户名已存在
        }
    }

    // 创建新用户
    strcpy(user->username, un);
    strcpy(user->password, pd);
    user->type = 1;    // 普通用户类型
    user->balance = 0; // 初始余额

    // 写入文件末尾
    if (fseek(fp, 0, SEEK_END))
    {
        perror("fseek");
        return FAILURE;
    }
    if (1 != fwrite(user, sizeof(User), 1, fp))
    {
        perror("fwrite");
    }

    free(user);
    fclose(fp);
    return SUCCESS; // 注册成功
}

/**
 * @brief 主显示界面路由函数
 * @return 系统状态码
 * @note 根据用户类型跳转到不同界面
 */
int display()
{
    if (user->type == 0) // 管理员用户
    {
        system("clear");
        administrators(); // 进入管理员界面
    }
    else // 普通用户
    {
        system("clear");
        user_function(); // 进入用户功能界面
    }
    return SUCCESS;
}

/**
 * @brief 航班信息排序功能
 * @param h 航班链表头节点指针
 * @return 操作状态码
 */
int sort_info(FlightNode *h)
{
    char select;
    printf("\n        >1：按出发时间排序        >2：按机票价格排序          >3：返回\n"
           "  请输入： ");
    scanf("%c", &select);
    while (getchar() != '\n')
        ; // 清空输入缓冲区

    switch (select)
    {
    case '1': // 按出发时间排序
        system("clear");
        sort_list(&h, compare_by_departure_time);
        break;
    case '2': // 按价格排序
        system("clear");
        sort_list(&h, compare_by_price);
        break;
    case '3': // 返回
        system("clear");
        return BACK;
    default:
        system("clear");
    }
    return SUCCESS;
}

/**
 * @brief 系统退出函数
 * @return 退出状态码
 * @note 释放资源并安全退出程序
 */
int exit_system()
{
    printf("感谢使用航班管理系统，再见！\n");

    // 释放全局资源
    if (user)
    {
        free(user);
        user = NULL; // 避免悬垂指针
    }

    if (List)
    {
        free_node(&List); // 释放航班链表内存
    }

    exit(0); // 终止程序
    return 0;
}