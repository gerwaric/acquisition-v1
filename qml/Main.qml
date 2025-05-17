import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import AppWindow

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "Acquisition"

    Label { text: "Hello" }

    menuBar: MainMenuBar {}

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            currentIndex: stackLayout.currentIndex
            onCurrentIndexChanged: stackLayout.currentIndex = currentIndex

            TabButton {
                height: implicitHeight
                width: implicitWidth;
                contentItem: Text {
                    padding: 5
                    text: qsTr("Authentication");
                    elide: Text.ElideNone
                }
            }
            TabButton {
                height: implicitHeight
                width: implicitWidth;
                contentItem: Text {
                    padding: 5
                    text: qsTr("Settings");
                    elide: Text.ElideNone
                }
            }
            TabButton {
                height: implicitHeight
                width: implicitWidth;
                contentItem: Text {
                    padding: 5
                    text: qsTr("Search");
                    elide: Text.ElideNone
                }
            }
        }

        Rectangle {
            height: 1
            color: palette.mid
            Layout.fillWidth: true
        }

        StackLayout {
            id: stackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: 0
            AuthenticationTab {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            SettingsTab {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            SearchTab {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    footer: Frame {
        width: parent.width
        padding: 10

        RowLayout {
            anchors.fill: parent
            spacing: 10
            Layout.fillWidth: true
            Label {
                text: "Status: Offline"
            }
        }
    }
}
