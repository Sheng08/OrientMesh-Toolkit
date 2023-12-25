import sys
sys.path.append('.')

import _meshlib
import numpy as np
import trimesh
import pytest
from python.obb import BoundingBox

obj_file_path = "assets/bunny.obj"

def load_obj(filename):
    mesh = trimesh.load(filename)
    return np.asarray(mesh.vertices)

@pytest.fixture
def sample_mesh_vertices():
    return load_obj(obj_file_path)

def test_axis_aligned_box(sample_mesh_vertices):
    # Python Side
    bbox_py = BoundingBox()
    bbox_py.compute_axis_aligned_box(sample_mesh_vertices)

    # C++(Pybind) Side
    bbox_cpp = _meshlib.load_and_compute_axis_aligned_box(obj_file_path)

    np.testing.assert_almost_equal(bbox_py.min, bbox_cpp.min)
    np.testing.assert_almost_equal(bbox_py.max, bbox_cpp.max)
    np.testing.assert_almost_equal(bbox_py.extent, bbox_cpp.extent)

def test_oriented_box(sample_mesh_vertices):
    # Python Side
    bbox_py = BoundingBox()
    bbox_py.compute_oriented_box(sample_mesh_vertices)

    # C++(Pybind) Side
    bbox_cpp = _meshlib.load_and_compute_oriented_box(obj_file_path)

    for point_py, point_cpp in zip(bbox_py.oriented_points, bbox_cpp.orientedPoints):
        np.testing.assert_almost_equal(point_py, point_cpp)

def test_empty_bounding_box():
    # Python Side
    bbox_py = BoundingBox()

    # C++(Pybind) Side
    bbox_cpp = _meshlib.BoundingBox()

    np.testing.assert_almost_equal(bbox_cpp.min, bbox_py.min)
    np.testing.assert_almost_equal(bbox_cpp.max, bbox_py.max)
