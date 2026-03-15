#pragma once
#include <cstdint>
#include <vector>
#include <string>

#define PROCESS_NAME     "Old Strike.exe"
#define MODULE_NAME      "GameAssembly.dll"
#define BASE_OFFSET      0x02635950
#define POLL_INTERVAL_MS 10

static const std::vector<uintptr_t> OFFSETS = { 0xB8, 0xC };