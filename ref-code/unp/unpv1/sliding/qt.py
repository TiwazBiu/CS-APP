import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QApplication, QWidget, QToolTip, 
    QPushButton, QMessageBox, QMainWindow, QApplication, 
    QAction, QMenu, QHBoxLayout, QVBoxLayout, QTextEdit,
    QLCDNumber, QSlider, QLineEdit)
from PyQt5.QtGui import QFont, QIcon, QColor


class MainWindow(QMainWindow):
    """docstring for MainWindow"""
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):

        exitAct = QAction(QIcon('zen.png'), '&Exit', self)
        exitAct.setShortcut('Ctrl+Q')
        exitAct.setStatusTip('Exit application')
        exitAct.triggered.connect(QApplication.instance().quit)

        newAct = QAction('New', self)

        impAct = QAction('Import mail', self)
        impMenu = QMenu('Import', self)
        impMenu.addAction(impAct)

        viewStatAct = QAction('View Statusbar', self, checkable=True)
        viewStatAct.setStatusTip('View Statusbar')
        viewStatAct.setChecked(True)
        viewStatAct.triggered.connect(self.toggleMenu)

        self.statusbar = self.statusBar()
        self.statusbar.showMessage('Ready')   

        self.setWindowTitle('menu')
        menubar = self.menuBar()
        menubar.setNativeMenuBar(False)
        OptMenu = menubar.addMenu('Option')


        OptMenu.addAction(viewStatAct)
        OptMenu.addAction(newAct)
        OptMenu.addMenu(impMenu)
        OptMenu.addAction(exitAct)

        wid = Zen()
        self.setCentralWidget(wid)
        self.setGeometry(300, 300, 300, 150)

        self.show()

    def toggleMenu(self, state):
        if (state):
            self.statusbar.show()
        else:
            self.statusbar.hide()

class Zen(QWidget):
    """docstring for Zen"""
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        QToolTip.setFont(QFont('SansSerif', 20))

        beginButton = QPushButton("开始接收")
        stopButton  = QPushButton("停止接收")
        quitButton  = QPushButton("退出")
        quitButton.clicked.connect(QApplication.instance().quit)


        self.ip = QLineEdit()
        ipLabel = QLabel(self)
        self.ipLabel.setText('ip:')
        h1 = QHBoxLayout()
        h1.setSpacing(2)
        h1.addWidget(ipLabel, self.ip)

        
        port = QLineEdit()
        windowSize = QLineEdit()
        timer0 = QLineEdit()
        timer1 = QLineEdit()
        cksum_err = QLineEdit()
        lost_err = QLineEdit()

        parabox = QVBoxLayout()
        parabox.addStretch(1)
        parabox.addWidget(socket)
        parabox.addWidget(windowSize)
        parabox.addWidget(timer0)
        parabox.addWidget(timer1)

        errbox = QVBoxLayout()
        errbox.addWidget(cksum_err)
        errbox.addWidget(lost_err)

        setbox = QVBoxLayout()
        setbox.addLayout(parabox)
        setbox.addLayout(errbox)

        
        
        outbox = QHBoxLayout()
        outbox.addStretch(1)
        outbox.addWidget(beginButton)
        outbox.addWidget(stopButton)
        outbox.addWidget(quitButton)

        lcd = QLCDNumber(self)
        lcd.setSegmentStyle(QLCDNumber.Flat)
        palette = lcd.palette()
        palette.setColor(palette.WindowText, QColor(85, 85, 255))
        palette.setColor(palette.Background, QColor(0, 170, 255))
        lcd.setPalette(palette)

        sld = QSlider(Qt.Horizontal, self)
        sld.valueChanged.connect(lcd.display)

        output = QTextEdit()
        output.setStatusTip('Output from terminal')

        resbox = QVBoxLayout()
        # vbox.addStretch(1)
        resbox.addWidget(lcd)
        resbox.addWidget(output)
        resbox.addWidget(sld)
        resbox.addLayout(outbox)


        whole = QHBoxLayout()
        whole.addLayout(setbox)
        whole.addLayout(resbox)

        self.setLayout(whole)


if __name__ == '__main__':
    app = QApplication(sys.argv)

    zen = MainWindow()

    sys.exit(app.exec_()) 