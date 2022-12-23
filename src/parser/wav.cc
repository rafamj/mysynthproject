//wav.cc
#include <fstream>
#include <string.h>
#include "wav.h"

void printError(const char *m) {
  printf("%s\n",m);
  exit(1);
}

static int readInt(ifstream &f) {
  int i;
  f.read((char *)&i,4);
  return i;
}

static void jump(ifstream &f, int n) {
    for(int i=0;i<n;i++) {
      char c;
      f.read(&c,1);
    }
}

unsigned char *readWav(size_t &l ,int &type, string filename) {
  unsigned char *buffer=0;
  wav_header w;
  fmt format;
  char header[4];
  bool ok=false;

  type=-1;
  ifstream f;
  f.open(filename);
  f.read((char *)&w,sizeof(wav_header));
  if(strncmp(w.riff_header,"RIFF",4)!=0 || strncmp(w.wave_header,"WAVE",4)!=0 ) {
    //printError("Error reading Wav file");
    type=-2;
    return 0;
  }
  while(!ok) {
  f.read(header,4);
    //printf("header %c%c%c%c %d %d\n",header[0],header[1],header[2],header[3],header[3],strcmp(header,"fmt"));
  if(strncmp(header,"fmt ",4)==0) {
    if(readInt(f)!=16) {
      type=-3;
      return 0;
      //printError("Error in length of fmt chunk");
    }
    f.read((char *)&format,sizeof(format));
  } else if(strncmp(header,"JUNK",4)==0) {
    int lenght=readInt(f);
    //printf("length of JUNK %d\n",lenght);
    jump(f,lenght);
  } else if(strncmp(header,"data",4)==0) {
    l=readInt(f);
    buffer= new unsigned char[l];
    f.read((char *)buffer,l);
    //int t=f.tellg();
    //printf("f.tellg() %d w.wav_size+8 %d\n",t,w.wav_size+8);
    //if(f.tellg()!=w.wav_size+8) {
   //   printError("Wrong lenght");
    //}
    ok=true;
  } else {
    int lenght=readInt(f);
    //printf("header %c%c%c%c lenght %d\n",header[0],header[1],header[2],header[3],lenght);
    jump(f,lenght);
    //printError("Unexpected chunk found");
  }
  }
  
  f.close();
  if(format.audio_format==1&&format.bit_depth==32) {
    type=1;
  } else if(format.audio_format==1&&format.bit_depth==16) {
    type=2;
  } else if(format.audio_format==3 && format.bit_depth==32) {
    type=0;
  } else {
    type=-4;
    return 0;
  }
  /*
  printf("float size %ld\n",sizeof(float));
  printf("audio_format %d\n",format.audio_format);
  printf("num_channels %d\n",format.num_channels);
  printf("sample_rate %d\n",format.sample_rate);
  printf("byte_rate %d\n",format.byte_rate);
  printf("sample_alignment %d\n",format.sample_alignment);
  printf("bit_depth  %d\n",format.bit_depth);
  printf("type %d\n",type);
  */
  return buffer;
}

/*
int main(int argc, char *argv[]) {
size_t l;
int type;

readWav(l,type,argv[1]);
}
*/
