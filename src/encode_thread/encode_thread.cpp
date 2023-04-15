#include "encode_thread.h"
#include "base64/base64.h"

using namespace encode_thread;

EncodeThread::EncodeThread()
{
}

void EncodeThread::start(capture_thread::CaptureThread *capture)
{
    this->capture = capture;
    bool encoder_ret = encoder.create(this->capture->getWidth(),
                                      this->capture->getHeight(),
                                      3,
                                      this->capture->getFPS());
    int ret = pthread_create(&thread_id, nullptr, func, (void *)this);
}

EncodeThread::~EncodeThread()
{
    pthread_cancel(thread_id);
}

void EncodeThread::run()
{
    cv::Mat mat;
    while (true)
    {
        mat = capture->get_mat();
        if (mat.empty())
        {
            pthread_exit(nullptr);
        }
        int ret_size = encoder.encodeFrame(mat);
        if (ret_size > 0)
        {
            uchar *buf = encoder.getEncodedFrame();
            // 转为base64
            std::string base64 = base64_encode(buf, ret_size);
            // 只存储最新5帧
            pthread_mutex_lock(&mat_buf_mutex);
            if (mat_buf.size() > 5)
            {
                mat_buf.erase(mat_buf.begin());
            }
            mat_buf.push_back(base64);
            pthread_mutex_unlock(&mat_buf_mutex);
            pthread_cond_broadcast(&mat_buf_cond);
            cv::waitKey(1);
        }
    }
}

std::string EncodeThread::getFrame()
{
    pthread_mutex_lock(&mat_buf_mutex);
    while (mat_buf.size() == 0)
    {
        pthread_cond_wait(&mat_buf_cond, &mat_buf_mutex);
    }
    std::string mat = *mat_buf.begin();
    mat_buf.erase(mat_buf.begin());
    pthread_mutex_unlock(&mat_buf_mutex);
    return mat;
}

void *EncodeThread::func(void *ptr)
{
    EncodeThread *m_encoder = (EncodeThread *)ptr;
    m_encoder->run();
    return nullptr;
}
