#include "stdafx.h"
#include "macro.h"

namespace process
{
	HWND hwnd;

	std::string get_system_window_dir()
	{
		char system_dir[MAX_PATH] = { 0 };
		if (GetSystemDirectory(system_dir, MAX_PATH) == 0)
			Exit();

		char* end = strrchr(system_dir, '\\');

		if (end == nullptr)
			Exit();

		std::string results(system_dir, 0, end + 1 - system_dir);
		results += "Fonts\\";
		return results;
	}

	BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
	{
		DWORD dwPid = 0;
		GetWindowThreadProcessId(hWnd, &dwPid);
		if (dwPid == lParam)
		{
			hwnd = hWnd;
			return FALSE;
		}
		return TRUE;
	}

	HWND get_process_window()
	{
		if (hwnd)
			return hwnd;

		EnumWindows(EnumWindowCallBack, GetCurrentProcessId());

		if (hwnd == NULL)
			Exit();

		return hwnd;
	}
}