CC = g++
CFLAGS =  -O3 -Wall
OBJECTS =  asyncLoop.o oscSQ.o oscSaw.o oscTri.o midi2cv.o filter.o midiMonitor.o adsr.o split.o pager.o select.o glider.o delay.o device.o poly.o 
OBJECTS := $(OBJECTS) oscDiv.o switch.o switchSum.o filter4LP.o midiSelect.o midi2control.o oscPWM.o delay2.o
OBJECTS := $(OBJECTS) chch.o oscWT.o mean.o
OBJECTS := $(OBJECTS) alsa/alsaIn.o alsa/alsaOut.o alsa/alsaSeq.o alsa/alsaFunc.o
OBJECTSSYNTH := $(OBJECTS) synth.o
OBJECTSMAIN := $(OBJECTS) main.o parser/createDev.o parser/parser.o parser/table.o  parser/wav.o



all: modular synth

modular: $(OBJECTSMAIN) 
	 $(CC) $(OBJECTSMAIN) $(CFLAGS) -o modular -lasound -lm

synth: $(OBJECTSSYNTH) 
	$(CC) $(OBJECTSSYNTH) $(CFLAGS) -o synth -lasound -lm

.cc.o:
	$(CC) -c $(CFLAGS) -o $@ $<

.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(OBJECTS) main.o synth.o parser/createDev.o parser/parser.o parser/table.o  parser/wav.o synth modular 
