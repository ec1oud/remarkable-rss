import QtQuick 2.0
import QtQuick.Window 2.0

Window {
	width: 320
	height: 240
	visible: true
	color: "black"

  Text {
	anchors.fill: parent
	anchors.margins: 10
      text: "<b>Hello</b> <i>World!</i>"
//      font.family: "Helvetica"
      font.pointSize: 20
	color: "white"
  }

}
