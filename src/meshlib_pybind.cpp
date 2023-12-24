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
        .def(py::init<const Mesh&>())
        .def("read", &Mesh::read)
        .def("write", &Mesh::write)
        .def_readwrite("halfEdges", &Mesh::halfEdges)
        .def_readwrite("vertices", &Mesh::vertices)
        .def_readwrite("uvs", &Mesh::uvs)
        .def_readwrite("edges", &Mesh::edges)
        .def_readwrite("faces", &Mesh::faces)
        .def_readwrite("boundaries", &Mesh::boundaries);

    py::class_<BoundingBox>(mod, "BoundingBox")
        .def(py::init<>())
        .def(py::init<const Eigen::Vector3d&, const Eigen::Vector3d&>())
        .def(py::init<const Eigen::Vector3d&>())
        .def("expandToInclude", (void (BoundingBox::*)(const Eigen::Vector3d&)) &BoundingBox::expandToInclude)
        .def("expandToInclude", (void (BoundingBox::*)(const BoundingBox&)) &BoundingBox::expandToInclude)
        .def("maxDimension", &BoundingBox::maxDimension)
        .def("contains", &BoundingBox::contains)
        .def("computeAxisAlignedBox", &BoundingBox::computeAxisAlignedBox)
        .def("computeOrientedBox", &BoundingBox::computeOrientedBox)
        .def_readwrite("min", &BoundingBox::min)
        .def_readwrite("max", &BoundingBox::max)
        .def_readwrite("extent", &BoundingBox::extent)
        .def_readwrite("orientedPoints", &BoundingBox::orientedPoints)
        .def_readwrite("type", &BoundingBox::type);

    py::class_<Vertex>(mod, "Vertex")
        .def(py::init<>())
        .def_property_readonly("he", [](const Vertex &v) { return &(*v.he); }, py::return_value_policy::reference_internal)
        .def_readwrite("position", &Vertex::position)
        .def_readwrite("index", &Vertex::index)
        .def_property_readonly("isIsolated", &Vertex::isIsolated);

    py::class_<Face>(mod, "Face")
        .def(py::init<>())
        .def_property_readonly("isBoundary", &Face::isBoundary)
        .def_property_readonly("area", &Face::area)
        .def_property_readonly("normal", &Face::normal)
        .def_property_readonly("he", [](const Face& f) { return &(*f.he);}, py::return_value_policy::reference_internal);

    py::class_<Edge>(mod, "Edge")
        .def(py::init<>())
        .def_property_readonly("halfEdge", [](const Edge &e) { return &(*e.he); }, py::return_value_policy::reference_internal)
        .def_property_readonly("length", &Edge::length);

    py::class_<HalfEdge>(mod, "HalfEdge")
        .def(py::init<>())
        .def_property_readonly("next", [](HalfEdge &he) { return &(*he.next); }, py::return_value_policy::reference_internal)
        .def_property_readonly("flip", [](HalfEdge &he) { return &(*he.flip); }, py::return_value_policy::reference_internal)
        .def_property_readonly("vertex", [](HalfEdge &he) { return &(*he.vertex); }, py::return_value_policy::reference_internal)
        .def_property_readonly("edge", [](HalfEdge &he) { return &(*he.edge); }, py::return_value_policy::reference_internal)
        .def_property_readonly("face", [](HalfEdge &he) { return &(*he.face); }, py::return_value_policy::reference_internal)
        .def_readwrite("uv", &HalfEdge::uv)
        .def_readwrite("normal", &HalfEdge::normal)
        .def_readonly("onBoundary", &HalfEdge::onBoundary);

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