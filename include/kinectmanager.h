#pragma once

#include "kinectdevice.h"
#include "gesturedetector.h"

#include <QObject>
#include <QImage>
#include <memory>

class KinectManager : public QObject
{
    Q_OBJECT
public:
    static std::shared_ptr<KinectManager> getInstance();

    explicit KinectManager(QObject *parent = nullptr);
    QImage getDepth();
    QImage getVideo();
    std::shared_ptr<GestureDetector> gestureDetector();

signals:
    void depthFrameReady();
    void videoFrameReady();

private:
    void onDepthFrameReady();
    void onVideoFrameReady();
    void detectGestures();

    static std::shared_ptr<KinectManager> m_manager;
    QImage m_depthImage{};
    QImage m_videoImage{};
    freenect::Freenect m_freenect{};
    std::unique_ptr<KinectDevice> m_device{};
    std::shared_ptr<GestureDetector> m_detector{};
};
