#pragma once

#include <string>
#include <Windows.h>
#include <iphlpapi.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <wincrypt.h> // Windows 加密库
#include <VMProtectSDK.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "crypt32.lib")

class MachineCodeGenerator {
public:
    // 获取第一个网络适配器的MAC地址
    static std::string GetFirstMacAddress() {

        std::vector<char> buffer;
        DWORD bufferSize = 0;

        // 获取缓冲区大小
        if (GetAdaptersInfo(nullptr, &bufferSize) != ERROR_BUFFER_OVERFLOW) {
            return "Error";
        }

        buffer.resize(bufferSize);
        PIP_ADAPTER_INFO adapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(buffer.data());

        if (GetAdaptersInfo(adapterInfo, &bufferSize) != NO_ERROR) {
            return "Error";
        }

        // 获取第一个适配器的MAC地址
        std::stringstream macStream;
        macStream << std::hex << std::setfill('0');
        for (int i = 0; i < adapterInfo->AddressLength; ++i) {
            macStream << std::setw(2) << static_cast<int>(adapterInfo->Address[i]);
        }

        return macStream.str();

    }

    // 使用 Windows CryptoAPI 生成32位哈希值（MD5）
    static std::string GenerateMachineCode() {

        std::string macAddress = GetFirstMacAddress();
        if (macAddress == "Error") {
            return "Error";
        }

        // 初始化 Windows 加密上下文
        HCRYPTPROV hProv = NULL;
        HCRYPTHASH hHash = NULL;
        BYTE hash[16]; // MD5 哈希固定为 16 字节
        DWORD hashLength = sizeof(hash);

        if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
            return "Error";
        }

        if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
            CryptReleaseContext(hProv, 0);
            return "Error";
        }

        // 计算哈希值
        if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(macAddress.c_str()), macAddress.size(), 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return "Error";
        }

        if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLength, 0)) {
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return "Error";
        }

        // 转换哈希值为32位十六进制字符串
        std::stringstream hashStream;
        hashStream << std::hex << std::setfill('0');
        for (DWORD i = 0; i < hashLength; ++i) {
            hashStream << std::setw(2) << static_cast<int>(hash[i]);
        }

        // 释放资源
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);

        return hashStream.str();

    }
};


