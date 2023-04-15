#pragma once
#include <list>
#include <pthread.h>
#include <string>
#include "x264Encoder/x264Encoder.h"
#include "capture_thread/capture_thread.h"

namespace encode_thread
{
    class EncodeThread
    {
    public:
        EncodeThread();
        virtual ~EncodeThread();
        void run();
        void start(capture_thread::CaptureThread *capture);
        std::string getFrame();
        static void *func(void *ptr);

    protected:
        std::list<std::string> mat_buf;
        pthread_mutex_t mat_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t mat_buf_cond = PTHREAD_COND_INITIALIZER;
        pthread_t thread_id;
        Encoder::x264Encoder encoder;
        capture_thread::CaptureThread *capture;
    };
};