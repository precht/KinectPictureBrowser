#include "kinectdevice.h"

KinectDevice::KinectDevice(freenect_context* p_ctx, int p_index)
    : QObject(nullptr)
    , freenect::FreenectDevice(p_ctx, p_index)
{
    setTiltDegrees(tiltAngle);
    connect(&m_timer, &QTimer::timeout, [&]()
    {
        if (m_newDepthFrame)
            emit depthFrameReady();
        if (m_newVideoFrame)
            emit videoFrameReady();
    });
    startDepth();
    startVideo();
    m_timer.start(timeMs);
}

KinectDevice::~KinectDevice()
{
    stopDepth();
    stopVideo();
}

QImage KinectDevice::getDepth()
{
    PthreadMutex::ScopedLock lock(m_depthMutex);
    if (not m_newDepthFrame)
        return {};
    return m_depthImage.copy();
}

QImage KinectDevice::getVideo()
{
    PthreadMutex::ScopedLock lock(m_videoMutex);
    if (not m_newVideoFrame)
        return {};
    return m_videoImage.copy();
}

void KinectDevice::DepthCallback(void* p_depth, uint32_t)
{
    PthreadMutex::ScopedLock lock(m_depthMutex);
    uint16_t* depth = static_cast<uint16_t*>(p_depth);
    QRgb* image = reinterpret_cast<QRgb*>(m_depthImage.bits());

    constexpr QRgb white = 0xFFFFFFFF;
    constexpr QRgb black = 0xFF000000;

    for (uint32_t row = 0 ; row < height ; row++)
        for (uint32_t col = 0; col < width; col++)
        {
            const uint32_t depthIdx = row * width + col;
            const uint32_t imageIdx = (row + 1) * width - col - 1;
            image[imageIdx] = depth[depthIdx] < activeDepth ? white : black;
        }

    m_newDepthFrame = true;
}

void KinectDevice::VideoCallback(void* p_video, uint32_t)
{
    PthreadMutex::ScopedLock lock(m_videoMutex);
    uint8_t* video = static_cast<uint8_t*>(p_video);
    QRgb* image = reinterpret_cast<QRgb*>(m_videoImage.bits());

    for (uint32_t row = 0 ; row < height ; row++)
        for (uint32_t col = 0; col < width; col++)
        {
            const uint32_t videoIdx = 3 * (row * width + col);
            const uint32_t imageIdx = (row + 1) * width - col - 1;
            image[imageIdx] = QColor{video[videoIdx], video[videoIdx+1], video[videoIdx+2]}.rgb();
        }

    m_newVideoFrame = true;
}
