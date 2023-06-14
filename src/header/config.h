#pragma once

#ifndef DLL_API
#ifdef DLL
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#endif

#define PI 3.1415926