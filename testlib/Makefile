CC = g++ -std=c++11
IFLAG = -I. -I/jsoncpp/include/ #-I/usr/local/include/
LFLAG = -L. -L/jsoncpp/lib/ #-L/usr/local/lib/
LFLAG += -ljsonparser -lpthread -ljsoncpp -lopencv_core -lopencv_highgui #-lopencv_imgcodecs
CPP = test.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) -o a.out

clean:
	rm -rf *.o a.out

