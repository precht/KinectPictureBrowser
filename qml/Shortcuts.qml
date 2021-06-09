import QtQuick 2.13

Item
{
    Shortcut
    {
        sequence: "Left"
        onActivated: browserManager.prevPicture()
    }
    Shortcut
    {
        sequence: "Right"
        onActivated: browserManager.nextPicture()
    }
    Shortcut
    {
        sequence: "Up"
        onActivated: browserManager.zoomIn()
    }
    Shortcut
    {
        sequence: "Down"
        onActivated: browserManager.zoomOut()
    }
    Shortcut
    {
        sequence: "ESC"
        onActivated: browserManager.switchToMenuPage()
    }
}
