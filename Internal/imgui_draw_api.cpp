#include "stdafx.h"
#include "imgui_draw_api.h"
#include "hooks.h"
#include "process.h"
#include "imgui/imgui.h"
//#include "imgui/examples/imgui_impl_win32.h"
//#include "imgui/examples/imgui_impl_dx12.h"
#include "hooker.h"
#include "macro.h"

namespace d3d12
{
	//D3D12
	IDXGISwapChain3*           pSwapChain;
	ID3D12Device*              pDevice;
	ID3D12CommandQueue*        pCommandQueue;
	ID3D12Fence*               pFence;
	ID3D12DescriptorHeap*      pSrvDescHeap;
	ID3D12DescriptorHeap*      pRtvDescHeap;
	ID3D12GraphicsCommandList* pCommandList;

	//ARRAY
	FrameContext*                FrameContextArray;
	ID3D12Resource**             pID3D12ResourceArray;
	D3D12_CPU_DESCRIPTOR_HANDLE* RenderTargetDescriptorArray;

	//HANDLE
	HANDLE hSwapChainWaitableObject;
	HANDLE hFenceEvent;

	//CONST VALUE
	UINT NUM_FRAMES_IN_FLIGHT;
	UINT NUM_BACK_BUFFERS;

	//VALUE
	UINT   frame_index = 0;
	UINT64 fenceLastSignaledValue = 0;
}

namespace imgui
{
	bool is_ready;
	bool is_need_reset_imgui;

	bool IsReady()
	{
		return is_ready;
	}

	void reset_imgui_request()
	{
		is_need_reset_imgui = true;
	}

	__forceinline bool get_is_need_reset_imgui()
	{
		return is_need_reset_imgui;
	}

	void init_d3d12(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue)
	{

		//D3D12
		d3d12::pSwapChain = pSwapChain;
		d3d12::pCommandQueue = pCommandQueue;

		if (!SUCCEEDED(d3d12::pSwapChain->GetDevice(__uuidof(ID3D12Device), (void **)&d3d12::pDevice)))
			Exit();

		//CONST VALUE OK
		{
			DXGI_SWAP_CHAIN_DESC1 desc;

			if (!SUCCEEDED(d3d12::pSwapChain->GetDesc1(&desc)))
				Exit();

			d3d12::NUM_BACK_BUFFERS = desc.BufferCount;
			d3d12::NUM_FRAMES_IN_FLIGHT = desc.BufferCount;
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.NumDescriptors = d3d12::NUM_BACK_BUFFERS;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 1;

			if (!SUCCEEDED(d3d12::pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&d3d12::pRtvDescHeap))))
				Exit();
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.NumDescriptors = 1;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NodeMask = 0;

			if (!SUCCEEDED(d3d12::pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&d3d12::pSrvDescHeap))))
				Exit();
		}

		if (!SUCCEEDED(d3d12::pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&d3d12::pFence))))
			Exit();

		//ARRAY
		d3d12::FrameContextArray = new FrameContext[d3d12::NUM_FRAMES_IN_FLIGHT];
		d3d12::pID3D12ResourceArray = new ID3D12Resource*[d3d12::NUM_BACK_BUFFERS];
		d3d12::RenderTargetDescriptorArray = new D3D12_CPU_DESCRIPTOR_HANDLE[d3d12::NUM_BACK_BUFFERS];


		//INIT ARRAY
		for (UINT i = 0; i < d3d12::NUM_FRAMES_IN_FLIGHT; ++i)
		{
			if (!SUCCEEDED(d3d12::pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&d3d12::FrameContextArray[i].CommandAllocator))))
				Exit();
		}

		SIZE_T nDescriptorSize = d3d12::pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12::pRtvDescHeap->GetCPUDescriptorHandleForHeapStart();

		for (UINT i = 0; i < d3d12::NUM_BACK_BUFFERS; ++i)
		{
			d3d12::RenderTargetDescriptorArray[i] = rtvHandle;
			rtvHandle.ptr += nDescriptorSize;
		}

		//D3D12
		if (!SUCCEEDED(d3d12::pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3d12::FrameContextArray[0].CommandAllocator, NULL, IID_PPV_ARGS(&d3d12::pCommandList))) ||
			!SUCCEEDED(d3d12::pCommandList->Close()))
		{
			Exit();
		}

		//HANDLE
		d3d12::hSwapChainWaitableObject = d3d12::pSwapChain->GetFrameLatencyWaitableObject();

		d3d12::hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		if (d3d12::hFenceEvent == NULL)
			Exit();

		//CreateRenderTarget
		ID3D12Resource* pBackBuffer;
		for (UINT i = 0; i < d3d12::NUM_BACK_BUFFERS; ++i)
		{
			if (!SUCCEEDED(d3d12::pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer))))
				Exit();

			d3d12::pDevice->CreateRenderTargetView(pBackBuffer, NULL, d3d12::RenderTargetDescriptorArray[i]);
			d3d12::pID3D12ResourceArray[i] = pBackBuffer;
		}
	}

	void _clear()
	{
		d3d12::pSwapChain = nullptr;
		d3d12::pDevice = nullptr;
		d3d12::pCommandQueue = nullptr;

		if (d3d12::pFence)
		{
			d3d12::pFence->Release();
			d3d12::pFence = nullptr;
		}

		if (d3d12::pSrvDescHeap)
		{
			d3d12::pSrvDescHeap->Release();
			d3d12::pSrvDescHeap = nullptr;
		}

		if (d3d12::pRtvDescHeap)
		{
			d3d12::pRtvDescHeap->Release();
			d3d12::pRtvDescHeap = nullptr;
		}

		if (d3d12::pCommandList)
		{
			d3d12::pCommandList->Release();
			d3d12::pCommandList = nullptr;
		}

		if (d3d12::FrameContextArray)
		{
			for (UINT i = 0; i < d3d12::NUM_FRAMES_IN_FLIGHT; ++i)
			{
				if (d3d12::FrameContextArray[i].CommandAllocator)
				{
					d3d12::FrameContextArray[i].CommandAllocator->Release();
					d3d12::FrameContextArray[i].CommandAllocator = nullptr;
				}
			}

			delete[] d3d12::FrameContextArray;
			d3d12::FrameContextArray = NULL;
		}

		if (d3d12::pID3D12ResourceArray)
		{
			for (UINT i = 0; i < d3d12::NUM_BACK_BUFFERS; ++i)
			{
				if (d3d12::pID3D12ResourceArray[i])
				{
					d3d12::pID3D12ResourceArray[i]->Release();
					d3d12::pID3D12ResourceArray[i] = nullptr;
				}
			}

			delete[] d3d12::pID3D12ResourceArray;
			d3d12::pID3D12ResourceArray = NULL;
		}

		if (d3d12::RenderTargetDescriptorArray)
		{
			delete[] d3d12::RenderTargetDescriptorArray;
			d3d12::RenderTargetDescriptorArray = NULL;
		}

		//HANDLE
		if (d3d12::hSwapChainWaitableObject)
		{
			d3d12::hSwapChainWaitableObject = nullptr;
		}

		if (d3d12::hFenceEvent)
		{
			CloseHandle(d3d12::hFenceEvent);
			d3d12::hFenceEvent = nullptr;
		}

		//CONST VALUE
		d3d12::NUM_FRAMES_IN_FLIGHT = 0;
		d3d12::NUM_BACK_BUFFERS = 0;

		//VALUE
		d3d12::frame_index = 0;
	}

	//COPY IMGUI CPP
	void clear()
	{
		if (d3d12::FrameContextArray)
		{//说明未重置过, 由于 Msg 函数可能会连续执行几次, 当重置过后再执行就会出错.
			FrameContext* frameCtxt = &d3d12::FrameContextArray[d3d12::frame_index % d3d12::NUM_FRAMES_IN_FLIGHT];

			UINT64 fenceValue = frameCtxt->FenceValue;

			if (fenceValue == 0)
				return; // No fence was signaled

			frameCtxt->FenceValue = 0;

			bool bNotWait = d3d12::pFence->GetCompletedValue() >= fenceValue;

			if (!bNotWait)
			{
				d3d12::pFence->SetEventOnCompletion(fenceValue, d3d12::hFenceEvent);

				WaitForSingleObject(d3d12::hFenceEvent, INFINITE);
			}

			_clear();
		}
	}

	FrameContext* WaitForNextFrameResources()
	{
		UINT nextFrameIndex = d3d12::frame_index + 1;
		d3d12::frame_index = nextFrameIndex;

		HANDLE waitableObjects[] = { d3d12::hSwapChainWaitableObject, NULL };
		constexpr DWORD numWaitableObjects = 1;

		FrameContext* frameCtxt = &d3d12::FrameContextArray[nextFrameIndex % d3d12::NUM_FRAMES_IN_FLIGHT];

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

		return frameCtxt;
	}


	void reinit(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue)
	{
		init_d3d12(pSwapChain, pCommandQueue);
		ImGui_ImplDX12_CreateDeviceObjects();
	}

	////////////////////////////////////////////////////////
	//*
	//*     功能: ImGui 开始函数
	//*   返回值: 由指定字体文件路径获取到的字体的对象, 如果已初始化, 直接返回之前第一次已经加载过的字体文件.
	//*    参数1: D3D12交换链对象
	//*    参数2: D3D12命令队列对象
	//*    参数3: 修改 Imgui UI 样式的函数
	//*    参数4: 指定字体的文件路径
	//*    参数5: 字体的大小
	//*
	//////////////////////////////////////////////////////////
	ImFont* start(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue, type::tImguiStyle SetStyleFunction, const char* main_font_path, float font_size)
	{
		static ImFont* s_main_font;

		if (is_ready && get_is_need_reset_imgui())
		{
			//重置 imgui.
			reinit(pSwapChain, pCommandQueue);

			//恢复信号
			is_need_reset_imgui = false;
		}

		if (is_ready)
			return s_main_font;

		init_d3d12(pSwapChain, pCommandQueue);

		//Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		if (SetStyleFunction == nullptr)
			ImGui::StyleColorsDark();
		else
			SetStyleFunction();

		ImGui_ImplWin32_Init(process::get_process_window());

		ImGui_ImplDX12_Init(
			d3d12::pDevice,
			d3d12::NUM_FRAMES_IN_FLIGHT,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			d3d12::pSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
			d3d12::pSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

		ImFont* main_font = io.Fonts->AddFontFromFileTTF(main_font_path, font_size, NULL, io.Fonts->GetGlyphRangesChineseFull());

		if (main_font == nullptr)
			Exit();

		s_main_font = main_font;

		//HOOK
		hooks::hook_WndProc();

		//IS OK
		is_ready = true;



		return s_main_font;
	}

	////////////////////////////////////////////////////////
	//*
	//*     功能: ImGui 添加字体函数
	//*   返回值: 由指定字体文件路径获取到的字体的对象
	//*    参数1: 指定字体的文件路径
	//*    参数2: 字体的大小
	//*
	//////////////////////////////////////////////////////////
	ImFont* add_font(const char* font_path, float font_size)
	{
		if (!is_ready)
			return nullptr;

		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->AddFontFromFileTTF(font_path, font_size, NULL, io.Fonts->GetGlyphRangesChineseFull());

		if (font == nullptr)
			Exit();

		return font;
	}

	void imgui_frame_header()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	HRESULT imgui_frame_end(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		// Rendering
		FrameContext* frameCtxt = WaitForNextFrameResources();
		UINT backBufferIdx = d3d12::pSwapChain->GetCurrentBackBufferIndex();

		{
			frameCtxt->CommandAllocator->Reset();
			static D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.pResource = d3d12::pID3D12ResourceArray[backBufferIdx];
			d3d12::pCommandList->Reset(frameCtxt->CommandAllocator, NULL);
			d3d12::pCommandList->ResourceBarrier(1, &barrier);
			d3d12::pCommandList->OMSetRenderTargets(1, &d3d12::RenderTargetDescriptorArray[backBufferIdx], FALSE, NULL);
			d3d12::pCommandList->SetDescriptorHeaps(1, &d3d12::pSrvDescHeap);
		}

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12::pCommandList);


		static D3D12_RESOURCE_BARRIER barrier = { 0 };
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.pResource = d3d12::pID3D12ResourceArray[backBufferIdx];

		d3d12::pCommandList->ResourceBarrier(1, &barrier);
		d3d12::pCommandList->Close();

		d3d12::pCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&d3d12::pCommandList);

		HRESULT results = ogr_function::Present(d3d12::pSwapChain, SyncInterval, Flags);

		UINT64 fenceValue = d3d12::fenceLastSignaledValue + 1;
		d3d12::pCommandQueue->Signal(d3d12::pFence, fenceValue);
		d3d12::fenceLastSignaledValue = fenceValue;
		frameCtxt->FenceValue = fenceValue;

		return results;
	}

	void imgui_no_border(type::tESP esp_function, ImFont* font)
	{
		ImGuiStyle& Style = ImGui::GetStyle();
		const float bor_size = Style.WindowBorderSize;
		Style.WindowBorderSize = 0.0f;

		esp_function(font);

		Style.WindowBorderSize = bor_size;
	}
}
