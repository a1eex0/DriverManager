// DriverManager.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DriverManager.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
WCHAR szDriverFilePath[MAX_LOADSTRING];         // 驱动文件路径
WCHAR szServiceName[MAX_LOADSTRING];            // 服务名称
WCHAR szResultLog[1000];                        // 结果输出缓冲区
DWORD dv = -5;                                  // 垂直滑动量
HWND hWnd;                                      // 主窗口句柄
HWND hdDriverPathEdit,hdSerNameEdit, hdResultLogEdit;   // 编辑控件句柄
HWND hdOpenButton, hdAboutButton, hdRegisterButton, hdStartButton, hdStopButton, hdDeleteButton;   // 按钮控件句柄

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL                InitWindowControl();
BOOL                GetDriverFilePath();
BOOL                RegisterServer();
BOOL                StartServer();
BOOL                StopServer();
BOOL                DeleteServer();





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRIVERMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRIVERMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;     // 无菜单栏
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   // 创建主窗口样式
   hWnd = CreateWindowW(
       szWindowClass,
       szTitle,
       WS_EX_LAYERED | WS_MINIMIZEBOX | WS_OVERLAPPED,  // 设置窗口样式，不可改变窗口大小
       600, 300,    // x，y 显示坐标
       400, 235,    // 窗体长宽大小
       nullptr, nullptr, hInstance, nullptr);
   // 初始化窗口控件
   InitWindowControl();

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_CREATE   - 创建窗口
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HBRUSH hBrush;
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case BN_CLICKED:
            // 点击“打开”按钮事件处理
            if (hdOpenButton == (HWND)lParam)
            {
                GetDriverFilePath();
            }// 点击“关于”按钮事件处理
            else if (hdAboutButton == (HWND)lParam)
            {
                MessageBox(hWnd, 
                    L"    这是一个自动化创建、启动、停止、卸载服务的小工具。\r\n"
                    L"        你必须使用管理员权限打开才能保证它正常运行。\r\n"
                    L"      仅填写服务名称可以实现对服务的启动、停止和卸载。\r\n"
                    L"     当前版本仅支持对无参数的服务进行启动、停止和卸载。\r\n"
                    L"\tDriverManager v1.0\tBy alee",
                    L"关于", 
                    MB_OK);
            }// 点击“注册”按钮事件处理
            else if (hdRegisterButton == (HWND)lParam)
            {
                dv++;
                RegisterServer();
                
            }// 点击“启动”按钮事件处理
            else if (hdStartButton == (HWND)lParam)
            {
                dv++;
                StartServer();
            }// 点击“停止”按钮事件处理
            else if (hdStopButton == (HWND)lParam)
            {
                dv++;
                StopServer();
            }// 点击“卸载”按钮事件处理
            else if (hdDeleteButton == (HWND)lParam)
            {
                dv++;
                DeleteServer();
            }
            break;
        default:
            break;
        }
        break;
    case WM_CTLCOLORSTATIC:     // 静态控件背景颜色设置
        hBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));   // 白色背景
        return (INT_PTR)hBrush;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//
//  函数: InitWindowControl()
//
//  目标: 初始化窗口控件。
//
BOOL InitWindowControl()
{
    /****************第一排控件*********************/
    // 创建静态控件
    HWND hdPathLabel = CreateWindowW(
        L"STATIC",
        L"文件路径：",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 5,
        100, 20,
        hWnd, 0, hInst, 0);
    // 创建路径获取编辑控件
    hdDriverPathEdit = CreateWindowW(
        L"EDIT",
        L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        100, 5,
        200, 20,
        hWnd, 0, hInst, 0);
    // 创建打开按钮控件
    hdOpenButton = CreateWindowW(
        L"BUTTON",
        L"打开",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        310, 5,
        60, 20,
        hWnd, 0, hInst, 0);

    /****************第二排控件*********************/
    // 创建静态控件
    HWND hdSerNameLabel = CreateWindowW(
        L"STATIC",
        L"服务名称：",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        0, 35,
        100, 20,
        hWnd, 0, hInst, 0);
    // 创建服务名称编辑控件
    hdSerNameEdit = CreateWindowW(
        L"EDIT",
        L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        100, 35,
        100, 20,
        hWnd, 0, hInst, 0);
    // 创建关于按钮控件
    hdAboutButton = CreateWindowW(
        L"BUTTON",
        L"关于",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        310, 35,
        60, 20,
        hWnd, 0, hInst, 0);
    /****************第三排控件*********************/
    // 创建注册按钮控件
    hdRegisterButton = CreateWindowW(
        L"BUTTON",
        L"注册服务",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        15, 65,
        70, 20,
        hWnd, 0, hInst, 0);
    // 创建启动按钮控件
    hdStartButton = CreateWindowW(
        L"BUTTON",
        L"启动服务",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        110, 65,
        70, 20,
        hWnd, 0, hInst, 0);
    // 创建停止按钮控件
    hdStopButton = CreateWindowW(
        L"BUTTON",
        L"停止服务",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        205, 65,
        70, 20,
        hWnd, 0, hInst, 0);
    // 创建卸载按钮控件
    hdDeleteButton = CreateWindowW(
        L"BUTTON",
        L"卸载服务",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        300, 65,
        70, 20,
        hWnd, 0, hInst, 0);

    /****************第四排控件*********************/
    hdResultLogEdit = CreateWindowW(
        L"EDIT",
        L"",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_CENTER | ES_MULTILINE | ES_READONLY | ES_WANTRETURN,
        15, 95,
        355, 90,
        hWnd, 0, hInst, 0);
    /****************控件美化*********************/
    // 自定义显示字体(均为默认选项)
    HFONT hFont = CreateFont(
        -15,                        // 字符高度
        -7.5,                       // 字符宽度
        0,                          // 转义向量与 X 轴角度
        0,                          // 字符基线与 X 轴角度
        FW_NORMAL,                  // 粗细范围
        0,                          // 是否斜体
        0,                          // 是否带下划线
        0,                          // 是否带删除线
        DEFAULT_CHARSET,            // 字符集
        OUT_CHARACTER_PRECIS,       // 输出精度
        CLIP_CHARACTER_PRECIS,      // 剪辑精度
        DEFAULT_QUALITY,            // 输出质量
        FF_DONTCARE,                // 字体音调和系列
        NULL);                      // 字符名
    // 设置控件显示字体（用于美化显示内容）
    SendMessage(hdPathLabel, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdDriverPathEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdOpenButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdSerNameLabel, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdSerNameEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdAboutButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdSerNameEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdRegisterButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdStartButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdStopButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdDeleteButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    SendMessage(hdResultLogEdit, WM_SETFONT, (WPARAM)hFont, (LPARAM)TRUE);
    return TRUE;
}


//
//  函数: GetDriverFilePath()
//
//  目标: 通过文件选择框，获取驱动文件路径并设置默认服务名称。
//
BOOL GetDriverFilePath()
{
    OPENFILENAME file = { 0 };              // 定义一个空的 OPENFILENAME 类型
    file.lStructSize = sizeof(file);        // 定义结构大小，必须优先定义
    file.hwndOwner = hWnd;                  // 定义所属窗口的句柄
    file.lpstrFile = szDriverFilePath;      // 定义全路径文件名的存储缓冲区
    file.nMaxFile = MAX_LOADSTRING;         // 定义全路径文件名缓冲区的长度
    file.lpstrFileTitle = szServiceName;     // 定义文件名的缓冲区
    file.nMaxFileTitle = MAX_LOADSTRING;    // 定义文件名的缓冲区长度
    // 文件筛选的类型字符串，多个类型使用";"隔开，最后应当以"\0\0"结尾。结构为："显示"\0"选取类型"\0"显示"\0"选取类型"\0\0
    file.lpstrFilter = L"Drive Files(*.exe;*.dll;*.sys)\0*.exe;*.dll;*.sys\0All Files(*.*)\0*.*\0\0";
    GetOpenFileName(&file);                 // 调用 GetOpenFileName 函数以 file 配置打开对话框选取文件
    // 填充文件路径编辑控件
    Edit_SetText(hdDriverPathEdit, szDriverFilePath);
    // 去除文件后缀名
    WCHAR* p = szServiceName;
    for (size_t i = wcslen(szServiceName)-1; i >=0 ; i--)
    {
		if (*(p + i) == 0x2e)
        {
            *(p + i) = 0x00;
            break;
        }
        *(p + i) = 0x00;
    }// 使用无后缀的文件名填充服务名称
    Edit_SetText(hdSerNameEdit, szServiceName);

    return TRUE;
}

//
//  函数: RegisterServer()
//
//  目标: 注册服务。
//
BOOL RegisterServer()
{
    WCHAR szTemp[MAX_LOADSTRING];
    // 获取服务管理器句柄
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCManager == NULL)
    {
        CloseServiceHandle(hSCManager);
        Edit_SetText(hdResultLogEdit, L"----请以管理员权限打开！----\r\n");
        return FALSE;
    }
    // 从编辑控件中获取文件路径、服务名称和输出日志
    Edit_GetText(hdSerNameEdit, szServiceName, MAX_LOADSTRING);
    Edit_GetText(hdDriverPathEdit, szDriverFilePath, MAX_LOADSTRING);
    Edit_GetText(hdResultLogEdit, szResultLog, 1000);
    // 创建服务
    SC_HANDLE hService = CreateServiceW(
        hSCManager,                 // 服务管理器句柄
        szServiceName,              // 注册服务名
        szServiceName,              // 显示服务名
        SERVICE_ALL_ACCESS,         // 服务权限
        SERVICE_KERNEL_DRIVER,      // 服务类型
        SERVICE_AUTO_START,         // 服务启动类型
        SERVICE_ERROR_IGNORE,       // 服务报错后操作系统处理方式
        szDriverFilePath,           // 服务文件路径
        NULL,                       // 服务分组名称
        NULL,                       // 服务所在分组启动顺序
        NULL,                       // 当前服务依赖的服务列
        NULL,                       // 服务启动的用户身份
        NULL);                      // 服务启动的用户的密码
    if (hService != NULL)
    {
        swprintf_s(szTemp, L"---- 服务：[%ls] 创建成功！ ----\r\n", szServiceName);
    }
    else
    {
		switch (GetLastError())
		{
		case ERROR_DUPLICATE_SERVICE_NAME:
			swprintf_s(szTemp, L"[%ls] 服务名称或显示名称已存在！\r\n", L"ERROR_DUPLICATE_SERVICE_NAME");
			break;
		case ERROR_INVALID_HANDLE:
			swprintf_s(szTemp, L"[%ls] 服务管理器句柄无效！\r\n", L"ERROR_INVALID_HANDLE");
			break;
		case ERROR_INVALID_NAME:
			swprintf_s(szTemp, L"[%ls] 服务名称无效！\r\n", L"ERROR_INVALID_NAME");
			break;
		case ERROR_INVALID_PARAMETER:
			swprintf_s(szTemp, L"[%ls] 服务参数无效！\r\n", L"ERROR_INVALID_PARAMETER");
			break;
        default:
            swprintf_s(szTemp, L"服务创建失败：[%d]\r\n", GetLastError());
            break;
		}
    }
    wcscat_s(szResultLog, szTemp);

    Edit_SetText(hdResultLogEdit, szResultLog);
    Edit_Scroll(hdResultLogEdit, dv, 0);

    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    return TRUE;
}


//
//  函数: StartServer()
//
//  目标: 启动服务。
//
BOOL StartServer()
{
    WCHAR szTemp[MAX_LOADSTRING];
    // 获取服务管理器句柄
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCManager == NULL)
    {
        CloseServiceHandle(hSCManager);
        Edit_SetText(hdResultLogEdit, L"----请以管理员权限打开！----\r\n");
        return FALSE;
    }
    // 从编辑控件中获取服务名称和输出日志
    Edit_GetText(hdSerNameEdit, szServiceName, MAX_LOADSTRING);
    Edit_GetText(hdResultLogEdit, szResultLog, 1000);
    // 打开服务
    SC_HANDLE hService = OpenServiceW(
        hSCManager,              // SC 句柄
        szServiceName,           // 服务名称 
        SC_MANAGER_ALL_ACCESS);  // 服务权限
    if (hService == NULL)
    {
        switch (GetLastError())
        {
        case ERROR_INVALID_NAME:
            swprintf_s(szTemp, L"[%ls] 服务名称无效！\r\n", szServiceName);
            break;
        case ERROR_SERVICE_DOES_NOT_EXIST:
            swprintf_s(szTemp, L"[%ls] 服务不存在！\r\n", szServiceName);
            break;
        default:
            swprintf_s(szTemp, L"服务打开失败：[%d]\r\n", GetLastError());
            break;
        }
    }
    else
    {
        // 启动服务
        if (!StartServiceW(
            hService,    // 服务名称
            NULL,        // 参数数组个数
            NULL))       // 参数数组
        {
            switch (GetLastError())
            {
            case ERROR_SERVICE_ALREADY_RUNNING:
                swprintf_s(szTemp, L"[%ls] 服务的实例已运行！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_DEPENDENCY_DELETED:
                swprintf_s(szTemp, L"[%ls] 服务不存在或标记为要删除！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_DEPENDENCY_FAIL:
                swprintf_s(szTemp, L"[%ls] 服务依赖于另一个未能启动的服务！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_DISABLED:
                swprintf_s(szTemp, L"[%ls] 服务已被禁用！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_MARKED_FOR_DELETE:
                swprintf_s(szTemp, L"[%ls] 服务已被标记为要删除！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_REQUEST_TIMEOUT:
                swprintf_s(szTemp, L"[%ls] 服务的进程已启动！\r\n", szServiceName);
                break;
            case 577:
                swprintf_s(szTemp, L"服务启动失败，驱动签名未通过系统认证！\r\n");
                break;
            default:
                swprintf_s(szTemp, L"服务启动失败：[%d]\r\n", GetLastError());
                break;
            }
        }
        else
        {
            swprintf_s(szTemp, L"---- 服务：[%ls] 启动成功！ ----\r\n", szServiceName);
        }
    }
    wcscat_s(szResultLog, szTemp);
    
    Edit_SetText(hdResultLogEdit, szResultLog);
    Edit_Scroll(hdResultLogEdit, dv, 0);

    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    return TRUE;
}


//
//  函数: StopServer()
//
//  目标: 停止服务。
//
BOOL StopServer()
{
    WCHAR szTemp[MAX_LOADSTRING];
    SERVICE_STATUS ss;
    // 获取服务管理器句柄
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCManager == NULL)
    {
        CloseServiceHandle(hSCManager);
        Edit_SetText(hdResultLogEdit, L"----请以管理员权限打开！----\r\n");
        return FALSE;
    }
    // 从编辑控件中获取服务名称和输出日志
    Edit_GetText(hdSerNameEdit, szServiceName, MAX_LOADSTRING);
    Edit_GetText(hdResultLogEdit, szResultLog, 1000);
    // 打开服务
    SC_HANDLE hService = OpenServiceW(
        hSCManager,             // SC 句柄
        szServiceName,           // 服务名称 
        SC_MANAGER_ALL_ACCESS); // 服务权限
    if (hService == NULL)
    {
        switch (GetLastError())
        {
        case ERROR_INVALID_NAME:
            swprintf_s(szTemp, L"[%ls] 服务名称无效！\r\n", szServiceName);
            break;
        case ERROR_SERVICE_DOES_NOT_EXIST:
            swprintf_s(szTemp, L"[%ls] 服务不存在！\r\n", szServiceName);
            break;
        default:
            swprintf_s(szTemp, L"服务打开失败：[%d]\r\n", GetLastError());
            break;
        }
    }
    else
    {
        // 停止服务
        if (!ControlService(
            hService,
            SERVICE_CONTROL_STOP,
            &ss))
        {
            switch (GetLastError())
            {
            case ERROR_DEPENDENT_SERVICES_RUNNING:
                swprintf_s(szTemp, L"-- [%ls] 服务无法停止！\r\n", szServiceName);
                break;
            case ERROR_SERVICE_NOT_ACTIVE:
                swprintf_s(szTemp, L"-- [%ls] 服务尚未启动！\r\n", szServiceName);
                break;
            case ERROR_SHUTDOWN_IN_PROGRESS:
                swprintf_s(szTemp, L"系统正在关闭！\r\n");
                break;
            default:
                swprintf_s(szTemp, L"服务停止失败：[%d]\r\n", GetLastError());
                break;
            }
        }
        else
        {
            swprintf_s(szTemp, L"---- 服务：[%ls] 停止成功！ ----\r\n", szServiceName);
        }
    }
    wcscat_s(szResultLog, szTemp);

    Edit_SetText(hdResultLogEdit, szResultLog);
    Edit_Scroll(hdResultLogEdit, dv, 0);

    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    return TRUE;
}


//
//  函数: DeleteServer()
//
//  目标: 卸载服务。
//
BOOL DeleteServer()
{
    WCHAR szTemp[MAX_LOADSTRING];
    // 获取服务管理器句柄
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCManager == NULL)
    {
        CloseServiceHandle(hSCManager);
        Edit_SetText(hdResultLogEdit, L"----请以管理员权限打开！----\r\n");
        return FALSE;
    }
    // 从编辑控件中获取服务名称和输出日志
    Edit_GetText(hdSerNameEdit, szServiceName, MAX_LOADSTRING);
    Edit_GetText(hdResultLogEdit, szResultLog, 1000);
    // 打开服务
    SC_HANDLE hService = OpenServiceW(
        hSCManager,             // SC 句柄
        szServiceName,           // 服务名称 
        SC_MANAGER_ALL_ACCESS); // 服务权限
    if (hService == NULL)
    {
        switch (GetLastError())
        {
        case ERROR_INVALID_NAME:
            swprintf_s(szTemp, L"[%ls] 服务名称无效！\r\n", szServiceName);
            break;
        case ERROR_SERVICE_DOES_NOT_EXIST:
            swprintf_s(szTemp, L"[%ls] 服务不存在！\r\n", szServiceName);
            break;
        default:
            swprintf_s(szTemp, L"服务打开失败：[%d]\r\n", GetLastError());
            break;
        }
    }
    else
    {
        // 卸载服务
        if (!DeleteService(hService))
        {
            if (GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE)
            {
                swprintf_s(szTemp, L"[%ls] 服务已被标记为要删除！\r\n", szServiceName);
            }
            else
            {
                swprintf_s(szTemp, L"服务卸载失败：[%d]\r\n", GetLastError());
            }
        }
        else
        {
            swprintf_s(szTemp, L"---- 服务：[%ls] 卸载成功！ ----\r\n", szServiceName);
        }
    }
    wcscat_s(szResultLog, szTemp);
    Edit_SetText(hdResultLogEdit, szResultLog);
    Edit_Scroll(hdResultLogEdit, dv, 0);

    CloseServiceHandle(hSCManager);
    CloseServiceHandle(hService);
    return TRUE;
}