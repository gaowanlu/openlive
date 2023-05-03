#include "conf.h"

std::string Conf::Conf::path = "";
int64_t Conf::Conf::encodeBufferLen = 0;
int64_t Conf::Conf::captureBufferLen = 0;

void Conf::Conf::setPath(const std::string &path)
{
    Conf::Conf::path = path;
}

std::string Conf::Conf::getPath()
{
    return Conf::Conf::path;
}

void Conf::Conf::setEncodeBufferLen(int64_t len)
{
    Conf::Conf::encodeBufferLen = len;
}

int64_t Conf::Conf::getEncodeBufferLen()
{
    if (Conf::Conf::encodeBufferLen == 0)
        return Conf::Conf::encodeBufferLen;
    return 5;
}

void Conf::Conf::setCaptureBufferLen(int64_t len)
{
    Conf::Conf::captureBufferLen = len;
}

int64_t Conf::Conf::getCaptureBufferLen(int64_t len)
{
    if (Conf::Conf::captureBufferLen == 0)
        return Conf::Conf::captureBufferLen;
    return 5;
}

void Conf::Conf::setConf(std::string key, std::string value)
{
    if (key == "path")
    {
        setPath(value);
    }
}

void Conf::Conf::setConf(std::string key, int64_t value)
{
    if (key == "encodeBufferLen")
    {
        setEncodeBufferLen(value);
    }
    else if (key == "captureBufferLen")
    {
        setCaptureBufferLen(value);
    }
}
