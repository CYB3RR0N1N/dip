BUILD = build


.PHONY: all 

all: main.cpp
	g++ -o $(BUILD)/main main.cpp -lglut -lGL -g