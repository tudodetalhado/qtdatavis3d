/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVis3D module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import com.digia.QtDataVis3D 1.0
import "."

Item {
    id: mainview
    width: 800
    height: 600
    visible: true

    Item {
        id: dataView
        width: parent.width - tableView.width
        height: parent.height
        anchors.right: parent.right;

        BarDataMapping {
            id: valueMapping
            rowRole: "year"
            columnRole: "month"
            valueRole: "expenses"
            rowCategories: ["2000", "2001", "2002", "2003", "2004"]
            columnCategories: ["Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
        }

        ListModel {
            id: dataModel
            ListElement{ year: "2000"; month: "Jan"; expenses: "3";  income: "8" }
            ListElement{ year: "2000"; month: "Feb"; expenses: "8";  income: "14" }
            ListElement{ year: "2000"; month: "Mar"; expenses: "10"; income: "20" }
            ListElement{ year: "2000"; month: "Apr"; expenses: "12"; income: "24" }
            ListElement{ year: "2000"; month: "May"; expenses: "10"; income: "19" }
            ListElement{ year: "2000"; month: "Jun"; expenses: "5";  income: "8" }
            ListElement{ year: "2000"; month: "Jul"; expenses: "1";  income: "4" }
            ListElement{ year: "2000"; month: "Aug"; expenses: "7";  income: "12" }
            ListElement{ year: "2000"; month: "Sep"; expenses: "4";  income: "16" }
            ListElement{ year: "2000"; month: "Oct"; expenses: "22"; income: "33" }
            ListElement{ year: "2000"; month: "Nov"; expenses: "16"; income: "25" }
            ListElement{ year: "2000"; month: "Dec"; expenses: "2";  income: "7" }

            ListElement{ year: "2001"; month: "Jan"; expenses: "4";  income: "5"  }
            ListElement{ year: "2001"; month: "Feb"; expenses: "4";  income: "7"  }
            ListElement{ year: "2001"; month: "Mar"; expenses: "11"; income: "14"  }
            ListElement{ year: "2001"; month: "Apr"; expenses: "16"; income: "22"  }
            ListElement{ year: "2001"; month: "May"; expenses: "3";  income: "5"  }
            ListElement{ year: "2001"; month: "Jun"; expenses: "4";  income: "8"  }
            ListElement{ year: "2001"; month: "Jul"; expenses: "7";  income: "9"  }
            ListElement{ year: "2001"; month: "Aug"; expenses: "9";  income: "13"  }
            ListElement{ year: "2001"; month: "Sep"; expenses: "1";  income: "6"  }
            ListElement{ year: "2001"; month: "Oct"; expenses: "14"; income: "25"  }
            ListElement{ year: "2001"; month: "Nov"; expenses: "19"; income: "29"  }
            ListElement{ year: "2001"; month: "Dec"; expenses: "5";  income: "7"  }

            ListElement{ year: "2002"; month: "Jan"; expenses: "14"; income: "22"  }
            ListElement{ year: "2002"; month: "Feb"; expenses: "5";  income: "7"  }
            ListElement{ year: "2002"; month: "Mar"; expenses: "1";  income: "9"  }
            ListElement{ year: "2002"; month: "Apr"; expenses: "1";  income: "12"  }
            ListElement{ year: "2002"; month: "May"; expenses: "5";  income: "9"  }
            ListElement{ year: "2002"; month: "Jun"; expenses: "5";  income: "8"  }
            ListElement{ year: "2002"; month: "Jul"; expenses: "3";  income: "7"  }
            ListElement{ year: "2002"; month: "Aug"; expenses: "1";  income: "5"  }
            ListElement{ year: "2002"; month: "Sep"; expenses: "2";  income: "4"  }
            ListElement{ year: "2002"; month: "Oct"; expenses: "10"; income: "13"  }
            ListElement{ year: "2002"; month: "Nov"; expenses: "12"; income: "17"  }
            ListElement{ year: "2002"; month: "Dec"; expenses: "6";  income: "9"  }

            ListElement{ year: "2003"; month: "Jan"; expenses: "2";  income: "6"  }
            ListElement{ year: "2003"; month: "Feb"; expenses: "4";  income: "8"  }
            ListElement{ year: "2003"; month: "Mar"; expenses: "7";  income: "12"  }
            ListElement{ year: "2003"; month: "Apr"; expenses: "9";  income: "15"  }
            // rest of 2003 missing on purpose, as well as whole of 2004
        }

        CategoryAxis {
            id: rowAxis
            categoryLabels: ["2000", "2001", "2002", "2003", "2004"]
        }
        CategoryAxis {
            id: columnAxis
            categoryLabels: ["January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October", "November", "December"]
        }
        ValueAxis {
            id: incomeAxis
            min: 0
            max: 35
            labelFormat: "%.2f M\u20AC"
            title: "Monthly income"
        }
        ValueAxis {
            id: expensesAxis
            min: 0
            max: 35
            labelFormat: "-%.2f M\u20AC"
            title: "Monthly expenses"
        }

        Bars3D {
            id: testchart
            width: dataView.width
            height: dataView.height
            gridVisible: true
            shadowQuality: Bars3D.ShadowNone
            selectionMode: Bars3D.ModeItem
            labelTransparency: Bars3D.TransparencyNone
            theme: Bars3D.ThemeBrownSand
            rows: 5
            columns: 12
            mapping: valueMapping
            barThickness: 0.5
            barSpacing: Qt.size(0.5, 0.5)
            barSpacingRelative: false
            barType: Bars3D.BevelBars
            rowAxis: rowAxis
            columnAxis: columnAxis
            valueAxis: expensesAxis
            itemLabelFormat: "@valueTitle for @colLabel, @rowLabel: @valueLabel"

            onDataResolved: {
                // Can't select a bar until data has been resolved from model to proxy
                //selectedBarPos = Qt.point(0, 5)
            }
        }

    }

    Component.onCompleted: {
        testchart.data = dataModel
    }


    TableView {
        id: tableView
        x: 0
        y: 0
        width: 300
        height: parent.height - mappingToggle.height - shadowToggle.height
        TableViewColumn{ role: "year"  ; title: "Year" ; width: 80 }
        TableViewColumn{ role: "month" ; title: "Month" ; width: 80 }
        TableViewColumn{ role: "expenses" ; title: "Expenses" ; width: 70 }
        TableViewColumn{ role: "income" ; title: "Income" ; width: 70 }
        model: dataModel
    }

    Button {
        id: mappingToggle
        anchors.bottom: parent.bottom
        width: tableView.width
        text: "Show Income"
        onClicked: {
            if (valueMapping.valueRole == "expenses") {
                valueMapping.valueRole = "income"
                text = "Show Expenses"
                testchart.valueAxis = incomeAxis
            } else {
                valueMapping.valueRole = "expenses"
                text = "Show Income"
                testchart.valueAxis = expensesAxis
            }
        }
    }

    Button {
        id: shadowToggle
        anchors.bottom: mappingToggle.top
        width: tableView.width
        text: "Toggle Shadows"
        onClicked: {
            if (testchart.shadowQuality == Bars3D.ShadowNone) {
                testchart.shadowQuality = Bars3D.ShadowLow;
            } else {
                testchart.shadowQuality = Bars3D.ShadowNone;
            }
        }
    }
}
