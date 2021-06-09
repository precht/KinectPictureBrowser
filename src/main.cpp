#include "kinectimagestream.h"
#include "kinectmanager.h"
#include "browsermanager.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>

double calculateScaleRatio()
{
    static constexpr double magicCoef = 0.7;
    const auto screen = QGuiApplication::primaryScreen()->geometry();
    double heightRatio = 0;
    double widthRatio = 0;
    if (screen.height() < screen.width())
    {
        heightRatio = magicCoef * screen.height() / 2160;
        widthRatio = magicCoef * screen.width() / 3840;
    }
    else
    {
        heightRatio = magicCoef * screen.height() / 3840;
        widthRatio = magicCoef * screen.width() / 2160;
    }
    return qMin(heightRatio, widthRatio);
}

void loadQmlFile(QApplication& app, QQmlApplicationEngine& engine, const QUrl& url)
{
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl)
                     {
                        if (!obj && url == objUrl)
                            QCoreApplication::exit(-1);
                     },
                     Qt::QueuedConnection);
    engine.load(url);
}

void createConnections(KinectManager* kinect, BrowserManager* browser)
{
    auto detector = kinect->gestureDetector();
    QObject::connect(detector.get(), &GestureDetector::left,
                     browser, &BrowserManager::prevPicture);
    QObject::connect(detector.get(), &GestureDetector::right,
                     browser, &BrowserManager::nextPicture);
    QObject::connect(detector.get(), &GestureDetector::down,
                     browser, &BrowserManager::zoomIn);
    QObject::connect(detector.get(), &GestureDetector::up,
                     browser, &BrowserManager::zoomOut);
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qmlRegisterType<KinectImageStream>("KinectImageStream", 1, 0, "KinectImageStream");
    QQmlApplicationEngine engine;

    const QVariant scaleRatio {calculateScaleRatio()};
    auto kinectManager = KinectManager::getInstance();
    BrowserManager browserManager { &app };
    createConnections(kinectManager.get(), &browserManager);

    engine.rootContext()->setContextProperty("scaleRatio", scaleRatio);
    engine.rootContext()->setContextProperty("kinectManager", kinectManager.get());
    engine.rootContext()->setContextProperty("browserManager", &browserManager);

    loadQmlFile(app, engine, QStringLiteral("qrc:/PictureBrowserWindow.qml"));
    loadQmlFile(app, engine, QStringLiteral("qrc:/KinectViewerWindow.qml"));

    return app.exec();
}
