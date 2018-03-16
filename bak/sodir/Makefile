CC = g++ -c -std=c++11 -fPIC
IFLAG = -I/usr/local/include/ -I.
LFLAG = -L/usr/local/lib
LFLAG += -ljsoncpp -lpthread -lopencv_core -lopencv_highgui -lopencv_imgcodecs
CPP = VideoInfoJsonManager.cpp filenameio.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) #-o libjsonparser.o

clean:
	rm -rf *.o libjsonparser.o

