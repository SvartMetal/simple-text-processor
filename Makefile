BUILD_PATH = ./build
SRC_PATH = ./src

CPP = g++

CFLAGS = -DDebug -ggdb -Werror 
LFLAGS = 

BASE_OBJECTS  = $(BUILD_PATH)/streamutil.o
BUILD_OBJECTS = $(BUILD_PATH)/main.o $(BASE_OBJECTS)
TEST_OBJECTS = $(BUILD_PATH)/test.o $(BASE_OBJECTS)

.PHONY: build
build: $(BUILD_PATH)/main

.PHONY: test
test: $(BUILD_PATH)/test

$(BUILD_PATH)/main: $(BUILD_OBJECTS) 
	$(CPP) $(LFLAGS) $(BUILD_OBJECTS) -o $@

$(BUILD_PATH)/main.o: $(SRC_PATH)/main.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp $(SRC_PATH)/%.h
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_PATH)/test: $(TEST_OBJECTS)
	$(CPP) $(LFLAGS) $(TEST_OBJECTS) -o $@

$(BUILD_PATH)/test.o: $(SRC_PATH)/test.cpp
	$(CPP) $(CFLAGS) -c $< -o $@



.PHONY: clean
clean: 
	rm -rf $(BUILD_PATH)/*.o $(BUILD_PATH)/main $(BUILD_PATH)/test


