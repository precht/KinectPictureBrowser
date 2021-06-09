#include "kinectimagestream.h"
#include <QPainter>

KinectImageStream::KinectImageStream(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_manager(KinectManager::getInstance())
{
    connect(m_manager.get(), &KinectManager::depthFrameReady,
            this, &KinectImageStream::onDepthFrameReady);
    connect(m_manager.get(), &KinectManager::videoFrameReady,
            this, &KinectImageStream::onVideoFrameReady);
}

void KinectImageStream::paint(QPainter *painter)
{
    const int w = static_cast<int>(width());
    const int h = static_cast<int>(height());

    QImage image = m_image.scaled(w, h, Qt::KeepAspectRatio);
    painter->drawImage(static_cast<int>(w - image.width()) >> 1,
                       static_cast<int>(height() - image.height()) >> 1,
                       image);
}

QString KinectImageStream::source() const
{
    return m_source;
}

void KinectImageStream::setSource(QString source)
{
    if (m_source == source)
        return;
    m_source = source;

    if (m_source == "depth")
        initDepthImage();
    else if (m_source == "video")
        initVideoImage();
    else
       assert (false);
    update();
}

void KinectImageStream::initDepthImage()
{
    m_type = DEPTH;
    m_image = {};
}

void KinectImageStream::initVideoImage()
{
    m_type = VIDEO;
    m_image = {};
}

void KinectImageStream::onDepthFrameReady()
{
    if (m_type != DEPTH)
        return;

    m_image = m_manager->getDepth();
    update();
}

void KinectImageStream::onVideoFrameReady()
{
    if (m_type != VIDEO)
        return;
    m_image = m_manager->getVideo();
    update();
}

