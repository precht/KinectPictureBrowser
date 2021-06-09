import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

ApplicationWindow
{
    visible: true
    title: "PicturesBrowser"
    width: 0.5 * Screen.width
    height: 0.5 * Screen.height

    MenuPage
    {
        anchors.fill: parent
        visible: browserManager.menuPageVisible
    }

    PicturesPage
    {
        anchors.fill: parent
        visible: browserManager.picturesPageVisible
    }

    Shortcuts { }

    onClosing:
    {
        Qt.quit();
    }
}
