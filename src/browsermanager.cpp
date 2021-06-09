#include "browsermanager.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QKeySequence>
#include <QAction>
#include <QKeySequence>
#include <QDir>
#include <cmath>

BrowserManager::BrowserManager(QObject *parent)
    : QObject(parent)
    , m_directoryPath(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation))
{ }

////////////////////////////////////// MAIN LOGIC /////////////////////////////////////////////////////////////////////

void BrowserManager::nextPicture()
{
    if (m_pictureFilenames.size() == 0)
        return;

    m_pictureIdx = (m_pictureIdx + 1) % m_pictureFilenames.size();
    setPicturePath(m_pictureFilenames.at(m_pictureIdx));
    if (not qFuzzyCompare(m_pictureScale, 1.0))
        setPictureScale(1.0);
}

void BrowserManager::prevPicture()
{
    if (m_pictureFilenames.size() == 0)
        return;

    m_pictureIdx--;
    if (m_pictureIdx < 0)
        m_pictureIdx += m_pictureFilenames.size();
    setPicturePath(m_pictureFilenames.at(m_pictureIdx));
    if (not qFuzzyCompare(m_pictureScale, 1.0))
        setPictureScale(1.0);
}

void BrowserManager::zoomIn()
{
    static constexpr double step = 1.41421356237;
    const double val = m_pictureScale * step;
    if (val >= 4.0)
        return;
    setPictureScale(val);
}

void BrowserManager::zoomOut()
{
    static constexpr double step = 1.41421356237;
    const double val = m_pictureScale / step;
    if (val <= 0.25)
        return;
    setPictureScale(val);
}

void BrowserManager::openDefaultFileBrowser()
{
    const auto& str = QFileDialog::getExistingDirectory(nullptr, "Open Directory", directoryPath(),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    setDirectoryPath(str);
}

void BrowserManager::switchToPicturesPage()
{
    setMenuPageVisible(false);
    setPicturesPageVisible(true);
}

void BrowserManager::switchToMenuPage()
{
    setPicturesPageVisible(false);
    setMenuPageVisible(true);
}

void BrowserManager::searchForPicturesInDirectory()
{
    QDir dir {directoryPath()};
    m_pictureFilenames = dir.entryList(QStringList() << "*.jpg" << "*JPG" << "*.png" << "*.PNG" << "*.jpeg" << "*.JPEG",
                                       QDir::Files);
    m_pictureIdx = 0;
    setPicturePath(m_pictureFilenames.at(m_pictureIdx));
}

////////////////////////////////////// PROPERTIES /////////////////////////////////////////////////////////////////////

bool BrowserManager::menuPageVisible() const
{
    return m_menuPageVisible;
}

void BrowserManager::setMenuPageVisible(bool menuPageVisible)
{
    if (m_menuPageVisible == menuPageVisible)
        return;

    m_menuPageVisible = menuPageVisible;
    emit menuPageVisibleChanged(m_menuPageVisible);
}

bool BrowserManager::picturesPageVisible() const
{
    return m_picturesPageVisible;
}

void BrowserManager::setPicturesPageVisible(bool picturesPageVisible)
{
    if (m_picturesPageVisible == picturesPageVisible)
        return;

    m_picturesPageVisible = picturesPageVisible;
    emit picturesPageVisibleChanged(m_picturesPageVisible);
}

QString BrowserManager::picturePath() const
{
    return m_picturePath;
}

void BrowserManager::setPicturePath(const QString& picturePath)
{
    QString path = "file:" + directoryPath() + "/" + picturePath;
    if (m_picturePath == path)
        return;

    m_picturePath = std::move(path);
    emit picturePathChanged(m_picturePath);
}

double BrowserManager::pictureScale() const
{
    return m_pictureScale;
}

void BrowserManager::setPictureScale(double pictureScale)
{
    if (qFuzzyCompare(m_pictureScale, pictureScale))
        return;

    m_pictureScale = pictureScale;
    emit pictureScaleChanged(m_pictureScale);
}

QString BrowserManager::directoryPath() const
{
    return m_directoryPath;
}

void BrowserManager::setDirectoryPath(const QString& directoryPath)
{
    if (m_directoryPath == directoryPath)
        return;

    m_directoryPath = directoryPath;
    searchForPicturesInDirectory();
    emit directoryPathChanged(m_directoryPath);
}
