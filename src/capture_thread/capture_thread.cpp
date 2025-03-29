#include <iostream>

#include "capture_thread.h"

using namespace capture_thread;

CaptureThread::CaptureThread()
{
}

CaptureThread::CaptureThread(int dev)
{
    if (isOpen())
    {
        return;
    }
    capture.open(dev);
}

CaptureThread::CaptureThread(const std::string &path)
{
    if (isOpen())
    {
        return;
    }
    capture.open(path.c_str());
}

void CaptureThread::open(int dev, int width, int height, int fps)
{
    if (isOpen())
    {
        return;
    }
    capture.open(dev);
    if (width > 0 && height > 0)
    {
        capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
    if (fps > 0)
    {
        capture.set(cv::CAP_PROP_FRAME_FPS, fps);
    }
}

void CaptureThread::open(const std::string &path, int width, int height, int fps)
{
    if (isOpen())
    {
        return;
    }
    capture.open(path.c_str());
    if (width > 0 && height > 0)
    {
        capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
        capture.set(cv::CAP_PROP_FRAM_HEIGHT, height);
    }
    if (fps > 0)
    {
        capture.set(cv::CAP_PROP_FRAME_FPS, fps);
    }
}

CaptureThread::~CaptureThread()
{
    capture.release();
    cancel();
}

bool CaptureThread::isOpen()
{
    return capture.isOpened();
}

void *CaptureThread::func(void *ptr)
{
    CaptureThread *capture = (CaptureThread *)ptr;
    capture->run();
    pthread_exit(nullptr);
}

bool CaptureThread::start()
{
    if (!isOpen())
    {
        return false;
    }
    int ret = pthread_create(&thread_id, nullptr, func, (void *)this);
    if (ret != 0)
    {
        return false;
    }
    return true;
}

int CaptureThread::getWidth()
{
    if (!isOpen())
    {
        return -1;
    }
    return capture.get(cv::CAP_PROP_FRAME_WIDTH);
}

int CaptureThread::getHeight()
{
    if (!isOpen())
    {
        return -1;
    }
    return capture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

int CaptureThread::getFPS()
{
    if (!isOpen())
    {
        return -1;
    }
    return capture.get(cv::CAP_PROP_FPS);
}

void CaptureThread::run()
{
    if (isOpen() == false)
    {
        pthread_exit(nullptr);
    }

    cv::Mat m_mat;
    while (true)
    {
        capture >> m_mat;
        if (m_mat.empty())
        {
            pthread_exit(nullptr);
        }
        // 只存储最新的5帧
        pthread_mutex_lock(&buffer_mutex);
        if (buffer.size() > 5)
        {
            buffer.erase(buffer.begin());
        }
        cv::Mat copy(m_mat);
        buffer.push_back(copy);
        pthread_mutex_unlock(&buffer_mutex);
        pthread_cond_broadcast(&buffer_cond);
        cv::waitKey(1);
    }
}

cv::Mat CaptureThread::get_mat()
{
    pthread_mutex_lock(&buffer_mutex);
    while (buffer.size() == 0)
    {
        pthread_cond_wait(&buffer_cond, &buffer_mutex);
    }
    cv::Mat mat = *buffer.begin();
    buffer.erase(buffer.begin());
    pthread_mutex_unlock(&buffer_mutex);
    return mat;
}

void CaptureThread::cancel()
{
    pthread_cancel(thread_id);
}
