import os
import subprocess
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QListWidget, QListWidgetItem,
    QLineEdit, QLabel, QHBoxLayout, QPushButton
)
from PyQt5.QtGui import QIcon, QFont, QColor, QPixmap
from PyQt5.QtCore import Qt
import win32gui  # 用于图标提取
import win32com.client  # 用于解析.lnk文件


class DesktopIconManager(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("桌面图标管理工具")
        self.setWindowIcon(QIcon("res/app_icon.svg"))  # 自定义窗口图标
        self.setFixedSize(600, 800)  # 固定窗口大小
        self.setStyleSheet(self.get_stylesheet())  # 应用样式

        # 主布局
        main_layout = QVBoxLayout(self)

        # 标题
        title_label = QLabel("桌面图标管理工具")
        title_label.setFont(QFont("Arial", 18, QFont.Bold))
        title_label.setAlignment(Qt.AlignCenter)
        title_label.setObjectName("titleLabel")
        main_layout.addWidget(title_label)

        # 搜索框
        search_layout = QHBoxLayout()
        self.search_bar = QLineEdit()
        self.search_bar.setPlaceholderText("🔍 搜索应用程序...")
        self.search_bar.textChanged.connect(self.filter_icons)
        self.search_bar.setObjectName("searchBar")
        search_layout.addWidget(self.search_bar)

        refresh_button = QPushButton("刷新")
        refresh_button.clicked.connect(self.load_desktop_icons)
        refresh_button.setObjectName("refreshButton")
        search_layout.addWidget(refresh_button)

        main_layout.addLayout(search_layout)

        # 图标列表
        self.list_widget = QListWidget()
        self.list_widget.setObjectName("iconList")
        self.list_widget.itemClicked.connect(self.open_application)
        main_layout.addWidget(self.list_widget)

        # 加载桌面图标
        self.load_desktop_icons()

    def load_desktop_icons(self):
        """加载桌面上的所有图标"""
        self.list_widget.clear()

        desktop_path = os.path.join(os.environ['USERPROFILE'], 'Desktop')
        items = os.listdir(desktop_path)

        for item in items:
            item_path = os.path.join(desktop_path, item)
            if os.path.isdir(item_path):
                self.add_list_item(item, item_path, "文件夹")
            elif item.endswith(".lnk") or item.endswith(".exe"):
                self.add_list_item(item, item_path, "应用程序")
            else:
                self.add_list_item(item, item_path, "文件")  # 添加其他文件

    def add_list_item_simple(self, name, path, item_type):
        """添加图标项到列表中"""
        item = QListWidgetItem(f"{name} [{item_type}]")
        item.setData(Qt.UserRole, path)  # 存储完整路径
        if item_type == "文件夹":
            item.setIcon(QIcon("res/folder_icon.svg"))  # 文件夹图标
        else:
            item.setIcon(QIcon("res/app_icon.svg"))  # 默认程序图标
        self.list_widget.addItem(item)

    def add_list_item(self, name, path, item_type):
        """添加图标项到列表中"""
        item = QListWidgetItem(f"{name} [{item_type}]")
        item.setData(Qt.UserRole, path)  # 存储完整路径

        icon = self.get_icon(path, item_type)
        if icon:
            item.setIcon(icon)

        self.list_widget.addItem(item)

    def get_icon(self, path, item_type):
        """根据路径和类型获取图标"""
        if item_type == "文件夹":
            return QIcon.fromTheme('folder')  # 使用系统默认文件夹图标

        try:
            if path.endswith(".lnk"):
                # 解析.lnk文件并获取目标路径
                shell = win32com.client.Dispatch("WScript.Shell")
                shortcut = shell.CreateShortcut(path)
                target_path = shortcut.TargetPath
                path = target_path  # 更新path为实际目标路径

            # 提取图标
            large, small = win32gui.ExtractIconEx(path, 0)
            if small:  # 如果成功提取了图标
                hicon = small[0]  # 获取小图标句柄
                pixmap = QPixmap.fromWinHICON(hicon)
                icon = QIcon(pixmap)
                win32gui.DestroyIcon(hicon)  # 销毁图标句柄以释放资源
                return icon
        except Exception as e:
            print(f"无法提取 {path} 的图标: {e}")

        # 如果失败，则返回默认图标
        return QIcon("res/app_icon.svg") if item_type != "文件夹" else QIcon("res/folder_icon.svg")

    def filter_icons(self, text):
        """根据搜索栏过滤图标"""
        for i in range(self.list_widget.count()):
            item = self.list_widget.item(i)
            item.setHidden(text.lower() not in item.text().lower())

    def open_application(self, item):
        """打开选中的程序或文件夹"""
        path = item.data(Qt.UserRole)
        if os.path.isdir(path):
            os.startfile(path)
        else:
            subprocess.Popen(path, shell=True)

    @staticmethod
    def get_stylesheet():
        """定义 QSS 样式表"""
        return """
            QWidget {
                background-color: #f0f0f5;
            }
            #titleLabel {
                color: #333;
                padding: 10px;
                border-bottom: 2px solid #5d8aa8;
            }
            #searchBar {
                border: 1px solid #ccc;
                border-radius: 5px;
                padding: 5px;
                font-size: 14px;
            }
            #refreshButton {
                background-color: #5d8aa8;
                color: white;
                border: none;
                border-radius: 5px;
                padding: 5px 10px;
                font-size: 14px;
            }
            #refreshButton:hover {
                background-color: #3c6e91;
            }
            #iconList {
                background-color: white;
                border: 1px solid #ccc;
                border-radius: 5px;
                padding: 5px;
                font-size: 14px;
            }
            QListWidget::item {
                padding: 8px;
                border: none;
                border-bottom: 1px solid #eee;
            }
            QListWidget::item:hover {
                background-color: #e8f0fe;
            }
            QListWidget::item:selected {
                background-color: #5d8aa8;
                color: white;
            }
            /* 美化滚动条 */
            QScrollBar:vertical {
                border: none;
                background-color: #f0f0f5;
                width: 12px;
                margin: 0px 0px 0px 0px;
            }
            QScrollBar::handle:vertical {
                background-color: #c0c0c0;
                border-radius: 6px;
                min-height: 20px;
            }
            QScrollBar::handle:vertical:hover {
                background-color: #a0a0a0;
            }
            QScrollBar::add-line:vertical,
            QScrollBar::sub-line:vertical {
                background: none;
            }
            QScrollBar::add-page:vertical,
            QScrollBar::sub-page:vertical {
                background: none;
            }
            QScrollBar:horizontal {
                border: none;
                background-color: #f0f0f5;
                height: 12px;
                margin: 0px 0px 0px 0px;
            }
            QScrollBar::handle:horizontal {
                background-color: #c0c0c0;
                border-radius: 6px;
                min-width: 20px;
            }
            QScrollBar::handle:horizontal:hover {
                background-color: #a0a0a0;
            }
            QScrollBar::add-line:horizontal,
            QScrollBar::sub-line:horizontal {
                background: none;
            }
            QScrollBar::add-page:horizontal,
            QScrollBar::sub-page:horizontal {
                background: none;
            }
        """


if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    window = DesktopIconManager()
    window.show()
    sys.exit(app.exec_())