import QtQuick 2.0

Item {
    DropArea {
        id: board
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: "green"
            visible: parent.containsDrag
        }
    }
}
