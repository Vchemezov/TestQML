// Test QML GUI

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import custom 1.0

ApplicationWindow {
    id: mainWindow
    title: qsTr("Test QML")

    color: "gray"
    width: 720
    height: 480

    signal sendFilename(name: string)

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem { text: qsTr("&Open...")
                        shortcut: "Ctrl+O"
                        onTriggered: openDialog.show()
                     }
            MenuSeparator { }
            MenuItem {  text: qsTr("&Quit")
                        shortcut: "Esc"
                        onTriggered: Qt.quit()}
        }
    }
    ColumnLayout {
        Rectangle {
            id: file
            width: mainWindow.width
            height: 25
            color: "steelblue"
            border.color: "black"
            border.width: 2

            RowLayout {
                Label {
                    x: 5
                    y: 4
                    font.pointSize: 12
                    text: qsTr("Filename:")
                    color: "white"
                }

                Label {
                    id: mainFilename
                    y: 4
                    font.pointSize: 12
                    color: "white"
                    onTextChanged: mainWindow.sendFilename(mainFilename.text)
                }
            }
        }

        DataVisualyzer {
            window: mainWindow
            id: mainViewRect
            visible: true
            errorLog: eLog
            anchors.top: file.bottom
            height: (mainWindow.height - file.height)
            width: mainWindow.width
        }

        ErrorProvider{
            id: eLog
            onErrorAdded: {
                for (var i = 0; i < count; i++)  {
                    alertMessageBox.text += errors[i] + "\n"
                }

                alertMessageBox.visible = true
            }
        }
    }

        OpenFileDialog {
            id: openDialog
            onAcceptedText: (name) => {
                mainFilename.text = name
            }
        }

    MessageDialog {
        id: alertMessageBox
        title: "Alert!"
        text: ""
        visible: false
        onAccepted: {
            visible: false
        }
    }
}
