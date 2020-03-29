#include "stdafx.h"
#include "esp.h"
#include "sdk.h"
#include "hotkey.h"



//I have deleted imgui, this set of source code is for reference only!Please copy the data you need manually!
//This source code is shared by someone in China. If necessary, please join the following link by yourself!
//discord:[lvzhu#7565]     
//https://discord.gg/5Wy2H7N





namespace draw
{
	namespace esp_data
	{
		float width;
		float hight;

		constexpr double PI = 3.141592654f;

		float           RadarPosX = 0;
		constexpr float RadarPosY = 30;
		constexpr long  RadarSize = 220;
		constexpr long  RadarRadius = RadarSize / 2;
		constexpr float RadarLineInterval = 1.0f;
		constexpr float BackgroundInterval = 5.0f;

		constexpr DWORD VisibleEnemyColor = 0xFFFFFF00;
		constexpr DWORD VisibleFriendlyColor = 0xFF9097FF;
		constexpr DWORD NotVisibleEnemyColor = 0xFFFF0000;
		constexpr DWORD NotVisibleFriendlyColor = 0xFFFFFFFF;
	}

	bool w2s(const Vector3& WorldPos, POINT* ScreenPos)
	{
		const cod::refdef_t* refdef = cod::get_camera();

		Vector3 vLocal, vTrans;

		vLocal = WorldPos - refdef->ViewLocation;

		vTrans.x = vLocal.Dot(refdef->Matrix[1]);
		vTrans.y = vLocal.Dot(refdef->Matrix[2]);
		vTrans.z = vLocal.Dot(refdef->Matrix[0]);

		if (vTrans.z < 0.01f)
			return false;

		ScreenPos->x = ((refdef->Width / 2) *  (1 - (vTrans.x / refdef->FovX / vTrans.z)));
		ScreenPos->y = ((refdef->Height / 2) *  (1 - (vTrans.y / refdef->FovY / vTrans.z)));

		if (ScreenPos->x > refdef->Width || ScreenPos->y > refdef->Height || ScreenPos->x < 0 || ScreenPos->y < 0)
			return false;

		return true;
	}

	namespace aimbot
	{
		constexpr float Init_Distance = 10000.0f;

		float g_MinDistance = Init_Distance;
		POINT g_AimPosition;

		__forceinline void reset_aimbot()
		{
			g_MinDistance = Init_Distance;
		}

		__forceinline bool key_down(int vk_key)
		{
			return ((GetAsyncKeyState(vk_key) & 0x8000) ? 1 : 0);
		}

		__forceinline unsigned long get_current_aimbot_pos0(unsigned long ui_index)
		{
			switch (ui_index)
			{
			case 0://头部
				return cod::BONE_POS_HEAD;
			case 1://颈部
				return cod::BONE_POS_NECK;
			case 2://胸部
				return cod::BONE_POS_CHEST;
			};
			return cod::BONE_POS_HEAD;
		}

		unsigned long get_aimbot_key()
		{
			switch (hotkey::AimKey)
			{
			case 0:
				return VK_LBUTTON;
			case 1:
				return VK_RBUTTON;
			case 2:
				return 70;
			}
			return VK_LBUTTON;
		}

		unsigned long get_current_aimbot_pos(const bool is_key_down)
		{
			unsigned long gones_index = hotkey::AimKey;
			if (gones_index != 3)
			{
				return get_current_aimbot_pos0(gones_index);
			}
			else
			{
				static unsigned long ramdom_gones_index = cod::BONE_POS_HEAD;
				if (!is_key_down)
				{
					static std::default_random_engine s_random_engine(static_cast<unsigned int>(time(nullptr) * 28));
					static const std::uniform_int_distribution<long> cs_fndistribution(0, 2);
					ramdom_gones_index = get_current_aimbot_pos0(cs_fndistribution(s_random_engine));
				}
				return ramdom_gones_index;
			}
			return cod::BONE_POS_HEAD;
		}

		void get_aimbot_pos(const unsigned long i, const float aimbot_fov, unsigned long gones_index)
		{
			Vector3 HitPos;
			POINT ScreenPos;

			if (cod::get_bone_pos(i, gones_index, &HitPos) && w2s(HitPos, &ScreenPos))
			{
				const float x = ScreenPos.x - (esp_data::width / 2);
				const float y = ScreenPos.y - (esp_data::hight / 2);
				float center_dis = std::sqrt((x * x) + (y * y));
				if (center_dis < g_MinDistance && center_dis <= aimbot_fov)
				{
					g_MinDistance = center_dis;
					g_AimPosition.x = x;
					g_AimPosition.y = y;
				}
			}
		}

		void aimbot_now(const bool is_key_down)
		{
			if (is_key_down && g_MinDistance != Init_Distance)
			{
				INPUT Input;
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_MOVE;
				Input.mi.dx = (long)(g_AimPosition.x * 0.1f);
				Input.mi.dy = (long)(g_AimPosition.y * 0.1f);

				SendInput(1, &Input, sizeof(INPUT));
			}
		}
	}

	void esp2(ImFont* font);

	void DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;
		window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), thickness);
	}

	void DrawBox(const float x, const float y, const float width, const float height, const uint32_t color, float thickness)
	{
		DrawLine(ImVec2(x, y), ImVec2(x + width, y), color, thickness);
		DrawLine(ImVec2(x, y), ImVec2(x, y + height), color, thickness);
		DrawLine(ImVec2(x, y + height), ImVec2(x + width, y + height), color, thickness);
		DrawLine(ImVec2(x + width, y), ImVec2(x + width, y + height), color, thickness);
	}

	void FillRectangle(const float x, const float y, const float width, const float hight, const uint32_t color)
	{
		const float end_y = y + hight;
		for (float curr_y = y; curr_y < end_y; ++curr_y)
		{
			DrawLine(ImVec2(x, curr_y), ImVec2(x + width, curr_y), color, 1.5f);
		}
	}

	void DrawCircle(const ImVec2& position, float radius, uint32_t color, float thickness)
	{
		constexpr double M_PI = 3.14159265358979323846;
		float step = (float)M_PI * 2.0f / thickness;
		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			DrawLine(ImVec2(radius * cosf(a) + position.x, radius * sinf(a) + position.y), ImVec2(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y), color, 1.5f);
		}
	}
	void DrawSketchEdgeText2(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center, uint32_t EdgeColor = 0xFF000000)
	{
		constexpr float fStrokeVal1 = 1.0f;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		float a = (color >> 24) & 0xff;
		float r = (color >> 16) & 0xff;
		float g = (color >> 8) & 0xff;
		float b = (color) & 0xff;
		float Edge_a = (EdgeColor >> 24) & 0xff;
		float Edge_r = (EdgeColor >> 16) & 0xff;
		float Edge_g = (EdgeColor >> 8) & 0xff;
		float Edge_b = (EdgeColor) & 0xff;
		std::stringstream steam(text);
		std::string line;
		float y = 0.0f;
		int i = 0;
		while (std::getline(steam, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
			if (center)
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - fStrokeVal1, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + fStrokeVal1, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * i) - fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * i) + fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
			}
			else
			{
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) - fStrokeVal1, (pos.y + textSize.y * i)), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x) + fStrokeVal1, (pos.y + textSize.y * i)), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x), (pos.y + textSize.y * i) - fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2((pos.x), (pos.y + textSize.y * i) + fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
			}
			y = pos.y + textSize.y * (i + 1);
			i++;
		}
	}

	void header()
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::Begin("A", reinterpret_cast<bool*>(true), ImVec2(0, 0), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);
	}

	void end()
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
	}


	void esp(ImFont* font)
	{
		header();
		esp2(font);
		end();
	}

	namespace radar
	{
		void show_radar_background()
		{
			esp_data::RadarPosX = esp_data::width - esp_data::RadarSize - 50;

			//边框
			draw::FillRectangle(
				esp_data::RadarPosX - esp_data::BackgroundInterval,
				esp_data::RadarPosY - esp_data::BackgroundInterval,
				esp_data::RadarSize + (esp_data::BackgroundInterval * 2),
				esp_data::RadarSize + (esp_data::BackgroundInterval * 2),
				0xFFFFFEFE);

			draw::FillRectangle(
				esp_data::RadarPosX,
				esp_data::RadarPosY,
				esp_data::RadarSize,
				esp_data::RadarSize,
				0xFFA8AAAA);

			//描边
			draw::DrawLine(
				ImVec2(esp_data::RadarPosX, esp_data::RadarPosY + esp_data::RadarLineInterval),
				ImVec2(esp_data::RadarPosX, esp_data::RadarPosY + esp_data::RadarSize - esp_data::RadarLineInterval + 1),
				0xFF010000,
				1.3f);

			draw::DrawLine(
				ImVec2(esp_data::RadarPosX + esp_data::RadarSize, esp_data::RadarPosY + esp_data::RadarLineInterval),
				ImVec2(esp_data::RadarPosX + esp_data::RadarSize, esp_data::RadarPosY + esp_data::RadarSize - esp_data::RadarLineInterval + 1),
				0xFF010000,
				1.3f);

			draw::DrawLine(
				ImVec2(esp_data::RadarPosX, esp_data::RadarPosY),
				ImVec2(esp_data::RadarPosX + esp_data::RadarSize - esp_data::RadarLineInterval + 2, esp_data::RadarPosY),
				0xFF010000,
				1.3f);

			draw::DrawLine(
				ImVec2(esp_data::RadarPosX, esp_data::RadarPosY + esp_data::RadarSize),
				ImVec2(esp_data::RadarPosX + esp_data::RadarSize - esp_data::RadarLineInterval + 2, esp_data::RadarPosY + esp_data::RadarSize),
				0xFF010000,
				1.3f);

			//十字线
			draw::DrawLine(
				ImVec2(esp_data::RadarPosX + esp_data::RadarRadius, esp_data::RadarPosY + esp_data::RadarLineInterval),
				ImVec2(esp_data::RadarPosX + esp_data::RadarRadius, esp_data::RadarPosY + esp_data::RadarSize - esp_data::RadarLineInterval),
				0xFF010000,
				1.3f);

			draw::DrawLine(
				ImVec2(esp_data::RadarPosX, esp_data::RadarPosY + esp_data::RadarRadius),
				ImVec2(esp_data::RadarPosX + esp_data::RadarSize - esp_data::RadarLineInterval, esp_data::RadarPosY + esp_data::RadarRadius),
				0xFF010000,
				1.3f);

			//自己的位置
			draw::DrawBox(esp_data::RadarPosX + esp_data::RadarRadius - 3, esp_data::RadarPosY + esp_data::RadarRadius - 3, 5, 5, 0xFF000100, 1.0f);
			draw::FillRectangle(esp_data::RadarPosX + esp_data::RadarRadius - 2, esp_data::RadarPosY + esp_data::RadarRadius - 2, 4, 4, 0xFFFFFFFF);
		}

		ImVec2 rotate(const ImVec2& center, const ImVec2& pos, float angle)
		{
			constexpr double PI = 3.1415926535898;
			ImVec2 Return;
			angle *= -(PI / 180.0f);
			float cos_theta = cos(angle);
			float sin_theta = sin(angle);
			Return.x = (cos_theta * (pos[0] - center[0]) - sin_theta * (pos[1] - center[1])) + center[0];
			Return.y = (sin_theta * (pos[0] - center[0]) + cos_theta * (pos[1] - center[1])) + center[1];
			return Return;
		}

		Vector2 radar_rotate(const float x, const float y, float angle)
		{
			angle = (float)(angle * (esp_data::PI / 180.f));
			float cosTheta = (float)cos(angle);
			float sinTheta = (float)sin(angle);
			Vector2 returnVec;
			returnVec.x = cosTheta * x + sinTheta * y;
			returnVec.y = sinTheta * x - cosTheta * y;
			return returnVec;
		}

		void draw_entity(const ImVec2& pos, float angle, DWORD color)
		{
			constexpr long up_offset = 7;
			constexpr long lr_offset = 5;

			for (int FillIndex = 0; FillIndex < 5; ++FillIndex)
			{
				ImVec2 up_pos(pos.x, pos.y - up_offset + FillIndex);
				ImVec2 left_pos(pos.x - lr_offset + FillIndex, pos.y + up_offset - FillIndex);
				ImVec2 right_pos(pos.x + lr_offset - FillIndex, pos.y + up_offset - FillIndex);

				ImVec2 p0 = rotate(pos, up_pos, angle);
				ImVec2 p1 = rotate(pos, left_pos, angle);
				ImVec2 p2 = rotate(pos, right_pos, angle);

				draw::DrawLine(p0, p1, FillIndex == 0 ? 0xFF010000 : color, 1.0f);
				draw::DrawLine(p1, p2, FillIndex == 0 ? 0xFF010000 : color, 1.0f);
				draw::DrawLine(p2, p0, FillIndex == 0 ? 0xFF010000 : color, 1.0f);
			}
		}
		void draw_death_entity(const ImVec2& pos, DWORD color)
		{
			constexpr float line_radius = 5;
			ImVec2 p0(pos.x - line_radius, pos.y - line_radius);
			ImVec2 p1(pos.x + line_radius, pos.y + line_radius);
			ImVec2 p3(pos.x - line_radius, pos.y + line_radius);
			ImVec2 p4(pos.x + line_radius, pos.y - line_radius);
			draw::DrawLine(p0, p1, color, 1.5f);
			draw::DrawLine(p3, p4, color, 1.5f);
		}

		void draw_entity(const cod::pos_info* local_entity, const cod::pos_info* entity, bool IsFriendly, bool IsAlive, DWORD color)
		{
			const float local_rotation = local_entity->get_rotation();
			float rotation = entity->get_rotation();

			rotation = rotation - local_rotation;

			if (rotation < 0)
				rotation = 360.0f - std::fabs(rotation); //爲了讓我雷達視角轉換后， 矯正敵人的所看方向.

			float x_distance = local_entity->m_position.x - entity->m_position.x;
			float y_distance = local_entity->m_position.y - entity->m_position.y;

			float zoom = hotkey::RaderZoom * 0.001f;

			x_distance *= zoom;
			y_distance *= zoom;

			Vector2 point_offset = radar_rotate(x_distance, y_distance, local_rotation);

			constexpr long positiveRadarRadius = esp_data::RadarRadius - 5;
			constexpr long negaRadarRadius = (esp_data::RadarRadius * -1) + 5;

			if (point_offset.x > positiveRadarRadius)
			{
				point_offset.x = positiveRadarRadius;
			}
			else if (point_offset.x < negaRadarRadius)
			{
				point_offset.x = negaRadarRadius;
			}

			if (point_offset.y > positiveRadarRadius)
			{
				point_offset.y = positiveRadarRadius;
			}
			else if (point_offset.y < negaRadarRadius)
			{
				point_offset.y = negaRadarRadius;
			}

			if (IsAlive)
			{
				draw_entity(ImVec2(esp_data::RadarPosX + esp_data::RadarRadius + point_offset.x, esp_data::RadarPosY + esp_data::RadarRadius + point_offset.y), rotation, color);
			}
			else
			{
				draw_death_entity(ImVec2(esp_data::RadarPosX + esp_data::RadarRadius + point_offset.x, esp_data::RadarPosY + esp_data::RadarRadius + point_offset.y), IsFriendly ? 0xFFFFFFFF : 0xFF010000);
			}
		}
	}


	bool update_true_base_pos()
	{
		auto* ptr = cod::get_camera();
		if (ptr)
		{
			esp_data::width = ptr->Width;
			esp_data::hight = ptr->Height;
			return true;
		}
		return false;
	}


	void crosshair()
	{
		constexpr long crosshair_size = 10.0f;
		draw::DrawLine(ImVec2((esp_data::width / 2), (esp_data::hight / 2) - crosshair_size),
			ImVec2((esp_data::width / 2), (esp_data::hight / 2) + crosshair_size), 0xFFFFFFFF, 1.5f);
		draw::DrawLine(ImVec2((esp_data::width / 2) - crosshair_size, (esp_data::hight / 2)),
			ImVec2((esp_data::width / 2) + crosshair_size, (esp_data::hight / 2)), 0xFFFFFFFF, 1.5f);
	}

	void look_aimbot_fov(const float aimbot_fov)
	{
		draw::DrawCircle(ImVec2((esp_data::width / 2), (esp_data::hight / 2)), aimbot_fov, 0xFFFFFFFF, 100.0f);
	}

	__forceinline float get_distance(const Vector3& lhs, const Vector3& rhs)
	{
		const float distanceX = lhs.x - rhs.x;
		const float distanceY = lhs.y - rhs.y;
		const float distanceZ = lhs.z - rhs.z;
		return std::sqrt((distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ));
	}

	float XAngle(const Vector3& LocalPos, const Vector3& WorldPos)
	{
		float dl = sqrt((WorldPos.x - LocalPos.x) * (WorldPos.x - LocalPos.x) + (WorldPos.y - LocalPos.y) * (WorldPos.y - LocalPos.y));

		if (dl == 0.0f)
			dl = 1.0f;

		float dl2 = abs(WorldPos.x - LocalPos.x);
		float teta = ((180.0f / D3DX_PI) * acos(dl2 / dl));

		if (WorldPos.x < LocalPos.x)
			teta = 180.0f - teta;

		if (WorldPos.y < LocalPos.y)
			teta = teta * -1.0f;

		if (teta > 180.0f)
			teta = (360.0f - teta) * (-1.0f);

		if (teta < -180.0f)
			teta = (360.0f + teta);

		return teta;
	}

	void RotatePointAlpha(float x, float y, float z, float alpha, Vector3* outVec3)
	{
		Matrix4x4 rot1;
		Vector4 vec = { x, z, y, 1.0f };
		D3DXMatrixRotationY((D3DXMATRIX*)&rot1, alpha * D3DX_PI / 180.0f);
		D3DXVec4Transform((D3DXVECTOR4*)&vec, (const D3DXVECTOR4*)&vec, (const D3DXMATRIX*)&rot1);

		outVec3->x = vec.x;
		outVec3->y = vec.z;
		outVec3->z = vec.y;
	};

	bool w2s(const Vector3& WorldPos, POINT* ScreenPos, POINT* BoxSize)
	{
		const cod::refdef_t* refdef = cod::get_camera();

		float angleX = XAngle(refdef->ViewLocation, WorldPos);

		Vector3 posl, posr;

		RotatePointAlpha(-16.0f, 0.0f, 65.0f, -angleX + 90.0f, &posl);
		RotatePointAlpha(16.0f, 0.0f, -5.0f, -angleX + 90.0f, &posr);

		Vector3 vposl, vposr;

		vposl.x = WorldPos.x + posl.x;
		vposl.y = WorldPos.y + posl.y;
		vposl.z = WorldPos.z + posl.z;

		vposr.x = WorldPos.x + posr.x;
		vposr.y = WorldPos.y + posr.y;
		vposr.z = WorldPos.z + posr.z;

		POINT screenPosl, screenPosr;

		if (!w2s(vposl, &screenPosl) || !w2s(vposr, &screenPosr))
		{
			return false;
		}

		BoxSize->x = abs(screenPosr.x - screenPosl.x);
		BoxSize->y = abs(screenPosl.y - screenPosr.y);

		ScreenPos->x = screenPosr.x - BoxSize->x;
		ScreenPos->y = screenPosl.y;

		return true;
	}

	bool bones_to_screen(Vector3* BonePosArray, POINT* ScreenPosArray, const long Count)
	{
		for (long i = 0; i < Count; ++i)
		{
			if (!w2s(BonePosArray[i], &ScreenPosArray[i]))
				return false;
		}
		return true;
	}

	bool valid_bones(POINT* screenPos, const long Count, float x, float y, float w, float h)
	{
		constexpr long size = 30;

		float r = x + w + size;
		float b = y + h;

		x -= size;

		for (long i = 0; i < Count; ++i)
		{
			if (screenPos[i].x < x || screenPos[i].x > r || screenPos[i].y < y || screenPos[i].y > b)
				return false;
		}
		return true;
	}

	__forceinline void draw_bones(POINT* bones_screenPos, long count, DWORD color)
	{
		long last_count = count - 1;
		for (long i = 0; i < last_count; ++i)
			draw::DrawLine(ImVec2(bones_screenPos[i].x, bones_screenPos[i].y), ImVec2(bones_screenPos[i + 1].x, bones_screenPos[i + 1].y), color, 1.8f);
	}

	void draw_bones(unsigned long i, DWORD color, float x, float y, float w, float h)
	{
		Vector3 header_to_bladder[6], right_foot_to_bladder[5], left_foot_to_bladder[5], right_hand[5], left_hand[5];
		POINT screen_header_to_bladder[6], screen_right_foot_to_bladder[5], screen_left_foot_to_bladder[5], screen_right_hand[5], screen_left_hand[5];

		if (cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_HEAD, &header_to_bladder[0]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_NECK, &header_to_bladder[1]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_CHEST, &header_to_bladder[2]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_MID, &header_to_bladder[3]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_TUMMY, &header_to_bladder[4]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_PELVIS, &header_to_bladder[5]) &&

			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_FOOT_1, &right_foot_to_bladder[1]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_FOOT_2, &right_foot_to_bladder[2]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_FOOT_3, &right_foot_to_bladder[3]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_FOOT_4, &right_foot_to_bladder[4]) &&

			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_FOOT_1, &left_foot_to_bladder[1]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_FOOT_2, &left_foot_to_bladder[2]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_FOOT_3, &left_foot_to_bladder[3]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_FOOT_4, &left_foot_to_bladder[4]) &&

			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_HAND_1, &right_hand[1]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_HAND_2, &right_hand[2]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_HAND_3, &right_hand[3]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_LEFT_HAND_4, &right_hand[4]) &&

			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_HAND_1, &left_hand[1]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_HAND_2, &left_hand[2]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_HAND_3, &left_hand[3]) &&
			cod::get_bone_pos(i, cod::BONE_INDEX::BONE_POS_RIGHT_HAND_4, &left_hand[4]))
		{
			right_foot_to_bladder[0] = header_to_bladder[5];
			left_foot_to_bladder[0] = header_to_bladder[5];
			right_hand[0] = header_to_bladder[2];
			left_hand[0] = header_to_bladder[2];

			if (!bones_to_screen(header_to_bladder, screen_header_to_bladder, 6))
				return;

			if (!bones_to_screen(right_foot_to_bladder, screen_right_foot_to_bladder, 5))
				return;

			if (!bones_to_screen(left_foot_to_bladder, screen_left_foot_to_bladder, 5))
				return;

			if (!bones_to_screen(right_hand, screen_right_hand, 5))
				return;

			if (!bones_to_screen(left_hand, screen_left_hand, 5))
				return;

			if (!valid_bones(screen_header_to_bladder, 6, x, y, w, h) ||
				!valid_bones(screen_right_foot_to_bladder, 5, x, y, w, h) ||
				!valid_bones(screen_left_foot_to_bladder, 5, x, y, w, h) ||
				!valid_bones(screen_right_hand, 5, x, y, w, h) ||
				!valid_bones(screen_left_hand, 5, x, y, w, h))
			{
				return;
			}

			draw_bones(screen_header_to_bladder, 6, color);
			draw_bones(screen_right_foot_to_bladder, 5, color);
			draw_bones(screen_left_foot_to_bladder, 5, color);
			draw_bones(screen_right_hand, 5, color);
			draw_bones(screen_left_hand, 5, color);
		}
	}

	__forceinline double angle_to_radians(float angle_value)
	{
		return angle_value * (esp_data::PI / 180.0f);
	}

	__forceinline float get_eye_distance(float angle, long line_length)
	{
		if (angle == 180.0f)
			return 0;

		return sin(angle_to_radians(angle)) * line_length;
	}

	void eye(unsigned long local_i, unsigned long i, DWORD color)
	{
		constexpr long eye_line_init_length = 500;

		Vector3 EyePos;
		POINT sp;

		if (cod::get_bone_pos(i, cod::BONE_POS_HEAD, &EyePos) && w2s(EyePos, &sp))
		{
			const float Line_Percentage = 59.2f / (get_distance(cod::get_control_info().HeadersPos, EyePos) / 20.0f);
			const Vector2 local_angle = cod::get_entity_angle(local_i);

			Vector2 angle = cod::get_entity_angle(i);
			angle.x -= local_angle.x;

			if (angle.x < 0)
				angle.x = 360.0f - std::fabs(angle.x);

			float off_x = get_eye_distance(angle.x, eye_line_init_length * Line_Percentage) * -1;
			float off_y = get_eye_distance(angle.y, eye_line_init_length * Line_Percentage) * -1;

			//画眼线
			draw::DrawLine(
				ImVec2(sp.x, sp.y),
				ImVec2(sp.x + off_x, sp.y + off_y), color, 1.4f);

			//画头部的小方块
			draw::DrawBox(sp.x - 3, sp.y - 3, 5, 5, 0xFF000100, 1.0f);
			draw::FillRectangle(sp.x - 2, sp.y - 2, 4, 4, color);
		}
	}

	void esp2(ImFont* font)
	{
		if (!update_true_base_pos())
			return;

		aimbot::reset_aimbot();

		unsigned long local_index;

		cod::clientinfo_t* local_clientinfo = cod::get_local_clientinfo(&local_index);
		
		if (!local_clientinfo)
			return;
		
		cod::pos_info* local_pos_info = (cod::pos_info*)local_clientinfo->m_PosInfoPtr;
	
		if (!local_pos_info)
			return;
		
		cod::entity_t* local_entity = nullptr;
		bool is_show_local_function = false;
		bool is_all_kill = false;
		DWORD LocalTeamId = 0;
		
		if (local_clientinfo)
		{
			local_entity = cod::get_entity(local_index);

			if (!local_entity)
				return;

			is_show_local_function = local_clientinfo->m_is_valid && local_entity->is_alive();
			
			is_all_kill = local_clientinfo->m_TeamId == 0;
			LocalTeamId = local_clientinfo->m_TeamId;
		}
		
		const bool aimbot_key_down = aimbot::key_down(aimbot::get_aimbot_key());
		
		const unsigned long aimbot_pos = aimbot::get_current_aimbot_pos(aimbot_key_down);
	
		if (is_show_local_function)
		{
			if (hotkey::Rader)
				radar::show_radar_background();

			if (hotkey::Crosshair)
				crosshair();

			if (hotkey::ShowFov)
				look_aimbot_fov(hotkey::AimbotFov);
		}

		bool is_found_visible_base_address = false;

		for (unsigned long i = 0; i < 150; ++i)
		{
			if (local_clientinfo && local_index == i)
				continue;

			cod::clientinfo_t* clientinfo_t = cod::get_clientinfo(i);

			if (!clientinfo_t || clientinfo_t->m_is_valid != 1 || i != clientinfo_t->m_entity_index)
				continue;

			cod::pos_info* pos_info = (cod::pos_info*)clientinfo_t->m_PosInfoPtr;

			if (!pos_info)
				continue;

			cod::entity_t* entity = cod::get_entity(i);

			if (!entity || !entity->is_valid())
				continue;

			DWORD TeamId = clientinfo_t->m_TeamId;

			if (is_all_kill)
				TeamId = 1;

			if (!is_found_visible_base_address)
			{
				is_found_visible_base_address = cod::get_visible_list_base_address(i);
			}

			bool is_friendly = TeamId == LocalTeamId;

			if (is_friendly && !hotkey::Friendly)
				continue;

			DWORD color;

			float fshow_bones;
			bool bVisible = cod::is_visible(i, &fshow_bones);

			if (bVisible && hotkey::Visible)
				color = is_friendly ? esp_data::VisibleFriendlyColor : esp_data::VisibleEnemyColor;
			else
				color = is_friendly ? esp_data::NotVisibleFriendlyColor : esp_data::NotVisibleEnemyColor;

			const bool is_alive = entity->is_alive();

			if (pos_info->m_position.x == 0 && pos_info->m_position.y == 0 && pos_info->m_position.z == 0)
				continue;

			float fdistance = 0;

			if (local_pos_info)
			{
				fdistance = get_distance(local_pos_info->m_position, pos_info->m_position) / 20;

				if (fdistance + 1.0f > hotkey::ShowLimit)
					continue;

				if (hotkey::Rader && is_show_local_function)
					radar::draw_entity(local_pos_info, pos_info, is_friendly, is_alive, color);
			}

			if (!is_alive)
				continue;

			POINT ScreenPos, BoxSize;

			if (!w2s(pos_info->m_position, &ScreenPos, &BoxSize))
				continue;

			float show_info_offset = 0;

			if (hotkey::Box)
			{
				draw::DrawBox(ScreenPos.x, ScreenPos.y, BoxSize.x, BoxSize.y, color, 1.3f);
			}

			if (hotkey::Names)
			{
				std::string name = cod::get_name(i);

				draw::DrawSketchEdgeText2(font, name, ImVec2(
					ScreenPos.x + (BoxSize.x / 2),
					ScreenPos.y + BoxSize.y + 5 + show_info_offset),
					18.0f,
					color, true);

				show_info_offset += 15;
			}

			if (hotkey::Distance && local_pos_info)
			{
				std::string distance_string("[ ");
				distance_string += std::to_string(fdistance);
				std::string::size_type end = distance_string.find('.');
				if (end != std::string::npos && (end + 2) <= (distance_string.length() - 1))
					distance_string.erase(distance_string.begin() + end + 2, distance_string.end());
				distance_string += "m ]";
				draw::DrawSketchEdgeText2(font, distance_string, ImVec2(
					ScreenPos.x + (BoxSize.x / 2),
					ScreenPos.y + BoxSize.y + 5 + show_info_offset),
					18.0f,
					color, true);
			}

			bool is_show_eye_and_bones = (local_pos_info && fdistance + 1.0f <= 90.0f) || (fshow_bones > 0.0f && fshow_bones < 1.0f) || bVisible;

			if (hotkey::Bones && is_show_eye_and_bones)
			{
				draw_bones(i, color, ScreenPos.x, ScreenPos.y, BoxSize.x, BoxSize.y);
			}

			if (hotkey::Eye && is_show_local_function && is_show_eye_and_bones)
			{
				eye(local_index, i, color);
			}

			if (hotkey::AimBot)
			{
				aimbot::get_aimbot_pos(i, hotkey::AimbotFov, aimbot_pos);
			}
		}
		aimbot::aimbot_now(aimbot_key_down);
		cod::write_visible_base_address();
	}
}

