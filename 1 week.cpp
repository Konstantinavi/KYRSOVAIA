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

int main() {
  return 0;
}
