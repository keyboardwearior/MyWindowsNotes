#include <stdio.h>
#include <windows.h>

int main(void){

//     int MessageBox(
//   [in, optional] HWND    hWnd,A handle to the owner window of the message box to be created
//   [in, optional] LPCTSTR lpText, Message to show
//   [in, optional] LPCTSTR lpCaption, caption
//   [in]           UINT    uType, The contents and behavior of the dialog box
// );

int result = MessageBox(NULL, "Hello from windows", "Hi there", MB_OK);
return 0;
}