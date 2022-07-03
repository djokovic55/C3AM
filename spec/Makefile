CC = "g++"
PROJECT =  main
SRC = 1d_vect_seam.cpp 

LIBS = `pkg-config opencv4 --cflags --libs`

$(PROJECT) : $(SRC)
	$(CC) -g $(SRC) -lsystemc -fno-omit-frame-pointer -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./$(PROJECT) ../data/forest.jpg 10
clean:
	rm -f *.o all $(PROJECT) callgrind.* result.jpg

