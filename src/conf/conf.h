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
	static void setCaptureWidth(int64_t width);
	static int64_t getCaptureWidth();
	static void setCaptureHeight(int64_t height);
	static int64_t getCaptureHeight();
	static void setFPS(int64_t fps);
	static int64_t  getFPS();

    private:
        static std::string path;
        static int64_t encodeBufferLen;
        static int64_t captureBufferLen;
	static int64_t captureWidth;
	static int64_t captureHeight;
	static int64_t fps;
    };
}
