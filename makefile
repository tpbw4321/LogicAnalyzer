All:
	gcc -I /opt/vc/include -I /usr/include -I /usr/include/libusb-1.0/ -lusb-1.0 -lshapes -lm -std=c99 -o LogicAnalyzer main.c scope.c cmdargs.c queue.c usbcomm.c stack.c data.c

