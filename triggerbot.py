import pymem
import pymem.process
import ctypes
import pyautogui
import time

PROCESS_NAME  = "Old Strike.exe"
MODULE_NAME   = "GameAssembly.dll"
BASE_OFFSET   = 0x02635950
OFFSETS       = [0xB8, 0xC]
POLL_INTERVAL = 0.01

PROCESS_ALL_ACCESS = 0x1F0FFF

def open_process_full(pid):
    return ctypes.windll.kernel32.OpenProcess(PROCESS_ALL_ACCESS, False, pid)

def read_bytes_raw(handle, address, size):
    buf = ctypes.create_string_buffer(size)
    read = ctypes.c_size_t(0)
    ctypes.windll.kernel32.ReadProcessMemory(handle, ctypes.c_void_p(address), buf, size, ctypes.byref(read))
    return buf.raw

def read_int(handle, address):
    buf = read_bytes_raw(handle, address, 4)
    return int.from_bytes(buf, 'little')

def read_ptr(handle, address):
    buf = read_bytes_raw(handle, address, 8)  # ponteiro 64-bit
    return int.from_bytes(buf, 'little')

def resolve_pointer_chain(handle, base_address, offsets):
    addr = read_ptr(handle, base_address)
    for offset in offsets[:-1]:
        addr = read_ptr(handle, addr + offset)
    return addr + offsets[-1]

def get_module_base(pm, module_name):
    module = pymem.process.module_from_name(pm.process_handle, module_name)
    return module.lpBaseOfDll

def main():
    print("Coded by CirqueiraDev\n")
    print(f"[*] Conectando ao processo: {PROCESS_NAME}")
    try:
        pm = pymem.Pymem(PROCESS_NAME)
        print(f"[+] Conectado! PID: {pm.process_id}")
    except pymem.exception.ProcessNotFound:
        print(f"[!] Processo não encontrado.")
        return

    handle = open_process_full(pm.process_id)

    module_base = get_module_base(pm, MODULE_NAME)
    print(f"[+] Base do módulo {MODULE_NAME}: {hex(module_base)}")

    base_address = module_base + BASE_OFFSET
    print(f"[+] Endereço base: {hex(base_address)}")
    print("[*] Pressione Ctrl+C para parar.\n")

    button_held = False

    while True:
        try:
            final_address = resolve_pointer_chain(handle, base_address, OFFSETS)
            value = read_int(handle, final_address)

            if value != 0 and not button_held:
                print(f"[+] Valor={value} | Segurando clique...")
                pyautogui.mouseDown(button='left')
                button_held = True

            elif value == 0 and button_held:
                print(f"[-] Valor=0 | Soltando clique...")
                pyautogui.mouseUp(button='left')
                button_held = False

            time.sleep(POLL_INTERVAL)

        except KeyboardInterrupt:
            if button_held:
                pyautogui.mouseUp(button='left')
            print("\n[*] Encerrado.")
            break
        except Exception as e:
            if button_held:
                pyautogui.mouseUp(button='left')
                button_held = False
            print(f"[!] Erro: {e}")
            time.sleep(1)

if __name__ == "__main__":
    main()