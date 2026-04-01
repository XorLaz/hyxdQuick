#include "hmemory.h"
#include <TlHelp32.h>
#include <psapi.h>
#include <VMProtectSDK.h>
#include "globalVar.h"
#include "driver.h"
ULONG64 MemoryControl::GetGameProcessID() {


    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process = { };
    ZeroMemory(&process, sizeof(PROCESSENTRY32));
    process.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &process)) {
        CloseHandle(hSnap);
        return NULL;
    }
    do {
        if (lstrcmpi(VMProtectDecryptStringW(L"hyxd.exe"), process.szExeFile) == 0) {
            return process.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnap, &process));
    CloseHandle(hSnap);
    return -1;

}




uint64_t MemoryControl::GetBaseAddress() {
    if (globalVar::isSucessfully) {
        drv.GetMoudleBase();
    }
    else
    {
        HMODULE hModule = nullptr;
        DWORD cbNeeded = 0;

        // 方法1：枚举所有模块（推荐）
        if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
            HMODULE* hModules = new HMODULE[cbNeeded / sizeof(HMODULE)];
            if (EnumProcessModules(hProcess, hModules, cbNeeded, &cbNeeded)) {
                for (DWORD i = 0; i < cbNeeded / sizeof(HMODULE); i++) {
                    wchar_t szModName[MAX_PATH];
                    if (GetModuleFileNameExW(hProcess, hModules[i], szModName, MAX_PATH)) {
                        if (lstrcmpi(szModName, VMProtectDecryptStringW(L"hyxd.exe")) == 0) {
                            hModule = hModules[i];
                            break;
                        }
                    }
                }
            }
            delete[] hModules;
        }

        return (uint64_t)hModule;
    }
   
}

// 底层读取函数，根据 ReadMod 选择读取方式
bool MemoryControl::Read(uint64_t Address, void* Temp, SIZE_T Size) {
    if (globalVar::isSucessfully) {
        u64 lpNumberOfBytesRead;
        return (bool)drv.ReadProcessMemory(Address, Temp, Size, 0, &lpNumberOfBytesRead);
    }
    else
    {
        return ReadProcessMemory(hProcess, (LPCVOID)Address, Temp, Size, NULL);
    }

}

void MemoryControl::ChangeProessID(uint32_t PID) {
    if (PID > 0){
        if (globalVar::isSucessfully) {
            this->processId = PID;
            drv.proceint(PID);
        }
        else {
            this->processId = PID;
            this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
        }
      
    }
   
}


