CC = g++ -std=c++11


cflags = -I./jsoncpp-src-0.5.0/include/ -I./base64/
INC = -L jsoncpp-src-0.5.0/libs/linux-gcc-5.4.1/


SRCS = VideoInfoJsonManager.cpp filenameio.cpp ./base64/base64.cpp JsonManager.cpp


C_FLAGS = -ljson_linux-gcc-5.4.1_libmt -lpthread -lopencv_core -lopencv_highgui 
# -lopencv_imgcodecs

LIBRARY = libjsonparser.so

all:
	$(CC) $(SRCS) $(C_FLAGS) -fPIC --shared -O2  -g -o $(LIBRARY) $(INC) $(cflags)
	cp $(LIBRARY) /home/VideoAnalysis/Bin/
	cp $(LIBRARY) jsoncpp-src-0.5.0/libs/linux-gcc-5.4.1/
clean:
	rm  libjsonparser.so

