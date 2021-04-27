#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

# OpenSource unlicensed
# Free to use and modify this source code
# MarcoAAG 04-19-21

import sys
import numpy as np

import pyqtgraph as pg
from pyqtgraph import PlotWidget, plot

from PyQt5.QtCore import QSize, Qt, QTimer, QRect
from PyQt5.QtGui import QColor, QPalette, QFont
from PyQt5.QtWidgets import (QApplication,
                             QMainWindow,
                             QMessageBox,
                             QPushButton,
                             QLabel,
                             QWidget,
                             QHBoxLayout,
                             QVBoxLayout,
                             QGridLayout)
import serial
import backEnd


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.port_name = "/dev/ttyUSB0"
        self.port = ""
        self.guiInit()


    def setTitle(self):
        self.title = QLabel("Pulsometer", self)
        font = QFont()
        font.setFamily('Montserrat')
        font.setWeight(QFont.Bold)
        self.title.setStyleSheet("color: rgb(255,255,255);")
        font.setPointSize(30)
        self.title.setFont(font)
        return(self.title)

    def infoInit(self):
        self.info = QLabel("Author:\t\tMarcoAAG\n\n\n\nVersion:\t\t      1.0")
        self.font = QFont()
        self.font.setFamily('Montserrat')
        self.font.setWeight(QFont.Thin)
        self.info.setStyleSheet("color: rgb(255,255,255);")
        self.font.setPointSize(15)
        self.info.setFont(self.font)
        return(self.info)

    def setBtn(self):
        self.button = QPushButton("Connect", self)
        self.button.setStyleSheet("background-color: rgb(119,54,54); color: rgb(255,255,255);")
        self.button.setCheckable(True)
        self.button.clicked.connect(self.btnClicked)
        return(self.button)

    def guiInit(self):
        self.setWindowTitle("Pulsometer")

        self.setGeometry(0, 0, 960, 540)
        self.setFixedSize(QSize(960,540))
        self.setMinimumSize(QSize(960,540))
        self.setMaximumSize(QSize(1920,1080))

#         self.layout = QVBoxLayout()
        self.layout = QGridLayout()

#       Set background
        self.setStyleSheet("background-color: rgb(50,50,50);")

#       Set Title
        self.layout.addWidget(self.setTitle(),0,0, alignment=Qt.AlignTop|Qt.AlignCenter)
        self.layout.addWidget(self.infoInit(),1,0, alignment=Qt.AlignHCenter|Qt.AlignCenter)


#       Set button
        self.layout.addWidget(self.setBtn(),2,0, alignment=Qt.AlignCenter|Qt.AlignBottom)

#         Organize layout into center
        self.widget = QWidget()
        self.widget.setLayout(self.layout)
        self.setCentralWidget(self.widget)

    def dialogWin(self):
        try:
            self.port =  backEnd.backEnd(self.port_name, 9600, serial.PARITY_NONE, serial.STOPBITS_ONE, serial.EIGHTBITS, 1)
            return 0
        except:
            self.dlg = QMessageBox(self)
            self.dlg.setWindowTitle("Error")
            self.dlg.setText("Not connected")
            self.dlg.setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);")
            self.dlg.setIcon(QMessageBox.Critical)
            self.dlg.setStandardButtons(QMessageBox.Retry | QMessageBox.Ok)
            self.dialogBtn = self.dlg.exec_()
            if(self.dialogBtn == QMessageBox.Ok):
                self.button.setEnabled(True)
                return 2
            else:
                return 1

    def connectionDlg(self):
        self.conn_dlg = QMessageBox(self)
        self.conn_dlg.setWindowTitle("Waiting")
        self.conn_dlg.setText("Connecting ...")
        self.conn_dlg.setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0,0,0);")
        self.conn_dlg.setIcon(QMessageBox.Information)
        self.conn_dlg.setStandardButtons(QMessageBox.Abort)
        self.conn_dlg_timer = pg.QtCore.QTimer()
        self.conn_dlg_timer.timeout.connect(self.connectionTimer)
        self.conn_dlg_timer.start(10)
        self.conn_dlg_btn = self.conn_dlg.exec_()
        if(self.conn_dlg_btn == QMessageBox.Abort):
            self.button.setEnabled(True)
            self.conn_dlg_timer.stop()

    def connectionTimer(self):
        print(self.port.protocolInit())

    def btnClicked(self):
        print("Clicked")
        self.button.setEnabled(False)
        is_connected = self.dialogWin()
        while(is_connected == 1):
            is_connected = self.dialogWin()

        if(is_connected == 0):
            self.connectionDlg()
#             self.layout.itemAt(1).widget().deleteLater()
#             self.plotGraph()


    def plotGraph(self):
        self.graphWidget = pg.PlotWidget(self)
        self.layout.addWidget(self.graphWidget,1,0)

        self.graphWidget.setGeometry(QRect(25,25,550,550))
        self.graphWidget.setBackground((40,40,40))

        self.hour = np.random.normal(size=300)
        self.plt = self.graphWidget.plot(self.hour)

        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(10)

    def update(self):
        self.hour[:-1] = self.hour[1:]
        self.hour[-1] = np.random.normal()
        self.plt.setData(self.hour)




app = QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec_()
