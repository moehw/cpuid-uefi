#include "cpuid.h"

VOID
EFIAPI
PrintColor(
    IN UINTN ColorAttribute,
    IN CONST CHAR16* Format,
    ...
)
{
    VA_LIST Marker;
    UINTN AttributeOriginal;

    VA_START(Marker, Format);

    AttributeOriginal = gST->ConOut->Mode->Attribute;
    gST->ConOut->SetAttribute(gST->ConOut, ColorAttribute);
    CHAR16* Data = CatVSPrint(NULL, Format, Marker);
    if (Data != NULL) {
        gST->ConOut->OutputString(gST->ConOut, Data);
        FreePool(Data);
    }

    gST->ConOut->SetAttribute(gST->ConOut, AttributeOriginal);
}

EFI_STATUS
EFIAPI
ClearScreen()
{
    return gST->ConOut->ClearScreen(gST->ConOut);
}

VOID
EFIAPI
Pause()
{
    UINTN EventIndex;
    CONST CHAR16 PauseText[] = L"Press any key to continue...";
    Print(PauseText);
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->Reset(gST->ConIn, FALSE);
    gST->ConOut->SetCursorPosition(gST->ConOut, 0, gST->ConOut->Mode->CursorRow);
    
    UINTN Index;
    for (Index = 0; Index < sizeof(PauseText); Index++) {
        Print(L" ");
    }

    gST->ConOut->SetCursorPosition(gST->ConOut, 0, gST->ConOut->Mode->CursorRow);
}

VOID
EFIAPI
PrintVendorId()
{
    EFI_STATUS Status;
    struct VENDOR_ID VendorId = { 0 };
    AsmCpuidEx(0, 0, 0, &VendorId.Ebx, &VendorId.Ecx, &VendorId.Edx);
    
    CHAR16 *VendorIdName = AllocatePool(sizeof(VendorId.Name) * sizeof(CHAR16));
    Status = AsciiStrToUnicodeStrS(VendorId.Name, VendorIdName, sizeof(VendorId.Name));
    if (!EFI_ERROR(Status)) {
        PrintColor(EFI_WHITE, L"VendorID : %s\n\n", VendorIdName);
        FreePool(VendorIdName);
    }
}

BOOLEAN
EFIAPI
PrintFeatureTable(
    IN UINT32 Function,
    IN UINT8  RegisterType,
    IN UINT32 RegisterValue,
    IN UINTN  Color1,
    IN UINTN  Color2
    )
{
    BOOLEAN IsOutput = FALSE;
    UINTN Index;
    UINTN CurrentColor = Color1;
    UINTN DiffColor = Color1 ^ Color2;

    for (Index = 0; Index < sizeof(Features); Index++) {
        if (Features[Index].Function < Function) {
            continue;
        }

        if (Features[Index].Function > Function) {
            break;
        }

        if (Features[Index].RegisterType == RegisterType) {
            IsOutput = TRUE;

            if (FLAG_ON(RegisterValue, Features[Index].BitMask)) {
                PrintColor(CurrentColor, L"%s", L"[+] ");
            }
            else {
                PrintColor(CurrentColor, L"%s", L"[-] ");
            }
            PrintColor(CurrentColor, L"%s\n", Features[Index].Name);
            CurrentColor ^= DiffColor;
        }
    }

    return IsOutput;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE* SystemTable
)
{
    UINT32 CONST Functions[] = {0, 0x80000000};
    UINT32 Index;
    UINT32 Eax = 0, Ebx = 0, Ecx = 0, Edx = 0;

    ClearScreen();

    // Full dump
    Print(L"  FUNC   :   EAX      EBX      ECX      EDX\n");
    for (Index = 0; Index < ARRAY_SIZE(Functions); Index++) {
        UINT32 MaxFunc = 0;
        UINT32 CurrentFunc = Functions[Index];
        AsmCpuidEx(CurrentFunc, 0, &MaxFunc, NULL, NULL, NULL);

        for (CurrentFunc; CurrentFunc <= MaxFunc; CurrentFunc++) {
            AsmCpuidEx(CurrentFunc, 0, &Eax, &Ebx, &Ecx, &Edx);
            if (!Eax && !Ebx && !Ecx && !Edx) continue;
            PrintColor(EFI_YELLOW, L"%08X ", CurrentFunc);
            PrintColor(EFI_LIGHTGRAY, L": %08X %08X %08X %08X\n", Eax, Ebx, Ecx, Edx);
        }
    }

    Print(L"\n");
    
    // Display features
    PrintVendorId();
    Pause();

    for (Index = 0; Index < ARRAY_SIZE(Functions); Index++) {
        UINT32 MaxFunc = 0;
        UINT32 CurrentFunc = Functions[Index];
        AsmCpuidEx(CurrentFunc, 0, &MaxFunc, NULL, NULL, NULL);

        for (CurrentFunc; CurrentFunc <= MaxFunc; CurrentFunc++) {
            AsmCpuidEx(CurrentFunc, 0, &Eax, &Ebx, &Ecx, &Edx);
            if (Edx) {
                if (PrintFeatureTable(CurrentFunc, REG_EDX, Edx, EFI_CYAN, EFI_WHITE)) {
                    Print(L"\n");
                    Pause();
                }
            }
            if (Ecx) {
                if (PrintFeatureTable(CurrentFunc, REG_ECX, Ecx, EFI_MAGENTA, EFI_WHITE)) {
                    Print(L"\n");
                    Pause();
                }
            }
        }
    }

    return EFI_SUCCESS;
}
