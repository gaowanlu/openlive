#pragma once

#include <unistd.h>
#include <stdint.h>
#include <x264.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace Encoder
{
    struct x264_encoder
    {
        x264_param_t param;
        char preset[20];
        char tune[20];
        char profile[20];
        x264_t *h;
        x264_picture_t pic_in;
        x264_picture_t pic_out;
        long colorspace;
        x264_nal_t *nal;
        int iframe;
        int iframe_size;
        int inal;
    };

    class x264Encoder
    {
    public:
        x264Encoder();
        x264Encoder(int videoWidth, int videoHeight, int channel, int fps);
        virtual ~x264Encoder();
        /**
         * @brief 创建编码器
         *
         * @param videoWitdth
         * @param videoHeight
         * @param channel
         * @param fps
         * @return true
         * @return false
         */
        bool create(int videoWitdth, int videoHeight, int channel = 3, int fps = 30);
        /**
         * @brief 编码帧
         *
         * @param frame 输入图像
         * @return int 返回编码后数据尺寸，0表示编码失败
         */
        int encodeFrame(const cv::Mat &frame);
        /**
         * @brief 获取编码后的帧数据
         *
         * @return uchar* 裸x264数据
         */
        uchar *getEncodedFrame() const;
        /**
         * @brief 释放编码器
         *
         */
        void destory();
        /**
         * @brief 编码器是否可用
         *
         * @return true
         * @return false
         */
        bool isValid() const;

    private:
        void init();

    public:
        int m_width;
        int m_height;
        int m_channel;
        int m_fps;

    protected:
        int m_width_step;
        int m_luma_size;
        int m_chroma_size;
        x264_encoder *m_encoder;
    };
};