#include <iostream>
#include <stdexcept>
#include <csignal>
#include <Windows.h>

#include "config.h"
#include "process.h"
#include "memory.h"
#include "mouse.h"

static bool g_running = true;
static bool g_buttonHeld = false;

void OnSignal(int) {
    g_running = false;
}

int main() {
    std::signal(SIGINT, OnSignal);

    std::cout << "Coded by CirqueiraDev\n";
    std::cout << "[*] Connecting to process: " << PROCESS_NAME << "\n";

    DWORD pid = FindProcessId(PROCESS_NAME);
    if (!pid) {
        std::cerr << "[!] Process not found.\n";
        return 1;
    }
    std::cout << "[+] Connected! PID: " << pid << "\n";

    HANDLE hProcess;
    try {
        hProcess = OpenProcessFull(pid);
    } catch (const std::exception& e) {
        std::cerr << "[!] " << e.what() << "\n";
        return 1;
    }

    uintptr_t moduleBase;
    try {
        moduleBase = GetModuleBase(hProcess, pid, MODULE_NAME);
    } catch (const std::exception& e) {
        std::cerr << "[!] " << e.what() << "\n";
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "[+] Module base " << MODULE_NAME << ": 0x" << std::hex << moduleBase << "\n";

    uintptr_t baseAddress = moduleBase + BASE_OFFSET;
    std::cout << "[+] Base address: 0x" << baseAddress << std::dec << "\n";
    std::cout << "[*] Press Ctrl+C to stop.\n\n";

    while (g_running) {
        try {
            uintptr_t finalAddress = ResolvePointerChain(hProcess, baseAddress, OFFSETS);
            int32_t value = ReadMemory<int32_t>(hProcess, finalAddress);

            if (value != 0 && !g_buttonHeld) {
                std::cout << "[+] Value=" << value << " | Holding click...\n";
                MouseDown();
                g_buttonHeld = true;
            } else if (value == 0 && g_buttonHeld) {
                std::cout << "[-] Value=0 | Releasing click...\n";
                MouseUp();
                g_buttonHeld = false;
            }
        } catch (const std::exception& e) {
            if (g_buttonHeld) {
                MouseUp();
                g_buttonHeld = false;
            }
            std::cerr << "[!] Error: " << e.what() << "\n";
            Sleep(1000);
        }

        Sleep(POLL_INTERVAL_MS);
    }

    if (g_buttonHeld)
        MouseUp();

    CloseHandle(hProcess);
    std::cout << "[*] Stopped.\n";
    return 0;
}
