//table.h
#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector> 
#include "../device.h"

using namespace std;

class Device;
class Symbol;
class Token;
class Table; //only para Symbol print


class DevDef {
  public:
  string name;
  string init;
  string inputs;
  string outputs;
  /*
  vector<Symbol *> init;
  vector<Symbol *> inputs;
  vector<Symbol *> outputs;
  vector<Symbol *> minputs;
  vector<Symbol *> moutputs;
  */
  bool dynamic;
  string body; //for user defined devices
  //size_t textIndex; //for user defined devices
  int nLine;       // " "
  //DevDef(string n,vector<Symbol *> it,vector<Symbol *> i,vector<Symbol *> o, vector<Symbol *> mi,vector<Symbol *> mo, bool d=true, string bod="", int nl=0):name(n),init(it),inputs(i),outputs(o),minputs(mi),moutputs(mo){dynamic=d;body=bod;nLine=nl;}
  DevDef(string n,string it,string i,string o, bool d=true, string bod="", int nl=0):name(n),init(it),inputs(i),outputs(o){dynamic=d;body=bod;nLine=nl;}
  void print();
};


class DevObj {
  public: //o escribir un constructor
  string name;
  string className;
  bool dynamic;
  bool visited;
  vector<Symbol *> inputs;
  vector<Symbol *> outputs;
  vector<Symbol *> minputs;
  vector<Symbol *> moutputs;
  vector<DevObj *> connections; //to keep track of the connections, where every input points
  Device *dev;
};

class Field {
  public:
  int n; //number of field
  int io; //0 input 1 output
  Symbol *f; //the type of field (array or real or integer)
  DevObj *devObj;
  Field(int nn, int nio, Symbol *fi, DevObj *dev){n=nn;io=nio;f=fi;devObj=dev;};
};

class Midi {
  public:
};

class Input {
  public:
  vector<Symbol *> alm; //an input from a user def device can point to several inputs inside the device
  Input():alm(0){}
};

class Array;

union Value {
  string *ident;
  int    integer;
  double real;
  DevObj *devObj;
  DevDef *devDef;
  Field *field;
  Array *array;
  Midi *midi;
  Input *input;
  string *str;
};


class Symbol {
  public:
  enum Type {NEW,IDENTIFIER,INTEGER,REAL,DEVOBJ,DEVDEF,FIELD,ARRAY,MIDI,INPUT,OUTPUT,STRING};
    Type type;
    string name;
    Value value;
    Symbol(string n) {type=NEW; name=n;}
    Symbol(Type t,string n, string *v) {type=t; name=n;value.str=v;} //used for strings
    Symbol(string n, string *v) {type=IDENTIFIER; name=n;value.ident=v;}
    Symbol(string n, int i) {type=INTEGER; name=n;value.integer=i;}
    Symbol(string n, double r) {type=REAL; name=n;value.real=r;}
    Symbol(string n, DevDef *d) {type=DEVDEF; name=n;value.devDef=d;}
    Symbol(string n, DevObj *d) {type=DEVOBJ; name=n;value.devObj=d;}
    Symbol(string n, Field *f) {type=FIELD; name=n;value.field=f;}
    Symbol(string n, Array *a) {type=ARRAY; name=n;value.array=a;}
    Symbol(string n, Midi *m) {type=MIDI; name=n;value.midi=m;}
    Symbol(string n, Input *i) {type=INPUT; name=n;value.input=i;}
    Symbol(Symbol *s) {type=s->type;name=s->name;value=s->value;}
    Type getType() { return type;}
    bool is(Type t) { return type==t;}
    bool isNumber() { return type==REAL || type==INTEGER;}
    bool isInputOrField() { return type==INPUT || type==FIELD;}
    string symbol2string();
    void asControl();
    void print();
};

class Array {
  public:
  Symbol::Type type;
  vector<Symbol *> alm;
  //int w; //total size of the array including sub arrays  ?????:wq
  Array(int n):alm(n){}
  //Array(const vector<Symbol *> &a,int n):alm(a),w(n){}
  Array(const vector<Symbol *> &a):alm(a){type=a.size()==0?Symbol::NEW:a[0]->type;}
};

#define MAX_TABLE 300

class Table {
  public://
  Symbol *alm[MAX_TABLE];  
  Table() {init();}
  void init() {for(int i=0;i<MAX_TABLE;i++) alm[i]=0;}
  void insert(Symbol *s);
  Symbol *search(string n);
  //Symbol *search(DevObj *dev);
  vector<Symbol *> searchDevObj(string type);
  void print() {for(int i=0;i<MAX_TABLE;i++) {if(alm[i]) {printf("%d ",i);alm[i]->print();}}}
  ~Table() {} ///
};
#endif

