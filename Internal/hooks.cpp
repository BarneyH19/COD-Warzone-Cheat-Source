#include "stdafx.h"
#include "hooks.h"
#include "hooker.h"
#include "macro.h"
#include "process.h"
#include "VTManager.h"
#include "sdk.h"

namespace hooks
{
	void hook_present()
	{
		QWORD swap_chain_base_address = *(QWORD*)(cod_data::image_base + decrypt::D3D12_SWAP_CHAIN_OFFSET);

		if (swap_chain_base_address == 0 || *(QWORD*)(cod_data::image_base + decrypt::D3D12_CommandQueue_OFFSET) == 0)
			Exit();

		//hooked Present
		ogr_function::Present = VTManager::modify<type::tPresent>((void*)swap_chain_base_address, 8, hooker::hkPresent); //vt hook
	}

	void hook_WndProc()
	{
		ogr_function::WndProc = (WNDPROC)SetWindowLongPtrW(process::get_process_window(), GWLP_WNDPROC, (LONG_PTR)hooker::hkWndProc);
	}
}
