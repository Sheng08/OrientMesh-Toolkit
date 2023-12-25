# OrientMesh-Toolkit: Extracting and Visualizing Oriented Bounding Boxes from 3D Triangular Meshes

## Basic Information

> **Project Overview:** Extracting and Visualizing Oriented Bounding Boxes (OBB)
> from Triangular Meshes using C++ and Python

Triangular meshes[^1] stand as a cornerstone in computer graphics, essential in
the portrayal and animation of 3D objects. A critical aspect of processing
these meshes involves extracting **Oriented Bounding Boxes (OBB)**[^2].
_**OrientMesh Toolkit**_ presents an optimal solution to this task, fusing
efficient algorithms with an interactive visual interface. This combination
not only simplifies OBB extraction but also enhances user comprehension
through vivid visual representations.

- 📂 **GitHub Repository**: [https://github.com/Sheng08/OrientMesh-Toolkit](https://github.com/Sheng08/OrientMesh-Toolkit)
- 💻 **Implemented In**:
  - **Core Library**: C++
  - **Bindings & Interface(GUI)**: Python

## Problem to Solve

Triangular meshes, particularly when it comes to intricate surface detailing,
can be both complex and computationally intensive. Operations on these meshes
often demand significant resources. Having a precise oriented bounding box aids
in simplifying many computational geometry problems related to surface meshes,
like collision detection, mesh simplification, and many more. However,
generating an optimal OBB for surface meshes is a non-trivial task. OrientMesh
aims to offer a reliable, optimized, and easily-integrable solution for this
challenge.

<p align="center">
    <img src="./doc/pic/OOB_mesh_sample1.png" alt="" height="250"style="margin-right: 15px;"/>
    <img src="./doc/pic/OOB_mesh_sample2.png" alt="" height="250"/>
</p>

## Prospective Users

_**OrientMesh Toolkit**_ targets game developers, computer science students,
researchers, and professionals working with 3D models, especially in areas
requiring accurate bounding boxes like game development, simulation, and
augmented reality.

## System Architecture

_**OrientMesh Toolkit**_ utilizes C++ for its core computational tasks,
ensuring performance optimization. Moreover, Python is employed for its
bindings, making it seamlessly usable for a wider audience and also aiding
in GUI development and design.

**Components**:

- **Triangular Mesh Data Structures (C++)**: This component serves as the
  foundation, hosting the mesh structures essential for subsequent operations.
- **Algorithm for Optimal OBB Extraction (C++)**: A dedicated component that
  undertakes the crucial task of extracting the Oriented Bounding Boxes from
  the meshes.
- **API (C++ and Python)**: Interface that facilitates seamless communication
  between different modules and allows extensibility for future needs.
- **Python Bindings Using `pybind11`**: This bridges the gap between C++
  computations and Python's versatility, opening up the toolkit for a wider
  user base.
- **Interactive GUI (Python `PyQt`)**: A visual platform enabling users to
  visualize the mesh and extracted OBB interactively.

**Mesh data structure**:

```cpp
struct Vertex {
    glm::vec3 coordinates;
    glm::vec3 normal;       // Optional
};

struct Edge {
    Vertex* start;
    Vertex* end;
    std::vector<Face*> faces;
};

struct Face {
    Vertex* vertices[3];
    Edge* edges[3];
    glm::vec3 normal;       // This will facilitate operations that require face normals.
};

class MeshData {
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Face> faces;

    // Additional functions for manipulating and querying the mesh
};
```

**System Flow Description**:

In the OrientMesh workflow, everything begins with the user providing 3D object
data in the form of a `.obj` file, which encapsulates information about vertices,
edges, and faces. Initially, this data is ingested and stored by the
`Mesh` class into specialized C++ data structures. Throughout this
process, we leverage both the `glm` library (OpenGL Mathematics[^3]) for
efficient handling of 3D data, and the `Eigen` library[^4] for advanced linear
algebra operations, ensuring both robustness and efficiency.

After that, the parsed data from the `Mesh` class is passed onto the
`BoundingBox` class. Here, utilizing methodologies such as Principal Component
Analysis (PCA)[^5], the OBB is computed from the
provided mesh data more efficiently. The `BoundingBox` assesses the eigen
vectors of the object's distribution to determine a 3D box that minimally
encapsulates the object.

Upon completion of the OBB extraction, all data is amalgamated through the
`OOBInterface` class. Harnessing the power of pybind11 and PyQt, marrying the
efficiency of C++ with the flexibility of Python, this interface acts as a
bridge between C++ and Python, facilitating users to conveniently access and
utilize OBB data and its associated operations.

<p align="center">
    <img src="./doc/pic/system_flow.png" alt="" width="400"/>
</p>

## Algorithm

### 3D Mesh Oriented Bounding Box (OBB) using PCA

1. **Extract vertex coordinates from the mesh**:

  ```math
  \mathbf{p}_i, \quad i = 1, 2, \ldots, N
  ```

2. **Compute the mean of the vertex coordinates**:

  ```math
  \mathbf{m} = \frac{1}{N} \sum_{i=1}^{N} \mathbf{p}_i
  ```

3. **Center the vertex coordinates**:

  ```math
  \mathbf{q}_i = \mathbf{p}_i - \mathbf{m}
  ```

4. **Compute the covariance matrix**:

  ```math
  \mathbf{C} = \frac{1}{N} \sum_{i=1}^{N} \mathbf{q}_i \mathbf{q}_i^T
  ```

5. **Decompose the covariance matrix to get the principal directions**:

  ```math
  \mathbf{C} \mathbf{v}_j = \lambda_j \mathbf{v}_j
  ```

  <p align="center">
    where $\mathbf{v}_j$ are the eigenvectors (principal directions) and
    $lambda_j$ are the eigenvalues of $\mathbf{C}$.
  </p>

6. **Project vertices onto the principal directions to get the dimensions of the OBB**:

  ```math
  D_j = \max_{i} (\mathbf{p}_i \cdot \mathbf{v}_j) - \min_{i} (\mathbf{p}_i \cdot \mathbf{v}_j), \quad j = 1, 2, 3
  ```

  <p align="center">
   where $D_1, D_2, D_3$ represent the dimensions ($L, W, H$ ) respectively.
  </p>

### Build the Environment

* To install the on Ubuntu/Debian like linux operating systems
```bash
sudo apt-get update
sudo apt-get install -y build-essential make libeigen3-dev freeglut3-dev python3 python3-pip python3-pytest python3-numpy
```

* Install the Python dependencies
```bash
pip3 install -r python/requirements.txt
```

* Build the project
```bash
make
```

### How to use
* Test the project
```
make test
```
* UI Demo the project
```
make demo
```

## References

1. [Oriented Bounding Boxes in VEX](https://www.andynicholas.com/post/oriented-bounding-boxes-in-vex)
2. [cpm-glm-aabb](http://iauns.github.io/cpm-glm-aabb/class_c_p_m___g_l_m___a_a_b_b___n_s_1_1_a_a_b_b.html)
3. [Trimesh](https://trimesh.org/)
4. [PCA-OBB (GitHub Repository)](https://github.com/chuxu1793/PCA-OBB)
5. [SIMD-OBB (GitHub Repository)](https://github.com/taqu/OBB)
7. Chia-Tche Chang, Bastien Gorissen, and Samuel Melchior. 2011. Fast oriented bounding box optimization on the rotation group SO(3,ℝ). ACM Trans. Graph. 30, 5, Article 122 (October 2011), 16 pages. <https://doi.org/10.1145/2019627.2019641>
8. Vitsas, N., Evangelou, I., Papaioannou, G. and Gkaravelis, A. (2023), Parallel Transformation of Bounding Volume Hierarchies into Oriented Bounding Box Trees. Computer Graphics Forum, 42: 245-254. <https://doi.org/10.1111/cgf.14758>

[^1]: [Triangle mesh](https://en.wikipedia.org/wiki/Triangle_mesh)
[^2]: [Oriented Bounding Boxes on Wikipedia](https://en.wikipedia.org/wiki/Oriented_bounding_box)
[^3]: [OpenGL Programming/Bounding box](https://en.wikibooks.org/wiki/OpenGL_Programming/Bounding_box)
[^4]: [Eigen OBB (Library Source Code)](https://github.com/ForeverDavid/3d-workspace/blob/master/MathLibrary/Bounding/OBB.h)
[^5]: [Principal Component Analysis (PCA)](https://en.wikipedia.org/wiki/Principal_component_analysis)
