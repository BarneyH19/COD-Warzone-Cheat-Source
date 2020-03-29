#pragma once
#include "stdafx.h"
#include "imgui/imgui.h"

struct FrameContext
{
	ID3D12CommandAllocator* CommandAllocator;
	UINT64                  FenceValue;
};

namespace type
{
	typedef void(CALLBACK* tImguiStyle)();
	typedef void(CALLBACK* tESP)(ImFont*);
}

namespace imgui
{
	////////////////////////////////////////////////////////
	//*
	//*     功能: 让 Msg 函数发送重置 Imgui 的接口
	//*
	//////////////////////////////////////////////////////////
	void reset_imgui_request();
	bool get_is_need_reset_imgui();

	////////////////////////////////////////////////////////
	//*
	//*     功能: ImGui 开始函数
	//*   返回值: 由指定字体文件路径获取到的字体的对象
	//*    参数1: D3D12交换链对象
	//*    参数2: D3D12命令队列对象
	//*    参数3: 修改 Imgui UI 样式的函数
	//*    参数4: 指定字体的文件路径
	//*    参数5: 字体的大小
	//*
	//////////////////////////////////////////////////////////
	ImFont* start(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue, type::tImguiStyle SetStyleFunction, const char* main_font_path, float font_size);

	////////////////////////////////////////////////////////
	//*
	//*     功能: Imgui 是否初始化成功
	//*
	//////////////////////////////////////////////////////////
	bool IsReady();

	////////////////////////////////////////////////////////
	//*
	//*     功能: 改变窗口特性时必须调用此函数释放一些关联资源
	//*
	//////////////////////////////////////////////////////////
	void clear();

	////////////////////////////////////////////////////////
	//*
	//*     功能: ImGui 添加字体函数
	//*   返回值: 由指定字体文件路径获取到的字体的对象
	//*    参数1: 指定字体的文件路径
	//*    参数2: 字体的大小
	//*
	//////////////////////////////////////////////////////////
	ImFont* add_font(const char* font_path, float font_size);

	////////////////////////////////////////////////////////
	//*
	//*     功能: 画 Imgui 必须使用的头部和尾部, 在这之间画 UI 或 ESP
	//*
	//////////////////////////////////////////////////////////
	void imgui_frame_header();
	HRESULT imgui_frame_end(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	////////////////////////////////////////////////////////
	//*
	//*     功能: 画 ESP
	//*
	//////////////////////////////////////////////////////////
	void imgui_no_border(type::tESP esp_function, ImFont* font);
}