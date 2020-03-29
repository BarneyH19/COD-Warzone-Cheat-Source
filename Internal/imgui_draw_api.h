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
	//*     ����: �� Msg ������������ Imgui �Ľӿ�
	//*
	//////////////////////////////////////////////////////////
	void reset_imgui_request();
	bool get_is_need_reset_imgui();

	////////////////////////////////////////////////////////
	//*
	//*     ����: ImGui ��ʼ����
	//*   ����ֵ: ��ָ�������ļ�·����ȡ��������Ķ���
	//*    ����1: D3D12����������
	//*    ����2: D3D12������ж���
	//*    ����3: �޸� Imgui UI ��ʽ�ĺ���
	//*    ����4: ָ��������ļ�·��
	//*    ����5: ����Ĵ�С
	//*
	//////////////////////////////////////////////////////////
	ImFont* start(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue, type::tImguiStyle SetStyleFunction, const char* main_font_path, float font_size);

	////////////////////////////////////////////////////////
	//*
	//*     ����: Imgui �Ƿ��ʼ���ɹ�
	//*
	//////////////////////////////////////////////////////////
	bool IsReady();

	////////////////////////////////////////////////////////
	//*
	//*     ����: �ı䴰������ʱ������ô˺����ͷ�һЩ������Դ
	//*
	//////////////////////////////////////////////////////////
	void clear();

	////////////////////////////////////////////////////////
	//*
	//*     ����: ImGui ������庯��
	//*   ����ֵ: ��ָ�������ļ�·����ȡ��������Ķ���
	//*    ����1: ָ��������ļ�·��
	//*    ����2: ����Ĵ�С
	//*
	//////////////////////////////////////////////////////////
	ImFont* add_font(const char* font_path, float font_size);

	////////////////////////////////////////////////////////
	//*
	//*     ����: �� Imgui ����ʹ�õ�ͷ����β��, ����֮�仭 UI �� ESP
	//*
	//////////////////////////////////////////////////////////
	void imgui_frame_header();
	HRESULT imgui_frame_end(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	////////////////////////////////////////////////////////
	//*
	//*     ����: �� ESP
	//*
	//////////////////////////////////////////////////////////
	void imgui_no_border(type::tESP esp_function, ImFont* font);
}