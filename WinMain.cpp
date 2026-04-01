#define WIN32_LEAN_AND_MEAN  // 减少 Windows 头文件的冗余定义
#include <Windows.h>
#include <cstdio>
#include <thread>
#include <iostream>
#include <VMProtectSDK.h>
#include "initDraw.h"
#include "Thread.h"
#include "hmemory.h"
#include "globalVar.h"
#include "HttpsRequest/login.h"
#include "driver.h"


void CreateConsole() {
    VMProtectBeginMutation("CreateConsole");
    AllocConsole();
    // 重定向标准输入输出流到控制台
    freopen("CON", "w", stdout);  // 重定向标准输出
    freopen("CON", "w", stderr);  // 重定向标准错误输出
    freopen("CON", "r", stdin);   // 重定向标准输入
    VMProtectEnd();
}

std::string GetClipboardText() {
    VMProtectBeginMutation("GetClipboardText");
    // 尝试打开剪切板
    if (!OpenClipboard(nullptr)) {
        return "";
    }

    // 获取剪切板中的 Unicode 文本（CF_UNICODETEXT）
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }

    // 锁定内存并读取数据
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }

    // 转换宽字符 (wchar_t) 到 UTF-8 (std::string)
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, pszText, -1, nullptr, 0, nullptr, nullptr);
    std::string text(bufferSize, '\0');
    WideCharToMultiByte(CP_UTF8, 0, pszText, -1, &text[0], bufferSize, nullptr, nullptr);

    // 清理资源
    GlobalUnlock(hData);
    CloseClipboard();

    // 移除末尾的 '\0'（如果有）
    if (!text.empty() && text.back() == '\0') {
        text.pop_back();
    }

    return text;
    VMProtectEnd();
}

void GetProcessID() {
    VMProtectBeginMutation("GetProcessID");
    for (;;) {

        uint16_t PID = globalVar::mem.GetGameProcessID();
        if (PID > 0) {
            if (globalVar::isSucessfully) {
                drv.proceint((u32)PID);
                if (drv.GetMoudleBase() > 0x1000)
                    break;
            }
            else
            {
                globalVar::hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, PID);
                if (globalVar::hProcess)
                    break;
            }
        }
       
        Sleep(10000);
    }
    VMProtectEnd();
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    VMProtectBeginUltra("wWinMain");
    //CreateConsole();

    std::string result = (loginHyxdQuick(GetClipboardText(), versionsNOW));   // hyxd9O6C7N6H1N7F
    if (result == VMProtectDecryptStringA("Yes")) {
        std::string Announce = login::Announcement();
        if(Announce!= VMProtectDecryptStringA("NULL"))
            MessageBoxA(NULL, Announce.c_str(), "", MB_ICONINFORMATION | MB_OK);
        ExpirationTime = VMProtectDecryptStringA("Expiration date : ") + login::GetExpried(true);

        int msgboxID = MessageBoxW(
            NULL,
            VMProtectDecryptStringW(L"打住！还有最关键的一步！您是否要使用ollvm云特征内核模式访问游戏，这可大大提升性能以及稳定性，但可能在较久的操作系统上蓝屏、无效果。不过我们强烈推荐你点击\"是\"，如果你发现会蓝屏或无效果，那么您可以点击\"否\"，但这也意味着更容易被侦测到！"),
            VMProtectDecryptStringW(L"重要提示"),
            MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON1
        );

        if (msgboxID == IDYES) {
            // 用户点击了"是"
            MessageBoxW(NULL, VMProtectDecryptStringW(L"您已选择启用ollvm云特征内核模式"), VMProtectDecryptStringW(L"选择确认"), MB_ICONINFORMATION);
            globalVar::isSucessfully = true;
            hyxdQuickFile = globalFun::GetExePath();


            std::string Card = login::TakeVariableData(VMProtectDecryptStringA("7F6R0K3U9H9V7B0R"), cards, LoginToken, VMProtectDecryptStringA("4111"), VMProtectDecryptStringA("llvmCard"));

                if (drv.Loaddriver(VMProtectDecryptStringA(Card.c_str())) == 0) {
                        MessageBoxA(NULL, VMProtectDecryptStringA("正在初始化...请点击确定"), "提示", MB_ICONINFORMATION);

                        // Step 1: 计算需要的宽字符缓冲区大小
                        int wideLen = MultiByteToWideChar(
                                CP_ACP,                // ANSI 代码页
                                0,                     // 无特殊标志
                                hyxdQuickFile.c_str(),      // 输入 ANSI 字符串
                                -1,                    // 自动计算长度（包括 NULL 终止符）
                                NULL,                  // 不输出（仅计算长度）
                                0                      // 缓冲区大小为 0（仅计算）
                        );

                        // Step 2: 分配缓冲区并转换
                        std::wstring widePath(wideLen, L'\0');
                        MultiByteToWideChar(
                                CP_ACP, 0,
                                hyxdQuickFile.c_str(), -1,
                                &widePath[0], wideLen
                        );

                        // Step 3: 调用 API（LPWSTR = widePath.c_str()）
                        STARTUPINFOW si = { sizeof(si) };
                        PROCESS_INFORMATION pi;
                        CreateProcessW(
                                NULL,
                                const_cast<LPWSTR>(widePath.c_str()), // 去 const（API 设计问题）
                                NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
                        );
                    return 0;
                }
            
          
          


        }
        else if (msgboxID == IDNO) {
            // 用户点击了"否"
            MessageBoxW(NULL, VMProtectDecryptStringW(L"您已选择禁用ollvm云特征内核模式"), VMProtectDecryptStringW(L"选择确认"), MB_ICONINFORMATION);
        }


        

        std::thread t1(GetProcessID);
        std::thread t2(Thread::GetData::Thread_GetAddress);
        std::thread t3(Thread::GetData::Thread_ReadData);
        std::thread t4(Thread::GetData::Thread_ProcessingData);
        std::thread t5(Thread::GetData::Thread_ReadView);
        std::thread t6(Thread::EndThread::EndSoftWare);
        std::thread t7(Thread::AimBotThread::AimBotMain);

        if (!ToolCreateWindow((PVOID)Thread::DrawThread::Loop::Loop)) {
            return 1;
        }
    }
    else
    {
        MessageBoxA(NULL, result.c_str(), "", MB_ICONERROR | MB_OK);
    }
    return 0;

    VMProtectEnd();
}