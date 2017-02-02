import QtQuick 2.8
import QtQml.Models 2.2

Item {
    GridView {
        model: manaModel
        delegate: Column {
            Text { text: amt }
        }

        Image {
            source: "wizard3-512.png"
        }
    }

    ListModel {
        id: manaModel
        Item { property var amt: 5 }
    }
}
