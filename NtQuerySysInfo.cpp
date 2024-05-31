#include <Windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <Psapi.h>

#pragma comment(lib, "ntdll")

#define NumberOfDeviceDrivers 1024
#define SystemHandleInformation 0x10
#define SystemHandleInformationSize 1024 * 1024 * 2

extern "C" NTSTATUS NtQuerySystemInformation(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeIndex;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;


int main()
{
    LPVOID drivers[NumberOfDeviceDrivers];  // Array of 1024 64-bit pointer addresses to device driver's load address
    DWORD NumberOfBytesReturned;
    int NumberOfDrivers;                           //Hold the number of drivers that are loaded in the system space
    WCHAR DriverNameBuffer[NumberOfDeviceDrivers];

    PSYSTEM_HANDLE_INFORMATION SystemInformation = (PSYSTEM_HANDLE_INFORMATION)calloc(1, SystemHandleInformationSize);
    NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, SystemInformation, SystemHandleInformationSize, nullptr);
    printf("Number of Handles: %d\n", SystemInformation->NumberOfHandles);
    /*
    It's recommended not to iterate over the entire handles of the system since it might make the program unstable...
    for (int i = 0; i < SystemInformation->NumberOfHandles; i++) 
    */
    for (int i = 0; i < 50; i++) //Listing only the first 50 handles within the system...
    {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO Info = SystemInformation->Handles[i];
        printf("PID: %d     Kernel Object Address: 0x%p     Handle Value: 0x%d\n", Info.UniqueProcessId, Info.Object, Info.HandleValue);
    }
    printf("\n[*] Leaking Device Drivers Base Addresses...\n\n");
    if (EnumDeviceDrivers(drivers, sizeof(drivers), &NumberOfBytesReturned)) //Returns the base addresses of the drivers to the "drivers" 64-bit pointer array
    {
        // Getting the name of each driver base on it's base address and storing the result in "DriverNameBuffer" variable
        
        NumberOfDrivers = NumberOfBytesReturned / sizeof(drivers[0]);
        for (int i = 0; i < NumberOfDrivers; i++)
        {
            if (GetDeviceDriverBaseName(drivers[i], DriverNameBuffer, sizeof(DriverNameBuffer) / sizeof(DriverNameBuffer[0])))
                wprintf(L"\t%d: Driver Name: %s            Base Address: 0x%p\n", i, DriverNameBuffer, drivers[i]);
        }
    }
    else
        return 1;
    return 0;
}