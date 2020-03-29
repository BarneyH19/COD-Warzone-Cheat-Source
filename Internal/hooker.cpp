#include "stdafx.h"
#include "hooker.h"
#include "process.h"
#include "sdk.h"
#include "esp.h"
#include "ui.h"
#include "imgui_draw_api.h"
//#include "imgui/examples/imgui_impl_dx12.h"
#include "imgui_draw_api.h"
#include "imgui_draw_api.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ogr_function
{
	type::tPresent Present;
	type::tWndProc WndProc;
}

namespace hooker
{
	HRESULT STDMETHODCALLTYPE hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!pSwapChain)
			return ogr_function::Present(pSwapChain, SyncInterval, Flags);

		static char szFontPath[MAX_PATH] = { 0 };

		if (strlen(szFontPath) == 0)
		{
			std::string font_path(process::get_system_window_dir());
			font_path += "msyh.ttc";
			memcpy(szFontPath, font_path.c_str(), font_path.length());
		}

		ImFont* main_font = imgui::start(
			static_cast<IDXGISwapChain3*>(pSwapChain),
			reinterpret_cast<ID3D12CommandQueue*>(*(QWORD*)(cod_data::image_base + decrypt::D3D12_CommandQueue_OFFSET)),
			nullptr,
			szFontPath,
			20.0f);

		imgui::imgui_frame_header();

		//esp
		imgui::imgui_no_border(draw::esp, main_font);

		//ui
		draw::ui();

		return imgui::imgui_frame_end(pSwapChain, SyncInterval, Flags);
	}

	LRESULT hkWndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp)
	{
		switch (Msg)
		{
		case 0x403:   //未知消息, 不重置D3D会发生错误
		case WM_SIZE: //必须重新初始化绘图与D3D12相关数据
		{
			if (Msg == WM_SIZE && wp == SIZE_MINIMIZED)
				break;

			if (imgui::IsReady())
			{
				//必须在此处的重置工作
				imgui::clear();

				ImGui_ImplDX12_InvalidateDeviceObjects();

				//必须发送信号给 imgui, 让他们进行重置工作
				imgui::reset_imgui_request();
			}
			break;
		}
		};

		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wp, lp);
		return ogr_function::WndProc(hWnd, Msg, wp, lp);
	}
}