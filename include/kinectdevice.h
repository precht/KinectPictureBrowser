#pragma once

#include "freenect.h"
#include "pthreadmutex.h"

#include <vector>
#include <atomic>
#include <QObject>
#include <QTimer>
#include <QImage>
#include <QThread>
#include <memory>

class KinectDevice
        : public QObject
        , public freenect::FreenectDevice
{
    Q_OBJECT
public:
    static constexpr int timeMs{30};
    static constexpr uint16_t activeDepth{700};
    static constexpr int width{640};
    static constexpr int height{480};
    static constexpr double tiltAngle{25};
    static std::shared_ptr<KinectDevice> getInstance();

    KinectDevice(freenect_context* p_ctx, int p_index);
    ~KinectDevice();
    QImage getDepth();
    QImage getVideo();

    // Do not call directly even in child
    void DepthCallback(void* p_depth, uint32_t) override;
    // Do not call directly even in child
    void VideoCallback(void* p_video, uint32_t) override;

signals:
    void depthFrameReady();
    void videoFrameReady();

private:
    QImage m_depthImage{width, height, QImage::Format_RGB32};
    QImage m_videoImage{width, height, QImage::Format_RGB32};
    PthreadMutex m_videoMutex{};
    PthreadMutex m_depthMutex{};
    std::atomic<bool> m_newVideoFrame{false};
    std::atomic<bool> m_newDepthFrame{false};
    QTimer m_timer;
    QThread m_thread;
};
