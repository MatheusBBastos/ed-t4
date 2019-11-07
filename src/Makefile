CC = gcc
# TODO: remover -g
COMPILER_FLAGS = -std=c99 -fstack-protector-all -g
LINKER_FLAGS = -lm
EXEC_NAME = siguel
OBJECTS = main.o distance.o heapsort.o geometry.o object.o circle.o rectangle.o block.o equipment.o \
    segment.o vertex.o building.o wall.o file_util.o static_list.o svg.o commands.o query.o data.o \
	comparators.o redblack_tree.o hash_table.o point.o text.o lists.o

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXEC_NAME) -Wall

main.o: main.c file_util.h modules/sig/object.h commands.h
	$(CC) -c main.c $(COMPILER_FLAGS)

distance.o: modules/aux/distance.c modules/aux/distance.h modules/sig/equipment.h
	$(CC) -c modules/aux/distance.c $(COMPILER_FLAGS)

segment.o: modules/aux/segment.c modules/aux/segment.h modules/aux/vertex.h
	$(CC) -c modules/aux/segment.c $(COMPILER_FLAGS)

vertex.o: modules/aux/vertex.c modules/aux/vertex.h modules/aux/segment.h
	$(CC) -c modules/aux/vertex.c $(COMPILER_FLAGS)

point.o: modules/aux/point.c modules/aux/point.h
	$(CC) -c modules/aux/point.c $(COMPILER_FLAGS)

heapsort.o: modules/aux/heapsort.c modules/aux/heapsort.h
	$(CC) -c modules/aux/heapsort.c $(COMPILER_FLAGS)

geometry.o: modules/sig/geometry.c modules/sig/geometry.h modules/sig/object.h
	$(CC) -c modules/sig/geometry.c $(COMPILER_FLAGS)

object.o: modules/sig/object.c modules/sig/object.h
	$(CC) -c modules/sig/object.c $(COMPILER_FLAGS)

circle.o: modules/sig/circle.c modules/sig/circle.h
	$(CC) -c modules/sig/circle.c $(COMPILER_FLAGS)

rectangle.o: modules/sig/rectangle.c modules/sig/rectangle.h
	$(CC) -c modules/sig/rectangle.c $(COMPILER_FLAGS)

text.o: modules/sig/text.c modules/sig/text.h
	$(CC) -c modules/sig/text.c $(COMPILER_FLAGS)

block.o: modules/sig/block.c modules/sig/block.h
	$(CC) -c modules/sig/block.c $(COMPILER_FLAGS)

equipment.o: modules/sig/equipment.c modules/sig/equipment.h
	$(CC) -c modules/sig/equipment.c $(COMPILER_FLAGS)

building.o: modules/sig/building.c modules/sig/building.h
	$(CC) -c modules/sig/building.c $(COMPILER_FLAGS)

wall.o: modules/sig/wall.c modules/sig/wall.h
	$(CC) -c modules/sig/wall.c $(COMPILER_FLAGS)

file_util.o: file_util.c file_util.h
	$(CC) -c file_util.c $(COMPILER_FLAGS)

static_list.o: modules/data_structures/static_list.c modules/data_structures/static_list.h
	$(CC) -c modules/data_structures/static_list.c $(COMPILER_FLAGS)

svg.o: modules/sig/svg.c modules/sig/svg.h modules/sig/object.h modules/sig/circle.h modules/sig/rectangle.h
	$(CC) -c modules/sig/svg.c $(COMPILER_FLAGS)

commands.o: commands.c commands.h file_util.h modules/data_structures/static_list.h modules/sig/svg.h modules/sig/geometry.h modules/sig/object.h
	$(CC) -c commands.c $(COMPILER_FLAGS)

query.o: query.c query.h modules/data_structures/static_list.h modules/sig/svg.h modules/sig/geometry.h modules/sig/object.h
	$(CC) -c query.c $(COMPILER_FLAGS)

data.o: data.c data.h modules/data_structures/redblack_tree.h modules/data_structures/hash_table.h modules/aux/point.h
	$(CC) -c data.c $(COMPILER_FLAGS)

comparators.o: comparators.c comparators.h
	$(CC) -c comparators.c $(COMPILER_FLAGS)

redblack_tree.o: modules/data_structures/redblack_tree.c modules/data_structures/redblack_tree.h
	$(CC) -c modules/data_structures/redblack_tree.c $(COMPILER_FLAGS)

hash_table.o: modules/data_structures/hash_table.c modules/data_structures/hash_table.h
	$(CC) -c modules/data_structures/hash_table.c $(COMPILER_FLAGS)

clean:
	rm -rf *.o
