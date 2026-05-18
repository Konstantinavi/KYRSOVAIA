#include "Common.h"
struct ProcessCpuHistory {
    unsigned long pid;
    unsigned long long lastKernel;
    unsigned long long lastUser;
};
struct DiskStats {
    unsigned long long lastRead;
    unsigned long long lastWrite;
    unsigned long long lastTime;
    bool initialized;
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
ProcCat DetectProcessCategory(unsigned long pid, const wchar_t* name) {
    if (_wcsicmp(name, L"svchost.exe") == 0 || _wcsicmp(name, L"csrss.exe") == 0 ||
        _wcsicmp(name, L"explorer.exe") == 0 || _wcsicmp(name, L"services.exe") == 0 ||
        _wcsicmp(name, L"lsass.exe") == 0) {
        return CAT_WINDOWS;
    }
    if (IsPidInGuiArray(pid)) return CAT_APP;
    return CAT_BACKGROUND;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd) && GetWindow(hwnd, GW_OWNER) == NULL) {
        unsigned long pid;
        GetWindowThreadProcessId(hwnd, &pid);
        AddPidToGuiArray(pid);
    }
    return TRUE;
}
void RefreshGuiProcessesList() {
    ClearGuiArray();
    EnumWindows(EnumWindowsProc, 0);
}
double GetRealDiskUsage(ProcessInfo& proc, unsigned long long currentRead, 
                        unsigned long long currentWrite, unsigned long long currentTime) {
    if (proc.lastUpdateTime != 0 && currentTime > proc.lastUpdateTime) {
        unsigned long long timeDelta = currentTime - proc.lastUpdateTime;
        unsigned long long readDiff = currentRead - proc.lastReadBytes;
        unsigned long long writeDiff = currentWrite - proc.lastWriteBytes;

        double speedMBps = (double)(readDiff + writeDiff) / (1048576.0 * (timeDelta / 1000.0));

        proc.lastReadBytes = currentRead;
        proc.lastWriteBytes = currentWrite;
        return speedMBps;
    }
    proc.lastReadBytes = currentRead;
    proc.lastWriteBytes = currentWrite;
    return 0.0;
}
int main(){
  retirn 0;
}
