import QtQuick 2.15
import QtQuick.Window 2.15
import QCustomPlot.MultipolePlot 1.0

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Basic potential")

    MultipolePlot {
        id: customPlot
        anchors.fill: parent

        Component.onCompleted: {
            customPlot.registerMultipoleField(multipoleField)
        }
    }
}
