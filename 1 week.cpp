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
double GetProcessCpu(unsigned long pid, unsigned long long curKernel, unsigned long long curUser, unsigned long long sysDelta) {
    int idx = FindPidInCpuHistory(pid);
    
    if (idx != -1) {
        unsigned long long prevTotal = cpuHistory[idx].lastKernel + cpuHistory[idx].lastUser;
        unsigned long long curTotal = curKernel + curUser;
        
        if (curTotal >= prevTotal && sysDelta > 0) {
            double percent = (double)((curTotal - prevTotal) * 100) / (double)sysDelta;
            cpuHistory[idx].lastKernel = curKernel;
            cpuHistory[idx].lastUser = curUser;
            return (percent > 100) ? 100 : (percent < 0 ? 0 : percent);
        }
    }
    else {
        int newIdx = AddPidToCpuHistory(pid);
        if (newIdx != -1) {
            cpuHistory[newIdx].lastKernel = curKernel;
            cpuHistory[newIdx].lastUser = curUser;
        }
    }
    return 0.0;
}
int main(){
  retirn 0;
}
