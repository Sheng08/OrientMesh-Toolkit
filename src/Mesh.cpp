#include "Mesh.h"

#include "MeshIO.h"

/**
 * Default constructor for Mesh.
 * Initializes an empty Mesh object.
 */
Mesh::Mesh() {
}

/**
 * Copy constructor for Mesh.
 * Creates a new Mesh object as a copy of an existing Mesh.
 * Parameter:
 *     mesh: const Mesh&, reference to the Mesh object to be copied.
 */
Mesh::Mesh(const Mesh& mesh) {
    *this = mesh;
}

/**
 * Functionality: Reads mesh data from a file and normalizes the mesh.
 * Parameter:
 *     fileName: const std::string&, the name of the file to read from.
 * Returns:
 *     bool: Returns true if the file is successfully read, false otherwise.
 */
bool Mesh::read(const std::string& fileName) {
    std::ifstream in(fileName.c_str());

    if (!in.is_open()) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return false;
    }

    bool readSuccessful = false;
    if ((readSuccessful = MeshIO::read(in, *this))) {
        normalize();
    }

    return readSuccessful;
}

/**
 * Functionality: Writes mesh data to a file.
 * Parameter:
 *     fileName: const std::string&, the name of the file to write to.
 * Returns:
 *     bool: Returns false as the method currently does not implement successful write checking.
 */
bool Mesh::write(const std::string& fileName) {
    std::ofstream out(fileName.c_str());

    if (!out.is_open()) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return false;
    }

    MeshIO::write(out, *this);

    return false;
}

/**
 * Functionality: Normalizes the mesh to fit within a unit sphere centered at the origin.
 * This method computes the center of mass of the mesh, translates the mesh to the origin, and rescales it.
 */
void Mesh::normalize() {
    // compute center of mass
    Eigen::Vector3d cm = Eigen::Vector3d::Zero();
    for (VertexCIter v = vertices.begin(); v != vertices.end(); v++) {
        cm += v->position;
    }
    cm /= (double)vertices.size();

    // translate to origin
    for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
        v->position -= cm;
    }

    // determine radius
    double rMax = 0;
    for (VertexCIter v = vertices.begin(); v != vertices.end(); v++) {
        rMax = std::max(rMax, v->position.norm());
    }

    // rescale to unit sphere
    for (VertexIter v = vertices.begin(); v != vertices.end(); v++) {
        v->position /= rMax;
    }
}
