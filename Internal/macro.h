#pragma once
#define QWORD unsigned __int64
#define EXPORT extern "C" _declspec(dllexport)
#define Exit() TerminateProcess(GetCurrentProcess(),0)
