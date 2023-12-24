import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout

def on_button_clicked():
    print("按钮被点击了！")

app = QApplication(sys.argv)

window = QWidget()
window.setWindowTitle("PyQt5 简单示例")

layout = QVBoxLayout()
button = QPushButton("点击我")
button.clicked.connect(on_button_clicked)  # 将按钮的 clicked 信号连接到槽函数
layout.addWidget(button)

window.setLayout(layout)
window.show()

sys.exit(app.exec_())
