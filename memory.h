#pragma once
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>

HANDLE OpenProcessFull(DWORD pid);
uintptr_t GetModuleBase(HANDLE hProcess, DWORD pid, const std::string& moduleName);
uintptr_t ResolvePointerChain(HANDLE hProcess, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);

template<typename T>
T ReadMemory(HANDLE hProcess, uintptr_t address) {
    T value{};
    ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), nullptr);
    return value;
}
