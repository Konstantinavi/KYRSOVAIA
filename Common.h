#pragma once
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>

enum ProcCat { CAT_APP, CAT_BACKGROUND, CAT_WINDOWS };

struct ProcessInfo {
    unsigned long pid;
    wchar_t* name = nullptr;
    size_t memoryUsage = 0;
    double cpuUsage = 0.0;
    double diskUsage = 0.0;
    double netUsage = 0.0;
    ProcCat category = CAT_BACKGROUND;
    wchar_t* statusStr = nullptr;

    unsigned long long lastReadBytes = 0;
    unsigned long long lastWriteBytes = 0;
    unsigned long long lastNetBytes = 0;
    unsigned long long lastUpdateTime = 0;
};

struct ProcessCpuTime {
    unsigned long pid;
    unsigned long long lastKernel;
    unsigned long long lastUser;
    bool isActive;
};

void ClearProcessStrings(ProcessInfo* processList, int count);
