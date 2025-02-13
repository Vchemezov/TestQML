// Test QML GUI

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.15
//import com.test 1.0

Window {
    id: openFile
    
    signal acceptedText(string name)

    title: qsTr("Open file")

    width: 369
    height: 166

    ColumnLayout {
        x: 10
        y: 10

        // Filename
        RowLayout {
            Label {
                text: qsTr("Filename:")
            }
            TextField {
                id: filename
                implicitWidth: 200
            }
            Button {
                text: qsTr("&Browse...")
                onClicked: filedialog.visible = true
            }
        }

        // Buttons: Proc, Quit
        RowLayout {
            Button {
                id: proc
                text: qsTr("&Ok")
                onClicked: {
                    openFile.acceptedText(filename.text)
                    openFile.close()
                }
            }
            Button {
                text: qsTr("&Cancel")
                onClicked: openFile.close()
            }
        }
    }

    FileDialog {
        id: filedialog
        title: qsTr("Select a file")
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "All files (*)", "Touchstone (*.s1p)" ]
        selectedNameFilter: "Touchstone (*.s1p)"
        onAccepted: {
            filename.text = fileUrl.toString().replace("file:///", "")
        }
    }
}