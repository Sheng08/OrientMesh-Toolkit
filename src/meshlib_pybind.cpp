#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include "Mesh.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include "Face.h"
#include "HalfEdge.h"

namespace py = pybind11;

PYBIND11_MODULE(_meshlib, mod) {
    mod.doc() = "Meshlib Python Bindings";

    py::class_<Mesh>(mod, "Mesh")
        .def(py::init<>())
        .def("read", &Mesh::read)
        .def("write", &Mesh::write)
        .def_readwrite("vertices", &Mesh::vertices)
        .def_readwrite("faces", &Mesh::faces);

    py::class_<BoundingBox>(mod, "BoundingBox")
        .def(py::init<>())
        .def("computeAxisAlignedBox", &BoundingBox::computeAxisAlignedBox)
        .def("computeOrientedBox", &BoundingBox::computeOrientedBox)
        .def_readwrite("min", &BoundingBox::min)
        .def_readwrite("max", &BoundingBox::max)
        .def_readwrite("extent", &BoundingBox::extent)
        .def_readwrite("orientedPoints", &BoundingBox::orientedPoints)
        .def_readwrite("type", &BoundingBox::type);

    py::class_<Vertex>(mod, "Vertex")
        .def(py::init<>())
        .def_readwrite("position", &Vertex::position)
        .def_readwrite("index", &Vertex::index)
        .def("isIsolated", &Vertex::isIsolated);

    py::class_<Face>(mod, "Face")
        .def(py::init<>())
        .def("isBoundary", &Face::isBoundary)
        .def("area", &Face::area)
        .def("normal", &Face::normal);

    py::class_<HalfEdge>(mod, "HalfEdge")
        .def(py::init<>())
        .def_readonly("next", &HalfEdge::next)
        .def_readonly("flip", &HalfEdge::flip)
        .def_readonly("vertex", &HalfEdge::vertex)
        .def_readonly("edge", &HalfEdge::edge)
        .def_readonly("face", &HalfEdge::face)
        .def_readwrite("uv", &HalfEdge::uv)
        .def_readwrite("normal", &HalfEdge::normal)
        .def_readwrite("onBoundary", &HalfEdge::onBoundary);

    mod.def("load_and_compute_axis_aligned_box", [](const std::string& filename) {
        Mesh mesh;
        mesh.read(filename);
        BoundingBox bbox;
        bbox.computeAxisAlignedBox(mesh.vertices);
        return bbox;
    });

    mod.def("load_and_compute_oriented_box", [](const std::string& filename) {
        Mesh mesh;
        mesh.read(filename);
        BoundingBox bbox;
        bbox.computeOrientedBox(mesh.vertices);
        return bbox;
    });
}