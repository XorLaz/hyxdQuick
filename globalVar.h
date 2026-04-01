#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <cstdint>
#include <vector>
#include <cmath>
#include <imgui.h>
#include <VMProtectSDK.h>
#include "hmemory.h"
#include "concurrentqueue.h"
#include "HumanBodyBones.h"
#include "driver.h"

inline std::string versionsNOW = "4.0";
inline std::string ExpirationTime = "";
inline std::string hyxdQuickFile = "";

namespace globalVar {
	inline MemoryControl mem;
	inline HANDLE hProcess = NULL;
	inline bool isSucessfully = false;
	inline bool g_bMenuVisible = true;
	inline uint32_t isInGame = 1;


	namespace imguiPtr {
		inline HWND hwndOverlay;
		inline ID3D11Device* pDevice = nullptr;
		inline ID3D11DeviceContext* pContext = nullptr;
		inline IDXGISwapChain* pSwapChain = nullptr;
		inline ID3D11RenderTargetView* pRenderTargetView = nullptr;



	}

	namespace GameData {


		namespace Offset {
			inline uint32_t inGame = NULL;
			inline uint32_t inGame_1 = 0x294;
			inline uint32_t View = NULL;
			inline uint32_t View_1 = 0x570;

			inline uint32_t Count = 0x6A628F0;
			inline uint32_t Count_1 = 0x8;
			inline uint32_t Count_2 = 0x0;
			inline uint32_t Count_3 = 0x8;
			inline uint32_t Count_4 = 0x8;
			inline uint32_t Count_5 = 0x18;
			inline uint32_t Count_6 = 0x68;

			namespace Player {
				inline uint32_t MySignName = 0x1D8;     // 会变      1D0
				inline uint32_t ControllerBehaviorCallback = 0xE0;
				inline uint32_t CharCtrlComponent = 0x30;
				inline uint32_t IEntity = 0x40;
				inline uint32_t IEntity_2 = 0x40;
				inline uint32_t ActorComponent = 0x8;
				inline uint32_t Actor = 0xD0;
				inline uint32_t Camp = 0xBC;   //  IEntity->Camp
				inline uint32_t Position = 0x130;
				inline uint32_t StateLayer = 0x290;   // 会变   288
				inline uint32_t Stater = 0x48;
				inline uint32_t Pose = 0x18;
				inline uint32_t BoneHead = 0x10;
				inline uint32_t BoneEnd = 0x18;
				inline uint32_t BoneMemorySize = 0xA0;  // 骨骼内存块大小

				inline uint32_t BoneNamePtr = 0x58;
				inline uint32_t BoneName = 0x18;
				inline uint32_t BoneNameLength = 0xC;




			};




		}


		namespace MainData {
			inline HWND hWnd = NULL;
			inline uint32_t ScreenX = 1920;
			inline uint32_t ScreenY = 1080;

			inline uint64_t hyxdAddress = 0;
			inline uint64_t hyxd = 0;
			inline uint64_t ViewAddress = 0;

			inline uint64_t CountAddress_1 = 0;
			inline uint64_t CountAddress_2 = 0;
			inline uint64_t CountAddress_3 = 0;
			inline uint64_t CountAddress_4 = 0;
			inline uint64_t CountAddress_5 = 0;
			inline uint64_t CountAddress_6 = 0;
			inline uint64_t CountAddress = 0;
			inline uint32_t Count = 0;

			inline uint64_t PlayerArrayInitAddress = 0;
			inline uint64_t PlayerArrayInit = 0;


			inline float Mx[4][4] = {};

		}


		namespace addressArray {
			struct Bone_Address {
				uint64_t BipedNeck;
				uint64_t BipedHead;
				uint64_t BipedLUpperArm;
				uint64_t BipedRUpperArm;
				uint64_t BipedLForearm;
				uint64_t BipedRForearm;
				uint64_t BipedLHand;
				uint64_t BipedRHand;
				uint64_t BipedSpine1;
				uint64_t BipedSpine;
				uint64_t BipedLThigh;
				uint64_t BipedRThigh;
				uint64_t BipedLCalf;
				uint64_t BipedRCalf;
				uint64_t BipedLFoot;
				uint64_t BipedRFoot;
			};


			struct Data_Address {
				uint64_t Actor;
				uint64_t IEntity;
				uint64_t CampAddress;
				uint64_t positionAddress;

				uint64_t PoseAddress;
				uint64_t Pose;

				uint64_t BoneHeadAddress;
				uint64_t BoneHead;

				uint64_t BoneEndAddress;
				uint64_t BoneEnd;

				Bone_Address PlayerBoneAddress;

				uint64_t StateLayer;
				uint64_t StateAddress;




			};

		}






		namespace PlayerData {
			struct D3DVector {
				float x;
				float y;
				float z;
			};

			struct D2DVector {
				float x;
				float y;
			};






			struct Player {

				struct Box {
					float X, Y, W, H;
				};

				struct SkeletalMatrix {
					float Matrix_10C, Matrix_110, Matrix_114;
					float Matrix_118, Matrix_11C, Matrix_120;
					float Matrix_124, Matrix_128, Matrix_12C;
					float Matrix_130, Matrix_134, Matrix_138;
				};

				uint64_t Actor;
				uint64_t IEntity;
				D3DVector Position;
				D2DVector Position_Pos;
				SkeletalMatrix BoneMatrix;
				D3DVector BonePosition[0x10];
				D2DVector BonePosition_Pos[0x10];
				D3DVector HeadMiddlePosition;
				D2DVector HeadMiddlePosition_Pos;
				Box Rect;
				uint32_t State;
				uint32_t Camp;
				float Distance;



			};



		}

		namespace LocalPlayerData {
			inline uint32_t Camp;
			inline uint64_t LocalAddressPtr;    // is IEntity   not Actor   七儿八的
			inline uint64_t LocalPositionAddress;
			inline PlayerData::D3DVector LocalPosition;




		}


		inline moodycamel::ConcurrentQueue<std::shared_ptr<std::vector<PlayerData::Player>>> Alldata;   //无锁队列 处理完的玩家数据

		inline std::shared_ptr<std::vector<addressArray::Data_Address>> MainArray  = std::make_shared<std::vector<addressArray::Data_Address>>();;

		inline std::shared_ptr<std::vector<PlayerData::Player>> RenderArray = std::make_shared<std::vector<PlayerData::Player>>();;
	}


}




namespace globalFun {

	namespace GetGameHWND {

		struct WindowSearchInfo {
			DWORD targetPid;
			HWND resultHwnd;
		};

		BOOL inline CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
			WindowSearchInfo* info = reinterpret_cast<WindowSearchInfo*>(lParam);
			DWORD windowPid = 0;
			GetWindowThreadProcessId(hwnd, &windowPid);

			if (windowPid == info->targetPid) {
				info->resultHwnd = hwnd;
				return FALSE; // 找到目标窗口，停止枚举
			}
			return TRUE; // 继续枚举
		}

		inline HWND GetTopWindowByPid(DWORD pid) {
			WindowSearchInfo info = { pid, NULL };
			EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&info));
			return info.resultHwnd;
		}

		inline HWND PIDGetProcessHWND(uint32_t PID) {
			HWND hwnd = GetTopWindowByPid(PID);
			return hwnd;
		}

		inline bool refreshWindow(HWND hwnd)
		{
			WINDOWINFO pwi = {};
			pwi.cbSize = sizeof(PWINDOWINFO);
			GetWindowInfo(globalVar::GameData::MainData::hWnd, &pwi);
			globalVar::GameData::MainData::ScreenX = pwi.rcClient.right - pwi.rcClient.left;
			globalVar::GameData::MainData::ScreenY = pwi.rcClient.bottom - pwi.rcClient.top;
			//printf("%d %d %d %d\n", pwi.rcClient.left, pwi.rcClient.top, globalVar::GameData::MainData::ScreenX, globalVar::GameData::MainData::ScreenY);
			return MoveWindow(hwnd, pwi.rcClient.left, pwi.rcClient.top, globalVar::GameData::MainData::ScreenX, globalVar::GameData::MainData::ScreenY, TRUE);
		}

	}




	namespace GetBaseAddress {

		inline uint64_t GetViewCodeAddress() {
			VMProtectBeginMutation("GetViewCodeAddress");
			const size_t size = 110000000; // 110 MB
			BYTE* hyxd_BYTE = new BYTE[size]; // 动态分配内存
			if (hyxd_BYTE == nullptr) {
				//printf("Memory allocation failed!\n");
				return -1; // 或者其他错误处理
			}


			uint64_t Base = globalVar::mem.GetBaseAddress();
			// 读取字节数据
			if (!globalVar::mem.Read(Base, hyxd_BYTE, size)) {
				//printf("Failed to read memory!\n");
				delete[] hyxd_BYTE; // 释放内存
				return -1; // 或者其他错误处理
			}

			// 要搜索的字节序列
			const BYTE target[] = { 72, 131, 236, 48, 69, 15, 183, 200, 72, 139, 217, 76, 139, 194, 72, 139, 209 };
			const size_t target_size = sizeof(target) / sizeof(target[0]);

			// 在数据中搜索字节序列
			for (size_t i = 0; i <= size - target_size; ++i) {
				if (std::memcmp(&hyxd_BYTE[i], target, target_size) == 0) {

					uint32_t relative = globalVar::mem.ReadValue<uint32_t>(Base + i + 0x14);
					uint32_t Offset = relative + i + 0x18;
					delete[] hyxd_BYTE; // 释放内存
					return Offset; // 返回找到的起始位置
				}
			}


			//printf("Byte sequence not found.\n");
			// 释放内存
			delete[] hyxd_BYTE;
			return -1; // 如果没有找到，返回 -1


			VMProtectEnd();
		}


		inline uint64_t GetPlayerCountCodeAddress() {
			VMProtectBeginMutation("GetPlayerCountCodeAddress");
			const size_t size = 110000000; // 110 MB
			BYTE* hyxd_BYTE = new BYTE[size]; // 动态分配内存
			if (hyxd_BYTE == nullptr) {
				//printf("Memory allocation failed!\n");
				return -1; // 或者其他错误处理
			}


			uint64_t Base = globalVar::mem.GetBaseAddress();
			// 读取字节数据
			if (!globalVar::mem.Read(Base, hyxd_BYTE, size)) {
				//printf("Failed to read memory!\n");
				delete[] hyxd_BYTE; // 释放内存
				return -1; // 或者其他错误处理
			}

			// 要搜索的字节序列
			const BYTE target[] = { 72, 139, 248, 72, 137, 68, 36, 48, 51, 246, 72, 133, 192 };
			const size_t target_size = sizeof(target) / sizeof(target[0]);

			// 在数据中搜索字节序列
			for (size_t i = 0; i <= size - target_size; ++i) {
				if (std::memcmp(&hyxd_BYTE[i], target, target_size) == 0) {
					uint32_t relative = globalVar::mem.ReadValue<uint32_t>(Base + i - 0x17);
					uint32_t Offset = relative + i - 0x13;
					delete[] hyxd_BYTE; // 释放内存
					return Offset; // 返回找到的起始位置
				}
			}


			//printf("Byte sequence not found.\n");
			// 释放内存
			delete[] hyxd_BYTE;
			return -1; // 如果没有找到，返回 -1
			VMProtectEnd();
		}

		inline uint64_t GetMySignOffset() {
			VMProtectBeginMutation("GetMySignOffset");
			const size_t size = 110000000; // 110 MB
			BYTE* hyxd_BYTE = new BYTE[size]; // 动态分配内存
			if (hyxd_BYTE == nullptr) {
				//printf("Memory allocation failed!\n");
				return -1; // 或者其他错误处理
			}


			uint64_t Base = globalVar::mem.GetBaseAddress();
			// 读取字节数据
			if (!globalVar::mem.Read(Base, hyxd_BYTE, size)) {
				//printf("Failed to read memory!\n");
				delete[] hyxd_BYTE; // 释放内存
				return -1; // 或者其他错误处理
			}

			// 要搜索的字节序列
			const BYTE target[] = { 243, 15, 92, 8, 243, 15, 89, 201, 243, 15, 89, 192, 243, 15, 88, 200, 243, 15, 89, 210, 243, 15, 88, 202, 15, 87, 246, 243, 15, 81, 241 };
			const size_t target_size = sizeof(target) / sizeof(target[0]);

			// 在数据中搜索字节序列
			for (size_t i = 0; i <= size - target_size; ++i) {
				if (std::memcmp(&hyxd_BYTE[i], target, target_size) == 0) {

					uint16_t Offset = globalVar::mem.ReadValue<uint16_t>(Base + i + 0x27) + 0x8;
					delete[] hyxd_BYTE; // 释放内存
					return Offset; // 返回找到的起始位置
				}
			}


			//printf("Byte sequence not found.\n");
			// 释放内存
			delete[] hyxd_BYTE;
			return -1; // 如果没有找到，返回 -1
			VMProtectEnd();
		}



		inline uint64_t GetStateOffset() {
			VMProtectBeginMutation("GetStateOffset");
			const size_t size = 110000000; // 110 MB
			BYTE* hyxd_BYTE = new BYTE[size]; // 动态分配内存
			if (hyxd_BYTE == nullptr) {
				//printf("Memory allocation failed!\n");
				return -1; // 或者其他错误处理
			}


			uint64_t Base = globalVar::mem.GetBaseAddress();
			// 读取字节数据
			if (!globalVar::mem.Read(Base, hyxd_BYTE, size)) {
				//printf("Failed to read memory!\n");
				delete[] hyxd_BYTE; // 释放内存
				return -1; // 或者其他错误处理
			}

			// 要搜索的字节序列
			const BYTE target[] = { 72, 139, 200, 72, 139, 216, 73, 59, 7 };
			const size_t target_size = sizeof(target) / sizeof(target[0]);

			// 在数据中搜索字节序列
			for (size_t i = 0; i <= size - target_size; ++i) {
				if (std::memcmp(&hyxd_BYTE[i], target, target_size) == 0) {

					uint16_t Offset = globalVar::mem.ReadValue<uint16_t>(Base + i + 0x2F);
					delete[] hyxd_BYTE; // 释放内存
					return Offset; // 返回找到的起始位置
				}
			}


			//printf("Byte sequence not found.\n");
			// 释放内存
			delete[] hyxd_BYTE;
			return -1; // 如果没有找到，返回 -1
			VMProtectEnd();
		}


		inline uint64_t GetinGameOffset() {
			VMProtectBeginMutation("GetinGameOffset");
			const size_t size = 110000000; // 110 MB
			BYTE* hyxd_BYTE = new BYTE[size]; // 动态分配内存
			if (hyxd_BYTE == nullptr) {
				//printf("Memory allocation failed!\n");
				return -1; // 或者其他错误处理
			}


			uint64_t Base = globalVar::mem.GetBaseAddress();
			// 读取字节数据
			if (!globalVar::mem.Read(Base, hyxd_BYTE, size)) {
				//printf("Failed to read memory!\n");
				delete[] hyxd_BYTE; // 释放内存
				return -1; // 或者其他错误处理
			}

			// 要搜索的字节序列
			const BYTE target[] = { 102, 15, 31, 68, 0 ,0, 72, 139, 20, 24, 72, 141, 64, 8, 72, 255, 2 };
			const size_t target_size = sizeof(target) / sizeof(target[0]);

			// 在数据中搜索字节序列
			for (size_t i = 0; i <= size - target_size; ++i) {
				if (std::memcmp(&hyxd_BYTE[i], target, target_size) == 0) {

					uint32_t relative = globalVar::mem.ReadValue<uint32_t>(Base + i + 0x7B);
					uint32_t Offset = relative + i + 0x7F;
					delete[] hyxd_BYTE; // 释放内存
					return Offset; // 返回找到的起始位置
				}
			}


			//printf("Byte sequence not found.\n");
			// 释放内存
			delete[] hyxd_BYTE;
			return -1; // 如果没有找到，返回 -1
			VMProtectEnd();
		}










	}



	namespace drawCheat {



		inline void DrawLine(float X, float Y, float X2, float Y2, ImColor color, float linewidth)
		{
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X2, Y2), color, linewidth);
		}

		inline void DrawCornerBox(float x, float y, float width, float height, ImColor color, float thickness) {
			// Draw top left corner
			DrawLine(x, y, x + width * 0.33F, y, color, thickness);
			DrawLine(x + width * 0.66F, y, x + width, y, color, thickness);

			// Draw vertical lines (left and right)
			DrawLine(x, y, x, y + height * 0.33F, color, thickness);
			DrawLine(x, y + height * 0.66F, x, y + height, color, thickness);
			DrawLine(x + width, y, x + width, y + height * 0.33F, color, thickness);
			DrawLine(x + width, y + height * 0.66F, x + width, y + height, color, thickness);

			// Draw bottom corners
			DrawLine(x, y + height, x + width * 0.33F, y + height, color, thickness);
			DrawLine(x + width * 0.66F, y + height, x + width, y + height, color, thickness);
		}

		inline void DrawBoneLine(const globalVar::GameData::PlayerData::Player& data, float X, float Y, float X2, float Y2, ImColor color, float linewidth)
		{
			// 计算两点之间的距离（勾股定理）
			float dx = X2 - X;
			float dy = Y2 - Y;
			float D = std::sqrt(dx * dx + dy * dy); // 2D距离公式
			if (!(data.Distance > 50 && D > 200)) {
				DrawLine(X, Y, X2, Y2, color, linewidth);
			}

		}



		inline void DrawBox(float X, float Y, float W, float H, ImColor color, float thickness)
		{
			ImDrawList* drawList = ImGui::GetBackgroundDrawList();
			// 使用 AddRect 函数绘制矩形
			drawList->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), color, 0.0f, 1, thickness);
		}

		inline std::string string_to_utf8(const std::string& str)
		{
			int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t* pwBuf = new wchar_t[nwLen + 1];
			memset(pwBuf, 0, nwLen * 2 + 2);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), pwBuf, nwLen);


			int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
			char* pBuf = new char[nLen + 1];
			memset(pBuf, 0, nLen + 1);
			WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

			std::string ret = pBuf;
			delete[] pwBuf;
			delete[] pBuf;
			return ret;
		}


		inline void DrawImGuiText(float x, float y, ImColor color, const char* fmt, ...)//绘制文本
		{
			static char buf[1024] = { 0 };
			va_list va_alist;
			va_start(va_alist, fmt);
			vsprintf_s(buf, fmt, va_alist);
			va_end(va_alist);
			ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), color, string_to_utf8(buf).c_str());
		}



		// 绘制圆形
		inline void DrawACircle(float Left, float Top, float Radius, ImColor color)
		{
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(Left, Top), Radius, color);
		}


	}

	using namespace globalVar::GameData::PlayerData;

	inline void printMatrix(float mat[4][4]) {
		printf("┌─────────────────────────────────────┐\n");
		for (int i = 0; i < 4; i++) {
			printf("│ ");
			for (int j = 0; j < 4; j++) {
				printf("%8.2f ", mat[i][j]);
			}
			printf(" │\n");
		}
		printf("└─────────────────────────────────────┘\n");
	}


	inline D2DVector matrixTransformScreenCoords(float(&Matrix)[4][4], const globalVar::GameData::PlayerData::D3DVector& player) {



		int w2 = globalVar::GameData::MainData::ScreenX / 2;
		int h2 = globalVar::GameData::MainData::ScreenY / 2;

		float Z = Matrix[3][0] * player.x + Matrix[3][1] * player.y + Matrix[3][2] * player.z + Matrix[3][3];


		if (Z <= 0)
			return { -114514.0F,-114514.0F };
		float proportion = 1 / Z;
		float X = w2 + (Matrix[0][0] * player.x + Matrix[0][1] * player.y + Matrix[0][2] * player.z + Matrix[0][3]) * proportion * w2;
		float Y = h2 - (Matrix[1][0] * player.x + Matrix[1][1] * player.y + Matrix[1][2] * player.z + Matrix[1][3]) * proportion * h2;
		return { X,Y };
	}

	inline float GetPlayerDistance(float(&Matrix)[4][4], const globalVar::GameData::PlayerData::D3DVector& player) {
		float Z = Matrix[3][0] * player.x + Matrix[3][1] * player.y + Matrix[3][2] * player.z + Matrix[3][3];
		return std::round(Z / 3);
	}

	inline float CalculateDistance3D(const globalVar::GameData::PlayerData::D3DVector& a, const globalVar::GameData::PlayerData::D3DVector& b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}


	inline D2DVector BoneToD2D(float(&Matrix)[4][4], const globalVar::GameData::PlayerData::D3DVector& player, const globalVar::GameData::PlayerData::Player::SkeletalMatrix& BoneMatrix) {
		globalVar::GameData::PlayerData::D3DVector Fuck = {};
		Fuck = player;
		Fuck.z = (float)((float)((float)(player.y * BoneMatrix.Matrix_120) + (float)(player.x * BoneMatrix.Matrix_114)) + (float)(player.z * BoneMatrix.Matrix_12C)) + BoneMatrix.Matrix_138;    // 最终算出来是Z坐标
		Fuck.y = (float)((float)((float)(player.y * BoneMatrix.Matrix_11C) + (float)(player.x * BoneMatrix.Matrix_110)) + (float)(player.z * BoneMatrix.Matrix_128)) + BoneMatrix.Matrix_134;    // 最终算出来是Y坐标
		Fuck.x = (float)((float)((float)(player.y * BoneMatrix.Matrix_118) + (float)(player.x * BoneMatrix.Matrix_10C)) + (float)(player.z * BoneMatrix.Matrix_124)) + BoneMatrix.Matrix_130;    // 最终算出来是X坐标


		int w2 = globalVar::GameData::MainData::ScreenX / 2;
		int h2 = globalVar::GameData::MainData::ScreenY / 2;
		float Z = Matrix[3][0] * Fuck.x + Matrix[3][1] * Fuck.y + Matrix[3][2] * Fuck.z + Matrix[3][3];
		if (Z <= 0)
			return { -114514.0F,-114514.0F };
		float proportion = 1 / Z;
		float X = w2 + (Matrix[0][0] * Fuck.x + Matrix[0][1] * Fuck.y + Matrix[0][2] * Fuck.z + Matrix[0][3]) * proportion * w2;
		float Y = h2 - (Matrix[1][0] * Fuck.x + Matrix[1][1] * Fuck.y + Matrix[1][2] * Fuck.z + Matrix[1][3]) * proportion * h2;
		return { X,Y };

	}


	// 定义一个函数来计算两点之间的距离
	inline double calculateDistance(double x1, double y1, double x2, double y2) {
		double dx = x2 - x1; // x 坐标差
		double dy = y2 - y1; // y 坐标差
		return sqrt(dx * dx + dy * dy); // 使用勾股定理计算距离
	}


	inline std::string GetExePath() {
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH); // 获取当前exe路径
		return std::string(buffer);
	}




	inline void mousemove(float tarx, float tary, float X, float Y, int smooth)
	{
		VMProtectBeginMutation("mousemove");
		float ScreenCenterX = (X / 2);
		float ScreenCenterY = (Y / 2);
		float TargetX = 0;
		float TargetY = 0;

		if (tarx != 0)
		{
			if (tarx > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - tarx);
				TargetX /= smooth;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (tarx < ScreenCenterX)
			{
				TargetX = tarx - ScreenCenterX;
				TargetX /= smooth;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}

		if (tary != 0)
		{
			if (tary > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - tary);
				TargetY /= smooth;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (tary < ScreenCenterY)
			{
				TargetY = tary - ScreenCenterY;
				TargetY /= smooth;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		if (globalVar::isSucessfully) {
			drv.Mouse_move(static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), 0);
		}
		else
		{
			(mouse_event)(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), 0, 0);
		}

		VMProtectEnd();
	}


}
