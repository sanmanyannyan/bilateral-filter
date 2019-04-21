CXX=g++
CXXFLAGS=-O3 -march=native
LIBRARY_PATH=-L/usr/local/lib
LIBRARY=-lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
INCLUDE=-I/usr/local/include/opencv4

run:bench
	./bench
bench:bench.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBRARY_PATH) $< -o $@ $(LIBRARY)

.PHONY:clean run
clean:
	rm -f bench


