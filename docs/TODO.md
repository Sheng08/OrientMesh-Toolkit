# TODO
撰寫安裝流程 必要套件等


sudo cp -r /usr/include/eigen3/Eigen  /usr/local/include

-I/usr/include/eigen3

Eigen 安裝？

sudo apt-get install libeigen3-dev
sudo apt-get install freeglut3-dev


Mesh 类:

vertices 和 faces 通常是可以从外部修改的，因为在3D处理程序中经常需要修改网格的顶点和面。
BoundingBox 类:

min, max, extent, orientedPoints 和 type 可以是可读写的，因为边界框的参数可能会根据对象的变换而变化。
Vertex 类:

position 和 index 很可能是可读写的，因为顶点的位置和索引在编辑网格时经常需要改变。
isIsolated 可能更适合为只读，因为它通常是由网格的拓扑结构决定的。
Face 类:

isBoundary, area 和 normal 可能更适合为只读，因为它们通常是由面的几何和拓扑属性决定的。
HalfEdge 类:

next, flip, vertex, edge, face 通常应该是只读的，因为这些都是拓扑结构的一部分，不应该随意修改。
uv, normal, onBoundary 可以是可读写的，特别是在涉及到UV映射和法线计算时。



# Install
# install: $(MODULE_SHARE_OBJS)
# 	@mkdir -p $(LIBDIR)
# 	mv $^ $(LIBDIR)


make -j4

 pip install PySide2