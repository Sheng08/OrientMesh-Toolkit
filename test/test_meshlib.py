#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
from utils import bcolors
import _meshlib

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

## Initialize MeshLib

obj_file_path = "assets/bunny.obj"

# Create Mesh object
mesh = _meshlib.Mesh()
mesh.read(obj_file_path)

# Print mesh data
# print(mesh.vertices)
# print(mesh.faces)

# Create BoundingBox object
bbox = _meshlib.BoundingBox()


## Compute AxisAlignedBox
print(f"\n{bcolors.OKGREEN}{bcolors.BOLD}Compute AxisAlignedBox{bcolors.ENDC}")
print("=" * 15 + " Use Pybind Module (computeAxisAlignedBox) " + "=" * 15)
bbox.computeAxisAlignedBox(mesh.vertices)
print(f"(Python) Mix: {bbox.min}")
print(f"(Python) Max: {bbox.max}")


print(f"\n{'=' * 15} Use Integrated Pybind Module (load_and_compute_axis_aligned_box) {'=' * 15}")
# Call the function to read the OBJ file and calculate the axis-aligned bounding box
bbox = _meshlib.load_and_compute_axis_aligned_box(obj_file_path)
min_val = bbox.min
max_val = bbox.max
print(f"(Python) Min: {min_val}")
print(f"(Python) Max: {max_val}")


## Compute OrientedBox
print(f"\n{bcolors.OKGREEN}{bcolors.BOLD}Compute OrientedBox{bcolors.ENDC}")
print("=" * 15 + " Use Pybind Module (computeOrientedBox) " + "=" * 15)
bbox.computeOrientedBox(mesh.vertices)
print(f"(Python) Oriented Points Xmin: {[round(coord, 8) for coord in bbox.orientedPoints[0]]}")
print(f"(Python) Oriented Points Xmax: {[round(coord, 8) for coord in bbox.orientedPoints[1]]}")
print(f"(Python) Oriented Points Ymin: {[round(coord, 8) for coord in bbox.orientedPoints[2]]}")
print(f"(Python) Oriented Points Ymax: {[round(coord, 8) for coord in bbox.orientedPoints[3]]}")
print(f"(Python) Oriented Points Zmin: {[round(coord, 8) for coord in bbox.orientedPoints[4]]}")
print(f"(Python) Oriented Points Zmax: {[round(coord, 8) for coord in bbox.orientedPoints[5]]}")
# for idx, point in enumerate(bbox.orientedPoints):
#     print(f"(Python) Oriented Points: {point.tolist()}")


print(f"\n{'=' * 15} Use Integrated Pybind Module (load_and_compute_oriented_box) {'=' * 15}")
# Call the function to read the OBJ file and calculate the oriented bounding box
bbox = _meshlib.load_and_compute_oriented_box(obj_file_path)
orientedPoints = bbox.orientedPoints
print(f"(Python) Oriented Points Xmin: {[round(coord, 8) for coord in orientedPoints[0]]}")
print(f"(Python) Oriented Points Xmax: {[round(coord, 8) for coord in orientedPoints[1]]}")
print(f"(Python) Oriented Points Ymin: {[round(coord, 8) for coord in orientedPoints[2]]}")
print(f"(Python) Oriented Points Ymax: {[round(coord, 8) for coord in orientedPoints[3]]}")
print(f"(Python) Oriented Points Zmin: {[round(coord, 8) for coord in orientedPoints[4]]}")
print(f"(Python) Oriented Points Zmax: {[round(coord, 8) for coord in orientedPoints[5]]}")
# for idx, point in enumerate(orientedPoints):
#     print(f"(Python) )riented Points: {point.tolist()}")