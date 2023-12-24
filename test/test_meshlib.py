import _meshlib

# obj_file_path = "assets/bunny.obj"

# # Call the function to read the OBJ file and calculate the axis-aligned bounding box
# bbox = _meshlib.load_and_compute_bbox(obj_file_path)

# # Get the minimum and maximum values ​​of the bounding box
# min_val = bbox.min
# max_val = bbox.max

# # Print minimum and maximum values
# print("Min:", min_val)
# print("Max:", max_val)

# Create Mesh object
mesh = _meshlib.Mesh()

# Read mesh from file
mesh.read("assets/bunny.obj")

# Print mesh data
# print(mesh.vertices)
# print(mesh.faces)

#############################################

# Create BoundingBox object
bbox = _meshlib.BoundingBox()

# Compute BoundingBox
bbox.computeAxisAlignedBox(mesh.vertices)

# Print BoundingBox data
print(f"(Python) Mix: {bbox.min}")
print(f"(Python) Max: {bbox.max}")