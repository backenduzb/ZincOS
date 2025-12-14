cmake_minimum_required(VERSION 3.25)
project(ZincOS C ASM_NASM)

enable_language(ASM_NASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib")
set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf32")

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../boot)

message(STATUS "SOURCE_DIR: ${SOURCE_DIR}")
message(STATUS "Checking kernel.c exists: ${SOURCE_DIR}/kernel/kernel.c")

if(NOT EXISTS ${SOURCE_DIR}/kernel/kernel.c)
    message(FATAL_ERROR "kernel.c not found at: ${SOURCE_DIR}/kernel/kernel.c")
endif()

include_directories(
    ${SOURCE_DIR}/kernel/config
    ${SOURCE_DIR}/kernel/functions/c/screen
    ${SOURCE_DIR}/kernel/functions/c/string
    ${SOURCE_DIR}/kernel
)

set(ASM_SOURCES
    ${SOURCE_DIR}/kernel/kernel.asm
    ${SOURCE_DIR}/kernel/keyboard/keyboard.asm
)

if(EXISTS ${SOURCE_DIR}/kernel/functions/asm/shutdown.asm)
    list(APPEND ASM_SOURCES ${SOURCE_DIR}/kernel/functions/asm/shutdown.asm)
else()
    message(STATUS "Note: shutdown.asm not found, skipping...")
endif()

set(ASM_OBJS "")

foreach(asmfile ${ASM_SOURCES})
    if(EXISTS ${asmfile})
        get_filename_component(filename ${asmfile} NAME_WE)
        set(objfile ${CMAKE_BINARY_DIR}/${filename}.o)
        
        message(STATUS "Adding ASM: ${asmfile}")
        
        add_custom_command(
            OUTPUT ${objfile}
            COMMAND nasm -f elf32 "${asmfile}" -o "${objfile}"
            DEPENDS ${asmfile}
            COMMENT "Assembling ${filename}.asm"
        )

        
        list(APPEND ASM_OBJS ${objfile})
    else()
        message(WARNING "ASM file not found: ${asmfile}")
    endif()
endforeach()

set(C_SOURCES
    ${SOURCE_DIR}/kernel/kernel.c
    ${SOURCE_DIR}/kernel/functions/c/screen/functions.c
    ${SOURCE_DIR}/kernel/functions/c/string/functions.c
    ${SOURCE_DIR}/kernel/keyboard/keyboard.c
    ${SOURCE_DIR}/kernel/timing/sleep.c
)

foreach(cfile ${C_SOURCES})
    if(NOT EXISTS ${cfile})
        message(WARNING "C file not found: ${cfile}")
    else()
        message(STATUS "Found C file: ${cfile}")
    endif()
endforeach()

add_executable(mykernel ${C_SOURCES} ${ASM_OBJS})

set_target_properties(mykernel PROPERTIES
    LINK_FLAGS "-T ${SOURCE_DIR}/linker.ld -m32 -nostdlib -static"
)

add_custom_command(TARGET mykernel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mykernel> ${CMAKE_BINARY_DIR}/../boot/mykernel.bin
    COMMENT "Copying kernel to boot directory"
)

add_custom_target(iso ALL
    DEPENDS mykernel
    COMMAND echo "=== Building ISO ==="
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/../iso/boot/grub
    COMMAND cp ${CMAKE_BINARY_DIR}/../boot/mykernel.bin ${CMAKE_BINARY_DIR}/../iso/boot/
    COMMAND echo "set timeout=10" > ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND echo "set default=0" >> ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND echo "menuentry \"Zinc OS\" {" >> ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND echo "    multiboot /boot/mykernel.bin" >> ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND echo "    boot" >> ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND echo "}" >> ${CMAKE_BINARY_DIR}/../iso/boot/grub/grub.cfg
    COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/../zinc-0.0.1.iso ${CMAKE_BINARY_DIR}/../iso/ 2>/dev/null
    COMMAND echo "=== SUCCESS! ==="
    COMMAND echo "ISO file: ${CMAKE_BINARY_DIR}/../zinc-0.0.1.iso"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Creating ISO image"
)
