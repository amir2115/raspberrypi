rm ./run2

g++  run2.cpp  -o run2 -lwiringPi -std=gnu++11 -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib -lopencv_objdetect -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


./run2
