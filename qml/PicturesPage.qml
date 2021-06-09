import QtQuick 2.13

Rectangle
{
    color: "transparent"

    Image
    {
        anchors.fill: parent
        asynchronous: true
        autoTransform: true
        transformOrigin: Item.Center
        fillMode: Image.PreserveAspectFit
        source: browserManager.picturePath
        scale: browserManager.pictureScale
    }
}
