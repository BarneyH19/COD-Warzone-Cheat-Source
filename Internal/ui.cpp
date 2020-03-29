#include "stdafx.h"
#include "ui.h"
#include "hotkey.h"
#include "imgui/imgui.h"
//#include "imgui/examples/imgui_impl_win32.h"
//#include "imgui/examples/imgui_impl_dx12.h"

namespace draw
{
	void ui()
	{
		ImGui::Begin("Internal", NULL, ImGuiWindowFlags_NoResize);
		ImGui::SetWindowSize(ImVec2(480, 550), ImGuiSetCond_Always);
		ImGui::Checkbox("Box ESP", &hotkey::Box);
		ImGui::Checkbox("Lines ESP", &hotkey::Eye);
		ImGui::Checkbox("Bones ESP", &hotkey::Bones);
		ImGui::Checkbox("Show Names", &hotkey::Names);
		ImGui::Checkbox("Show Distance", &hotkey::Distance);
		ImGui::Checkbox("Show Visible", &hotkey::Visible); //u8
		ImGui::Checkbox("Radar", &hotkey::Rader);
		ImGui::Checkbox("Crosshair", &hotkey::Crosshair);
		ImGui::Checkbox("Friendly ESP", &hotkey::Friendly);
		ImGui::Checkbox("Aimbot", &hotkey::AimBot);
		ImGui::Checkbox("Show FOV",  &hotkey::ShowFov);
		ImGui::SliderInt("Radar Zoom", &hotkey::RaderZoom, 10, 100);
		ImGui::SliderFloat("Aimbot FOV", &hotkey::AimbotFov, 0, 9999);
		const char* AimbotPoint[] = { u8"头部", u8"颈部", u8"腰部", u8"随机" };
		const char* AimbotKey[]   = { u8"左键", u8"右键", u8"#F" };
		ImGui::Combo("Aim Position", &hotkey::AimPos, AimbotPoint, 4);
		ImGui::Combo("Aim Key", &hotkey::AimKey, AimbotKey, 3);
		ImGui::SliderInt("Show Limit", &hotkey::ShowLimit, 50, 1000);
		ImGui::End();
	}
}