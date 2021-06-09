import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import KinectImageStream 1.0

ApplicationWindow
{
    objectName: "kinectViewerWindow";
    visible: true;
    width: 2 * 640;
    height: 2 * 2 * 480;
    title: qsTr("KinectViewer");

    Rectangle
    {
        anchors.fill: parent;
        color: "black";
    }

    Column
    {
        anchors.fill: parent;
        KinectImageStream
        {
            width: parent.width;
            height: parent.height / 2;
            source: "depth";
        }
        KinectImageStream
        {
            width: parent.width;
            height: parent.height / 2;
            source: "video";
        }
    }

    onClosing:
    {
        Qt.quit();
    }
}
