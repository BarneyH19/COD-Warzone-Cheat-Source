// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <intrin.h>
#include <stdio.h>
#include <iostream>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <intrin.h>
#include <string>
#include <map>
#include <vector>
#include <atomic>
#include <sstream>
#include <random>
#include <time.h>
//#include <d3dx9math.h>
#include <TlHelp32.h>
#include <list>

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d12.lib")