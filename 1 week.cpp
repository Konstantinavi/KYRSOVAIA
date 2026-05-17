#include "Common.h"
struct ProcessCpuHistory {
    unsigned long pid;
    unsigned long long lastKernel;
    unsigned long long lastUser;
};
ProcessCpuHistory* cpuHistory = nullptr;
int cpuHistoryCount = 0;
int cpuHistoryCapacity = 0;

int FindPidInCpuHistory(unsigned long pid) {
    for (int i = 0; i < cpuHistoryCount; i++) {
        if (cpuHistory[i].pid == pid) return i;
    }
    return -1;
}

int AddPidToCpuHistory(unsigned long pid) {
    if (cpuHistoryCount >= cpuHistoryCapacity) {
        int newCap = (cpuHistoryCapacity == 0) ? 100 : cpuHistoryCapacity * 2;
        ProcessCpuHistory* newArr = new ProcessCpuHistory[newCap];
        for (int i = 0; i < cpuHistoryCount; i++) {
            newArr[i] = cpuHistory[i];
        }
        delete[] cpuHistory;
        cpuHistory = newArr;
        cpuHistoryCapacity = newCap;
    }
    cpuHistory[cpuHistoryCount].pid = pid;
    cpuHistory[cpuHistoryCount].lastKernel = 0;
    cpuHistory[cpuHistoryCount].lastUser = 0;
    return cpuHistoryCount++;
}
int main(){
  retirn 0;
}
