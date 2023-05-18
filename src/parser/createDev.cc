//createDev.cc
#include <string>
#include "../vca.h"////
#include "../sum.h"////
#include "../sum1.h"////
#include "../sub.h"////
#include "../sub1.h"////
#include "../pan.h"////
#include "../oscSQ.h"
#include "../oscPWM.h"
#include "../oscSaw.h"
#include "../oscTri.h"
#include "../oscWT.h"
#include "../alsa/alsaOut.h"
#include "../alsa/alsaIn.h"
#include "../alsa/alsaSeq.h"
#include "../delay.h"
#include "../delay2.h"
#include "../mult.h"
#include "../mult1.h"
#include "../div.h"
#include "../div1.h"
#include "../rand.h"
#include "../midi2cv.h"
#include "../split.h"
#include "../poly.h"
#include "../pager.h"
#include "../select.h"
#include "../adsr.h"
#include "../atan.h"
#include "../filter.h"
#include "../filter4LP.h"
#include "../constant.h"
#include "../soundMonitor.h"
#include "../midiMonitor.h"
#include "../glider.h"
#include "../oscDiv.h"
#include "../switch.h"
#include "../switchSum.h"
#include "../midiSelect.h"
#include "../midi2control.h"
#include "../chch.h"
#include "../mean.h"
#include "wav.h"
#include  "parser.h"

using namespace std;

Symbol *Parser::createDevDef(string n, string init, string input, string output, bool d, string body, int nLine) {
  //printf("createDevDef %s\n",n.c_str());
  return new Symbol(n,new DevDef(n,init,input,output,d,body,nLine));
}

void Parser::initDevices() {
  table.insert(createDevDef("AlsaOut","s:dev='default'","left, right","",false));
  table.insert(createDevDef("AlsaIn","s:dev='default'","","left, right"));
  table.insert(createDevDef("AlsaSeq","i:channel=-1, s:dev='default'","m:in","m:out",false));
  table.insert(createDevDef("Constant","v","","value",false));
  table.insert(createDevDef("Midi2cv","","m:in","note,vol,gate",false));
  table.insert(createDevDef("OscPWM","","note, pitchbend, pw","out"));
  table.insert(createDevDef("OscSaw","","note, pitchbend","out"));
  table.insert(createDevDef("OscSQ","","note, pitchbend","out"));
  table.insert(createDevDef("OscTri","","note, pitchbend","out"));
  table.insert(createDevDef("OscWT","s:filename","note, pitchbend, table","out"));
  table.insert(createDevDef("Vca","","in,cv","out"));
  table.insert(createDevDef("Mult","","in[2]","out"));
  table.insert(createDevDef("Mult1","v","in","out"));
  table.insert(createDevDef("Div","","in[2]","out"));
  table.insert(createDevDef("Div1","v","in","out"));
  table.insert(createDevDef("Atan","","in","out"));
  table.insert(createDevDef("Sum","","in[2]","out"));
  table.insert(createDevDef("Sum_1","v","in","out"));
  table.insert(createDevDef("Glider","","in, time","out"));
  table.insert(createDevDef("Sub","","in[2]","out"));
  table.insert(createDevDef("Sub_1","v","in","out"));
  table.insert(createDevDef("Pan","","in,cv","out[2]"));
  table.insert(createDevDef("Filter","","in,cutoff,reso","out"));
  table.insert(createDevDef("Filter4LP","","in,cutoff,reso","out"));
  table.insert(createDevDef("Rand","","","out"));
  table.insert(createDevDef("Delay","","in","out"));
  table.insert(createDevDef("Delay2","","in, del","out"));
  table.insert(createDevDef("Split","i:note","m:in","m:out[2]",false));
  table.insert(createDevDef("Chch","i:channel","m:in","m:out",false));
  table.insert(createDevDef("Poly","i:n=6","m:in","pitchbend, m:out[n],control[128]",false));
  table.insert(createDevDef("Pager","i:pags, i:input[], i:down, i:up","m:in","control[pags][size(input)]",false));
  table.insert(createDevDef("Select","i:c[],i:s[]","m:in","control[size(s)][size(c)]",false));
  table.insert(createDevDef("Adsr","","gate,a,d,s,r","out"));
  table.insert(createDevDef("OscDiv","","note","out[8]"));
  table.insert(createDevDef("Switch","","in[12][8], m:note","out[128]"));
  table.insert(createDevDef("SwitchSum","","in[12][8], m:note","out"));
  table.insert(createDevDef("SoundMonitor","","in",""));
  table.insert(createDevDef("MidiMonitor","","m:in","",false));
  table.insert(createDevDef("MidiSelect","i:c[],i:s[],i:tr[][]","m:in","m:out",false));
  table.insert(createDevDef("Midi2Control","","m:in","control[128]",false));
  table.insert(createDevDef("Mean","i:n","v[n]","out"));
}


static bool integerOk(Symbol *s,int min, int max) {
  return s->type==Symbol::INTEGER && s->value.integer >=min && s->value.integer <=max;
}

vector<unsigned int> Parser::readIntArray(Symbol *s) {
      vector<unsigned int> r;

      if(s->type!=Symbol::ARRAY) {
        printError("Array expected");
      }
      int n=s->value.array->alm.size();
      for(int i=0; i<n; i++) {
        if(s->value.array->alm[i]->type==Symbol::INTEGER) {
	    r.push_back(s->value.array->alm[i]->value.integer);
	} else if(s->value.array->alm[i]->type==Symbol::ARRAY) {
	   vector<unsigned int> v=readIntArray(s->value.array->alm[i]);
	   r.insert(r.end(),v.begin(),v.end());
        } else {
	    printError("Controller must be an integer");
        }
      }
  return r;
}

#define VALUE(x) x->type==Symbol::REAL?x->value.real:x->value.integer

Device *Parser::createDev(string typeName, vector<Symbol *> &init) { //init is modified
  Device *d=0;
    if(typeName.compare("OscSQ")==0) {
      d=new OscSQ();
    } else if(typeName.compare("OscPWM")==0) {
      d=new OscPWM();
    } else if(typeName.compare("OscSaw")==0) {
      d=new OscSaw();
    } else if(typeName.compare("OscTri")==0) {
      d=new OscTri();
    } else if(typeName.compare("OscWT")==0) {
      size_t l;
      int type;
      unsigned char *buffer=readWav(l,type,*init[0]->value.str);
      if(type<0) printError("Error reading wav file");
      d=new OscWT(buffer,l,type);
    } else if(typeName.compare("Vca")==0) {
      d=new Vca();
    } else if(typeName.compare("Mult")==0) {
      d=new Mult();
    } else if(typeName.compare("Mult1")==0) {
      d=new Mult1(VALUE(init[0]));
    } else if(typeName.compare("Div")==0) {
      d=new Div();
    } else if(typeName.compare("Div1")==0) {
        d=new Div1(VALUE(init[0]));
    } else if(typeName.compare("Atan")==0) {
      d=new Atan();
    } else if(typeName.compare("Pan")==0) {
      d=new Pan();
    } else if(typeName.compare("Sum")==0) {
      d=new Sum();
    } else if(typeName.compare("Sum_1")==0) {
      d=new Sum_1(VALUE(init[0]));
    } else if(typeName.compare("Sub")==0) {
      d=new Sub();
    } else if(typeName.compare("Sub_1")==0) {
      d=new Sub_1(VALUE(init[0]));
    } else if(typeName.compare("Rand")==0) {
      d=new Rand();
    } else if(typeName.compare("Glider")==0) {
      d=new Glider();
    } else if(typeName.compare("Delay")==0) {
      d=new Delay();
    } else if(typeName.compare("Delay2")==0) {
      d=new Delay2();
    } else if(typeName.compare("Pager")==0) {
      unsigned int *in=new unsigned int[init[1]->value.array->alm.size()];
      for(size_t i=0;i<init[1]->value.array->alm.size();i++) in[i]=init[1]->value.array->alm[i]->value.integer;
      sample **sia=new sample*[init[1]->value.array->alm.size()];
      sample *soa=new sample[init[1]->value.array->alm.size()*init[0]->value.integer];
      d=new Pager(sia,soa,init[0]->value.integer,in,init[1]->value.array->alm.size(),init[2]->value.integer,init[3]->value.integer);
    } else if(typeName.compare("Select")==0) {
        d=new Select(new sample[init[0]->value.array->alm.size()*init[1]->value.array->alm.size()],&readIntArray(init[0])[0],init[0]->value.array->alm.size(),&readIntArray(init[1])[0],init[1]->value.array->alm.size());
    } else if(typeName.compare("Adsr")==0) {
      d=new Adsr();
    } else if(typeName.compare("Filter")==0) {
      d=new Filter();
    } else if(typeName.compare("Filter4LP")==0) {
      d=new Filter();
    } else if(typeName.compare("AlsaOut")==0) {
      d=new AlsaOut(init[0]->value.str->c_str());
    } else if(typeName.compare("AlsaIn")==0) {
      d=new AlsaIn(init[0]->value.str->c_str());
    } else if(typeName.compare("AlsaSeq")==0) {
      if(integerOk(init[0],-1,15)) {
        d=new AlsaSeq(init[0]->value.integer,init[1]->value.str->c_str());
      } else printError("Wrong channel number");
    } else if(typeName.compare("Constant")==0) {
      d=new Constant(VALUE(init[0]));
    } else if(typeName.compare("Midi2cv")==0) {
      d=new Midi2cv();
    } else if(typeName.compare("Split")==0) {
      d=new Split(init[0]->value.integer);
    } else if(typeName.compare("Chch")==0) {
      d=new Chch(init[0]->value.integer);
    } else if(typeName.compare("OscDiv")==0) {
      d=new OscDiv();
    } else if(typeName.compare("Switch")==0) {
      d=new Switch();
    } else if(typeName.compare("SwitchSum")==0) {
      d=new SwitchSum();
    } else if(typeName.compare("Poly")==0) {
      bool *na=new bool[init[0]->value.integer];
      unsigned int *notes=new unsigned int[init[0]->value.integer];
      d=new Poly(init[0]->value.integer,na,notes);
    } else if(typeName.compare("SoundMonitor")==0) {
      d=new SoundMonitor();
    } else if(typeName.compare("MidiMonitor")==0) {
      d=new MidiMonitor();
    } else if(typeName.compare("MidiSelect")==0) {
      d=new MidiSelect(&readIntArray(init[0])[0],init[0]->value.array->alm.size(),&readIntArray(init[1])[0],init[1]->value.array->alm.size(),&readIntArray(init[2])[0],init[2]->value.array->alm.size());
    } else if(typeName.compare("Midi2Control")==0) {
      d=new Midi2Control();
    } else if(typeName.compare("Mean")==0) {
      //printf("Mean %d",init[0]->value.integer);
      int n=init[0]->value.integer;
      sample **si=new sample*[n];
      d=new Mean(si,n);
    } else {
      printError("Unknown device");
    }
    return d;
}
