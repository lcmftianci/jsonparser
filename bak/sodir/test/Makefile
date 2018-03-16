CC = g++ -std=c++11
IFLAG = -I/usr/local/include/ -I.
LFLAG = -L/usr/local/lib -L.
LFLAG += -ljsonparser -lpthread -ljsoncpp -lopencv_core -lopencv_highgui #-lopencv_imgcodecs
CPP = test.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) -o a.out

clean:
	rm -rf *.o a.out

