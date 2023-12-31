#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import _meshlib
from PySide2.QtWidgets import QApplication, QOpenGLWidget
from PySide2.QtGui import QOpenGLFunctions
from PySide2.QtCore import Qt, QPoint
from OpenGL.GL import *
from OpenGL.GLU import *


class OBBGLWidget(QOpenGLWidget, QOpenGLFunctions):
    def __init__(self, mesh_file, parent=None):
        super(OBBGLWidget, self).__init__(parent)
        self.gl = None
        self.lastPos = QPoint()
        self.rotationX = 0.0
        self.rotationY = 0.0
        self.translationX = 0.0
        self.translationY = 0.0
        self.zoomFactor = 0.5
        self.mesh_file = mesh_file
        self.mesh = _meshlib.Mesh()
        self.bbox = None
        self.x, self.y, self.z = 0, 0, -2.5
        self.fovy, self.clipNear, self.clipFar = 50., 0.01, 1000.
        self.drawAABB = True
        self.window().setWindowTitle("Axis-Aligned Bounding Box")
        self.loadMesh()
        self.initializeView()


    def initializeGL(self):
        self.gl = QOpenGLFunctions(self.context())
        self.gl.initializeOpenGLFunctions()
        self.gl.glClearColor(0, 0, 0, 1)

    def initializeView(self):
        self.rotationX = 0.0
        self.rotationY = 0.0
        self.translationX = 0.0
        self.translationY = 0.0
        self.zoomFactor = 1.0  # Reset zoom factor

    def resizeGL(self, w, h):
        aspect = w / h
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(self.fovy, aspect, self.clipNear, self.clipFar)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        # gluLookAt(觀察者位置, 觀察點位置, 向上向量)
        gluLookAt(0, 0, self.z, self.x, self.y, 0, 0, 1, 0)

        # glTranslatef(self.x, self.y, self.z)
        glRotatef(self.rotationX, 1.0, 0.0, 0.0)
        glRotatef(self.rotationY, 0.0, 1.0, 0.0)

        glScalef(self.zoomFactor, self.zoomFactor, self.zoomFactor)


        self.drawFaces()
        self.drawBoundingBox()
        self.drawCoordinateAxes()

    def loadMesh(self):
        self.mesh.read(self.mesh_file)
        self.computeBoundingBox()

    def computeBoundingBox(self):
        if self.drawAABB:
            self.bbox = _meshlib.load_and_compute_axis_aligned_box(
                self.mesh_file)
        else:
            self.bbox = _meshlib.load_and_compute_oriented_box(self.mesh_file)

    def drawCoordinateAxes(self):
        axisLength = 10.0  # Adjust this for a sufficiently long axis
        arrowLength = 0.2 * axisLength  # Length of the arrowhead
        arrowWidth = 0.05 * axisLength  # Width of the arrowhead base

        glLineWidth(2.0)  # Set line width

        # Begin drawing lines
        glBegin(GL_LINES)

        # X Axis in red
        glColor3f(1.0, 0.0, 0.0)  # Red color
        glVertex3f(0.0, 0.0, 0.0)
        glVertex3f(axisLength, 0.0, 0.0)

        # Arrowhead for X Axis
        glVertex3f(axisLength, 0.0, 0.0)
        glVertex3f(axisLength - arrowLength, arrowWidth, 0.0)
        glVertex3f(axisLength, 0.0, 0.0)
        glVertex3f(axisLength - arrowLength, -arrowWidth, 0.0)

        # Y Axis in green
        glColor3f(0.0, 1.0, 0.0)  # Green color
        glVertex3f(0.0, 0.0, 0.0)
        glVertex3f(0.0, axisLength, 0.0)

        # Arrowhead for Y Axis
        glVertex3f(0.0, axisLength, 0.0)
        glVertex3f(arrowWidth, axisLength - arrowLength, 0.0)
        glVertex3f(0.0, axisLength, 0.0)
        glVertex3f(-arrowWidth, axisLength - arrowLength, 0.0)

        # Z Axis in blue
        glColor3f(0.0, 0.0, 1.0)  # Blue color
        glVertex3f(0.0, 0.0, 0.0)
        glVertex3f(0.0, 0.0, axisLength)

        # Arrowhead for Z Axis
        glVertex3f(0.0, 0.0, axisLength)
        glVertex3f(0.0, arrowWidth, axisLength - arrowLength)
        glVertex3f(0.0, 0.0, axisLength)
        glVertex3f(0.0, -arrowWidth, axisLength - arrowLength)

        glEnd()

        glLineWidth(1.0)

    def drawBox(self, b1, b2, b3, b4, b5, b6, b7, b8):
        glColor4f(1.0, 1.0, 1.0, 0.6)

        glBegin(GL_LINE_LOOP)
        glVertex3d(b1[0], b1[1], b1[2])
        glVertex3d(b2[0], b2[1], b2[2])
        glVertex3d(b3[0], b3[1], b3[2])
        glVertex3d(b4[0], b4[1], b4[2])
        glEnd()

        glBegin(GL_LINE_LOOP)
        glVertex3d(b5[0], b5[1], b5[2])
        glVertex3d(b6[0], b6[1], b6[2])
        glVertex3d(b7[0], b7[1], b7[2])
        glVertex3d(b8[0], b8[1], b8[2])
        glEnd()

        glBegin(GL_LINES)
        glVertex3d(b1[0], b1[1], b1[2])
        glVertex3d(b5[0], b5[1], b5[2])

        glVertex3d(b2[0], b2[1], b2[2])
        glVertex3d(b6[0], b6[1], b6[2])

        glVertex3d(b3[0], b3[1], b3[2])
        glVertex3d(b7[0], b7[1], b7[2])

        glVertex3d(b4[0], b4[1], b4[2])
        glVertex3d(b8[0], b8[1], b8[2])
        glEnd()

    def drawFaces(self):
        glColor4f(0.0, 0.0, 1.0, 0.6)  # Set color for the faces

        for face in self.mesh.faces:
            if face.isBoundary:  # Skip boundary faces if any
                continue

            glBegin(GL_LINE_LOOP)  # Start drawing a face

            half_edge = face.he   # Get the starting half edge of the face
            start_he = half_edge  # Remember the start to detect loop end

            while True:
                glVertex3d(half_edge.vertex.position[0],
                           half_edge.vertex.position[1],
                           half_edge.vertex.position[2])

                half_edge = half_edge.next  # Move to the next half edge
                if half_edge == start_he:
                    break

            glEnd()

    def drawBoundingBox(self):
        if self.drawAABB:
            # Axis-Aligned Bounding Box
            min_point = self.bbox.min
            max_point = self.bbox.max
            extent = self.bbox.extent

            # Calculate the vertices of the bounding box
            b2 = (min_point[0] + extent[0], min_point[1], min_point[2])
            b3 = (min_point[0] + extent[0],
                  min_point[1] + extent[1], min_point[2])
            b4 = (min_point[0], min_point[1] + extent[1], min_point[2])
            b5 = (max_point[0] - extent[0],
                  max_point[1] - extent[1], max_point[2])
            b6 = (max_point[0], max_point[1] - extent[1], max_point[2])
            b8 = (max_point[0] - extent[0], max_point[1], max_point[2])

            # Draw the box
            self.drawBox(min_point, b2, b3, b4, b5, b6, max_point, b8)
        else:
            # Oriented Bounding Box
            oriented_points = self.bbox.orientedPoints

            # Calculate the vertices of the bounding box based on oriented points
            b1 = oriented_points[0] + oriented_points[2] + oriented_points[4]
            b2 = oriented_points[1] + oriented_points[2] + oriented_points[4]
            b3 = oriented_points[1] + oriented_points[2] + oriented_points[5]
            b4 = oriented_points[0] + oriented_points[2] + oriented_points[5]
            b5 = oriented_points[0] + oriented_points[3] + oriented_points[4]
            b6 = oriented_points[1] + oriented_points[3] + oriented_points[4]
            b7 = oriented_points[1] + oriented_points[3] + oriented_points[5]
            b8 = oriented_points[0] + oriented_points[3] + oriented_points[5]

            # Draw the box
            self.drawBox(b1, b2, b3, b4, b5, b6, b7, b8)

    def toggleMesh(self):
        # Toggle between different mesh states
        self.drawAABB = not self.drawAABB
        self.computeBoundingBox()

        # Update the window title
        if self.drawAABB:
            self.window().setWindowTitle("Axis-Aligned Bounding Box")
        else:
            self.window().setWindowTitle("Oriented Bounding Box")

        self.update()

    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_R:
            self.initializeView()
            self.update()
        elif key == Qt.Key_Escape:
            self.close()
        elif key == Qt.Key_Space:
            self.toggleMesh()
        elif key in (Qt.Key_B, Qt.Key_A, Qt.Key_D, Qt.Key_W, Qt.Key_S):
            self.adjustCamera(key)
        self.update()  # Trigger a repaint

    def adjustCamera(self, key):
        # Adjust camera position based on key press
        if key == Qt.Key_A:
            self.x += 0.03
        elif key == Qt.Key_D:
            self.x -= 0.03
        elif key == Qt.Key_W:
            self.y += 0.03
        elif key == Qt.Key_S:
            self.y -= 0.03

    def adjustZoom(self, key):
        # Adjust zoom level based on key press
        if key == Qt.Key_Up:
            self.z += 0.03
        elif key == Qt.Key_Down:
            self.z -= 0.03

    def wheelEvent(self, event):
        # Adjust zoom factor based on wheel delta
        self.zoomFactor += event.angleDelta().y() / 1200  # Adjust this value as needed
        self.zoomFactor = max(0.1, min(5.0, self.zoomFactor))  # Limit the zoom factor range

        self.update()

    def mousePressEvent(self, event):
        self.lastPos = event.pos()

    def mouseMoveEvent(self, event):
        dx = event.x() - self.lastPos.x()
        dy = event.y() - self.lastPos.y()

        if event.buttons() & Qt.LeftButton:
            self.rotateObject(dx, dy)
        elif event.buttons() & Qt.RightButton:
            self.translateObject(dx, dy)

        self.lastPos = event.pos()
        self.update()

    def rotateObject(self, dx, dy):
        # Update the rotation angle
        self.rotationX += dy * 0.3
        self.rotationY += dx * 0.3

    def translateObject(self, dx, dy):
        self.translationX += dx * 0.03
        self.translationY -= dy * 0.03


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = OBBGLWidget('assets/bunny.obj')
    window.show()
    sys.exit(app.exec_())
