import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    Image {
        source: "old_paper_2-wallpaper-1280x720.jpg"
        id: item1
        anchors.fill: parent
        Hand {
            id: hand
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottomMargin: 140
            anchors.rightMargin: 50
            anchors.leftMargin: 50
        }

        Board {
            id: board
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 174
            anchors.top: parent.top
            anchors.topMargin: 257
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
        }
        /*
        Avatar {
            id: avatar
            x: 1129
            y: 332
            width: 143
            height: 150
        }*/
    }
}
