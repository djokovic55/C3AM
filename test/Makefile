CC = "g++"
PROJECT =  main	 
SRC = main.cpp 

LIBS = `pkg-config opencv4 --cflags --libs`
																		
$(PROJECT) : $(SRC)
	$(CC) -g $(SRC) -lsystemc -fno-omit-frame-pointer -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./$(PROJECT) 
clean:
	rm -f *.o all $(PROJECT) callgrind.* 

