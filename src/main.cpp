#include <node.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <list>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include "base64/base64.h"
#include "x264Encoder/x264Encoder.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

namespace Demo
{
    std::list<pthread_t> thread_list;
    pthread_t thread;
    std::list<std::string> mat_buf;
    pthread_mutex_t mat_buf_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t mat_buf_cond = PTHREAD_COND_INITIALIZER;
    Encoder::x264Encoder encoder;

    /**
     * @brief 线程函数
     *
     * @return void*
     */
    void *thread_func(void *)
    {
        // cv::namedWindow("window");
        // 打开相机
        cv::Mat m_mat;
        cv::VideoCapture capture(0);
        if (!capture.isOpened())
        {
            pthread_exit(nullptr);
        }
        bool encoder_ret = encoder.create(capture.get(cv::CAP_PROP_FRAME_WIDTH),
                                          capture.get(cv::CAP_PROP_FRAME_HEIGHT),
                                          3,
                                          capture.get(cv::CAP_PROP_FPS));
        if (!encoder_ret)
        {
            return nullptr;
        }
        while (1)
        {
            capture >> m_mat;
            if (m_mat.empty())
            {
                pthread_exit(nullptr);
            }
            // 加入到编码器
            int ret_size = encoder.encodeFrame(m_mat);
            if (ret_size > 0)
            {
                uchar *buf = encoder.getEncodedFrame();
                // 转为base64
                std::string base64 = base64_encode(buf, ret_size);
                // 只存储最新10帧
                pthread_mutex_lock(&mat_buf_mutex);
                if (mat_buf.size() > 10)
                {
                    mat_buf.erase(mat_buf.begin());
                }
                mat_buf.push_back(base64);
                pthread_mutex_unlock(&mat_buf_mutex);
                pthread_cond_broadcast(&mat_buf_cond);
                cv::waitKey(1);
            }
            // 压缩图像
            // std::vector<uchar> buf;
            // cv::imencode(".jpg", m_mat, buf);
            // // 转换为base64
            // std::string base64 = base64_encode(buf.data(), buf.size());
            // // 只存储最新10帧
            // pthread_mutex_lock(&mat_buf_mutex);
            // if (mat_buf.size() > 10)
            // {
            //     mat_buf.erase(mat_buf.begin());
            // }
            // mat_buf.push_back(base64);
            // pthread_mutex_unlock(&mat_buf_mutex);
            // pthread_cond_broadcast(&mat_buf_cond);
            // // cv::imshow("window", m_mat);
            // cv::waitKey(1);
        }
        return nullptr;
    }

    /**
     * @brief 获取帧数据
     *
     * @param args
     */
    void get_mat(const FunctionCallbackInfo<Value> &args)
    {
        pthread_mutex_lock(&mat_buf_mutex);
        while (mat_buf.size() == 0)
        {
            pthread_cond_wait(&mat_buf_cond, &mat_buf_mutex);
        }
        std::string mat = *mat_buf.begin();
        mat_buf.erase(mat_buf.begin());
        pthread_mutex_unlock(&mat_buf_mutex);
        args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), mat.c_str()).ToLocalChecked());
    }

    /**
     * @brief 线程启动
     *
     * @param args
     */
    void thread_start(const FunctionCallbackInfo<Value> &args)
    {
        int ret = pthread_create(&thread, nullptr, thread_func, nullptr);
        if (ret == 0)
        {
            thread_list.push_back(thread);
        }
        std::string str;
        str = ((ret == 0) ? "true" : "false");
        args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), str.c_str()).ToLocalChecked());
    }
};

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "thread_start", Demo::thread_start);
    NODE_SET_METHOD(exports, "get_mat", Demo::get_mat);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);

// sudo apt install v4l-utils
// sudo apt install libopencv-dev
// sudo apt install pkg-config
// npm run build && node index

/*
"<!(pkg-config x264 --cflags)"
*/