#include "memory.h"
#include <Psapi.h>
#include <stdexcept>

HANDLE OpenProcessFull(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
        throw std::runtime_error("Failed to open process (run as Administrator)");
    return hProcess;
}

uintptr_t GetModuleBase(HANDLE hProcess, DWORD pid, const std::string& moduleName) {
    HMODULE hMods[1024];
    DWORD cbNeeded;

    if (!EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
        throw std::runtime_error("Failed to enumerate modules (run as Administrator)");

    DWORD count = cbNeeded / sizeof(HMODULE);
    for (DWORD i = 0; i < count; i++) {
        char modName[MAX_PATH];
        if (GetModuleFileNameExA(hProcess, hMods[i], modName, sizeof(modName))) {
            std::string path(modName);
            std::string base = path.substr(path.find_last_of("\\/") + 1);
            if (base == moduleName)
                return reinterpret_cast<uintptr_t>(hMods[i]);
        }
    }

    throw std::runtime_error("Module not found: " + moduleName);
}

uintptr_t ResolvePointerChain(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets) {
    uintptr_t addr = ReadMemory<uintptr_t>(hProcess, baseAddress);

    for (size_t i = 0; i < offsets.size() - 1; i++)
        addr = ReadMemory<uintptr_t>(hProcess, addr + offsets[i]);

    return addr + offsets.back();
}