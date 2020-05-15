#pragma once

//
// Basic UEFI Libraries
//
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>


//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>


#define REVERSE_16(A) (((A & 0x0FF00) >> 8) | ((A & 0x0FF) << 8))
#define REVERSE_32(A) (REVERSE_16(A) << 16 | (REVERSE_16(A >> 16)))

#define FLAG_ON(A, Mask) (((A) & (Mask)) != 0)

#define REG_EAX 0x00
#define REG_EBX 0x01
#define REG_ECX 0x02
#define REG_EDX 0x04

#pragma warning(disable:4201) // nonstandart extension used: nameless struct/union
struct VENDOR_ID {
    union VENDOR_ID_UNION
    {
        CHAR8 Name[13];
        struct VENDOR_ID_REG {
            UINT32 Ebx;
            UINT32 Edx;
            UINT32 Ecx;
        };
    };
};

struct CPUID_FLAGS {
    UINT32  Function;
    UINT8   RegisterType;
    UINT32  BitMask;
    CHAR16* Name;
};

// Sorted by function number
STATIC struct CPUID_FLAGS Features[] = {
    /* Standard (0000_0001h) */
    { 0x00000001, REG_EDX, 0x00000001, L"FPU - x87 FPU on chip"},
    { 0x00000001, REG_EDX, 0x00000002, L"VME - Virtual-8086 Mode Enhancement"},
    { 0x00000001, REG_EDX, 0x00000004, L"DE - Debugging Extensions"},
    { 0x00000001, REG_EDX, 0x00000008, L"PSE - Page Size Extensions"},
    { 0x00000001, REG_EDX, 0x00000010, L"TSC - Time Stamp Counter"},
    { 0x00000001, REG_EDX, 0x00000020, L"MSR - Model Spec. Reg., RDMSR and WRMSR support"},
    { 0x00000001, REG_EDX, 0x00000040, L"PAE - Physical Address Extensions"},
    { 0x00000001, REG_EDX, 0x00000080, L"MCE - Machine Check Exception"},
    { 0x00000001, REG_EDX, 0x00000100, L"CMPXCHG8B instruction"},
    { 0x00000001, REG_EDX, 0x00000200, L"APIC on Chip"},
    { 0x00000001, REG_EDX, 0x00000400, L"Reserved"},
    { 0x00000001, REG_EDX, 0x00000800, L"SYSENTER and SYSEXIT instructions"},
    { 0x00000001, REG_EDX, 0x00001000, L"MTRR - Memory Type Range Registers"},
    { 0x00000001, REG_EDX, 0x00002000, L"PGE - PTE Global Bit"},
    { 0x00000001, REG_EDX, 0x00004000, L"MCA - Machine Check Architecture"},
    { 0x00000001, REG_EDX, 0x00008000, L"CMOV - Conditional Move instruction"},
    { 0x00000001, REG_EDX, 0x00010000, L"PAT - Page Attribute Table"},
    { 0x00000001, REG_EDX, 0x00020000, L"PSE-36 - 36-bit Page Size Extension"},
    { 0x00000001, REG_EDX, 0x00040000, L"Processor Serial Number"},
    { 0x00000001, REG_EDX, 0x00080000, L"CLFLUSH instruction"},
    { 0x00000001, REG_EDX, 0x00100000, L"Reserved"},
    { 0x00000001, REG_EDX, 0x00200000, L"DS - Debug Store"},
    { 0x00000001, REG_EDX, 0x00400000, L"ACPI - TM and Clock Ctrl"},
    { 0x00000001, REG_EDX, 0x00800000, L"MMX instruction set"},
    { 0x00000001, REG_EDX, 0x01000000, L"FXSAVE/FXRSTOR instructions"},
    { 0x00000001, REG_EDX, 0x02000000, L"SSE instructions"},
    { 0x00000001, REG_EDX, 0x04000000, L"SSE2 instructions"},
    { 0x00000001, REG_EDX, 0x08000000, L"SS - Self-Snoop (CPU Cache)"},
    { 0x00000001, REG_EDX, 0x10000000, L"HTT - Hyper-threading"},
    { 0x00000001, REG_EDX, 0x20000000, L"TM - Thermal Monitor"},
    { 0x00000001, REG_EDX, 0x40000000, L"IA64 Processor Emulating x86"},
    { 0x00000001, REG_EDX, 0x80000000, L"PBE - Pending Break Enable"},

    { 0x00000001, REG_ECX, 0x00000001, L"SSE3 instructions"},
    { 0x00000001, REG_ECX, 0x00000002, L"PCLMULQDQ - Carryles Multiplication"},
    { 0x00000001, REG_ECX, 0x00000004, L"64-bit Debug Store Area"},
    { 0x00000001, REG_ECX, 0x00000008, L"MONITOR/MWAIT instructions"},
    { 0x00000001, REG_ECX, 0x00000010, L"DS-CPL - CPL Qualified Debug Store"},
    { 0x00000001, REG_ECX, 0x00000020, L"VMX - Virtual Machine Extensions"},
    { 0x00000001, REG_ECX, 0x00000040, L"SMX - Safer Mode Extensions"},
    { 0x00000001, REG_ECX, 0x00000080, L"EST - Enhanced Intel SpeedStep"},
    { 0x00000001, REG_ECX, 0x00000100, L"TM2 - Thermal Monitor 2"},
    { 0x00000001, REG_ECX, 0x00000200, L"SSSE3 instructions"},
    { 0x00000001, REG_ECX, 0x00000400, L"L1 Context ID"},
    { 0x00000001, REG_ECX, 0x00000800, L"Silicon Debug Interface"},
    { 0x00000001, REG_ECX, 0x00001000, L"FMA - Fused Multiply-Add"},
    { 0x00000001, REG_ECX, 0x00002000, L"CMPXCHG16B instruction"},
    { 0x00000001, REG_ECX, 0x00004000, L"xTPR Update Control"},
    { 0x00000001, REG_ECX, 0x00008000, L"PDCM - Perfmon/Debug Capability MSR"},
    { 0x00000001, REG_ECX, 0x00010000, L"Reserved"},
    { 0x00000001, REG_ECX, 0x00020000, L"PCID - Process-context Identifiers"},
    { 0x00000001, REG_ECX, 0x00040000, L"DCA - Direct Cache Access for DMA"},
    { 0x00000001, REG_ECX, 0x00080000, L"SSE4.1"},
    { 0x00000001, REG_ECX, 0x00100000, L"SSE4.2"},
    { 0x00000001, REG_ECX, 0x00200000, L"x2APIC"},
    { 0x00000001, REG_ECX, 0x00400000, L"MOVBE instruction"},
    { 0x00000001, REG_ECX, 0x00800000, L"POPCNT instruction"},
    { 0x00000001, REG_ECX, 0x01000000, L"TSC deadline"},
    { 0x00000001, REG_ECX, 0x02000000, L"AES instruction"},
    { 0x00000001, REG_ECX, 0x04000000, L"XSAVE/XRSTOR instruction"},
    { 0x00000001, REG_ECX, 0x08000000, L"OS XSAVE/XRSTOR"},
    { 0x00000001, REG_ECX, 0x10000000, L"AVX instruction"},
    { 0x00000001, REG_ECX, 0x20000000, L"F16C - 16-bit FP Conversion instructions"},
    { 0x00000001, REG_ECX, 0x40000000, L"RDRAND instruction"},
    { 0x00000001, REG_ECX, 0x80000000, L"Hypervisor present"},
};
