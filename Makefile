# 變數
CXX = g++
CXXFLAGS = -lGL -lGLU -lglut
OBJDIR = obj/
SRC = $(filter-out main.cpp, $(wildcard *.cpp))
OBJ = $(patsubst %.cpp,$(OBJDIR)%.o,$(SRC))

# 創建 obj 資料夾
$(shell mkdir -p $(OBJDIR))

# 目標
main: main.o $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# 特殊規則
$(OBJDIR)main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

# 一般規則
$(OBJDIR)%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

# 清理
clean:
	rm -rf $(OBJDIR) *.o main
