#pragma once
#include <windows.h>
#include <winternl.h>


#if _WIN64
#define PTRMAXVAL ((PVOID64)0x000F000000000000)
#define pVOID PVOID64
#else
#if _WIN32
#define PTRMAXVAL ((PVOID)0xFFF00000)
#define pVOID PVOID
#endif
#endif

//检测地址是否在用户层空间
#define IsAddressValid(ptr) (((ptr >= 0x40000) && ((pVOID)ptr < PTRMAXVAL) && ((pVOID)ptr != nullptr)) ? TRUE : FALSE)

#pragma comment(lib,"22_MT_x64.lib")
 

typedef unsigned long long u64;
typedef unsigned long u32;

typedef PVOID64 ptr;
typedef PVOID64 p;
typedef u32* dptr;
typedef u64* qptr;

class Driver
{
public:
	Driver();
	~Driver();
public:
	//安装驱动
	BOOL Loaddriver(const char* key);
	//停止并卸载驱动
	BOOL Unloaddriver();
	//驱动是否安装
	BOOL IsInstall();
	//设置进程
	BOOL proceint(u32 ProcessId);
	//鼠标移动
	void Mouse_move(long x, long y, unsigned short button_flags);
	//鼠标移动2
	void Mouse_move2(long x, long y, unsigned short button_flags);
	//读取内存   moshi 0=默认  1=MDL 2=物理 3=切换CR3
	BOOL ReadProcessMemory(u64 addr, ptr buffer, u32 size, u32 moshi, u64* lpNumberOfBytesRead);
	//写入内存   moshi 0=默认  1=MDL 2=物理 3=切换CR3
	BOOL WriteProcessMemory(u64 addr, ptr buffer, u32 nSize, u32 moshi, u64* lpNumberOfBytesWritten);
	//取主模块地址
	u64 GetMoudleBase();
	//取模块地址
	u64 GetMoudleBaseEx(const char* name);
	//强删文件  字符串格式 示例  "c://1.exe"
	void QSDeleteFileEx(const char* name);
	//防止截图 最高支持到win11 24h2
	BOOL Protect_sprite_content(u64 handle, u32 attributes);
	//申请内存
	u64 AllocateMemory(u64 size);
	//释放内存
	void FreeMemory(u64 addrs);
	//修改内存属性
	u32 ProtectMemory(u64 addrs, u64 size, u32 Protect);
	//查询内存属性
	void QueryMemory(u64 addrs, PMEMORY_BASIC_INFORMATION pdata);	
    //解密初始化
	void InitializeDecrypt(u64 base);
    //通用解密
	u64 KernelDecrypt(u64 buff);
	//进程保护隐藏 开启  参数2 是否隐藏  支持多进程 2025.5.9 版本开始 加入了回调保护机制 即使您忘记了恢复 也不会出现蓝屏的情况了 回调监控到程序崩溃或者退出会提前恢复 
	void Hide(DWORD pid, BOOL ishide);
    //进程保护隐藏 恢复  参数2 是否隐藏  支持多进程 2025.5.9 版本开始 加入了回调保护机制 即使您忘记了恢复 也不会出现蓝屏的情况了 回调监控到程序崩溃或者退出会提前恢复 
	void RemoveHide(DWORD pid, BOOL ishide);
    //进程保护隐藏 恢复全部
	void RemoveHideAll();
	//键盘按下
	void KeyDown(USHORT VirtualKey);
	//键盘弹起
	void KeyUp(USHORT VirtualKey);
	//鼠标左键按下
	void MouseLeftButtonDown();
	//鼠标左键弹起
	void MouseLeftButtonUp();
	//鼠标右键按下
	void MouseRightButtonDown();
	//鼠标右键弹起
	void MouseRightButtonUp();
	//鼠标中键按下
	void MouseMiddleButtonDown();
	//鼠标中键弹起
	void MouseMiddleButtonUp();
	//鼠标相对移动
	void MouseMoveRELATIVE(LONG dx, LONG dy);
	//鼠标绝对
	void MouseMoveABSOLUTE(LONG dx, LONG dy);
	// 向上滚动垂直滚轮
	void ScrollVerticalUp(USHORT units);
	// 向下滚动垂直滚轮
	void ScrollVerticalDown(USHORT units);
	//创建远线程
    void CreateRemoteThreadEx(ULONG pid, ULONG64 addr);
//万能读模板
	template<typename T>
	T RPM(unsigned long long Addr);
//万能读模板
	template<typename T>
	T RPM(unsigned long long Addr, unsigned long Size);
//万能读模板
	template<typename T>
	bool RPM(unsigned long long Addr, T OuterBuffer, unsigned long Size);
	template<typename T>
	bool RPM(unsigned long long Addr, unsigned long Size, T OuterBuffer);
//万能写模板
	template<typename T>
	bool WPM(unsigned long long Addr, T value);
//万能写模板
	template<typename T>
	bool WPM(unsigned long long Addr, T value, unsigned long Size);

};

//万能读模板  需要使用CR3解密模式 改下下面的0 为1
template<typename T> inline T Driver::RPM(unsigned long long Addr)
{
	T readBuffer{};
	u64 lpNumberOfBytesRead;
	ReadProcessMemory( Addr, &readBuffer, sizeof(T),0, &lpNumberOfBytesRead);
	return (T)readBuffer;
}

template<typename T> inline T Driver::RPM(unsigned long long Addr, unsigned long Size)
{
	T readBuffer = {};
	u64 lpNumberOfBytesRead;
	ReadProcessMemory( Addr, &readBuffer, Size,0, &lpNumberOfBytesRead);
	return (T)readBuffer;
}

template<typename T> inline bool Driver::RPM(unsigned long long Addr, T OuterBuffer, unsigned long Size)
{
	u64 lpNumberOfBytesRead;
	ReadProcessMemory(  Addr, OuterBuffer, Size,0, &lpNumberOfBytesRead);
	return TRUE;
}
template<typename T> inline bool Driver::RPM(unsigned long long Addr, unsigned long Size, T OuterBuffer)
{
	u64 lpNumberOfBytesRead;
	ReadProcessMemory( Addr, OuterBuffer, Size, 0, &lpNumberOfBytesRead);
	return TRUE;
}

//万能写模板  需要使用CR3解密模式 改下下面的0 为1
template<typename T> inline bool Driver::WPM(unsigned long long Addr, T value)
{
	u64 lpNumberOfBytesRead;
	WriteProcessMemory( Addr, &value, sizeof(T),0, &lpNumberOfBytesRead);
	return TRUE;
}

template<typename T> inline bool Driver::WPM(unsigned long long Addr, T value, unsigned long Size)
{
	u64 lpNumberOfBytesRead;
	WriteProcessMemory( Addr, &value, Size, 0,&lpNumberOfBytesRead);
	return TRUE;
}

extern Driver drv;

