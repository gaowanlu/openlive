#pragma once

#include <string>
#include <cstdint>

namespace Conf
{
    class Conf
    {
    public:
        Conf() = delete;

    public:
        static void setPath(const std::string &path);
        static void setEncodeBufferLen(int64_t len);
        static int64_t getEncodeBufferLen();
        static void setCaptureBufferLen(int64_t len);
        static int64_t getCaptureBufferLen(int64_t len);
        static std::string getPath();
        static void setConf(std::string key, std::string value);
        static void setConf(std::string key, int64_t value);

    private:
        static std::string path;
        static int64_t encodeBufferLen;
        static int64_t captureBufferLen;
    };
}