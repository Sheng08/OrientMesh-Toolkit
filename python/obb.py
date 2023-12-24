import numpy as np
import trimesh

class BoundingBox:
    def __init__(self, points=None):
        if points is None:
            self.min = np.array([0.] * 3)
            self.max = np.array([0.] * 3)
        else:
            self.min = np.min(points, axis=0)
            self.max = np.max(points, axis=0)
        self.extent = self.max - self.min
        self.type = None
        self.oriented_points = []

    def load_obj(self, filename):
        mesh = trimesh.load(filename)
        return mesh.vertices

    def expand_to_include(self, point):
        self.min = np.minimum(self.min, point)
        self.max = np.maximum(self.max, point)
        self.extent = self.max - self.min

    def compute_axis_aligned_box(self, vertices):
        self.type = "Axis Aligned"
        for v in vertices:
            self.expand_to_include(v)

    def compute_oriented_box(self, vertices):
        self.type = "Oriented"
        self.orientedPoints = []

        # Compute the mean of the vertices
        center = np.mean(vertices, axis=0)

        # Adjust vertices based on the mean and compute covariance matrix
        adjusted_vertices = vertices - center
        covariance = np.dot(adjusted_vertices.T, adjusted_vertices) / len(vertices)
        #  covariance = np.cov(np.array(adjusted_positions).T)

        # Perform PCA - compute the eigenvectors
        eigenvalues, eigenvectors = np.linalg.eigh(covariance)
        # Adjust the direction of eigenvectors if necessary
        # For example, if the first and third eigenvectors need to be flipped:
        eigenvectors[:, [0, 2]] = -eigenvectors[:, [0, 2]]


        # Project points on each principal axis and find min and max
        min_proj = np.full(3, np.inf)
        max_proj = np.full(3, -np.inf)
        for v in adjusted_vertices:
            projections = eigenvectors.T.dot(v)
            min_proj = np.minimum(min_proj, projections)
            max_proj = np.maximum(max_proj, projections)

        # Calculate oriented bounding box points
        self.oriented_points = []
        for i in range(3):
            point_min = eigenvectors[:, i] * min_proj[i] + center
            point_max = eigenvectors[:, i] * max_proj[i] + center
            self.oriented_points.extend([point_min, point_max])

    def print_axis_aligned_box(self):
        print(f"Axis Aligned Box Min: {self.min}")
        print(f"Axis Aligned Box Max: {self.max}")
        print(f"Axis Aligned Box Extent: {self.extent}")

    def print_oriented_points(self):
        labels = ['Xmin', 'Xmax', 'Ymin', 'Ymax', 'Zmin', 'Zmax']
        for i, p in enumerate(self.oriented_points):
            print(f"Oriented Point {labels[i]}: [{p[0]:.8f}, {p[1]:.8f}, {p[2]:.8f}]")



if __name__ == "__main__":
    filename = 'assets/bunny.obj'
    bbox = BoundingBox()
    vertices = bbox.load_obj(filename)

    bbox.compute_axis_aligned_box(vertices)
    bbox.print_axis_aligned_box()

    bbox.compute_oriented_box(vertices)
    bbox.print_oriented_points()
