import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Rectangle
{
    color: "transparent"

    ColumnLayout
    {
        width: 1000 * scaleRatio;
        spacing: 50 * scaleRatio
        anchors.centerIn: parent

        Label
        {
            text: "Enter directory with pictures:"
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 50 * scaleRatio
            font.bold: true
        }

        TextField
        {
            Layout.fillWidth: true
            font.pixelSize: 50 * scaleRatio
            text: browserManager.directoryPath
        }

        RowLayout
        {
            Layout.alignment: Qt.AlignHCenter
            spacing: 100 * scaleRatio

            Button
            {
                text: "Browse"
                width: 300 *  scaleRatio
                Layout.preferredWidth: width
                font.pixelSize: 50 * scaleRatio
                font.bold: true
                hoverEnabled: true
                onReleased: browserManager.openDefaultFileBrowser()
            }
            Button
            {
                text: "Start"
                width: 300 * scaleRatio
                Layout.preferredWidth: width
                font.pixelSize: 50 * scaleRatio
                font.bold: true
                hoverEnabled: true
                onReleased: browserManager.switchToPicturesPage()
            }
        }
    }
}
