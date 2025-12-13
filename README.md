```
███████╗██╗███╗   ██╗ ██████╗
╚══███╔╝██║████╗  ██║██╔════╝
  ███╔╝ ██║██╔██╗ ██║██║     
 ███╔╝  ██║██║╚██╗██║██║     
███████╗██║██║ ╚████║╚██████╗
╚══════╝╚═╝╚═╝  ╚═══╝ ╚═════╝
```

## ZINC OS

Minimal, educational operating system project built from scratch.

---

## ✨ Features

* Custom boot process
* VGA text-mode output
* Kernel written in C (with optional ASM)
* Simple memory and screen management
* Clean, readable, hackable codebase

---

## 🧱 Project Structure

```
ZINC
├── kernel
│   ├── config
│   │   ├── colors.c
│   │   ├── functions.c
│   │   └── settings.c
│   ├── functions
│   │   └── shutdown.asm
│   ├── kernel.asm
│   ├── kernel.c
│   ├── keyboard
│   │   ├── keyboard.asm
│   │   └── keyboard.c
│   ├── o
│   │   ├── functions_shutdown.o
│   │   ├── kernel_asm.o
│   │   ├── kernel.o
│   │   └── keyboard.o
│   └── timing
│       └── sleep.c
└── linker.ld

7 directories, 14 files

others coming soon ...
```

---

## 🚀 Build & Run

Requirements:

* `gcc` (cross-compiler recommended)
* `ld`, `make`

Build:

```bash
./build.sh
```
