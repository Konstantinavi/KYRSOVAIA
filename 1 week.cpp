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

    int elementsToCopy = (currentCapacity < needed) ? currentCapacity : needed;
    for (int i = 0; i < elementsToCopy; i++) {
        if (cpuHistoryList != nullptr) {
            newCpuHistory[i] = cpuHistoryList[i];
        }
    }

    delete[] processList;
    delete[] cpuHistoryList;

    processList = newProcessList;
    cpuHistoryList = newCpuHistory;
    currentCapacity = newCapacity;
    cpuHistoryCapacity = newCapacity; 
}


int main() {
  return 0;
}
