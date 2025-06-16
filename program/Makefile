CXX := g++
CXX_FLAGS := --std=c++17

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
PARS_DIR := $(SRC_DIR)/parser
BLOCKS_DIR := $(SRC_DIR)/building_blocks
TIME_TESTS_DIR := time_tests









objects := $(OBJ_DIR)/ColorMoves.o\
		   $(OBJ_DIR)/SeifertMoves.o\
		   $(OBJ_DIR)/PD_X.o\
		   $(OBJ_DIR)/Diagram.o\
		   $(OBJ_DIR)/Crossing.o\
		   $(OBJ_DIR)/Edge.o\
		   $(OBJ_DIR)/Color.o\
		   $(OBJ_DIR)/CrossingsCollection.o\
		   $(OBJ_DIR)/EdgesCollection.o\
		   $(OBJ_DIR)/ColorsCollection.o\
		   $(OBJ_DIR)/Graph.o



all: $(OBJ_DIR) $(BIN_DIR) $(objects)
	$(CXX) $(CXX_FLAGS) -o $(BIN_DIR)/main $(objects) $(SRC_DIR)/main.cpp

#dummy command
# $(OBJ_DIR)/dummy_main.o: $(objects) $(SRC_DIR)/dummy_main.cpp
# 	$(CXX) $(CXX_FLAGS) -c $(SRC_DIR)/dummy_main.cpp -o $@

$(TIME_TESTS_DIR)/measure_braiding_only.out: $(OBJ_DIR) $(BIN_DIR) $(objects) $(TIME_TESTS_DIR)/measure_braiding_only.cpp
	$(CXX) $(CXX_FLAGS) -o $(TIME_TESTS_DIR)/measure_braiding_only.out $(objects) $(TIME_TESTS_DIR)/measure_braiding_only.cpp


#MAIN BUILDING BLOCKS
$(OBJ_DIR)/Graph.o: $(BLOCKS_DIR)/Graph.cpp\
					 $(BLOCKS_DIR)/CrossingsCollection.h\
					 $(BLOCKS_DIR)/EdgesCollection.h\
					 $(BLOCKS_DIR)/ColorsCollection.h\
					 $(PARS_DIR)/Diagram.h\
					$(BLOCKS_DIR)/Graph.h 
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/Graph.cpp -o $@

$(OBJ_DIR)/ColorsCollection.o: $(BLOCKS_DIR)/ColorsCollection.cpp $(BLOCKS_DIR)/Color.h $(BLOCKS_DIR)/ColorsCollection.h 
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/ColorsCollection.cpp -o $@
$(OBJ_DIR)/EdgesCollection.o: $(BLOCKS_DIR)/EdgesCollection.cpp $(BLOCKS_DIR)/Edge.h $(BLOCKS_DIR)/EdgesCollection.h 
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/EdgesCollection.cpp -o $@
$(OBJ_DIR)/CrossingsCollection.o: $(BLOCKS_DIR)/CrossingsCollection.cpp $(PARS_DIR)/PD_X.h $(BLOCKS_DIR)/Crossing.h $(BLOCKS_DIR)/CrossingsCollection.h 
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/CrossingsCollection.cpp -o $@

$(OBJ_DIR)/Color.o: $(BLOCKS_DIR)/Color.cpp $(BLOCKS_DIR)/Crossing.h $(BLOCKS_DIR)/Color.h 
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/Color.cpp -o $@
$(OBJ_DIR)/Edge.o: $(BLOCKS_DIR)/Edge.cpp $(BLOCKS_DIR)/Crossing.h $(BLOCKS_DIR)/Edge.h
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/Edge.cpp -o $@
$(OBJ_DIR)/Crossing.o: $(BLOCKS_DIR)/Crossing.cpp $(PARS_DIR)/PD_X.h $(BLOCKS_DIR)/Crossing.h
	$(CXX) $(CXX_FLAGS) -c $(BLOCKS_DIR)/Crossing.cpp -o $@



## PARSER
$(OBJ_DIR)/Diagram.o: $(PARS_DIR)/Diagram.cpp $(PARS_DIR)/ColorMoves.h $(PARS_DIR)/Diagram.h
	$(CXX) $(CXX_FLAGS) -c $(PARS_DIR)/Diagram.cpp -o $@
$(OBJ_DIR)/PD_X.o: $(PARS_DIR)/PD_X.cpp $(PARS_DIR)/PD_X.h
	$(CXX) $(CXX_FLAGS) -c $(PARS_DIR)/PD_X.cpp -o $@
$(OBJ_DIR)/SeifertMoves.o: $(PARS_DIR)/SeifertMoves.cpp $(PARS_DIR)/SeifertMoves.h
	$(CXX) $(CXX_FLAGS) -c $(PARS_DIR)/SeifertMoves.cpp -o $@
$(OBJ_DIR)/ColorMoves.o: $(PARS_DIR)/ColorMoves.cpp $(PARS_DIR)/ColorMoves.h
	$(CXX) $(CXX_FLAGS) -c $(PARS_DIR)/ColorMoves.cpp -o $@



$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm $(TIME_TESTS_DIR)/measure_braiding_only.out