# Old Strike Trigger Bot

Trigger bot for Old Strike using Windows API to read memory and simulate mouse input.

<div align="center">
 
```txt
This project is for educational and research purposes only.
The author does not condone cheating or violating the terms of service of any software.
Any misuse of this code is the sole responsibility of the user.
```
</div>

<br>

## Requirements

- Windows x64
- GCC via MSYS2 (mingw-w64-ucrt-x86_64-gcc)

## Configuration

Edit `config.h` to match your target:

| Constant | Description |
|---|---|
| `PROCESS_NAME` | Target process executable name |
| `MODULE_NAME` | Module to read the base address from |
| `BASE_OFFSET` | Offset from the module base |
| `OFFSETS` | Pointer chain offsets |
| `POLL_INTERVAL_MS` | Polling interval in milliseconds |

## Building

```bash
g++ -O2 -std=c++17 -o triggerbot.exe main.cpp memory.cpp process.cpp mouse.cpp -luser32 -lpsapi
```

## Usage

Run as Administrator, then launch the executable:

```bash
triggerbot.exe
```

Press `Ctrl+C` to stop.

## Project Structure

```
triggerbot/
├── config.h       # Constants and pointer chain configuration
├── process.h/cpp  # Finds the target process by name
├── memory.h/cpp   # Memory reading and pointer chain resolution
├── mouse.h/cpp    # Left mouse button input via SendInput
└── main.cpp       # Main loop
```

## Python Version
 
A Python version (`triggerbot.py`) is also available in the repository. It requires `pymem` and `pyautogui` and works the same way, but is slower and depends on a Python installation. The C++ version is recommended for better performance and no external dependencies.
 
To run the Python version:
 
```bash
pip install pymem pyautogui
python triggerbot.py
```
