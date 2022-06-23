CC = "g++"
PROJECT =  main	 
SRC = main.cpp vp.cpp soft.cpp dma.cpp hard.cpp utils.cpp intcon.cpp

LIBS = `pkg-config opencv4 --cflags --libs`
																		
$(PROJECT) : $(SRC)
	$(CC) -g $(SRC) -lsystemc -fno-omit-frame-pointer -o $(PROJECT) $(LIBS)

profiling: 
	valgrind --tool=callgrind ./$(PROJECT) ..data/light.jpeg 10
clean:
	rm -f *.o all $(PROJECT) callgrind.* result.jpg	

