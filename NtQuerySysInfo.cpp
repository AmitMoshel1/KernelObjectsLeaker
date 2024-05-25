#include <Windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ntdll")

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
    PSYSTEM_HANDLE_INFORMATION SystemInformation = (PSYSTEM_HANDLE_INFORMATION)calloc(1, SystemHandleInformationSize);
    NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, SystemInformation, SystemHandleInformationSize, nullptr);
    printf("Number of Handles: %d\n", SystemInformation->NumberOfHandles);

    for (int i = 0; i < SystemInformation->NumberOfHandles; i++)
    {
        SYSTEM_HANDLE_TABLE_ENTRY_INFO Info = SystemInformation->Handles[i];
        printf("PID: %d     Kernel Object Address: 0x%p     Handle Value: 0x%d\n", Info.UniqueProcessId, Info.Object, Info.HandleValue);
        Sleep(500);
    }
    return 0;
}