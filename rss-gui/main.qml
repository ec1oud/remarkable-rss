import QtQuick 2.6
import QtQuick.Window 2.1
import Rss 1.0

Window {
    width: 1404
    height: 1872
    visible: true

    Rectangle {
        anchors.fill: parent
        ListView {
            anchors.fill: parent
            model: NewsModel {
                url: "http://feeds.bbci.co.uk/news/rss.xml?edition=uk"
            }
            delegate: Item {
                width: parent.width
                height: Math.max(132, titleText.height + storyText.height + 4)
                Image {
                    id: storyImage
                    y: 2
                    height: 128
                    source: image
                    fillMode: Image.PreserveAspectFit
                }
                Text {
                    id: titleText
                    text: title
                    x: storyImage.width + 2
                    width: parent.width - x - 2
                    elide: Text.ElideRight
                    font.weight: Font.DemiBold
                }
                Text {
                    id: storyText
                    x: storyImage.width + 2
                    width: parent.width - x - 2
                    y: titleText.height
                    wrapMode: Text.WordWrap
                    text: description
                }
                Rectangle {
                    width: parent.width; height: 1
                    color: "#888"
                }
            }
        }
    }
}
