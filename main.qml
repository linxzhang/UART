import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 640
    title: "Uart Debug Application"
    readonly property int fixedHeight: 25
    contentItem.anchors.margins: 10

    property bool linecheckstatus: true

    header: ToolBar {
            height: 35
            spacing: 5
            GridLayout {
                columns: 3
                ToolButton {
                    id: id_uartopen
                    Layout.column: 0
                    Layout.preferredWidth: 32
                    width: 32
                    height: 32
                    onClicked: {
                        SerialUI.toolbaropen()
                        id_uartopen.enabled = false;
                        id_uartstop.enabled = true;
                    }
                    background: Image {
                        source: "start.png"
                        width: 32
                        height: 32
                        //fillMode: Image.PreserveAspectCrop
                        fillMode: Image.PreserveAspectFit
                    }
                } // end ToolButton

                ToolButton {
                    id: id_uartstop
                    Layout.column: 1
                    Layout.preferredWidth: 32
                    width: 32
                    height: 32
                    enabled: false
                    onClicked: {
                        SerialUI.toolbarstop()
                        id_uartstop.enabled = false;
                        id_uartopen.enabled = true;
                    }
                    background: Image {
                        source: "stop.png"
                        width: 32
                        height: 32
                        //fillMode: Image.PreserveAspectCrop
                        fillMode: Image.PreserveAspectFit
                    }
                } // end ToolButton

                ToolButton {
                    Layout.column: 2
                    Layout.preferredWidth: 32
                    width: 32
                    height: 32
                    onClicked: SerialUI.toolbarabout()
                    background: Image {
                        source: "about.png"
                        width: 32
                        height: 32
                        //fillMode: Image.PreserveAspectCrop
                        fillMode: Image.PreserveAspectFit
                    }
                } // end ToolButton
            } // end RowLayout
        }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        RowLayout{
            Layout.preferredHeight: 5
        }

        // 中间区域，左右分割
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // 左侧区域
            ColumnLayout {
                Layout.preferredWidth: 260
                Layout.alignment: Qt.AlignTop
                GridLayout {
                    rows: 5
                    GroupBox {
                        Layout.row: 0
                        title: "Uart configuration"
                        Layout.preferredWidth: 240
                        GridLayout {
                            columns: 2
                            rows: 5
                            Label {
                                Layout.column: 0
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("uart port")
                            }
                            ComboBox {
                                Layout.column: 1
                                Layout.preferredHeight: fixedHeight
                                editable: true
                                id: id_serialport
                                model: comboModel.comboList
                                onActivated: {
                                    console.log("combomodel activated " + id_serialport.currentIndex)
                                    comboModel.currentIndex = id_serialport.currentIndex
                                 }

                                objectName: "serialport"
                                onAccepted: {
                                        if (find(editText) === -1)
                                            comboModel.append(editText)
                                        console.log("port text vluae " + editText)
                                        id_serialport.model = null // Reset the model
                                        id_serialport.model = comboModel.comboList // Reassign the model to refresh the view
                                        id_serialport.currentIndex = comboModel.comboList.length - 1
                                        comboModel.currentIndex = id_serialport.currentIndex
                                        console.log("serialport current index = " + id_serialport.currentIndex)
                                    }
                                // If editable, this property holds the text in the edit field
                                editText: "Editable Text"
                            }

                            Label {
                                Layout.column: 0
                                Layout.row: 1
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("baudrate")
                            }
                            ComboBox {
                                id: id_serialbaudrate
                                editable: true
                                Layout.column: 1
                                Layout.row: 1
                                Layout.preferredHeight: fixedHeight
                                model: baudratecombo.baudratecomboList
                                onActivated: {
                                    console.log("baudrate activated " + id_serialbaudrate.currentIndex)
                                    baudratecombo.currentIndex = id_serialbaudrate.currentIndex
                                 }

                                onAccepted: {
                                        if (find(editText) === -1)
                                            baudratecombo.append(editText)
                                        console.log("baudrate text vluae " + editText)
                                        id_serialbaudrate.model = null // Reset the model
                                        id_serialbaudrate.model = baudratecombo.baudratecomboList // Reassign the model to refresh the view
                                        id_serialbaudrate.currentIndex = baudratecombo.baudratecomboList.length - 1
                                        baudratecombo.currentIndex = id_serialbaudrate.currentIndex
                                        console.log("baudrate current index = " + id_serialbaudrate.currentIndex)
                                }
                                // If editable, this property holds the text in the edit field
                                editText: "Editable Text"
                                currentIndex: 2
                            }

                            Label {
                                Layout.column: 0
                                Layout.row: 2
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("data bit")
                            }
                            ComboBox {
                                id: id_combodatabit
                                Layout.column: 1
                                Layout.row: 2
                                Layout.preferredHeight: fixedHeight
                                model: [ "5", "6", "7", "8"]
                                currentIndex: 3
                                onCurrentIndexChanged: combomngr.setComboBoxIndex("id_combodatabit", currentIndex)
                            }

                            Label {
                                Layout.column: 0
                                Layout.row: 3
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("parity bit")
                            }
                            ComboBox {
                                id: id_paritybit
                                Layout.column: 1
                                Layout.row: 3
                                Layout.preferredHeight: fixedHeight
                                model: ["None", "Even", "Odd", "Mark", "Space"]
                                onCurrentIndexChanged: combomngr.setComboBoxIndex("id_paritybit", currentIndex)
                            }

                            Label {
                                Layout.column: 0
                                Layout.row: 4
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("stop bit")
                            }
                            ComboBox {
                                id: id_combostopbit
                                Layout.column: 1
                                Layout.row: 4
                                Layout.preferredHeight: fixedHeight
                                model: ["1", "2"]
                                currentIndex: 0
                                onCurrentIndexChanged: combomngr.setComboBoxIndex("id_combostopbit", currentIndex)
                            }

                            Label {
                                Layout.column: 0
                                Layout.row: 5
                                Layout.preferredHeight: fixedHeight
                                text: qsTr("Flow control")
                            }
                            ComboBox {
                                id: id_comboflowctl
                                Layout.column: 1
                                Layout.row: 5
                                Layout.preferredHeight: fixedHeight
                                model: ["None", "RTS/CTS", "XON/XOFF"]
                                currentIndex: 0
                                onCurrentIndexChanged: combomngr.setComboBoxIndex("id_comboflowctl", currentIndex)
                            }
                        }
                    } // end GroupBox

                    GroupBox {
                        Layout.row: 1
                        Layout.preferredWidth: 240
                        title: "receiving configuration"
                        GridLayout {
                            columns: 2
                            rows: 2

                            Row {
                                Layout.preferredHeight: fixedHeight
                                ButtonGroup {
                                id: rcvradioGroup
                                // ButtonGroup is exclusive by default
                                }
                                RadioButton {
                                    id: id_rcvascii
                                    text: "ASCII"
                                    checked: true // Set the default checked button
                                    implicitHeight: 25
                                    Layout.preferredHeight: fixedHeight
                                    onCheckedChanged: {
                                        if (checked) {
                                            rcvradioGroup.checkedButton = this
                                        }
                                    }
                                }
                                RadioButton {
                                    text: "Hex"
                                    implicitHeight: 25
                                    Layout.preferredHeight: fixedHeight
                                    onCheckedChanged: {
                                        if (checked) {
                                            rcvradioGroup.checkedButton = this
                                        }
                                    }
                                }
                            } // end Row

                            Column {
                                Layout.row: 1
                                CheckBox {
                                    id: autolinefeed
                                    text: "auto line feed"
                                    checked: linecheckstatus
                                    indicator: Rectangle {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 20 // Set the width of the rectangle
                                        height: 15 // Set the height of the rectangle to match the CheckBox height
                                        color: "transparent"
                                        border.color: "black"
                                        Rectangle {
                                            width: 10
                                            height: 8
                                            color: autolinefeed.checked ? "green" : "transparent"
                                            anchors.centerIn: parent
                                        }
                                    }

                                } // end CheckBox

                                CheckBox {
                                    id: showsendmsg
                                    text: "display transmission"
                                    checked: true
                                    indicator: Rectangle {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 20 // Set the width of the rectangle
                                        height: 15 // Set the height of the rectangle to match the CheckBox height
                                        color: "transparent"
                                        border.color: "black"
                                        Rectangle {
                                            width: 10
                                            height: 8
                                            color: showsendmsg.checked ? "green" : "transparent"
                                            anchors.centerIn: parent
                                        }
                                    }
                                } // end CheckBox

                                CheckBox {
                                    id: chkshowtime
                                    text: "display time"
                                    checked: true
                                    indicator: Rectangle {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 20 // Set the width of the rectangle
                                        height: 15 // Set the height of the rectangle to match the CheckBox height
                                        color: "transparent"
                                        border.color: "black"
                                        Rectangle {
                                            width: 10
                                            height: 8
                                            color: chkshowtime.checked ? "green" : "transparent"
                                            anchors.centerIn: parent
                                        }
                                    }
                                } // end CheckBox
                            }

                        } // end GridLayout
                    } // end GroupBox

                    GroupBox {
                        Layout.row: 2
                        title: "Sending configuration"
                        Layout.preferredWidth: 240
                        GridLayout {
                            rows: 2
                            Row {

                                ButtonGroup {
                                id: sendradioGroup
                                // ButtonGroup is exclusive by default
                                }
                                RadioButton {
                                    text: "ASCII"
                                    checked: true // Set the default checked button

                                    implicitHeight: 25
                                    onCheckedChanged: {
                                        if (checked) {
                                            sendradioGroup.checkedButton = this
                                        }
                                    }
                                }
                                RadioButton {
                                    text: "Hex"

                                    implicitHeight: 25
                                    onCheckedChanged: {
                                        if (checked) {
                                            sendradioGroup.checkedButton = this
                                        }
                                    }
                                }
                            } // end Row


                            GridLayout {
                                Layout.row: 1
                                columns: 2
                                CheckBox {
                                    id: chkautorepeat
                                    text: "auto repeat"
                                    checked: false
                                    indicator: Rectangle {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 20 // Set the width of the rectangle
                                        height: 15 // Set the height of the rectangle to match the CheckBox height
                                        color: "transparent"
                                        border.color: "black"
                                        Rectangle {
                                            width: 10
                                            height: 8
                                            color: chkautorepeat.checked ? "green" : "transparent"
                                            anchors.centerIn: parent
                                        }
                                    }
                                    onClicked: {
                                        SerialUI.autoRepeat(chkautorepeat.checked ? true : false);
                                    }
                                } // end CheckBox
                                RowLayout {
                                    Layout.columnSpan: 1
                                    Layout.preferredHeight: fixedHeight
                                    TextField {
                                        id: sendinputField
                                        Layout.preferredWidth: 60
                                        Layout.preferredHeight: 30
                                        validator: IntValidator {} // Only allows integer numbers
                                        //font.pixelSize: 20
                                        // Add other properties and signal handlers as needed
                                        text: "1000"
                                    }


                                    Label {
                                        text: "ms"
                                    }
                                }
                            }
                        } // end GridLayout
                    } // end GroupBox

                }  // end GridLayout

            } // ColumnLayout

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // Define the background with a border for the ScrollView

                    background: Rectangle {
                        color: "transparent" // Set the background color inside the ScrollView
                        border.color: "black" // Set the border color
                        border.width: 2 // Set the border width
                        radius: 5 // Optional: Set the corner radius for rounded borders
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    TextArea {
                        id: rcvmsgtxtField
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        selectByMouse: true
                        placeholderText: qsTr("Receiving messages...")
                        text: ""
                        Connections {
                            target: backend // 'backend' is the instance of your C++ class exposed to QML
                            onSendMessageToQml: {
                                rcvmsgtxtField.text += message
                            }
                        }

                        /**
                          * add the pop-up menu
                          */
                        Menu {
                            id: contextMenu
/*
                            MenuItem {
                                text: "Cut"
                                onTriggered: rcvmsgtxtField.cut()
                            }
*/
                            MenuItem {
                                text: "Copy All"
                                onTriggered: {
                                    rcvmsgtxtField.selectAll();
                                    rcvmsgtxtField.copy();
                                }
                            }
                            MenuItem {
                                text: "Clear"
                                onTriggered: {
                                    rcvmsgtxtField.clear()
                                    SerialUI.clearReceiveData()
                                }
                            }
                        } // end Menu

                        // Open the context menu on right-click
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onPressed: {
                                if (mouse.button === Qt.RightButton) {
                                    contextMenu.popup(mouse.x, mouse.y)
                                }
                            }

                        } // end MouseArea

                    } // end TextArea
                } // end ScrollView
                GroupBox {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    //title: "send messages"
                    TextArea {
                        id: sndmsgtxtField
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        selectByMouse: true
                        placeholderText: qsTr("Send messages...")
                        text: ""
                        Connections {
                            target: txt_sendmessage
                            onSendMessageToQml: {
                                sndmsgtxtField.text += message
                            }
                        }

                        Menu {
                            id: sndcontextMenu
/*
                            MenuItem {
                                text: "Cut"
                                onTriggered: rcvmsgtxtField.cut()
                            }
*/
                            MenuItem {
                                text: "Copy All"
                                onTriggered: {
                                    sndmsgtxtField.selectAll();
                                    sndmsgtxtField.copy();
                                }
                            }
                            MenuItem {
                                text: "Clear"
                                onTriggered: {
                                    sndmsgtxtField.clear()
                                    SerialUI.clearSendData()
                                }
                            }
                        } // end Menu

                        // Open the context menu on right-click
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onPressed: {
                                if (mouse.button === Qt.RightButton) {
                                    sndcontextMenu.popup(mouse.x, mouse.y)
                                }
                            }

                        } // end MouseArea


                    }
                }

            }

        } // RowLayout

        // 底部区域
        RowLayout {
            Layout.preferredHeight: 50

            // 底部区域的内容
            TextField {
                id: id_inputdata
                Layout.fillWidth: true
                placeholderText: qsTr("Input messages...")
                text: txtUserInput.txtInput
                onTextChanged: txtUserInput.txtInput = text
            }

            Button {
                text: qsTr("Send")
                id: id_btnsendmsg
                // Function to handle the button activation
                function activate() {
                    SerialUI.updateChkboxValMap("chkautorepeat", chkautorepeat.checked ? true : false);
                    SerialUI.updateChkboxValMap("chkshowtime", chkshowtime.checked ? true : false);
                    SerialUI.updateChkboxValMap("autolinefeed", autolinefeed.checked ? true : false);
                    SerialUI.updateChkboxValMap("showsendmsg", showsendmsg.checked ? true : false);
                    SerialUI.btnSerialSend();
                }
                // Connect the click and key press events to the activate function
                onClicked: activate()
                    Keys.onReturnPressed: activate()
                    Keys.onEnterPressed: activate()

                    // Ensure the button is focusable
                    focus: true
            }
        } // end send messages

        RowLayout{
            Layout.preferredHeight: 3
        }

    }

    // footer
    footer: Item {
        width: parent.width
        height: 35 // Set the height of the status bar

        GroupBox {
            width: parent.width
            height: parent.height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            RowLayout {
                width: parent.width
                height: parent.height

                // Add more items or text to your status bar

                Rectangle {
                    Layout.fillWidth: true
                    Label {
                        text: SerialUI.rcvbytesText
                    }
                } // end Rectangle

                Rectangle {
                    Layout.fillWidth: true
                    Label {
                        text: SerialUI.sendbytesText
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
                Rectangle {
                    Layout.fillWidth: true
                    Label {
                        text: "Author: Linx.zhang"
                        horizontalAlignment: Text.AlignRight
                    }
                }

            } // end RowLayout
        } // end GroupBox
    } // end footer
}
