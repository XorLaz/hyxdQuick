#include <d3d11.h>
#include <dwmapi.h>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include "imgui_impl_win32.h"
#include "globalVar.h"
#include <settings.h>
#include <fonts.h>
#include <VMProtectSDK.h>
#pragma comment(lib, "d3d11.lib")


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ImColor BgColor = ImColor(0, 0, 0, 0);



// 生成一个随机字母（大写或小写）
char generate_random_char() {
	VMProtectBeginMutation("generate_random_char");
	// 生成一个随机数，范围在 0 到 51 之间
	int rand_num = std::rand() % 52;
	if (rand_num < 26) {
		// 生成大写字母 A-Z
		return 'A' + rand_num;
	}
	else {
		// 生成小写字母 a-z
		return 'a' + (rand_num - 26);
	}
	VMProtectEnd();
}

// 生成一段随机字母字符串
std::string generate_random_string(uint32_t length) {
	VMProtectBeginMutation("generate_random_string");
	std::string Name;
	Name.reserve(length);
	for (uint32_t index = NULL; index != length; index++) {
		Name  = Name + generate_random_char();
	}
	return Name;
	VMProtectEnd();
}

std::wstring ConvertToWideString(std::string& str)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring result(bufferSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], bufferSize);
	return result;
}


bool ToolCreateWindow(PVOID Call)
{
	
	VMProtectBeginMutation("ToolCreateWindow");
	std::string classname = generate_random_string(8 + std::rand() % 12);
	std::string windowname = generate_random_string(8 + std::rand() % 12);

	std::wstring classNameWide = ConvertToWideString(classname);
	std::wstring windowNameWide = ConvertToWideString(windowname);


	WNDCLASSEXW wc = {
	sizeof(wc),                  // cbSize
	CS_CLASSDC,                  // style
	WndProc,                     // lpfnWndProc
	0L,                          // cbClsExtra
	0L,                          // cbWndExtra
	GetModuleHandle(NULL),       // hInstance
	NULL,                        // hIcon
	NULL,                        // hCursor
	NULL,                        // hbrBackground
	NULL,                        // lpszMenuName
	classNameWide.c_str(),  // lpszClassName
	NULL                         // hIconSm
	};

	globalVar::GameData::MainData::ScreenX = GetSystemMetrics(SM_CXSCREEN);
	globalVar::GameData::MainData::ScreenY = GetSystemMetrics(SM_CYSCREEN);


	RegisterClassExW(&wc);
	// 移除 WS_EX_TRANSPARENT，保持透明窗口
	globalVar::imguiPtr::hwndOverlay = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED, classNameWide.c_str(), windowNameWide.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, globalVar::GameData::MainData::ScreenX, globalVar::GameData::MainData::ScreenY, NULL, NULL, GetModuleHandle(NULL), NULL);
	SetLayeredWindowAttributes(globalVar::imguiPtr::hwndOverlay, 0, 255, LWA_ALPHA);
	
	//SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLongA(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	//SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), NULL, LWA_COLORKEY);


	//创建设备
	//设置窗口背景透明

	POINT Point{};
	RECT Rect{};
	HWND deskhWnd = GetDesktopWindow();
	GetClientRect(deskhWnd, &Rect);
	ClientToScreen(deskhWnd, &Point);


	int random = rand() % 20;
	SetWindowPos(globalVar::imguiPtr::hwndOverlay, HWND_TOPMOST, (int)Point.x + 1, (int)Point.y + 1, (int)Rect.right - random, (int)Rect.bottom - random, SWP_SHOWWINDOW);
	MARGINS     Margin = { -1 };
	DwmExtendFrameIntoClientArea(globalVar::imguiPtr::hwndOverlay, &Margin);

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = globalVar::imguiPtr::hwndOverlay;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	D3D_FEATURE_LEVEL featureLevel;
	UINT createDeviceFlags = 0;
	HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &globalVar::imguiPtr::pSwapChain, &globalVar::imguiPtr::pDevice, &featureLevel, &globalVar::imguiPtr::pContext);
	if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
		res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &globalVar::imguiPtr::pSwapChain, &globalVar::imguiPtr::pDevice, &featureLevel, &globalVar::imguiPtr::pContext);
	if (res != S_OK)
		return 0;



	//初始化IMGUI
	ID3D11Texture2D* pBackBuffer;
	globalVar::imguiPtr::pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer == nullptr)
		return false;
	globalVar::imguiPtr::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &globalVar::imguiPtr::pRenderTargetView);
	pBackBuffer->Release();
	ShowWindow(globalVar::imguiPtr::hwndOverlay, SW_SHOWDEFAULT);
	UpdateWindow(globalVar::imguiPtr::hwndOverlay);
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	ImGui::StyleColorsLight();
	ImGui_ImplWin32_Init(globalVar::imguiPtr::hwndOverlay);
	ImGui_ImplDX11_Init(globalVar::imguiPtr::pDevice, globalVar::imguiPtr::pContext);

	fonts::inter_font = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 17, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::inter_font_b = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 18.5f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::inter_bold_font = io.Fonts->AddFontFromMemoryTTF(&inter_bold, sizeof inter_bold, 20, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::inter_bold_font2 = io.Fonts->AddFontFromMemoryTTF(&inter_bold, sizeof inter_bold, 17, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::inter_bold_font3 = io.Fonts->AddFontFromMemoryTTF(&inter_bold, sizeof inter_bold, 18, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::inter_bold_font4 = io.Fonts->AddFontFromMemoryTTF(&inter_bold, sizeof inter_bold, 16, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::combo_icon_font = io.Fonts->AddFontFromMemoryTTF(&combo_icon, sizeof combo_icon, 15, NULL, io.Fonts->GetGlyphRangesChineseFull());
	fonts::weapon_font = io.Fonts->AddFontFromMemoryTTF(&weapon, sizeof weapon, 15, NULL, io.Fonts->GetGlyphRangesChineseFull());

	ImGui::GetIO().Fonts->AddFontFromFileTTF(VMProtectDecryptStringA("C:\\Windows\\Fonts\\msyh.ttc"), 11.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
	SetLayeredWindowAttributes(globalVar::imguiPtr::hwndOverlay, 0, 255, LWA_ALPHA);
	
	//消息循环
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		((void(*)(void))Call)();
		ImGui::EndFrame();
		ImGui::Render();
		//调用call

		const float clear_color_with_alpha[4] = { BgColor.Value.x, BgColor.Value.y , BgColor.Value.z, BgColor.Value.w };
		globalVar::imguiPtr::pContext->OMSetRenderTargets(1, &globalVar::imguiPtr::pRenderTargetView, NULL);

		globalVar::imguiPtr::pContext->ClearRenderTargetView(globalVar::imguiPtr::pRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		globalVar::imguiPtr::pSwapChain->Present(features::VerticalSync, 0);
	}

	return true;
	VMProtectEnd();
}



LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	VMProtectBeginMutation("WndProc");
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
	VMProtectEnd();
}