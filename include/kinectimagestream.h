#pragma once

#include "kinectmanager.h"
#include <QQuickPaintedItem>
#include <QImage>

class KinectImageStream : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)

public:
    explicit KinectImageStream(QQuickItem *parent = nullptr);
    QString source() const;

public slots:
    void setSource(QString source);

protected:
    virtual void paint(QPainter *painter) override;

private:
    void initDepthImage();
    void initVideoImage();
    void onDepthFrameReady();
    void onVideoFrameReady();

    std::shared_ptr<KinectManager> m_manager{};
    QString m_source{};
    QImage m_image{};

    enum Type {NONE, DEPTH, VIDEO};
    Type m_type{NONE};
};
