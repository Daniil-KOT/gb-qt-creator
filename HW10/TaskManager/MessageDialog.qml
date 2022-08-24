import QtQuick 2.2
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window
{
    id: info
    width: 300
    height: 200
    function setInfo(nTitle, body)
    {
        title = nTitle
        textEdit.text = body
    }

    Flickable
    {
        anchors.fill: parent
        id:flick
        TextArea.flickable: TextArea
        {
            id: textEdit
            width: flick.width
            height: flick.height
            readOnly: true
            focus: true
            wrapMode: TextEdit.Wrap
            selectByMouse: true
            font.pixelSize: 20
        }
    }
}
