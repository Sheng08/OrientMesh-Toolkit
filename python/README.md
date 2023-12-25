# _meshlib API Documentation

## Mesh Class

`Mesh` is a class for representing and manipulating 3D meshes.

### Constructors

- `Mesh()`: Creates an empty `Mesh` object.
- `Mesh(const Mesh&)`: Copy constructor, creates a new object identical to the given `Mesh` object.

### Methods

- `read(filename: str)`: Reads mesh data from the given filename.
- `write(filename: str)`: Writes mesh data to the given filename.

### Properties

- `halfEdges`: The list of half-edges of the mesh.
- `vertices`: The list of vertices of the mesh.
- `uvs`: The list of UV coordinates of the mesh.
- `edges`: The list of edges of the mesh.
- `faces`: The list of faces of the mesh.
- `boundaries`: The list of boundaries of the mesh.

## BoundingBox Class

`BoundingBox` is a class for representing and manipulating 3D bounding boxes.

### Constructors

- `BoundingBox()`: Creates an empty `BoundingBox` object.
- `BoundingBox(const BoundingBox&)`: Copy constructor, creates a new object identical to the given `BoundingBox` object.

### Methods

- `expandToInclude(point: np.ndarray)`: Expands the bounding box to include the given point.
- `computeAxisAlignedBox(vertices: np.ndarray)`: Computes the axis-aligned bounding box of the given vertices.
- `computeOrientedBox()`: Computes the oriented bounding box of the `Mesh` object it is called on. The bounding box is returned as a `BoundingBox` object.
- `load_and_compute_axis_aligned_box(filename: str)`: This function loads a 3D mesh from the given filename and computes its axis-aligned bounding box. The bounding box is returned as a `BoundingBox` object.
- `load_and_compute_oriented_box(filename: str)`: This function loads a 3D mesh from the given filename and computes its oriented bounding box. The bounding box is returned as a `BoundingBox` object.

### Properties

- `min`: The minimum point of the bounding box.
- `max`: The maximum point of the bounding box.
- `extent`: The extent of the bounding box.
