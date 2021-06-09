#pragma once

#include <QObject>
#include <QVector>
#include <QStringList>

class QAction;

class BrowserManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool menuPageVisible
               READ menuPageVisible
               WRITE setMenuPageVisible
               NOTIFY menuPageVisibleChanged)

    Q_PROPERTY(bool picturesPageVisible
               READ picturesPageVisible
               WRITE setPicturesPageVisible
               NOTIFY picturesPageVisibleChanged)

    Q_PROPERTY(QString directoryPath
               READ directoryPath
               WRITE setDirectoryPath
               NOTIFY directoryPathChanged)

    Q_PROPERTY(QString picturePath
               READ picturePath
               WRITE setPicturePath
               NOTIFY picturePathChanged)

    Q_PROPERTY(double pictureScale
               READ pictureScale
               WRITE setPictureScale
               NOTIFY pictureScaleChanged)

public:
    explicit BrowserManager(QObject *parent = nullptr);
    BrowserManager(const BrowserManager&) = delete;
    BrowserManager& operator=(const BrowserManager&) = delete;
    BrowserManager(BrowserManager&&) = delete;
    BrowserManager& operator=(BrowserManager&&) = delete;

    bool menuPageVisible() const;
    bool picturesPageVisible() const;
    QString directoryPath() const;
    QString picturePath() const;

    double pictureScale() const;

signals:
    void menuPageVisibleChanged(bool menuPageVisible);
    void picturesPageVisibleChanged(bool picturesPageVisible);
    void picturePathChanged(QString picturePath);
    void directoryPathChanged(QString directoryPath);

    void pictureScaleChanged(double pictureScale);

public slots:
    void setMenuPageVisible(bool menuPageVisible);
    void setPicturesPageVisible(bool picturesPageVisible);
    void setPicturePath(const QString& picturePath);
    void setDirectoryPath(const QString& directoryPath);

    void openDefaultFileBrowser();
    void switchToPicturesPage();
    void switchToMenuPage();

    void nextPicture();
    void prevPicture();
    void zoomIn();
    void zoomOut();

    void setPictureScale(double pictureScale);

private:
    void searchForPicturesInDirectory();

    QStringList m_pictureFilenames{};
    QString m_picturePath{};
    QString m_directoryPath{};
    int m_pictureIdx{};
    bool m_menuPageVisible{true};
    bool m_picturesPageVisible{false};
    double m_pictureScale{1.0};
};

