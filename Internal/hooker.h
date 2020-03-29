#pragma once
#include "stdafx.h"

namespace type
{
	typedef HRESULT(STDMETHODCALLTYPE* tPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef LRESULT(CALLBACK* tWndProc)(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);
}

namespace ogr_function
{
	extern type::tPresent Present;
	extern type::tWndProc WndProc;
}

namespace hooker
{
	HRESULT STDMETHODCALLTYPE hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	LRESULT hkWndProc(HWND hWnd, UINT Msg, WPARAM wp, LPARAM lp);
}