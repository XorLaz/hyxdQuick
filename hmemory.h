#pragma once
#include <Windows.h>
#include <string>


class MemoryControl {
public:
    // 带参数的构造函数
    MemoryControl()  {
    }

    ~MemoryControl() {
    }

    void ChangeProessID(uint32_t PID);

    template <typename T>
    T ReadValue(uint64_t Address) {
        size_t SIZE = sizeof(T);
        T Temp = T();
        MemoryControl::Read(Address, (void*)&Temp, SIZE);
        return Temp;

    }
    bool Read(uint64_t Address, void* Temp, SIZE_T size);  // 读取字节流
    uint64_t GetBaseAddress();
    ULONG64 GetGameProcessID();

private:
    uint32_t processId;  // 进程ID
    HANDLE hProcess;
};


