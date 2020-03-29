#include "stdafx.h"
#include "sdk.h"
#include "macro.h"
#include "hooks.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		cod_data::init();
	    hooks::hook_present();
		break;
	}
	}
	return TRUE;
}