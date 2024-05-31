A code that's used to leak kernel object addresses during a research.

The code utilizes the "NtQuerySystemInformation()" Native API function to get the handle table information of the entire system:

        __kernel_entry NTSTATUS NtQuerySystemInformation(
                [in]            SYSTEM_INFORMATION_CLASS SystemInformationClass,
                [in, out]       PVOID                    SystemInformation,
                [in]            ULONG                    SystemInformationLength,
                [out, optional] PULONG                   ReturnLength

        );
        
Function's documentation: 
        
        https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation 

Update:

Added functionality that will leak all device drivers base addresses and names on the system using the following 2 API functions:

        BOOL EnumDeviceDrivers(
          [out] LPVOID  *lpImageBase,
          [in]  DWORD   cb,
          [out] LPDWORD lpcbNeeded
        );

        DWORD GetDeviceDriverBaseNameW(
          [in]  LPVOID ImageBase,
          [out] LPWSTR lpBaseName,
          [in]  DWORD  nSize
        );
        
Function's documentation: 
        
        https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getdevicedriverbasenamew)us/windows/win32/api/psapi/nf-psapi-getdevicedriverbasenamew
        https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumdevicedrivers
