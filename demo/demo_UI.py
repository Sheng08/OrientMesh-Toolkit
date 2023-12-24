#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
sys.path.append('.')

from python.OBBGLWidget import OBBGLWidget
from PySide2.QtWidgets import QApplication

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = OBBGLWidget('assets/bunny.obj')
    window.show()
    sys.exit(app.exec_())