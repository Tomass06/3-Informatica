TARGET = busqueda

CXX = g++
CXXFLAGS = -Wall -g

SRCS = main.cc node.cc
OBJS = $(SRCS:.cc=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)