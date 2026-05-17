#include "Common.h"

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
    int needed, int& currentCapacity) {
    if (needed <= currentCapacity) return;

    int newCapacity = (currentCapacity == 0) ? 100 : currentCapacity * 2;
    while (newCapacity < needed) newCapacity *= 2;

    ProcessInfo* newProcessList = new ProcessInfo[newCapacity]();
    ProcessCpuTime* newCpuHistory = new ProcessCpuTime[newCapacity]();

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
            std::wcerr << L"КРИТИЧЕСКАЯ ОШИБКА! Системная ошибка: " << e.what() << std::endl;
            exit(1);
        }
    }

    int elementsToCopy = (currentCapacity < needed) ? currentCapacity : needed;
    for (int i = 0; i < elementsToCopy; i++) {
        if (cpuHistoryList != nullptr) {
            newCpuHistory[i] = cpuHistoryList[i];
                    if (processList != nullptr) {
            newProcessList[i] = processList[i]; 
            newProcessList[i].name = nullptr;
            newProcessList[i].statusStr = nullptr;

            if (processList[i].name != nullptr) {
                size_t len = wcslen(processList[i].name) + 1;
                newProcessList[i].name = new(std::nothrow) wchar_t[len];
                if (newProcessList[i].name) wcscpy_s(newProcessList[i].name, len, processList[i].name);
            }
            if (processList[i].statusStr != nullptr) {
                size_t len = wcslen(processList[i].statusStr) + 1;
                newProcessList[i].statusStr = new(std::nothrow) wchar_t[len];
                if (newProcessList[i].statusStr) wcscpy_s(newProcessList[i].statusStr, len, processList[i].statusStr);
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
}


int main() {
  return 0;
}
