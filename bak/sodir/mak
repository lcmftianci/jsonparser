CC = g++ -std=c++11
IFLAG = -I/usr/local/include/ -I.
LFLAG = -L/usr/local/lib
LFLAG += -ljsoncpp -lpthread
CPP = testJsonManager.cpp VideoInfoJsonManager.cpp filenameio.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) -o a.out

clean:
	rm -rf a.out

