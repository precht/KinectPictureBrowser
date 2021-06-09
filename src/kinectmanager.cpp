#include "kinectmanager.h"

std::shared_ptr<KinectManager> KinectManager::m_manager{};

std::shared_ptr<KinectManager> KinectManager::getInstance()
{
    if (not m_manager)
        m_manager = std::make_unique<KinectManager>();
    return m_manager;
}

KinectManager::KinectManager(QObject *parent)
    : QObject(parent)
    , m_device(decltype(m_device){&(m_freenect.createDevice<KinectDevice>(0))})
    , m_detector(std::make_shared<GestureDetector>())
{
    connect(m_device.get(), &KinectDevice::depthFrameReady,
            this, &KinectManager::onDepthFrameReady);
    connect(m_device.get(), &KinectDevice::videoFrameReady,
            this, &KinectManager::onVideoFrameReady);
}

void KinectManager::onDepthFrameReady()
{
    m_depthImage = m_device->getDepth();
    m_detector->processFrame(m_depthImage);
    emit depthFrameReady();
}

void KinectManager::onVideoFrameReady()
{
    m_videoImage = m_device->getVideo();
    emit videoFrameReady();
}

QImage KinectManager::getDepth()
{
    return m_depthImage;
}

QImage KinectManager::getVideo()
{
    return m_videoImage;
}

std::shared_ptr<GestureDetector> KinectManager::gestureDetector()
{
    return m_detector;
}
