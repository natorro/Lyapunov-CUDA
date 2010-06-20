all: serial parallel

serial: serial.c
	gcc -O2 -m32 -framework GLUT -framework OpenGL -I/Developer/GPU\ Computing/C/common/inc/GL/ /Developer/GPU\ Computing/C/common/lib/darwin/libGLEW.a serial.c -o serial

parallel: parallel.cu
	nvcc -O2 -m32 -I/Developer/GPU\ Computing/C/common/inc/GL/ -I/Developer/GPU\ Computing/C/common/inc/ -Xlinker -framework,GLUT,-framework,OpenGL /Developer/GPU\ Computing/C/common/lib/darwin/libGLEW.a parallel.cu -o parallel

clean:
	rm -rf serial parallel *~ a.out *#*