#pragma once

#include <list>
#include <string>
#include <opencv2/opencv.hpp>
#include <pthread.h>

namespace capture_thread
{
    class CaptureThread
    {
    public:
        CaptureThread();
        CaptureThread(int dev);
        CaptureThread(const std::string &path);
        void open(int dev);
        void open(const std::string &path);
        virtual ~CaptureThread();
        bool isOpen();
        bool start();
        static void *func(void *ptr);
        int getWidth();
        int getHeight();
        int getFPS();
        void run();
        cv::Mat get_mat();
        void cancel();

    protected:
        cv::VideoCapture capture;
        std::list<cv::Mat> buffer;
        pthread_t thread_id;
        pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
    };

};