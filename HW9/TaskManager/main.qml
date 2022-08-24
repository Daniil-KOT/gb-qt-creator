import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import com.vv.filectrl
import "E:/code/GB/gb-qt-creator/HW9/TaskManager/CustomButton.qml"
Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Task Manager")

    property string taskname_: qtaskname.text
    property string deadline_: qdeadline.text
    property int progress_: qprogress.value

    TextEdit
    {
        id: qtaskname
        width: 240
        font.family: "Arial"
        font.pixelSize: 14
        text: "Enter task name"
        focus: true
        textMargin: 5
    }

    TextInput
    {
        id: qdeadline
        width: 240
        anchors.top: qtaskname.bottom
        text: "dd.mm.yyyy"
        validator: RegularExpressionValidator
        {
            regularExpression: /^(((0[1-9]|[12]\d|3[01])\.(0[13578]|1[02])\.((19|[2-9]\d)\d{2}))|((0[1-9]|[12]\d|30)\.(0[13456789]|1[012])\.((19|[2-9]\d)\d{2}))|((0[1-9]|1\d|2[0-8])\.02\.((19|[2-9]\d)\d{2}))|(29\.02\.((1[6-9]|[2-9]\d)(0[48]|[2468][048]|[13579][26])|(([1][26]|[2468][048]|[3579][26])00))))$/g;
        }
    }

    Slider
    {
        id: qprogress
        from: 0
        stepSize: 1
        to: 10
        anchors.top: qdeadline.bottom
        anchors.topMargin: 5
    }

    CustomButton
    {
        id: save
        text: "Save"
        onClicked:
        {
            taskname_ = qtaskname.text;
            deadline_ = qdeadline.text;
            progress_ = qprogress.value;
            filectrl.saveData(taskname_, deadline_, progress_);
        }
        anchors.top: qprogress.bottom
        anchors.topMargin: 5
    }

    CustomButton
    {
        id: load
        text: "Load task"
        onClicked:
        {
            filectrl.getNextTask();
        }
        anchors.top: qprogress.bottom
        anchors.left: save.right
        anchors.topMargin: 5
        anchors.leftMargin: 5
    }

    FileController
    {
        id: filectrl
    }
}
