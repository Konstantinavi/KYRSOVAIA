#include "Common.h"

long* guiPids = nullptr;
int guiPidsCapacity = 0;
int guiPidsCount = 0;

void ClearProcessStrings(ProcessInfo* processList, int count) {
    if (processList == nullptr) return;
    for (int i = 0; i < count; i++) {
        if (processList[i].name != nullptr) {
            delete[] processList[i].name;
            processList[i].name = nullptr;
        }
        if (processList[i].statusStr != nullptr) {
            delete[] processList[i].statusStr;
            processList[i].statusStr = nullptr;
        }
    }
}

void EnsureProcessesCapacity(ProcessInfo*& processList, ProcessCpuTime*& cpuHistoryList,
    int needed, int& currentCapacity, int& cpuHistoryCapacity) {
    if (needed <= currentCapacity) return;

    int newCapacity = (currentCapacity == 0) ? 100 : currentCapacity * 2;
    while (newCapacity < needed) newCapacity *= 2;

    ProcessInfo* newProcessList = nullptr;
    ProcessCpuTime* newCpuHistory = nullptr;

    try {
        newProcessList = new ProcessInfo[newCapacity]();
        newCpuHistory = new ProcessCpuTime[newCapacity]();
    }
    catch (const std::bad_alloc&) {
        delete[] newProcessList; 
        delete[] newCpuHistory;
        
        newCapacity = needed;
        try {
            newProcessList = new ProcessInfo[newCapacity]();
            newCpuHistory = new ProcessCpuTime[newCapacity]();
            std::wcerr << L"ПРЕДУПРЕЖДЕНИЕ: Мало памяти. Выделено впритык: " << newCapacity << std::endl;
        }
        catch (const std::bad_alloc& e) {
            delete[] newProcessList; 
            delete[] newCpuHistory;
            std::wcerr << L"КРИТИЧЕСКАЯ ОШИБКА! Системная ошибка: " << e.what() << std::endl;
            exit(1);
        }
    }

    int elementsToCopy = (currentCapacity < needed) ? currentCapacity : needed;
    for (int i = 0; i < elementsToCopy; i++) {
        if (cpuHistoryList != nullptr) {
            newCpuHistory[i] = cpuHistoryList[i]; 
        }

        if (processList != nullptr) {
            newProcessList[i] = processList[i]; 
            
            newProcessList[i].name = nullptr;
            newProcessList[i].statusStr = nullptr;

            if (processList[i].name != nullptr) {
                int len = static_cast<int>(wcslen(processList[i].name)) + 1;
                wchar_t* allocatedName = new(std::nothrow) wchar_t[len];
                if (allocatedName != nullptr) {
                    wcscpy_s(allocatedName, len, processList[i].name);
                    newProcessList[i].name = allocatedName;
                }
            }
            
            if (processList[i].statusStr != nullptr) {
                int len = static_cast<int>(wcslen(processList[i].statusStr)) + 1;
                wchar_t* allocatedStatus = new(std::nothrow) wchar_t[len];
                if (allocatedStatus != nullptr) {
                    wcscpy_s(allocatedStatus, len, processList[i].statusStr);
                    newProcessList[i].statusStr = allocatedStatus;
                }
            }
        }
    }

    if (processList != nullptr) {
        for (int i = 0; i < elementsToCopy; i++) {
            delete[] processList[i].name;
            delete[] processList[i].statusStr;
        }
        delete[] processList;
    }
    delete[] cpuHistoryList;

    processList = newProcessList;
    cpuHistoryList = newCpuHistory;
    currentCapacity = newCapacity;
    cpuHistoryCapacity = newCapacity; 
}

void EnsureGuiPidsCapacity(int needed) {
    if (needed <= guiPidsCapacity) return;

    int newCapacity = (guiPidsCapacity == 0) ? 100 : guiPidsCapacity * 2;
    while (newCapacity < needed) newCapacity *= 2;

    long* newArray = nullptr;
    try {
        newArray = new long[newCapacity];
    }
    catch (const std::bad_alloc&) {
        newCapacity = needed;
        try {
            newArray = new long[newCapacity];
        }
        catch (const std::bad_alloc&) {
            std::wcerr << L"КРИТИЧЕСКАЯ ОШИБКА: Недостаточно памяти для GUI PIDs!" << std::endl;
            exit(1);
        }
    }

    for (int i = 0; i < guiPidsCount; i++) {
        newArray[i] = guiPids[i];
    }

    delete[] guiPids;
    guiPids = newArray;
    guiPidsCapacity = newCapacity;
}

void AddPidToGuiArray(long pid) {
    for (int i = 0; i < guiPidsCount; i++) if (guiPids[i] == pid) return;
    EnsureGuiPidsCapacity(guiPidsCount + 1);
    guiPids[guiPidsCount++] = pid;
}

bool IsPidInGuiArray(long pid) {
    for (int i = 0; i < guiPidsCount; i++) if (guiPids[i] == pid) return true;
    return false;
}

void ClearGuiArray() {
    delete[] guiPids;
    guiPids = nullptr;
    guiPidsCount = 0;
    guiPidsCapacity = 0;
}

int main() {
    return 0;
}
