#pragma once
#include "stdafx.h"

namespace VTManager
{
	template<typename T> __forceinline T vfunc(void* base, __int64 index)
	{
		QWORD* vTabella = *(QWORD**)base;
		return (T)vTabella[index];
	}

	template<typename T> __forceinline T modify(void* base, __int64 index, T hk_function)
	{
		QWORD* vTabella = *(QWORD**)base;
		DWORD oldProtect;
		if (VirtualProtect(&vTabella[index], sizeof(QWORD), PAGE_READWRITE, &oldProtect))
		{
			QWORD ogr_function = vTabella[index];
			vTabella[index] = (QWORD)hk_function;
			if (VirtualProtect(&vTabella[index], sizeof(QWORD), oldProtect, &oldProtect))
			{
				return (T)ogr_function;
			}
		}
		return (T)0;
	}
}