#include "discord-1.h"
#include <iostream>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

std::wstring StringToWString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string SendHttpRequest(const std::string& server, const std::string& path) {
    HINTERNET hInternet = InternetOpen(L"HTTP_Request", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cerr << "InternetOpen failed" << std::endl;
        return "";
    }

    std::wstring wServer = StringToWString(server);
    HINTERNET hConnect = InternetConnect(hInternet, wServer.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        std::cerr << "InternetConnect failed" << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    std::wstring wPath = StringToWString(path);
    HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", wPath.c_str(), NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
    if (!hRequest) {
        std::cerr << "HttpOpenRequest failed" << std::endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    if (!HttpSendRequest(hRequest, NULL, 0, NULL, 0)) {
        std::cerr << "HttpSendRequest failed" << std::endl;
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::string response;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead) {
        buffer[bytesRead] = '\0';
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

void printMessage()
{
    std::string server = "example.com";
    std::string path = "/";
    std::string response = SendHttpRequest(server, path);
    std::cout << "Response: " << response << std::endl;
}