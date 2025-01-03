import sys
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow
from PyQt5.QtGui import QPixmap, QColor, QImage
from PyQt5.QtCore import Qt, QTimer, QPoint

class ColorPicker(QMainWindow):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Color Picker')
        self.setGeometry(100, 100, 300, 200)

        self.label = QLabel(self)
        self.label.setAlignment(Qt.AlignCenter)
        self.label.setText("Move the mouse to pick a color")
        self.setCentralWidget(self.label)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_color)
        self.timer.start(100)  # Update every 100ms

    def update_color(self):
        cursor_pos = QApplication.desktop().cursor().pos()
        screen = QApplication.primaryScreen()
        pixmap = screen.grabWindow(0, cursor_pos.x(), cursor_pos.y(), 1, 1)

        image = pixmap.toImage()
        color = QColor(image.pixel(0, 0))

        # 获取颜色的十六进制值和 RGB 值
        hex_color = color.name()
        rgb_color = f"rgb({color.red()}, {color.green()}, {color.blue()})"

        self.label.setText(
                        f"Position: {cursor_pos.x()}, {cursor_pos.y()}\n"
            f"Hex: {hex_color}\n"
            f"RGB: {rgb_color}"
        )

if __name__ == '__main__':
    app = QApplication(sys.argv)
    picker = ColorPicker()
    picker.show()
    sys.exit(app.exec_())