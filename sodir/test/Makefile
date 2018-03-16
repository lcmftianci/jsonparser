CC = g++ -g -std=c++11
IFLAG = -I/jsoncpp-src-0.5.0/include/ -I.
LFLAG = -L  ../jsoncpp-src-0.5.0/libs/linux-gcc-5.4.1/ -L./
LFLAG += -ljson_linux-gcc-5.4.1_libmt -lpthread -ljsonparser -lopencv_core -lopencv_highgui #-lopencv_imgcodecs
CPP = test.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) -o a.out

clean:
	rm -rf *.o a.out

