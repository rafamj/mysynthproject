CC = g++
CFLAGS =  -O3 -Wall
OBJECTS = main.o oscSQ.o oscSaw.o oscTri.o midi2cv.o filter.o midiMonitor.o adsr.o alsaSeq.o alsaOut.o alsaIn.o split.o pager.o select.o glide.o delay.o device.o poly.o 
OBJECTS := $(OBJECTS) oscDiv.o switch.o filter4LP.o midiSelect.o midi2control.o oscPWM.o delay2.o
OBJECTS := $(OBJECTS) chch.o
OBJECTS := $(OBJECTS) parser/createDev.o parser/parser.o parser/table.o  oscWT.o parser/wav.o



modular: $(OBJECTS)
	 $(CC) $(OBJECTS) $(CFLAGS) -o modular -lasound -lm

.cc.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(OBJECTS) modular 
