CC = "g++"
PROJECT =  1d_vect_seam 
SRC = 1d_vect_seam.cpp 

LIBS = `pkg-config opencv4 --cflags --libs`

$(PROJECT) : $(SRC)
	$(CC) -g $(SRC) -lsystemc -fno-omit-frame-pointer -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./$(PROJECT) forest.jpg 2
clean:
	rm -f *.o all $(PROJECT) callgrind.*	

