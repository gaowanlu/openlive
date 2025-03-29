#include "conf.h"

std::string Conf::Conf::path = "";
int64_t Conf::Conf::encodeBufferLen = 0;
int64_t Conf::Conf::captureBufferLen = 0;
int64_t Conf::Conf::captureWidth = 0;
int64_t Conf::Conf::captureHeight = 0;
int64_t Conf::Conf::fps = 0;

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

void Conf::Conf::setCaptureWidth(int64_t width)
{
    Conf::Conf::captureWidth = width;
}

int64_t Conf::Conf::getCaptureWidth()
{
    return Conf::Conf::captureWidth;
}

void Conf::Conf::setCaptureHeight(int64_t height)
{
    Conf::Conf::captureHeight = height;
}

int64_t Conf::Conf::getCaptureHeight()
{
    return Conf::Conf::captureHeight;
}

void Conf::Conf::setFPS(int64_t fps)
{
    Conf::Conf::fps = fps;
}

int64_t Conf::Conf::getFPS()
{
    return Conf::Conf::fps;
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
    else if (key == "captureWidth")
    {
        setCaptureWidth(value);
    }
    else if (key == "captureHeight")
    {
        setCaptureHeight(value);
    }
    else if (key == "captureFPS")
    {
        setCaptureFPS(value);
    }
}
