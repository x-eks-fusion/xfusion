import re
import json
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QTextBrowser,
    QVBoxLayout,
    QWidget,
    QDialog,
    QFormLayout,
    QComboBox,
    QPushButton,
    QCheckBox,
    QMenuBar,
    QMenu,
    QFileDialog,
    QHBoxLayout,
)
from PySide6.QtGui import QTextCursor, QTextCharFormat, QColor, QFont, QAction
from PySide6.QtCore import Qt, Signal, QObject
from datetime import datetime

CONFIG_NAME = "sim_log_display_config.json"

class sim_log_handler:
    def __init__(self, text_browser, show_timestamp=False):
        self.text_browser = text_browser
        self.show_timestamp = show_timestamp
        self.buffer = ""

    def emit(self, msg):
        self.buffer += msg
        while any(end in self.buffer for end in ['\n', '\r\n', '\n\r']):
            for end in ['\r\n', '\n\r', '\n']:  # 处理行结束符
                if end in self.buffer:
                    line, self.buffer = self.buffer.split(end, 1)
                    line += end  # 保留行结束符
                    if self.show_timestamp:
                        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S,%f')[:-3]
                        line = f"{timestamp} - {line}"
                    self.append_text(line)
                    break

    def append_text(self, msg):
        cursor = self.text_browser.textCursor()
        cursor.movePosition(QTextCursor.End)
        self.text_browser.setTextCursor(cursor)

        # 解析颜色代码
        color_pattern = re.compile(r'\x1b\[(\d+;)?(\d+)m')
        parts = color_pattern.split(msg)
        default_format = QTextCharFormat()

        for i, part in enumerate(parts):
            if i % 3 == 0:
                cursor.insertText(part, default_format)
            elif i % 3 == 2:
                color_code = int(part)
                color = self.get_text_color(color_code)
                format = QTextCharFormat()
                format.setForeground(color)
                default_format = format

    def get_text_color(self, color_code):
        if self.text_browser.property("theme") == "dark":
            if color_code == 31:
                return QColor.fromRgb(255, 85, 85)  # Red
            elif color_code == 32:
                return QColor.fromRgb(85, 255, 85)  # Green
            elif color_code == 33:
                return QColor.fromRgb(255, 255, 85)  # Yellow
            elif color_code == 34:
                return QColor.fromRgb(85, 85, 255)  # Blue
            elif color_code == 35:
                return QColor.fromRgb(255, 85, 255)  # Magenta
            elif color_code == 36:
                return QColor.fromRgb(85, 255, 255)  # Cyan
            elif color_code == 37:
                return QColor.fromRgb(255, 255, 255)  # White
        else:  # Light theme
            if color_code == 31:
                return QColor.fromRgb(139, 0, 0)  # Dark Red
            elif color_code == 32:
                return QColor.fromRgb(0, 100, 0)  # Dark Green
            elif color_code == 33:
                return QColor.fromRgb(184, 134, 11)  # Dark Yellow
            elif color_code == 34:
                return QColor.fromRgb(0, 0, 139)  # Dark Blue
            elif color_code == 35:
                return QColor.fromRgb(139, 0, 139)  # Dark Magenta
            elif color_code == 36:
                return QColor.fromRgb(0, 139, 139)  # Dark Cyan
            elif color_code == 37:
                return QColor.fromRgb(0, 0, 0)  # Black
        return QColor.fromRgb(0, 0, 0)  # Default to black


class sim_log_display(QMainWindow):
    def __init__(self, font_size=16, theme="dark", scr_w=1000, scr_h=800):
        super().__init__()
        self.setWindowTitle("日志显示器")
        self.setGeometry(300, 300, scr_w, scr_h)
        self.initializing = True
        self.text_browser = QTextBrowser()
        self.text_browser.setLineWrapMode(QTextBrowser.NoWrap)
        self.text_browser.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.text_browser.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)

        # 设置默认字体大小
        font = QFont()
        font.setPointSize(font_size)
        self.text_browser.setFont(font)

        layout = QVBoxLayout()
        layout.addWidget(self.text_browser)

        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

        self.log_handler = sim_log_handler(self.text_browser)
        self.set_theme(theme)

        # 添加菜单栏
        self.menu_bar = QMenuBar(self)
        self.setMenuBar(self.menu_bar)

        self.settings_menu = QMenu("设置", self)
        self.menu_bar.addMenu(self.settings_menu)
        self.settings_menu.setFont(font)

        self.font_size_menu = QMenu("字体大小", self.settings_menu)
        self.settings_menu.addMenu(self.font_size_menu)
        self.font_size_menu.setFont(font)

        for size in range(12, 25):
            action = QAction(str(size), self)
            action.setFont(font)
            action.triggered.connect(lambda checked, s=size: self.set_font_size(s))
            self.font_size_menu.addAction(action)

        self.theme_menu = QMenu("主题", self.settings_menu)
        self.settings_menu.addMenu(self.theme_menu)
        self.theme_menu.setFont(font)

        dark_theme_action = QAction("深色", self)
        dark_theme_action.triggered.connect(lambda: self.set_theme("dark"))
        self.theme_menu.addAction(dark_theme_action)

        light_theme_action = QAction("浅色", self)
        light_theme_action.triggered.connect(lambda: self.set_theme("light"))
        self.theme_menu.addAction(light_theme_action)

        # 添加底部复选框和按钮
        bottom_layout = QHBoxLayout()

        self.auto_save_checkbox = QCheckBox("自动保存到文件")
        bottom_layout.addWidget(self.auto_save_checkbox)

        self.show_timestamp_checkbox = QCheckBox("显示时间戳")
        self.show_timestamp_checkbox.setChecked(False)
        self.show_timestamp_checkbox.stateChanged.connect(self.toggle_timestamp)
        bottom_layout.addWidget(self.show_timestamp_checkbox)

        self.save_button = QPushButton("保存")
        self.save_button.clicked.connect(self.save_to_file)
        bottom_layout.addWidget(self.save_button)

        self.clear_button = QPushButton("清除")
        self.clear_button.clicked.connect(self.clear_window)
        bottom_layout.addWidget(self.clear_button)

        layout.addLayout(bottom_layout)

        # 设置底部控件的字体大小
        font.setPointSize(font_size)
        self.auto_save_checkbox.setFont(font)
        self.show_timestamp_checkbox.setFont(font)
        self.save_button.setFont(font)
        self.clear_button.setFont(font)

        # 设置窗口标题的字体大小
        title_font = QFont()
        title_font.setPointSize(font_size + 2)
        self.setFont(title_font)

        # 加载配置

        self.load_config()
        self.initializing = False

    def set_font_size(self, size):
        font = self.text_browser.font()
        font.setPointSize(size)
        self.text_browser.setFont(font)

        self.settings_menu.setFont(font)
        self.font_size_menu.setFont(font)
        self.theme_menu.setFont(font)

        # 更新底部控件的字体大小
        self.auto_save_checkbox.setFont(font)
        self.show_timestamp_checkbox.setFont(font)
        self.save_button.setFont(font)
        self.clear_button.setFont(font)

        # 更新窗口标题的字体大小
        title_font = QFont()
        title_font.setPointSize(size + 2)
        self.setFont(title_font)

        # 保存配置
        if not self.initializing:
            self.save_config()

    def set_theme(self, theme):
        if theme == "dark":
            self.text_browser.setStyleSheet(
                "background-color: #2b2b2b; color: #ffffff;"
            )
        else:
            self.text_browser.setStyleSheet(
                "background-color: #ffffff; color: #000000;"
            )
        self.text_browser.setProperty("theme", theme)

        # 保存配置
        if not self.initializing:
            self.save_config()

    def toggle_timestamp(self, state):
        self.log_handler.show_timestamp = bool(state)

        # 保存配置
        if not self.initializing:
            self.save_config()

    def save_to_file(self):
        options = QFileDialog.Options()
        file_name, _ = QFileDialog.getSaveFileName(
            self, "保存文件", "", "Text Files (*.txt);;All Files (*)", options=options
        )
        if file_name:
            with open(file_name, 'w') as file:
                file.write(self.text_browser.toPlainText())

    def clear_window(self):
        self.text_browser.clear()

    def log(self, msg):
        self.log_handler.emit(msg)

    def save_config(self):
        config = {
            "font_size": self.text_browser.font().pointSize(),
            "theme": self.text_browser.property("theme"),
            "show_timestamp": self.log_handler.show_timestamp,
        }
        with open(CONFIG_NAME, "w") as config_file:
            json.dump(config, config_file)

    def load_config(self):
        try:
            with open(CONFIG_NAME, "r") as config_file:
                config = json.load(config_file)
                self.set_font_size(config.get("font_size", 12))
                self.set_theme(config.get("theme", "dark"))
                self.show_timestamp_checkbox.setChecked(
                    config.get("show_timestamp", True)
                )
        except FileNotFoundError:
            pass

if __name__ == "__main__":

    app = QApplication.instance() or QApplication([])
    window = sim_log_display(16, "dark")  # 设置默认字体大小为16，默认主题为深色
    window.show()

    window.log("\x1b[0;31m DEBUG \x1b[0m\r\n")
    window.log("\x1b[0;32m INFO \x1b[0m\n\r")
    window.log("\x1b[0;33m WARN \x1b[0m\n")
    window.log("\x1b[0;34m ERROR \x1b[0m\r\n")
    app.exec()
