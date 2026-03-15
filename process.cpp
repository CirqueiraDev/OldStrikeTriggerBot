#include "process.h"
#include <TlHelp32.h>
#include <stdexcept>

DWORD FindProcessId(const std::string& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to create process snapshot");

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                CloseHandle(hSnapshot);
                return pe.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
    return 0;
}
