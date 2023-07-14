import QtQuick
import NativeHtmlElements

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "transparent"

    VideoNativeHtmlElement {
        width: 640
        height: 480
    }

    TextNativeHtmlElement {
        width: 640
        height: 100
    }

    Text {
        anchors.centerIn: parent
        text: "Hello video!"
    }
}
