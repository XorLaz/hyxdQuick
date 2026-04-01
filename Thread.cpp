#include <imgui.h>
#include <D3DX11.h>
#include <settings.h>
#include <fonts.h>
#include <images.h>
#include <imgui_internal.h>
#include <imgui_combo.hpp>
#include <vector>
#include <iostream>
#include <iomanip>
#include <VMProtectSDK.h>
#include "Thread.h"
#include "globalVar.h"
#include "hmemory.h"
#include <unordered_set>
#include <unordered_map>
#include "driver.h"


using namespace globalVar::GameData;
std::mutex data_mutex;
std::shared_ptr<std::vector<addressArray::Data_Address>> latest_batch;  // 始终保存最新一批数据
std::unordered_map<uint64_t, size_t> IEntityIndexMap;
std::unordered_map<uint64_t, size_t> actorIndexMap;
std::atomic<bool> initial_data_ready{ false };  // 首次数据就绪标志

PlayerData::D2DVector LineAimPos = { 0,0 };


void Thread::DrawThread::Menu::Menu() {
	VMProtectBeginMutation("Thread::DrawThread::Menu::Menu");
	D3DX11_IMAGE_LOAD_INFO image; ID3DX11ThreadPump* pump{ nullptr };
	if (pictures::logo_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &logo, sizeof(logo), &image, pump, &pictures::logo_img, 0);
	if (pictures::aim_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &aim, sizeof(aim), &image, pump, &pictures::aim_img, 0);
	if (pictures::misc_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &other, sizeof(other), &image, pump, &pictures::misc_img, 0);
	if (pictures::visual_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &visual, sizeof(visual), &image, pump, &pictures::visual_img, 0);
	if (pictures::pen_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &pen, sizeof(pen), &image, pump, &pictures::pen_img, 0);
	if (pictures::world_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, &world, sizeof(world), &image, pump, &pictures::world_img, 0);
	if (pictures::settings_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, setting, sizeof(setting), &image, pump, &pictures::settings_img, 0);
	if (pictures::keyboard_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, keyboard, sizeof(keyboard), &image, pump, &pictures::keyboard_img, 0);
	if (pictures::input_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, inputimg, sizeof(inputimg), &image, pump, &pictures::input_img, 0);
	if (pictures::wat_logo_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, wat, sizeof(wat), &image, pump, &pictures::wat_logo_img, 0);
	if (pictures::fps_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, fps, sizeof(fps), &image, pump, &pictures::fps_img, 0);
	if (pictures::player_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, player, sizeof(player), &image, pump, &pictures::player_img, 0);
	if (pictures::time_img == nullptr) D3DX11CreateShaderResourceViewFromMemory(globalVar::imguiPtr::pDevice, time_ico, sizeof(time_ico), &image, pump, &pictures::time_img, 0);


	{
		ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_Once);
		ImGui::SetNextWindowSize(settings::size_menu);
		ImGui::Begin(VMProtectDecryptStringA("hyxdQuick"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
		const auto& pos = ImGui::GetWindowPos();   //获取窗口位置
		const auto& draw_list = ImGui::GetWindowDrawList();   //获取绘制列表
		ImGuiStyle* style = &ImGui::GetStyle();    //获取风格
		style->Colors[ImGuiCol_WindowBg] = colors::menu::window_bg;
		style->Colors[ImGuiCol_Border] = colors::menu::border;
		style->ItemSpacing = ImVec2(0, 5);
		style->WindowPadding = ImVec2(0, 0);
		style->WindowRounding = 8.f;

		{
			ImGui::SetCursorPos(ImVec2(10, 10));

			ImGui::BeginChild(VMProtectDecryptStringA("General Tabs"), ImVec2(620, 60), true, ImGuiWindowFlags_NoBackground);

			const auto& pos = ImGui::GetWindowPos();
			const auto& draw_list = ImGui::GetWindowDrawList();

			draw_list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 620, pos.y + 60), ImGui::GetColorU32(menu::general_child), 10.f);
			draw_list->AddImage(pictures::logo_img, ImVec2(pos.x + 20, pos.y + 18), ImVec2(pos.x + 44, pos.y + 42), ImVec2(0, 0), ImVec2(1, 1), ImColor(colors::logo_color));
			draw_list->AddText(fonts::inter_bold_font, 20.f, ImVec2(pos.x + 53, pos.y + 20), ImColor(255, 255, 255), VMProtectDecryptStringA("hyxdQuick"));

			// 上半部分渐变（从透明到实色）
			draw_list->AddRectFilledMultiColor(
				ImVec2(pos.x + 150, pos.y + 15),  // 矩形左上角坐标
				ImVec2(pos.x + 151.5f, pos.y + 30), // 矩形右下角坐标（宽度仅1.5像素）
				ImGui::GetColorU32(colors::accent_color, 0), // 左上角颜色（透明）
				ImGui::GetColorU32(colors::accent_color, 0), // 右上角颜色（透明）
				ImGui::GetColorU32(colors::accent_color),   // 右下角颜色（实色）
				ImGui::GetColorU32(colors::accent_color)    // 左下角颜色（实色）
			);

			// 下半部分渐变（从实色到透明）
			draw_list->AddRectFilledMultiColor(
				ImVec2(pos.x + 150, pos.y + 30),  // 矩形左上角坐标
				ImVec2(pos.x + 151.5f, pos.y + 45), // 矩形右下角坐标（宽度仅1.5像素）
				ImGui::GetColorU32(colors::accent_color),   // 左上角颜色（实色）
				ImGui::GetColorU32(colors::accent_color),   // 右上角颜色（实色）
				ImGui::GetColorU32(colors::accent_color, 0), // 右下角颜色（透明）
				ImGui::GetColorU32(colors::accent_color, 0)  // 左下角颜色（透明）
			);


			{
				{
					if (misc::tab_count == 1) {
						misc::tab_width = 92;
					}
					else if (misc::tab_count == 2) {
						misc::tab_width = 90;
					}
					else if (misc::tab_count == 3) {
						misc::tab_width = 89;
					}
					else if (misc::tab_count == 4) {
						misc::tab_width = 90;
					}
					else if (misc::tab_count == 5) {
						misc::tab_width = 92;
					}
				}
				misc::anim_tab = ImLerp(misc::anim_tab, (float)(misc::tab_count * misc::tab_width), ImGui::GetIO().DeltaTime * 15.f);
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + 160 + misc::anim_tab, pos.y + 12), ImVec2(pos.x + 250 + misc::anim_tab, pos.y + 48), ImColor(25, 25, 25), 6);
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + 175 + misc::anim_tab, pos.y + 57), ImVec2(pos.x + 235 + misc::anim_tab, pos.y + 60), ImColor(colors::accent_color), 10, ImDrawCornerFlags_Top);
				ImGui::GetWindowDrawList()->AddShadowRect(ImVec2(pos.x + 175 + misc::anim_tab, pos.y + 57), ImVec2(pos.x + 235 + misc::anim_tab, pos.y + 60), ImGui::GetColorU32(colors::accent_color), 10.f, ImVec2(0, 0), ImDrawCornerFlags_Top, 10.f);


				ImGui::SetCursorPos(ImVec2(155, 12));

				ImGui::BeginGroup();
				{


					if (ImGui::Tab(VMProtectDecryptStringA("Visuals"), pictures::visual_img, ImVec2(95, 40), 0 == misc::tab_count))
						misc::tab_count = 0;

					ImGui::SameLine();

					if (ImGui::Tab(VMProtectDecryptStringA("Aimbot"), pictures::aim_img, ImVec2(95, 40), 1 == misc::tab_count))
						misc::tab_count = 1;

				}
				ImGui::EndGroup();
			}


			ImGui::EndChild();

		}




		{
			misc::alpha_child = ImLerp(misc::alpha_child, (misc::tab_count == misc::active_tab_count) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
			if (misc::alpha_child < 0.01f && misc::child_add < 0.01f) misc::active_tab_count = misc::tab_count;

			ImGui::SetCursorPos(ImVec2(10, 80));
			ImGui::BeginChild(VMProtectDecryptStringA("Main"), ImVec2(725, 440), true, ImGuiWindowFlags_NoBackground);


			ImGui::SetCursorPos(ImVec2(0, 100 - (misc::alpha_child * 100)));

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, misc::alpha_child * style->Alpha);

			if (misc::active_tab_count == 0)    // 视觉选项卡
			{
				ImGui::BeginGroup();
				{
					ImGui::BeginChildCustom(pictures::visual_img, VMProtectDecryptStringA("ESP"), ImVec2(304, 240), false, ImGuiWindowFlags_NoScrollWithMouse);
					{
						ImGui::Checkbox(VMProtectDecryptStringA("   ESP View Window"), &features::esp_perview);

						combo::MultiCombo(VMProtectDecryptStringA("   ESP Choose"), features::multi_esp, features::multi_preview, 4);

						ImGui::BeginGroup();
						combo::Combo(VMProtectDecryptStringA("   Color Setting"), &features::ColorSettingCurrent, features::ColorSettingList, IM_ARRAYSIZE(features::ColorSettingList), 1);
						if (features::ColorSettingCurrent == 0) {
							ImGui::ColorEdit4(VMProtectDecryptStringA("Box Color"), (float*)&colors::BoxColors, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
						}
						if (features::ColorSettingCurrent == 1) {
							ImGui::ColorEdit4(VMProtectDecryptStringA("Line Color"), (float*)&colors::LineColors, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
						}
						if (features::ColorSettingCurrent == 2) {
							ImGui::ColorEdit4(VMProtectDecryptStringA("Distance Color"), (float*)&colors::DistanceColors, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
						}
						if (features::ColorSettingCurrent == 3) {
							ImGui::ColorEdit4(VMProtectDecryptStringA("Bone Color"), (float*)&colors::BoneColors, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
						}
						ImGui::EndGroup();
					}
					ImGui::EndChildCustom();

				} ImGui::EndGroup();

				ImGui::SameLine(0, 10.f);


				ImGui::BeginGroup();
				{
					ImGui::BeginChildCustom(pictures::aim_img, VMProtectDecryptStringA("ESP Setting"), ImVec2(304, 260), false, ImGuiWindowFlags_NoScrollWithMouse);
					{
						ImGui::Checkbox(VMProtectDecryptStringA("   Team ESP"), &features::esp_Team);
						ImGui::Checkbox(VMProtectDecryptStringA("   Vertical Sync"), &features::VerticalSync);
						ImGui::Checkbox(VMProtectDecryptStringA("   Anti Screenshot"), &features::AntiScreenshot);
					}
					ImGui::EndChildCustom();

				} ImGui::EndGroup();

			}

			if (misc::active_tab_count == 1)    // 瞄准选项卡
			{

				ImGui::BeginGroup();
				{
					ImGui::BeginChildCustom(pictures::aim_img, VMProtectDecryptStringA("Aim"), ImVec2(304, 300), false, ImGuiWindowFlags_NoScrollWithMouse);
					{
						ImGui::Checkbox(VMProtectDecryptStringA("   AimBot"), &features::Aim);
						ImGui::Checkbox(VMProtectDecryptStringA("   DrawRange"), &features::DrawRange);
						ImGui::Keybind(pictures::keyboard_img, "   Aim key", &features::key, &features::mind, true);
						combo::Combo(VMProtectDecryptStringA("   Aim Pos"), &features::AimPosCurrent, features::AimPosList, IM_ARRAYSIZE(features::AimPosList), 1);
						ImGui::SliderInt(VMProtectDecryptStringA("   Aim Range"), &features::AimRange, 0, 1000);
						ImGui::SliderFloat(VMProtectDecryptStringA("   Smooth Speed"), &features::SmoothSpeed, 0, 100);


					}
					ImGui::EndChildCustom();

				} ImGui::EndGroup();

				ImGui::SameLine(0, 10.f);


				ImGui::BeginGroup();
				{
					ImGui::BeginChildCustom(pictures::aim_img, VMProtectDecryptStringA("Aim Setting"), ImVec2(304, 290), false, ImGuiWindowFlags_NoScrollWithMouse);
					{
						if (features::DrawRange)
							ImGui::ColorEdit4(VMProtectDecryptStringA("Range Color"), (float*)&colors::RangeColors, ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
					}
					ImGui::EndChildCustom();

				} ImGui::EndGroup();


			}



			ImGui::PopStyleVar();
			ImGui::Spacing();
			ImGui::EndChild();
		}


		// 获取窗口大小和位置
		const auto& window_pos = ImGui::GetWindowPos();
		const auto& window_size = ImGui::GetWindowSize();
		// 计算左下角位置（留 5px 边距）
		const float text_pos_x = window_pos.x + 10;  // 左侧留 10px 边距
		const float text_pos_y = window_pos.y + window_size.y - 20; // 底部留 20px 空间
		ImGui::GetFont()->Scale = 0.8f; // 临时缩小字体
		ImGui::PushFont(ImGui::GetFont());
		ImGui::GetWindowDrawList()->AddText(ImVec2(text_pos_x, text_pos_y), ImColor(150, 150, 150, 200), ExpirationTime.c_str());
		ImGui::PopFont();
		ImGui::GetFont()->Scale = 1.0f; // 恢复
		ImGui::End();



	}






	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	{
		features::preview_alpha = ImClamp(features::preview_alpha + (4.f * ImGui::GetIO().DeltaTime * (features::esp_perview ? 1.f : -1.f)), 0.f, 1.f);
		ImGuiStyle* style = &ImGui::GetStyle();
		ImGuiContext& g = *GImGui;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, features::preview_alpha* style->Alpha);     //开启压入风格
		ImGui::SetNextWindowPos(ImVec2(1000, 253), ImGuiCond_Once);   //设置窗口位置  
		ImGui::SetNextWindowSize(settings::size_preview);    //设置窗口大小

		if (features::esp_perview)    //是否显示绘制视图窗口
		{
			ImGui::Begin(VMProtectDecryptStringA("Esp Preview Window"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);

			const auto& pos = ImGui::GetWindowPos();
			const auto& draw_list = ImGui::GetWindowDrawList();

			{
				if (features::multi_esp[0] == true) {

					draw_list->AddRect(ImVec2(pos.x + 50, pos.y + 50), ImVec2(pos.x + 250, pos.y + 350), ImGui::GetColorU32(colors::BoxColors), 4.f);

				}

				if (features::multi_esp[1] == true) {
					const auto& size = ImGui::GetWindowSize();
					draw_list->AddLine(ImVec2(size.x / 2, 0), ImVec2(pos.x + 160, pos.y + 50), ImGui::GetColorU32(colors::LineColors));
				}

				if (features::multi_esp[2] == true) {

					draw_list->AddText(fonts::inter_bold_font2, 17.f, ImVec2(pos.x + 260, pos.y + 52), ImGui::GetColorU32(colors::DistanceColors), VMProtectDecryptStringA("114m"));
				}


				if (features::multi_esp[3] == true) {


					draw_list->AddCircleFilled(ImVec2(pos.x + 152, pos.y + 105), 11.f, ImGui::GetColorU32(colors::BoneColors), 20.f);

					draw_list->AddShadowCircle(ImVec2(pos.x + 152, pos.y + 105), 11.f, ImGui::GetColorU32(colors::BoneColors), 25.f, ImVec2(0, 0));

					draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 135), ImVec2(pos.x + 152, pos.y + 240), ImGui::GetColorU32(colors::BoneColors));

					// left hand

					draw_list->AddLine(ImVec2(pos.x + 100, pos.y + 180), ImVec2(pos.x + 152, pos.y + 135), ImGui::GetColorU32(colors::BoneColors));

					// right hand

					draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 135), ImVec2(pos.x + 204, pos.y + 180), ImGui::GetColorU32(colors::BoneColors));

					// left leg

					draw_list->AddLine(ImVec2(pos.x + 100, pos.y + 295), ImVec2(pos.x + 152, pos.y + 240), ImGui::GetColorU32(colors::BoneColors));

					// right leg

					draw_list->AddLine(ImVec2(pos.x + 152, pos.y + 240), ImVec2(pos.x + 204, pos.y + 295), ImGui::GetColorU32(colors::BoneColors));


				}


			}

			ImGui::End();  //窗口绘制结束
		}
		ImGui::PopStyleVar();   //结束压入风格
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	VMProtectEnd();



}

void Thread::DrawThread::Function::Function() {
	VMProtectBeginMutation("Thread::DrawThread::Function::Function");
	for (auto& data : *RenderArray) {
		if (globalVar::isInGame == 0)
		{

			//globalFun::drawCheat::DrawImGuiText(data.Rect.X, data.Rect.Y, ImColor(255, 255, 0, 255), "%d", data.State);
		    //globalFun::drawCheat::DrawImGuiText(data.Rect.X, data.Rect.Y + data.Rect.H + 10, ImColor(255, 0, 0, 255), "%llX", data.IEntity);
		   // globalFun::drawCheat::DrawImGuiText(10, 500, ImColor(255, 0, 0, 255), "%llX", LocalPlayerData::LocalAddressPtr);

			if (features::DrawRange) {
				globalFun::drawCheat::DrawACircle(MainData::ScreenX / 2, MainData::ScreenY / 2, features::AimRange, colors::RangeColors);
			}
			
			if (features::Aim && LineAimPos.x > 0 && LineAimPos.y > 0) {
				globalFun::drawCheat::DrawLine(MainData::ScreenX / 2, MainData::ScreenY / 2, LineAimPos.x, LineAimPos.y, ImColor(0, 255, 0), 1);
				if (GetAsyncKeyState(features::key)!=0x0)
				{
					globalFun::drawCheat::DrawLine(MainData::ScreenX / 2, MainData::ScreenY / 2, LineAimPos.x, LineAimPos.y, ImColor(255, 0, 0), 1);
				}
			}

			// 34正常活动 35跳伞中 26倒地
			// 跳过无效数据
			if (data.Position_Pos.x < -10000.0F || data.Position_Pos.y < -10000.0F) {
				continue;
			}

			if (data.State <= 25 || data.State >= 60) {
				continue;
			}


			if (!features::esp_Team) {
				if (data.Camp == 2139063679) {
					continue;
				}
			}


			if (features::multi_esp[0]) {
				globalFun::drawCheat::DrawCornerBox(data.Rect.X, data.Rect.Y, data.Rect.W, data.Rect.H, colors::BoxColors, 2);
			}
			if (features::multi_esp[1]) {
				globalFun::drawCheat::DrawLine(MainData::ScreenX / 2, 0, data.Position_Pos.x, data.Position_Pos.y - data.Rect.H, colors::LineColors, 1);
			}

			if (features::multi_esp[2]) {
				globalFun::drawCheat::DrawImGuiText(data.Rect.X + data.Rect.W, data.Rect.Y, colors::DistanceColors, VMProtectDecryptStringA("M:%.0f"), data.Distance);
			}
			if (features::multi_esp[3]) {


				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedHead].x, data.BonePosition_Pos[BipedHead].y, data.BonePosition_Pos[BipedNeck].x, data.BonePosition_Pos[BipedNeck].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedNeck].x, data.BonePosition_Pos[BipedNeck].y, data.BonePosition_Pos[BipedLUpperArm].x, data.BonePosition_Pos[BipedLUpperArm].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedNeck].x, data.BonePosition_Pos[BipedNeck].y, data.BonePosition_Pos[BipedRUpperArm].x, data.BonePosition_Pos[BipedRUpperArm].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedLUpperArm].x, data.BonePosition_Pos[BipedLUpperArm].y, data.BonePosition_Pos[BipedLForearm].x, data.BonePosition_Pos[BipedLForearm].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedLForearm].x, data.BonePosition_Pos[BipedLForearm].y, data.BonePosition_Pos[BipedLHand].x, data.BonePosition_Pos[BipedLHand].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedRUpperArm].x, data.BonePosition_Pos[BipedRUpperArm].y, data.BonePosition_Pos[BipedRForearm].x, data.BonePosition_Pos[BipedRForearm].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedRForearm].x, data.BonePosition_Pos[BipedRForearm].y, data.BonePosition_Pos[BipedRHand].x, data.BonePosition_Pos[BipedRHand].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedNeck].x, data.BonePosition_Pos[BipedNeck].y, data.BonePosition_Pos[BipedSpine1].x, data.BonePosition_Pos[BipedSpine1].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedSpine1].x, data.BonePosition_Pos[BipedSpine1].y, data.BonePosition_Pos[BipedSpine].x, data.BonePosition_Pos[BipedSpine].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedSpine].x, data.BonePosition_Pos[BipedSpine].y, data.BonePosition_Pos[BipedLThigh].x, data.BonePosition_Pos[BipedLThigh].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedSpine].x, data.BonePosition_Pos[BipedSpine].y, data.BonePosition_Pos[BipedRThigh].x, data.BonePosition_Pos[BipedRThigh].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedLThigh].x, data.BonePosition_Pos[BipedLThigh].y, data.BonePosition_Pos[BipedLCalf].x, data.BonePosition_Pos[BipedLCalf].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedLCalf].x, data.BonePosition_Pos[BipedLCalf].y, data.BonePosition_Pos[BipedLFoot].x, data.BonePosition_Pos[BipedLFoot].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedRThigh].x, data.BonePosition_Pos[BipedRThigh].y, data.BonePosition_Pos[BipedRCalf].x, data.BonePosition_Pos[BipedRCalf].y, colors::BoneColors, 1);
				globalFun::drawCheat::DrawBoneLine(data, data.BonePosition_Pos[BipedRCalf].x, data.BonePosition_Pos[BipedRCalf].y, data.BonePosition_Pos[BipedRFoot].x, data.BonePosition_Pos[BipedRFoot].y, colors::BoneColors, 1);

				float HeadRadius = globalFun::calculateDistance(data.BonePosition_Pos[BipedHead].x, data.BonePosition_Pos[BipedHead].y, data.HeadMiddlePosition_Pos.x, data.HeadMiddlePosition_Pos.y);
				
				globalFun::drawCheat::DrawACircle(data.HeadMiddlePosition_Pos.x, data.HeadMiddlePosition_Pos.y, HeadRadius, colors::BoneColors);

			}



		}

	}

	VMProtectEnd();
}



void Thread::EndThread::EndSoftWare() {
	VMProtectBeginMutation("Thread::EndThread::EndSoftWare");
	while (true) {

		if (GetAsyncKeyState(VK_END) & 0x8000) { // 检测End键
			//std::cout << "End键按下，退出程序" << std::endl;
			exit(0); // 直接退出程序
		}

		if ((GetAsyncKeyState(VK_HOME) & 0x8000) || (GetAsyncKeyState(VK_INSERT) & 0x8000)) { // 检测home键 和 ins键
			globalVar::g_bMenuVisible = !globalVar::g_bMenuVisible;

		}


		Sleep(100); // 避免CPU占用过高

	}

	VMProtectEnd();

}
void Thread::DrawThread::Loop::Loop() {
	VMProtectBeginMutation("Thread::DrawThread::Loop::Loop");
	if (globalVar::g_bMenuVisible) {
		// 菜单显示时，不让鼠标穿透，窗口可以接收到鼠标点击
		SetWindowLong(globalVar::imguiPtr::hwndOverlay, GWL_EXSTYLE, GetWindowLong(globalVar::imguiPtr::hwndOverlay, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
		DrawThread::Menu::Menu();
	}
	else {
		// 菜单隐藏时，启用鼠标穿透，窗口不接收到鼠标点击，事件穿透到下方窗口
		SetWindowLong(globalVar::imguiPtr::hwndOverlay, GWL_EXSTYLE, GetWindowLong(globalVar::imguiPtr::hwndOverlay, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
	}
	DrawThread::Function::Function();

	VMProtectEnd();
}
void Thread::GetData::Thread_ReadView() {
	VMProtectBeginMutation("Thread::GetData::Thread_ReadView");
	
	while (true) {
		if (MainData::ViewAddress >= 0x100000) {
			globalVar::mem.Read(MainData::ViewAddress, (void*)&MainData::Mx[0][0], 64);
		}
		Sleep(1);

	}

	VMProtectEnd();
}
void Thread::GetData::Thread_GetAddress() {
	VMProtectBeginMutation("Thread::GetData::Thread_GetAddress");
	do {
		globalVar::mem.ChangeProessID(globalVar::mem.GetGameProcessID());
		// 等待条件满足
	} while (globalVar::mem.GetBaseAddress() <= 0x10000);

	do {

		Offset::View = globalFun::GetBaseAddress::GetViewCodeAddress();   // Get View Base    Mother Fuck
		//std::cout << "View Base Address: " << Offset::View << std::endl;
		Offset::Count = globalFun::GetBaseAddress::GetPlayerCountCodeAddress();    // Get Player Count Base
		//std::cout << "Player Count Base Address: " << Offset::Count << std::endl;
		Offset::Player::MySignName = globalFun::GetBaseAddress::GetMySignOffset();
		//std::cout << "MySignName Offset: " << Offset::Player::MySignName << std::endl;
		Offset::Player::StateLayer = globalFun::GetBaseAddress::GetStateOffset();
		//std::cout << "StateLayer Offset: " << Offset::Player::StateLayer << std::endl;
		Offset::inGame = globalFun::GetBaseAddress::GetinGameOffset();
		//std::cout << "inGame Offset: " << Offset::inGame << std::endl;

		MainData::hWnd = globalFun::GetGameHWND::PIDGetProcessHWND(globalVar::mem.GetGameProcessID());

	} while (Offset::View <=0 || Offset::Count<=0 || Offset::Player::MySignName<=0 || Offset::Player::StateLayer<=0 || Offset::inGame<=0);


	addressArray::Data_Address fuck;

	// 用于快速查找玩家在数组中的位置

	while (true) {
		//printf("Thread_GetAddress running...");

		if (features::AntiScreenshot) {
			if (globalVar::isSucessfully) {
				drv.Protect_sprite_content((u64)globalVar::imguiPtr::hwndOverlay, 17);
				drv.Protect_sprite_content((u64)globalVar::imguiPtr::hwndOverlay, 1);
			}
			else {
				SetWindowDisplayAffinity(globalVar::imguiPtr::hwndOverlay, 17);
				SetWindowDisplayAffinity(globalVar::imguiPtr::hwndOverlay, 1);
			}
		}
		else
		{
			if (globalVar::isSucessfully) {
				drv.Protect_sprite_content((u64)globalVar::imguiPtr::hwndOverlay, 0);
			}
			else {
				SetWindowDisplayAffinity(globalVar::imguiPtr::hwndOverlay, 0);
			}
		}
		if (globalVar::isSucessfully)
			drv.QSDeleteFileEx(hyxdQuickFile.c_str());
		


		//主模块
		MainData::hyxdAddress = globalVar::mem.GetBaseAddress();
		MainData::hyxd = globalVar::mem.ReadValue<uint64_t>(MainData::hyxdAddress);

		//矩阵地址
		MainData::ViewAddress = globalVar::mem.ReadValue<uint64_t>(MainData::hyxdAddress + Offset::View) + Offset::View_1;

		//玩家数量
		MainData::CountAddress_1 = globalVar::mem.ReadValue<uint64_t>(MainData::hyxdAddress + Offset::Count);
		MainData::CountAddress_2 = globalVar::mem.ReadValue<uint64_t>(MainData::CountAddress_1 + Offset::Count_1);
		MainData::CountAddress_3 = globalVar::mem.ReadValue<uint64_t>(MainData::CountAddress_2 + Offset::Count_2);
		MainData::CountAddress_4 = globalVar::mem.ReadValue<uint64_t>(MainData::CountAddress_3 + Offset::Count_3);
		MainData::CountAddress_5 = globalVar::mem.ReadValue<uint64_t>(MainData::CountAddress_4 + Offset::Count_4);
		MainData::CountAddress_6 = globalVar::mem.ReadValue<uint64_t>(MainData::CountAddress_5 + Offset::Count_5);
		MainData::CountAddress = MainData::CountAddress_6 + Offset::Count_6;
		MainData::Count = globalVar::mem.ReadValue<uint32_t>(MainData::CountAddress);
		if (MainData::Count > 10000)
			continue;

		//玩家数组
		MainData::PlayerArrayInitAddress = MainData::CountAddress - 0x8;
		MainData::PlayerArrayInit = globalVar::mem.ReadValue<uint64_t>(MainData::PlayerArrayInitAddress);

		

		for (DWORD index = NULL; index != MainData::Count; index++) {
			uint64_t RotatableController = globalVar::mem.ReadValue<uint64_t>(MainData::PlayerArrayInit + index * 8);
			uint64_t ControllerBehaviorCallback = globalVar::mem.ReadValue<uint64_t>(RotatableController + Offset::Player::ControllerBehaviorCallback);
			uint64_t CharCtrlComponent = globalVar::mem.ReadValue<uint64_t>(ControllerBehaviorCallback + Offset::Player::CharCtrlComponent);
			uint64_t IEntity = globalVar::mem.ReadValue<uint64_t>(CharCtrlComponent + Offset::Player::IEntity);
			uint64_t IEntity_2 = globalVar::mem.ReadValue<uint64_t>(IEntity + Offset::Player::IEntity_2);
			uint64_t ActorComponent = globalVar::mem.ReadValue<uint64_t>(IEntity_2 + Offset::Player::ActorComponent);
			uint64_t Actor = globalVar::mem.ReadValue<uint64_t>(ActorComponent + Offset::Player::Actor);

			fuck.IEntity = IEntity;
			fuck.Actor = Actor;

			// 那么好 这一步我就要开始识别自己
			std::string isMySignName = "";
			isMySignName.resize(13);  //player_avatar
			globalVar::mem.Read(Actor + Offset::Player::MySignName, (void*)isMySignName.c_str(), 13);
			if (strcmp(isMySignName.c_str(), VMProtectDecryptStringA("player_avatar")) == 0) {  //然后他妈的识别为本人 
				LocalPlayerData::LocalAddressPtr = IEntity;    // IEntity
				LocalPlayerData::Camp = globalVar::mem.ReadValue<uint32_t>(LocalPlayerData::LocalAddressPtr + Offset::Player::Camp);
				LocalPlayerData::LocalPositionAddress = LocalPlayerData::LocalAddressPtr + Offset::Player::Position;
				globalVar::mem.Read(LocalPlayerData::LocalPositionAddress, (void*)&LocalPlayerData::LocalPosition.x, 12);
			}
			else
			{
				fuck.CampAddress = IEntity + Offset::Player::Camp;
				fuck.positionAddress = Actor + Offset::Player::Position;
				fuck.StateLayer = Actor + Offset::Player::StateLayer;
				fuck.StateAddress = globalVar::mem.ReadValue<uint64_t>(fuck.StateLayer) + Offset::Player::Stater;
				fuck.PoseAddress = Actor + Offset::Player::Pose;
				fuck.Pose = globalVar::mem.ReadValue<uint64_t>(fuck.PoseAddress);
				fuck.BoneHeadAddress = fuck.Pose + Offset::Player::BoneHead;
				fuck.BoneEndAddress = fuck.Pose + Offset::Player::BoneEnd;
				fuck.BoneHead = globalVar::mem.ReadValue<uint64_t>(fuck.BoneHeadAddress);
				fuck.BoneEnd = globalVar::mem.ReadValue<uint64_t>(fuck.BoneEndAddress);

				uint32_t SkeletonCount = (fuck.BoneEnd - fuck.BoneHead) / Offset::Player::BoneMemorySize;   //计算骨骼数量 用于循环

				if (SkeletonCount > 100000)
					SkeletonCount = 5000;
				for (DWORD Boneindex = NULL; Boneindex != SkeletonCount; Boneindex++) {

					uint64_t Skeleton = fuck.BoneHead + 0x88;   // 第一个骨骼的尾部指针 因为他妈的第一个骨骼点 是没有用的
					uint64_t CurrentBonePtr = Skeleton + Boneindex * Offset::Player::BoneMemorySize;   // 获取的是每一个骨骼点的尾部指针的（地址！！！）
					uint64_t CurrentBonePtr_Value = globalVar::mem.ReadValue<uint64_t>(CurrentBonePtr); // 获取的是每一个骨骼点的尾部指针

					uint64_t BonePositionAddress = CurrentBonePtr - 0x34;

					uint64_t BoneNamePtr = globalVar::mem.ReadValue<uint64_t>(CurrentBonePtr_Value + Offset::Player::BoneNamePtr);   // 用尾部指针加上骨骼名称的偏移 就是骨骼名称指针
					uint32_t BoneNameLength = globalVar::mem.ReadValue<uint32_t>(BoneNamePtr + Offset::Player::BoneNameLength);   // 骨骼名称的指针 加上 骨骼长度偏移 获取到傻逼名称长度
					if (BoneNameLength > 30)
						continue;
					std::string BoneName = "";
					BoneName.resize(BoneNameLength);  // 然后我定义了一个字符串并且指定了长度
					globalVar::mem.Read(BoneNamePtr + Offset::Player::BoneName, (void*)BoneName.c_str(), BoneNameLength);    // 获取骨骼名称

					

					if (strcmp(BoneName.c_str(), ("biped Head")) == 0) {
						fuck.PlayerBoneAddress.BipedHead = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped Neck")) == 0) {
						fuck.PlayerBoneAddress.BipedNeck = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L UpperArm")) == 0) {
						fuck.PlayerBoneAddress.BipedLUpperArm = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R UpperArm")) == 0) {
						fuck.PlayerBoneAddress.BipedRUpperArm = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L Forearm")) == 0) {
						fuck.PlayerBoneAddress.BipedLForearm = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R Forearm")) == 0) {
						fuck.PlayerBoneAddress.BipedRForearm = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L Hand")) == 0) {
						fuck.PlayerBoneAddress.BipedLHand = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R Hand")) == 0) {
						fuck.PlayerBoneAddress.BipedRHand = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped Spine1")) == 0) {
						fuck.PlayerBoneAddress.BipedSpine1 = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped Spine")) == 0) {
						fuck.PlayerBoneAddress.BipedSpine = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L Thigh")) == 0) {
						fuck.PlayerBoneAddress.BipedLThigh = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R Thigh")) == 0) {
						fuck.PlayerBoneAddress.BipedRThigh = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L Calf")) == 0) {
						fuck.PlayerBoneAddress.BipedLCalf = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R Calf")) == 0) {
						fuck.PlayerBoneAddress.BipedRCalf = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped L Foot")) == 0) {
						fuck.PlayerBoneAddress.BipedLFoot = BonePositionAddress;
					}
					else if (strcmp(BoneName.c_str(), ("biped R Foot")) == 0) {
						fuck.PlayerBoneAddress.BipedRFoot = BonePositionAddress;
					}

				}

				// 检查是否已经存在这个玩家
				if (globalVar::isInGame == 0) {
					auto it = actorIndexMap.find(fuck.IEntity);
					if (it != actorIndexMap.end()) {
						// 如果存在，更新数据
						(*MainArray)[it->second] = fuck;
					}
					else {
						// 如果不存在，添加新玩家并记录索引
						actorIndexMap[fuck.IEntity] = MainArray->size();
						MainArray->push_back(fuck);
					}
				}


			}



		}  //循环结束


		//锁区域-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
		{
			std::lock_guard<std::mutex> lock(data_mutex);
			latest_batch = MainArray;  // 更新最新数据
			if (!initial_data_ready) initial_data_ready = true;
		}
		//锁区域-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



		std::this_thread::sleep_for(std::chrono::seconds(1));  // 1秒生成一次



	}

	VMProtectEnd();
}

void Thread::GetData::Thread_ReadData() { //读取数据
	VMProtectBeginMutation("Thread::GetData::Thread_ReadData");

	// 等待首次数据就绪
	while (!initial_data_ready) {
		std::this_thread::yield();
	}
	std::shared_ptr<std::vector<addressArray::Data_Address>> current_batch;
	{
		std::lock_guard<std::mutex> lock(data_mutex);  // 只在这里加锁
		current_batch = latest_batch;  // 获取初始数据
	}  // 锁立即释放

	PlayerData::Player Temp = {};
	while (true) {


		// 尝试获取新数据 非阻塞
		{
			std::unique_lock<std::mutex> lock(data_mutex, std::try_to_lock);   //尝试加个锁 
			if (lock.owns_lock()) {     // 看看他妈了个逼的有没有被占用 如果被占用 那么就先用旧数据
				current_batch = latest_batch;  // 走到这里说明获取到锁了   更新到最新数据
			}
		}


		globalVar::isInGame = globalVar::mem.ReadValue<uint32_t>(globalVar::mem.ReadValue<uint64_t>(MainData::hyxdAddress + Offset::inGame) + Offset::inGame_1);


		if (globalVar::isInGame == 0) {


			auto batch = std::make_shared<std::vector<PlayerData::Player>>();
			size_t count = current_batch ? current_batch->size() : 0;
			batch->reserve(count);

			for (auto& data : *current_batch) {

				Temp.IEntity = data.IEntity;
				Temp.Actor = data.Actor;



				globalVar::mem.Read(data.positionAddress, (void*)&Temp.Position.x, 12); // 坐标
				globalVar::mem.Read(data.PlayerBoneAddress.BipedNeck, (void*)&Temp.BonePosition[BipedNeck].x, 12);   //骨骼
				globalVar::mem.Read(data.PlayerBoneAddress.BipedHead, (void*)&Temp.BonePosition[BipedHead].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLUpperArm, (void*)&Temp.BonePosition[BipedLUpperArm].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRUpperArm, (void*)&Temp.BonePosition[BipedRUpperArm].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLForearm, (void*)&Temp.BonePosition[BipedLForearm].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRForearm, (void*)&Temp.BonePosition[BipedRForearm].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLHand, (void*)&Temp.BonePosition[BipedLHand].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRHand, (void*)&Temp.BonePosition[BipedRHand].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedSpine1, (void*)&Temp.BonePosition[BipedSpine1].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedSpine, (void*)&Temp.BonePosition[BipedSpine].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLThigh, (void*)&Temp.BonePosition[BipedLThigh].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRThigh, (void*)&Temp.BonePosition[BipedRThigh].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLCalf, (void*)&Temp.BonePosition[BipedLCalf].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRCalf, (void*)&Temp.BonePosition[BipedRCalf].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedLFoot, (void*)&Temp.BonePosition[BipedLFoot].x, 12);
				globalVar::mem.Read(data.PlayerBoneAddress.BipedRFoot, (void*)&Temp.BonePosition[BipedRFoot].x, 12);



				data.StateAddress = globalVar::mem.ReadValue<uint64_t>(data.StateLayer) + Offset::Player::Stater;    //这个地址 被攻击后会变  所以这里要时时刻刻获取一下
				Temp.State = globalVar::mem.ReadValue<uint32_t>(data.StateAddress);

				Temp.Camp = globalVar::mem.ReadValue<uint32_t>(data.CampAddress);
				globalVar::mem.Read(Temp.Actor + 0x10C, (void*)&Temp.BoneMatrix.Matrix_10C, 48);



				batch->push_back(Temp);

			}
			Alldata.enqueue(batch);

		}
		else
		{

			// 创建新队列并原子替换旧队列
			//moodycamel::ConcurrentQueue<std::shared_ptr<std::vector<PlayerData::Player>>> newQueue;
			//newQueue.swap(Alldata); // 注意swap方向
			RenderArray->clear();
			MainArray->clear();
			latest_batch->clear();
			IEntityIndexMap.clear();
			actorIndexMap.clear();
		}

	}
	VMProtectEnd();
}
void Thread::GetData::Thread_ProcessingData() {

	VMProtectBeginMutation("Thread::GetData::Thread_ProcessingData");
	while (true) {

		std::shared_ptr<std::vector<PlayerData::Player>> EnemyData;
		if (Alldata.try_dequeue(EnemyData)) {
			for (auto& data : *EnemyData) {

				// 计算世界坐标到屏幕坐标的转换
				data.Position_Pos = globalFun::matrixTransformScreenCoords(MainData::Mx, data.Position);
				data.Distance = globalFun::GetPlayerDistance(MainData::Mx, data.Position);

				// 计算玩家方框位置和大小
				PlayerData::D3DVector FootPosition = data.Position;
				PlayerData::D3DVector HeadPosition = data.Position;
				HeadPosition.y += 1.63F;
				PlayerData::D2DVector FootPos = globalFun::matrixTransformScreenCoords(MainData::Mx, FootPosition);
				PlayerData::D2DVector HeadPos = globalFun::matrixTransformScreenCoords(MainData::Mx, HeadPosition);
				data.Rect.H = FootPos.y - HeadPos.y;
				data.Rect.W = data.Rect.H * 0.39720F;
				data.Rect.X = HeadPos.x - data.Rect.W / 2;
				data.Rect.Y = HeadPos.y;

				// 计算所有骨骼位置
				data.HeadMiddlePosition = data.BonePosition[BipedHead];
				data.HeadMiddlePosition.y = data.BonePosition[BipedHead].y + 0.13F;

				data.BonePosition_Pos[BipedNeck] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedNeck], data.BoneMatrix);
				data.BonePosition_Pos[BipedHead] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedHead], data.BoneMatrix);
				data.BonePosition_Pos[BipedLUpperArm] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLUpperArm], data.BoneMatrix);
				data.BonePosition_Pos[BipedRUpperArm] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRUpperArm], data.BoneMatrix);
				data.BonePosition_Pos[BipedLForearm] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLForearm], data.BoneMatrix);
				data.BonePosition_Pos[BipedRForearm] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRForearm], data.BoneMatrix);
				data.BonePosition_Pos[BipedLHand] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLHand], data.BoneMatrix);
				data.BonePosition_Pos[BipedRHand] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRHand], data.BoneMatrix);
				data.BonePosition_Pos[BipedSpine1] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedSpine1], data.BoneMatrix);
				data.BonePosition_Pos[BipedSpine] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedSpine], data.BoneMatrix);
				data.BonePosition_Pos[BipedLThigh] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLThigh], data.BoneMatrix);
				data.BonePosition_Pos[BipedRThigh] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRThigh], data.BoneMatrix);
				data.BonePosition_Pos[BipedLCalf] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLCalf], data.BoneMatrix);
				data.BonePosition_Pos[BipedRCalf] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRCalf], data.BoneMatrix);
				data.BonePosition_Pos[BipedLFoot] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedLFoot], data.BoneMatrix);
				data.BonePosition_Pos[BipedRFoot] = globalFun::BoneToD2D(MainData::Mx, data.BonePosition[BipedRFoot], data.BoneMatrix);

				data.HeadMiddlePosition_Pos = globalFun::BoneToD2D(MainData::Mx, data.HeadMiddlePosition,data.BoneMatrix);


				// 检查是否已经存在这个玩家
				if (globalVar::isInGame == 0) {
					auto it = IEntityIndexMap.find(data.IEntity);

					if (it != IEntityIndexMap.end()) {
						// 如果存在，更新数据
						(*RenderArray)[it->second] = data;
					}
					else {
						// 如果不存在，添加新玩家并记录索引
						IEntityIndexMap[data.IEntity] = RenderArray->size();
						RenderArray->push_back(data);
					}
				}



			}
		}



	}


	VMProtectEnd();
}
void Thread::AimBotThread::AimBotMain()
{
	VMProtectBeginMutation("Thread::AimBotThread::AimBotMain");
	while (true) {

		if (features::Aim && globalVar::isInGame == 0) {

			double NowAim = 114514.000000000F;
			uint64_t AimPlayer = 0x0;
			PlayerData::D3DVector AimBonePosition;
			PlayerData::D2DVector scpreenpos = { 0,0 };
			bool Lock = false;//锁定判断


			if (!RenderArray || RenderArray->empty()) {
				std::this_thread::sleep_for(std::chrono::seconds(1));  // 3秒生成一次
				continue;
			}


			for (auto& data : *RenderArray) {
				// 34正常活动 35跳伞中 26倒地
				// 跳过无效数据
				if (data.Position_Pos.x < -10000.0F || data.Position_Pos.y < -10000.0F) {
					continue;
				}

				if (data.State <= 25 || data.State >= 60) {
					continue;
				}
				if (!features::esp_Team) {
					if (data.Camp == 2139063679) {
						continue;
					}
				}



				if (!Lock) {

					auto AimRange = 0;
					if (features::AimPosCurrent == 0) {
						// 头部
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedHead].x, data.BonePosition_Pos[BipedHead].y);
					}
					else if (features::AimPosCurrent == 1) {
						// 颈部
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedNeck].x, data.BonePosition_Pos[BipedNeck].y);
					}
					else if (features::AimPosCurrent == 2) {
						// 左上臂
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLUpperArm].x, data.BonePosition_Pos[BipedLUpperArm].y);
					}
					else if (features::AimPosCurrent == 3) {
						// 右上臂
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRUpperArm].x, data.BonePosition_Pos[BipedRUpperArm].y);
					}
					else if (features::AimPosCurrent == 4) {
						// 左前臂
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLForearm].x, data.BonePosition_Pos[BipedLForearm].y);
					}
					else if (features::AimPosCurrent == 5) {
						// 右前臂
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRForearm].x, data.BonePosition_Pos[BipedRForearm].y);
					}
					else if (features::AimPosCurrent == 6) {
						// 左手
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLHand].x, data.BonePosition_Pos[BipedLHand].y);
					}
					else if (features::AimPosCurrent == 7) {
						// 右手
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRHand].x, data.BonePosition_Pos[BipedRHand].y);
					}
					else if (features::AimPosCurrent == 8) {
						// 上脊柱
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedSpine1].x, data.BonePosition_Pos[BipedSpine1].y);
					}
					else if (features::AimPosCurrent == 9) {
						// 脊柱
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedSpine].x, data.BonePosition_Pos[BipedSpine].y);
					}
					else if (features::AimPosCurrent == 10) {
						// 左大腿
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLThigh].x, data.BonePosition_Pos[BipedLThigh].y);
					}
					else if (features::AimPosCurrent == 11) {
						// 右大腿
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRThigh].x, data.BonePosition_Pos[BipedRThigh].y);
					}
					else if (features::AimPosCurrent == 12) {
						// 左小腿
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLCalf].x, data.BonePosition_Pos[BipedLCalf].y);
					}
					else if (features::AimPosCurrent == 13) {
						// 右小腿
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRCalf].x, data.BonePosition_Pos[BipedRCalf].y);
					}
					else if (features::AimPosCurrent == 14) {
						// 左脚
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedLFoot].x, data.BonePosition_Pos[BipedLFoot].y);
					}
					else if (features::AimPosCurrent == 15) {
						// 右脚
						AimRange = globalFun::calculateDistance(MainData::ScreenX / 2, MainData::ScreenY / 2, data.BonePosition_Pos[BipedRFoot].x, data.BonePosition_Pos[BipedRFoot].y);
					}


					if (NowAim > AimRange)
					{
						NowAim = AimRange;
						AimPlayer = data.IEntity;

						if (features::AimPosCurrent == 0) {  // 头部
							AimBonePosition = data.BonePosition[BipedHead];
						}
						else if (features::AimPosCurrent == 1) {  // 颈部
							AimBonePosition = data.BonePosition[BipedNeck];
						}
						else if (features::AimPosCurrent == 2) {  // 左上臂
							AimBonePosition = data.BonePosition[BipedLUpperArm];
						}
						else if (features::AimPosCurrent == 3) {  // 右上臂
							AimBonePosition = data.BonePosition[BipedRUpperArm];
						}
						else if (features::AimPosCurrent == 4) {  // 左前臂
							AimBonePosition = data.BonePosition[BipedLForearm];
						}
						else if (features::AimPosCurrent == 5) {  // 右前臂
							AimBonePosition = data.BonePosition[BipedRForearm];
						}
						else if (features::AimPosCurrent == 6) {  // 左手
							AimBonePosition = data.BonePosition[BipedLHand];
						}
						else if (features::AimPosCurrent == 7) {  // 右手
							AimBonePosition = data.BonePosition[BipedRHand];
						}
						else if (features::AimPosCurrent == 8) {  // 上脊柱
							AimBonePosition = data.BonePosition[BipedSpine1];
						}
						else if (features::AimPosCurrent == 9) {  // 脊柱
							AimBonePosition = data.BonePosition[BipedSpine];
						}
						else if (features::AimPosCurrent == 10) {  // 左大腿
							AimBonePosition = data.BonePosition[BipedLThigh];
						}
						else if (features::AimPosCurrent == 11) {  // 右大腿
							AimBonePosition = data.BonePosition[BipedRThigh];
						}
						else if (features::AimPosCurrent == 12) {  // 左小腿
							AimBonePosition = data.BonePosition[BipedLCalf];
						}
						else if (features::AimPosCurrent == 13) {  // 右小腿
							AimBonePosition = data.BonePosition[BipedRCalf];
						}
						else if (features::AimPosCurrent == 14) {  // 左脚
							AimBonePosition = data.BonePosition[BipedLFoot];
						}
						else if (features::AimPosCurrent == 15) {  // 右脚
							AimBonePosition = data.BonePosition[BipedRFoot];
						}

						scpreenpos = globalFun::BoneToD2D(MainData::Mx, AimBonePosition, data.BoneMatrix);
					}
				}



			}
			if (AimPlayer != 0) {
				Lock = true;
				if (std::sqrt((scpreenpos.x - MainData::ScreenX / 2) * (scpreenpos.x - MainData::ScreenX / 2) +
					(scpreenpos.y - MainData::ScreenY / 2) * (scpreenpos.y - MainData::ScreenY / 2)) <= features::AimRange)
				{
					LineAimPos = { scpreenpos.x, scpreenpos.y };
				}
				else
				{
					LineAimPos = { NULL,NULL };   //  人物离开了那个傻逼范围
				}

				if (std::sqrt((scpreenpos.x - MainData::ScreenX / 2) * (scpreenpos.x - MainData::ScreenX / 2) +
					(scpreenpos.y - MainData::ScreenY / 2) * (scpreenpos.y - MainData::ScreenY / 2)) <= features::AimRange && GetAsyncKeyState(features::key) != 0x0) {

					//mouse_event(1, (scpreenpos.x - (MainData::ScreenX / 2)) / 3, (scpreenpos.y - (MainData::ScreenY / 2)) / 3, 0, 0);

					globalFun::mousemove(scpreenpos.x, scpreenpos.y, MainData::ScreenX, MainData::ScreenY, features::SmoothSpeed);

				}
				AimPlayer = 0;
				Lock = false;

			}

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	VMProtectEnd();
}



