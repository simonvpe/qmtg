import QtQuick 2.7

Item {
    ListView {
        id: view
        anchors.fill: parent
        model: handModel
        orientation: ListView.Horizontal
        delegate: Image {
            id: img
            source: url
            state: "NotHovering"
            width: parent.width / 10.0
            fillMode: Image.PreserveAspectFit
            cache: true

            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 10
            Drag.hotSpot.y: 10

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { parent.state = "Hovering" }
                onExited:  { parent.state = "NotHovering" }
                drag.target: parent
            }
            states: [
                State {
                    name: "Hovering"
                    PropertyChanges { target: img; scale: 2.3 }
                    PropertyChanges { target: img; y: img.parent.y - 0.6*img.height }
                    PropertyChanges { target: img; z: 100 }
                },
                State {
                    name: "NotHovering"
                    PropertyChanges { target: img; scale: 1.0 }
                    PropertyChanges { target: img; y: img.parent.y }
                    PropertyChanges { target: img; z: 99 }
                }

            ]
            transitions: Transition {
                PropertyAnimation { properties: "scale,y"; easing.type: Easing.InOutQuint }
            }
        }
    }

    // Dummy hand
    ListModel {
        id: handModel
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417576.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417577.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417578.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417579.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417580.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417581.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417582.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417583.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417584.jpg" }
        ListElement { url: "https://image.deckbrew.com/mtg/multiverseid/417585.jpg" }
    }

    /*
    // Use sometime later to load image url's from deckbrew.com
    function request() {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED) {
                print('HEADERS_RECEIVED')
            } else if(xhr.readyState === XMLHttpRequest.DONE) {
                print('DONE1')
                var json = JSON.parse(xhr.responseText.toString())
                print('DON2')
                print('Result'+json.toString())
                view.model = json //editions //[0].image_url
            }
        }

        //xhr.open("GET", "http://api.flickr.com/services/feeds/photos_public.gne?format=json&nojsoncallback=1&tags=munich");
        xhr.open("GET","https://api.deckbrew.com/mtg/cards?page=2");
        xhr.send();
    }

    Component.onCompleted: {
        request()
    }
    */
}
