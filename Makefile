TARGET 				:= bounding-box
PYBIND_SOURCE 		:= meshlib_pybind.cpp

CXX           		:= g++
CXXFLAGS      		:= -O3 -Wall -std=c++17 -fPIC
INCLUDES      		+= -Iinclude -I/usr/include/eigen3 -I$(MKLINCLUDE)
LIBS          		:= -lGL -lGLU -lglut
LDFLAGS       		:= -shared $(LIBS)

OBJDIR        		:= obj/
SRCDIR        		:= src/
LIBDIR 				:= lib/
BINDIR 				:= bin/
SRC           		:= $(filter-out $(SRCDIR)$(PYBIND_SOURCE), $(wildcard $(SRCDIR)*.cpp))
OBJ           		:= $(SRC:$(SRCDIR)%.cpp=$(OBJDIR)%.o)

MKLINCLUDE    		:= /usr/include/mkl
MKLFLAGS      		:= -lblas
PYTHON        		:= python3
PYINCLUDE     		:= $(shell python3-config --includes)
PYBINDINCLUDE 		:= $(shell python3 -m pybind11 --includes)
PYCONFIG      		:= $(shell python3-config --extension-suffix)
MODULE_SHARE_OBJS   := _meshlib${PYCONFIG}

INSTALLDIR = install

.PHONY: all clean demo test install
default: all

# Create obj directory
$(shell mkdir -p $(OBJDIR))

# Target
all: $(TARGET) $(MODULE_SHARE_OBJS)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BINDIR)$@ $^ $(LIBS)

# Python module
$(MODULE_SHARE_OBJS): $(SRC) $(SRCDIR)$(PYBIND_SOURCE)
	@mkdir -p $(LIBDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(PYINCLUDE) $(PYBINDINCLUDE) $^ -o $(LIBDIR)$@ $(MKLFLAGS) $(LDFLAGS)

# General rule
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) -c $< $(CXXFLAGS) $(INCLUDES) -o $@

# Python meshlib demo with UI
export PYTHONPATH=$PYTHONPATH:$(PWD)/$(LIBDIR)
demo: $(LIBDIR)$(MODULE_SHARE_OBJS)
	$(PYTHON) demo/demo_UI.py

# Python meshlib test
export PYTHONPATH=$PYTHONPATH:$(PWD)/$(LIBDIR)
test: $(LIBDIR)$(MODULE_SHARE_OBJS)
	$(PYTHON) test/test_meshlib.py

# Install
install: $(BINDIR)$(TARGET) $(LIBDIR)$(MODULE_SHARE_OBJS)
	@mkdir -p $(INSTALLDIR)/include
	@mkdir -p $(INSTALLDIR)/bin
	@mkdir -p $(INSTALLDIR)/lib
	@mkdir -p $(INSTALLDIR)/python
	@mkdir -p $(INSTALLDIR)/test
	cp $(BINDIR)$(TARGET) $(INSTALLDIR)/bin
	cp -r include/*.h $(INSTALLDIR)/include
	cp -r $(LIBDIR)* $(INSTALLDIR)/lib
	cp -r python/* $(INSTALLDIR)/python
	cp -r test/* $(INSTALLDIR)/test

# Clean
clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(BINDIR) $(INSTALLDIR) *.o $(TARGET)
	find . -name "__pycache__" -type d -exec rm -rf {} +
	find . -name "*.pyc" -type f -delete
	find . -name ".pytest_cache" -type d -exec rm -rf {} +
