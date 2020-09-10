PROJ = grapher.out
CXX = g++
CXXFLAGS = `wx-config --libs --cxxflags` -Wall -w
OBJS = wxGraph.o Calculate.o

$(PROJ): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

wxGraph.o: wxGraph.cpp
	$(CXX) $(CXXFLAGS) -c $<

Calculate.o: Calculate.cpp Calculate.h
	$(CXX) $(CXXFLAGS) -c $<

# -f removes any error messages if the file is not present
.PHONY: clean run valgrind finish
clean:
	rm -rf $(OBJS)
	rm -f $(PROJ)
	rm -f *~ *.h.gch *#
run: $(PROJ)
	./$(PROJ) $(FILE)
valgrind: $(PROJ)
	valgrind ./$(PROJ)
finish:clean
	rm –rf -f proj.zip
	zip proj.zip ./*