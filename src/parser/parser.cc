//parser.cc
#include "devObjSet.h"
#include "../constant.h"
#include "../deviceSet.h"
#include "parser.h"


static bool isSpecial(char c) {
  return string("=().[],+-*/{}:;<\"'").find(c)!=string::npos;
}

static bool isSpace(char c) {
  return string(" \t").find(c)!=string::npos;
}

static string type2string(Type t) {
  switch(t) {
    case UNKNOWN: return "UNKNOWN";
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case SPECIAL: return "SPECIAL";
    case EOL: return "EOL";
    case END: return "END";
    case CONNECT: return "CONNECT";
    case STRING: return "STRING";
    default: return "UNKNOWN";
  }
}
/*
static string symbolType2string(Symbol::Type t) {
  switch(t) {
    case Symbol::NEW: return "NEW";
    case Symbol::IDENTIFIER: return "IDENTIFIER";
    case Symbol::INTEGER: return "INTEGER";
    case Symbol::REAL: return "REAL";
    case Symbol::DEVOBJ: return "DEVOBJ";
    case Symbol::DEVDEF: return "DEVDEF";
    case Symbol::FIELD: return "FIELD";
    case Symbol::ARRAY: return "ARRAY";
    case Symbol::MIDI: return "MIDI";
    case Symbol::INPUT: return "INPUT";
    case Symbol::OUTPUT: return "OUTPUT";
    default: return "UNKNOWN";
  }
}
*/
void Token::print() {
  printf("token %s %s\n",name.c_str(),type2string(type).c_str());
}

static string generateName(string s) {
  struct Name {
    string name;
    int n;
    Name(string na):name(na),n(0){}
  };
  static vector <Name> name;
  for(size_t i=0;i<name.size();i++) {
    if(name[i].name.compare(s)==0) {
      return string(name[i].name + to_string(name[i].n++));
    }
  }
  name.push_back(Name(s));
  return generateName(s);
}

Parser::Parser(){
  textIndex=0;
  nLine=0;
  nextT=0;
  saveText=false;
  set= new DevObjSet();
  aIn=0;
  asound=0;
  aseq=0;
}

void Parser::initString(string s) {
  text=s;
  textIndex=0;
  nextT=0;
  nextToken(); //call the first time to init nextT (lookahead)
}

void Parser::initFile(string filename) {
  ifstream f;
  f.open(filename);
  f.seekg(0,ios::end);
  size_t l=f.tellg();
  text=string(l,' ');
  f.seekg(0);
  f.read(&text[0],l);
  f.close();
  nLine=0;
  nextT=0;
  nextToken(); //call the first time to init nextT (lookahead)
  initDevices();
}

Parser::~Parser() {
}

void Parser::printError(const char *error) {
  //int endLine=errorIndex0;
  int endLine=errorIndex-1;
  int beginLine=endLine-1;
  while(text[endLine] && text[endLine] !='\n') endLine++;
  while(beginLine>0 && text[beginLine] !='\n') beginLine--;
  //printf("errorIndex %ld beginLine %d endLine %d %c\n",errorIndex,beginLine,endLine,text[errorIndex]);
  printf("%s\n%s^\n",text.substr(beginLine,endLine-beginLine).c_str(),string(errorIndex-beginLine-2,' ').c_str());
  printf("Error in line %d. %s.\n",nLine,error);
  exit(1);
}

void Parser::printError(const char *error, const char *more) {
  char buffer[500];
  sprintf(buffer,error,more);
  printError(buffer);
}

void Parser::expect(char c) {
  Token *t=nextToken();

  if(!t->is(c)) {
    if(c=='\n') {
      printError("'\\n' expected");
    } else {
      char buffer[2]={c,0};
      printError("'%s' expected",buffer);
    }
  }
}

Token *Parser::expect(Type ty) {
  Token *t=nextToken();

  if(t->type!=ty) {
      printError("%s expected",type2string(ty).c_str());
  }
  return t;
}

string generateArrayName(string name, int index) {
      char buffer[100];
      sprintf(buffer,"%s[%d]",name.c_str(),index);
      return buffer;
}


void Parser::parse(string end) {
  Token *t=lookAhead();
  while(t->type!=END && t->type!=UNKNOWN) {
    if(t->type==IDENTIFIER) {
      if(t->name.compare("def")==0) {
        nextToken();
        readDef();
	t=lookAhead();
      } else if(t->name.compare(end)==0) { 
        t=nextToken();
        break;  
      } else {
        Symbol *s=table.search(t->name);
	if((s && s->type==Symbol::DEVDEF)||t->name.compare("real")==0 || t->name.compare("int")==0) {
          nextToken();
	  string name;
	  if((s && s->type==Symbol::DEVDEF)) {
	    name=s->name;
	  } else {
	    name=t->name;
	  }
	  vector <Symbol *> r=readDeclaration(name);
	  for(size_t i=0; i<r.size();i++) {
            table.insert(r[i]);
	  }
          nextToken(); //consume the EOL
	} else {
	  if(!s) { //an undeclared scalar variable
	    table.insert(new Symbol(t->name));
	  }
	  //s=readVar(t);
          //expect('=');
          //Symbol *r=readExpression();
          readExpression();
          //expect(EOL);
          //assign(s,r); 
	}
      }
    } else if(t->type==EOL) { //empty line
      nextToken();
    } else {
      printError("Identifier expected. %s",t->name.c_str());
    }
    t=lookAhead();
  }
//exit(0);
}

void Parser::buildDeviceSet(string root, bool defining) { /////////este habra que borrarlo
    vector<Symbol *>sm=table.searchDevObj("SoundMonitor"); /////
    for(size_t i=0;i< sm.size();i++) {
      if(sm[i]->type==Symbol::DEVOBJ) {
        set->insert(sm[i]->value.devObj);
      } else {
        printError("error 23");
      }
    }
  Symbol *s=table.search(root);
    printf("p1 %d\n",s->type);
  if(s->type!=Symbol::NEW) { 
    list(s->value.devObj,defining);
  } else { //not found
    //printError("Device %s not found",root.c_str());
  }
}

void Parser::buildDeviceSet(DevObj *devObj, bool defining) {
    vector<Symbol *>sm=table.searchDevObj("SoundMonitor"); /////
    for(size_t i=0;i< sm.size();i++) {
      if(sm[i]->type==Symbol::DEVOBJ) {
        set->insert(sm[i]->value.devObj);
      } else {
        printError("error 23");
      }
    }

    list(devObj,defining);
}

void Parser::getDevices(DeviceSet *d) {
  for(int i=0;i<set->size();i++) {
    d->insert(set->getElement(i)->dev);
  }
}

void Parser::readDef() {
  size_t p1,p2;
  string name;
  string init;
  string inputs;
  string outputs;

  Token *t=expect(IDENTIFIER);
  name=t->name;
  t=nextToken();
  if(t->is('(')) {
    p1=errorIndex; //because of lookAhead
    while(!t->is(')')) {
      t=nextToken();
    }
   init=text.substr(p1,textIndex-p1-2); // -1 to exclude the ')' 
  }
  t=nextToken();
  while(t->type==EOL) t=nextToken();
  if(t->name.compare("inputs")==0) {
	  inputs=readLine();
  }
  t=nextToken();
  while(t->type==EOL) t=nextToken();
  if(t->name.compare("outputs")==0) {
	  outputs=readLine();
  }
  p1=errorIndex; //because of lookAhead
  int nl=nLine;
  while(t->name.compare("end")) {
    t=nextToken();
  }
  p2=textIndex;
  string body=text.substr(p1,p2-p1);
  Symbol *sd=createDevDef(name,init,inputs,outputs,true,body,nl);
  //Symbol *sd=new Symbol(name,new Devdef(name, init,inputs,outputs,true,body,nl));
 // for(size_t i=0;i<sd->value.devDef->inputs.size();i++) {
 // }
  table.insert(sd);
}



void Parser::list(DevObj *devObj,bool defining) {
  //printf("list %s\n",devObj->name.c_str());
  if(devObj->visited) {
    return;
  }
  devObj->visited=true; //// self loop??
  for(size_t i=0;i<devObj->connections.size();i++) {
    string name="(null)";
    {
    if(devObj->connections[i]) { 
     name=devObj->connections[i]->name;
    }
    }
    DevObj *dob=devObj->connections[i];
    if(dob==0) {
      if(defining) continue;
      printf("%s %ld no conectado\n",devObj->name.c_str(),i);
      exit(1);
    } else {
    //printf("%s %ld  conectado a %s\n",devObj->name.c_str(),i,dob->name.c_str());
    list(dob,defining);
    }
  }
  if(!set->contains(devObj) && devObj->dynamic) {
    set->insert(devObj);
  }
}

static int arraySize(vector<Symbol *> a) {
  int w=0;

  for(size_t i=0; i<a.size(); i++) {
    if(a[i]->type==Symbol::ARRAY) {
      w+=arraySize(a[i]->value.array->alm);
    } else {
      w++;
    }
  }
  return w;
}

Symbol *Parser::create(string typeName, string name, vector<int> dimensions, vector<Symbol *> init) {
    if(dimensions.size()==0) {
        return create(typeName, name,init);
    } else {
      int dim=dimensions[0];
      dimensions.erase(dimensions.begin());
      vector<Symbol *> a(dim);
      for(int i=0;i<dim;i++) {
        a[i]=create(typeName, generateArrayName(name,i),dimensions,init); //conserve the name in subarrays???
      }
      Array *arr=new Array(a);
      return new Symbol(name,arr);
    }
}


static void copyIO(vector<Symbol *>io, vector<Symbol *> &r){ //deep copy of io
  r=vector<Symbol *>(io.size());
  for(size_t i=0;i<io.size();i++) {
    if(io[i]->type==Symbol::ARRAY) {
      Array *a=new Array(io[i]->value.array->alm.size());
      r[i]=new Symbol(io[i]->name,a);
      copyIO(io[i]->value.array->alm,r[i]->value.array->alm);
    } else if(io[i]->type==Symbol::INPUT) {
      r[i]=new Symbol(io[i]->name,new Input());
    } else if(io[i]->type==Symbol::OUTPUT) {
      r[i]=new Symbol(io[i]->name);r[i]->type=Symbol::OUTPUT;
    } else {
      r[i]=io[i]; 
    }
  }
}

static Symbol::Type getType(Symbol * s) {
  if(s->type!=Symbol::ARRAY) {
    return s->type;
  } else {
    if(s->value.array->alm.size()==0) return Symbol::REAL;/////
    return getType(s->value.array->alm[0]);  //we assume that all array are homogeneous
  }
}

static vector<Symbol *> filterSymbols(vector<Symbol *> v,Symbol::Type type) {
  vector<Symbol *> r;
  for(size_t i=0; i<v.size();i++) {
    if(getType(v[i])==type) {
      r.push_back(v[i]);
    }
  }
  return r;
}

DevObj *Parser::createDev(string name, DevDef* def,vector<Symbol *> init) { //user defined dev
  Parser p;
  p.table.init();
  p.initDevices();

  DevObj *o=new DevObj;
  initParameters(def,o,init);
  p.initString(def->body);
  p.nextToken();
  for(size_t i=0;i<init.size();i++) {
    //printf("%ld ",i);init[i]->print();
    p.table.insert(init[i]);
  }
 
  for(size_t i=0;i<o->inputs.size();i++) {
    p.table.insert(o->inputs[i]);
  }
 
  for(size_t i=0;i<o->outputs.size();i++) {
    p.table.insert(o->outputs[i]);
  }
  for(size_t i=0;i<o->minputs.size();i++) {
    p.table.insert(o->minputs[i]);
  }
  for(size_t i=0;i<o->moutputs.size();i++) {
    p.table.insert(o->moutputs[i]);
  }
  p.text=def->body;
  p.nLine=def->nLine;
  p.parse("end");
  
    o->name=name;
    o->visited=false;
    o->dynamic=def->dynamic;
    o->className=def->name;
    o->connections=vector<DevObj *>(arraySize(o->inputs),0); //flatten the inputs from devDef, because in the device the inputs are in a linear array without subarrays
  Symbol *s;


  for(size_t i=0;i<o->inputs.size();i++) {
    s=p.table.search(o->inputs[i]->name);
    if(s->type==Symbol::INPUT && s->value.input->alm.size()==0) {
      printError("Unused input %s",s->name.c_str());
    }
  }
  for(size_t i=0;i<o->outputs.size();i++) {
    s=p.table.search(o->outputs[i]->name);
    if(s->type==Symbol::OUTPUT) {
      printError("Unused output %s",s->name.c_str());
    }
    p.buildDeviceSet(s->value.field->devObj,true);
  }
  for(int i=0;i<p.set->size();i++) {
    DevObj *s=p.set->getElement(i);
    string n=name+"."+s->name;
    s->name=n;
    s->visited=false;
    table.insert(new Symbol(n,s));
  }

return o;
}

static int completeFieldData(vector<Symbol *> io,int n) {
  for(size_t i=0; i<io.size();i++) {
    if(io[i]->type==Symbol::ARRAY) { 
      n=completeFieldData(io[i]->value.array->alm,n);
    } else {
      io[i]->value.field->n=n;
      n++;
    }
  }
  return n;
}

static Symbol *createField(Symbol *f,int IO, DevObj *dev) { //we need al the io vector for getBase
            if(f->type==Symbol::ARRAY) {
	      Array *a=new Array(f->value.array->alm.size());
	      for(size_t i =0; i<f->value.array->alm.size();i++) {
	        a->alm[i]=createField(f->value.array->alm[i],IO,dev);
	      }
	      return new Symbol(f->name,a);
	    } else {
	      return new Symbol(f->name, new Field(0,IO,f,dev));
	    }
}

static vector<Symbol *>  createObjFields(const vector<Symbol *> &defField, int io, DevObj *o) {
      vector<Symbol *> res=vector<Symbol *>(defField.size());
      for(size_t i=0;i<defField.size();i++) { 
	 res[i]=createField(defField[i],io,o);
      }
      return res;
}

Symbol *Parser::createConstant(sample value) {
   vector<Symbol *> init(1); //empty vector
   init[0]=new Symbol(generateName("const"),value);
   Symbol *c=create("Constant",init[0]->name,init);
   return c;
}

void Parser::initParameters(DevDef *def, DevObj *obj, vector<Symbol *> &init) {
Parser p;
  p.initString(def->init);
  vector<Symbol *> it=p.readDeclaration("I/O");
vector<Symbol *> paramDefs = it;

  if(paramDefs.size()<init.size()) {
     paramDefs[0]->print();
    printError("Error in number of init parameters");
  }
  for(size_t i=0;i<init.size();i++) {
    if(init[i]==0) {
      init[i]=paramDefs[i];
    }
    if(paramDefs[i]->type!=init[i]->type) {
      if(paramDefs[i]->type==Symbol::REAL && init[i]->type==Symbol::INTEGER) {
        int v=init[i]->value.integer;
        init[i]=paramDefs[i];
        init[i]->value.real=v;
      } else if(paramDefs[i]->type==Symbol::INTEGER && init[i]->type==Symbol::REAL) {
        sample v=init[i]->value.integer;
        init[i]=paramDefs[i];
        init[i]->value.integer=v;
      } else {
      paramDefs[i]->print();
      init[i]->print();
      printError("Wrong type of parameter");
      }
    }
    p.assign(paramDefs[i],init[i]);
  }
  for(size_t i=init.size();i<paramDefs.size();i++) {
    init.push_back(paramDefs[i]);
  }
  if(def->body.length()==0) { //predefined dev
    p.initString(def->inputs);
    vector<Symbol *> inp=p.readDeclaration("I/O");
  p.initString(def->outputs);
  vector<Symbol *> outp=p.readDeclaration("I/O");
  vector<Symbol *> inputs=filterSymbols(inp,Symbol::REAL);
  vector<Symbol *> outputs=filterSymbols(outp,Symbol::REAL);
  vector<Symbol *> minputs=filterSymbols(inp,Symbol::MIDI);
  vector<Symbol *> moutputs=filterSymbols(outp,Symbol::MIDI);
  copyIO(inputs,obj->inputs);
  copyIO(outputs,obj->outputs);
  copyIO(minputs,obj->minputs);
  copyIO(moutputs,obj->moutputs);
  } else {
    p.initString(def->inputs);
    vector<Symbol *> inp=p.readDeclaration("input");
  p.initString(def->outputs);
  vector<Symbol *> outp=p.readDeclaration("output");
  copyIO(inp,obj->inputs);
  copyIO(outp,obj->outputs);
  }
}

Symbol *Parser::create(string typeName, string name, vector<Symbol *> init) {
  size_t initSize=init.size();
  if(typeName.compare("real")==0) {
    sample v0=0.0;
    if(initSize==0) {
      v0=0.0;
    } else if (initSize==1 && init[0]->type==Symbol::REAL) {
      v0=init[0]->value.real;
    } else {
      printError("Error initialising %s",name.c_str());
    }
    return new Symbol(name,v0);
  } else if(typeName.compare("int")==0) {
    int v0=0;
    if(initSize==0) {
      v0=0;
    } else if (initSize==1 && init[0]->type==Symbol::INTEGER) {
      v0=init[0]->value.integer;
    } else {
      printError("Error initialising %s",name.c_str());
    }
    return new Symbol(name,v0);
  } else if(typeName.compare("string")==0) {
    string v0="";
    if(initSize==0) {
      v0="";
    } else if (initSize==1 && init[0]->type==Symbol::STRING) {
      v0=*init[0]->value.str;
    } else {
      printError("Error initialising %s",name.c_str());
    }
    return new Symbol(Symbol::STRING,name,&v0);
  } else if(typeName.compare("midi")==0) {
    return new Symbol(name,(Midi *)0);
  } else if(typeName.compare("devdef")==0) {
    return new Symbol(name,(DevDef *)0);
  } else if(typeName.compare("input")==0) {
    Symbol *s=new Symbol(name, new Input());
    return s;
  } else if(typeName.compare("output")==0) {
    Symbol *s=new Symbol(name);
    s->type=Symbol::OUTPUT;
    return s;
  }
  Symbol *s=table.search(typeName);
  if(s && s->type==Symbol::DEVDEF) {
      DevObj *o=new DevObj;
    if(s->value.devDef->body.length()==0) { //predefined Dev
      o->name=name;
      o->visited=false;
     o->dynamic=s->value.devDef->dynamic;
     o->className=s->value.devDef->name;
     initParameters(s->value.devDef,o,init);
     o->connections=vector<DevObj *>(arraySize(o->inputs),0); //flatten the inputs from devDef, because in the device the inputs are in a linear array without subarrays
      o->dev=createDev(typeName,init); //in a predefined Dev all fields point to itself
      o->inputs=createObjFields(o->inputs,0,o); //se puede poner en init parameters
      o->outputs=createObjFields(o->outputs,1,o);
      o->minputs=createObjFields(o->minputs,0,o);
      o->moutputs=createObjFields(o->moutputs,1,o);
      completeFieldData(o->inputs,0);
      completeFieldData(o->outputs,0);
      completeFieldData(o->minputs,0);
      completeFieldData(o->moutputs,0);
    } else { //user defined dev
      o=createDev(name, s->value.devDef,init); //habria que  añadr init
    }
    if(typeName.compare("AlsaOut")==0) {
      asound=(AlsaOut*)o->dev;
    } else  if(typeName.compare("AlsaIn")==0) {
      aIn=(AlsaIn*)o->dev;
    } else  if(typeName.compare("AlsaSeq")==0) {
      aseq=(AlsaSeq*)o->dev;
    }

    if(o->dev==0) { 
    //  printError("%s definition not found1",typeName.c_str());
    }
    return new Symbol(name,o);
  } else {
    //table.print();
    printError("%s definition not found2",typeName.c_str());
  }
  return 0;
}


static int findInVector(vector<Symbol *> v, string s) {
  for(size_t i=0;i<v.size();i++) {
    if(s.compare(v[i]->name)==0) return i;
  }
  return -1;
}

int Parser::readSlice(int &begin, int &end) {
    int r=0; //0 element 1 slice
    expect('[');
    Symbol *index=readExpression();
    if(index->type!=Symbol::INTEGER) {
      printError("Index must be a integer");
    }
    Token *t=lookAhead();
    if(t->is(']')) {
      expect(']');
      begin=end=index->value.integer;
    } else if (t->is(':')) {
      r=1;
      expect(':');
      Symbol *index1=readExpression();
      expect(']');
      if(index1->type!=Symbol::INTEGER) {
        printError("Index must be a integer");
      }
      begin=index->value.integer;
      end=index1->value.integer;
      if(begin<0 || end<begin) {
        printf("begin %d end %d\n",begin, end);
        printError("wrong index");
      }
    } else {
      t->print();
      printError("Error reading index");
    }
    return r;
}

static Symbol *findIO(DevObj *devObj,string name){
	  int ni,no,nmi,nmo;
	  ni=findInVector(devObj->inputs,name);
	  if(ni!=-1) {
	    return devObj->inputs[ni];
	  } 
	  no=findInVector(devObj->outputs,name);
	  if (no!=-1){
	      return devObj->outputs[no];
	  }
          nmi=findInVector(devObj->minputs,name);
	  if (nmi!=-1){
	    return devObj->minputs[nmi];
	  }
          nmo=findInVector(devObj->moutputs,name);
	  if (nmo!=-1){
	    return devObj->moutputs[nmo];
	  }
   return 0;
}

Symbol *Parser::processIO(Symbol *s, string name) {
        if(s && s->type==Symbol::DEVOBJ) { // !s -> new variable
	  s=findIO(s->value.devObj,name);
	  if(s==0) printError("I/O field %s not defined1",name.c_str());
        } else if(s && s->type==Symbol::ARRAY) { // !s -> new variable
	  Array *a=new Array(s->value.array->alm.size());
          for(size_t i=0;i<s->value.array->alm.size();i++) {
	    if(s->value.array->alm[i]->type!=Symbol::DEVOBJ) {
	      s->value.array->alm[i]->print();
	      printError("DevObj espaerado");
	    }
	    DevObj *devObj=s->value.array->alm[i]->value.devObj;
	    a->alm[i]=findIO(devObj,name);
	    if(a->alm[i]==0) {
	      printError("I/O field %s not defined2",name.c_str());
	    }
	  }
	  s=new Symbol(generateName("array"),a);
        } else {
	  s->print();
          printError("%s must be a device",s->name.c_str());
	}
  return s;
}

void Parser::assignParameters(Symbol *s, vector<Symbol *>parameters) {
  if(s->type!=Symbol::DEVOBJ) {
    s->print();
    printError("error device expected");
  }

  for(size_t i=0; i<parameters.size();i++) {
    if(parameters[i]) {
      assign(s->value.devObj->inputs[i],parameters[i]);
    }
  }
}

Symbol *Parser::readVar(Token *t) {
  Symbol *s=table.search(t->name); //t could be a new variable
  if(!s) printError("Undefined identifier %s",t->name.c_str());
  t=lookAhead();
  while(t->is('.') || t->is('[') || t->is('(')) {
    if(t->is('.')) {
      expect('.');
      Token *t=nextToken();
      if(t->type==IDENTIFIER) {
        s=processIO(s,t->name);
      } else if(t->is('{')) { //array of I/O
          Symbol *array=readArray(true); //read array of identifiers
	  Array *arr=new Array(array->value.array->alm.size());
	  for(size_t i=0;i< array->value.array->alm.size();i++) {
	    arr->alm[i]=processIO(s,array->value.array->alm[i]->name);
	  }
	  s=new Symbol(generateName("array"),arr);
      } else {
        printError("I/O field name expected");
      }
    } else if(t->is('[')) { //array or slice
       int begin,end,r;
       r=readSlice(begin,end);
       if(r==0) { //index
         int index=begin;
         if(s->type==Symbol::ARRAY) { //array element
          if(index<0||index>=(int)s->value.array->alm.size()) printError("Index out of limits");
	   s=s->value.array->alm[index];
         } else {
           printError("Not indexable2");
         }
       } else { //slice
         if(s->type==Symbol::ARRAY) { //array element
          if(end>=(int)s->value.array->alm.size()) {
	    printf("end %d %d\n",end,(int)s->value.array->alm.size());
	    s->print();
	    printError("Index out of limits");
	  }
	     Array *arr=new Array(end-begin+1);
	     Symbol *s1=new Symbol(generateName("array"),arr);
	     //for(int i=0;i<arr->w;i++) {
	     for(int i=0;i<end-begin+1;i++) {
	       arr->alm[i]=new Symbol(s->value.array->alm[begin+i]);
	     }
	   //s->value.array=arr;
	   s=s1;
         } else {
	   s->print();
           printError("Not indexable4");
         }
       }
     } else if(t->is('(')) {
         expect('(');
         vector<Symbol *>parameters=readCommaSeparatedExp(')',false,true); //null expr allowed f. ex. 3,,5
         assignParameters(s,parameters);
         t=lookAhead();
     } else if(t->type==EOL) {
       printError("Unexpected end of instruction");
     }
    t=lookAhead();
  }
  return s;
}

Symbol *Parser::readExpression() {
  Symbol *a=readSum();
  Symbol *b=0;
  Token *t=lookAhead();
  if (t->is('=')) {
    expect('=');
    b=readExpression(); 
    assign(a,b);
    return b;
  } else if (t->is(CONNECT)) {
    nextToken();
    b=readExpression(); 
    connect(a,b);
    return a;
  } else {
    return a;
  }
  //return readSum();
}

Symbol *Parser::createOperation1(string op,Symbol *a, Symbol *b) { //operate with a constant
  vector<Symbol *> init; //empty vector
  init.push_back(b);
  Symbol *m=create(op,generateName(op),init);
  table.insert(m);
  Symbol *in;
             in=m->value.devObj->inputs[0];
  assign(in,a);
  return m->value.devObj->outputs[0];
}

Symbol *Parser::createOperation(string op,Symbol *a, Symbol *b) {
  vector<Symbol *> init; //empty vector
  Symbol *m=create(op,generateName(op),init);
  table.insert(m);
  Symbol *in0,*in1;
             in0=m->value.devObj->inputs[0]->value.array->alm[0];
             in1=m->value.devObj->inputs[0]->value.array->alm[1];
  assign(in0,a);
  assign(in1,b);
  return m->value.devObj->outputs[0];
}


Symbol *Parser::sum(Symbol *a, Symbol *b) {
    if(!a->isInputOrField()&& b->isInputOrField()) {
      return sum(b,a);
    } else if(a->is(Symbol::INTEGER) && b->is(Symbol::INTEGER)) {
	return new Symbol(generateName("tmp"),a->value.integer+b->value.integer);
    } else if(a->is(Symbol::REAL) && b->is(Symbol::REAL)) {
	return new Symbol(generateName("tmp"),a->value.real+b->value.real);
    } else if(a->is(Symbol::REAL) && b->is(Symbol::INTEGER)) {
	return new Symbol(generateName("tmp"),a->value.real+b->value.integer);
    } else if(a->is(Symbol::INTEGER) && b->is(Symbol::REAL)) {
      return sum(b,a);
    } else if(a->isInputOrField() && (b->is(Symbol::REAL)||b->is(Symbol::INTEGER))) {
      return createOperation1("Sum_1",a,b);
    } else if(b->isInputOrField() && a->isInputOrField()) {
      return createOperation("Sum",a,b);
    } else {
      a->print();b->print();
      printError("Incompatible types in sum");
    }
    return 0;
}

Symbol *Parser::substract(Symbol *a, Symbol *b) {
    if(a->type==Symbol::INTEGER && b->type==Symbol::INTEGER) {
	return new Symbol(generateName("tmp"),a->value.integer-b->value.integer);
    } else if(a->type==Symbol::REAL && b->type==Symbol::REAL) {
	return new Symbol(generateName("tmp"),a->value.real-b->value.real);
    } else if(a->type==Symbol::REAL && b->type==Symbol::INTEGER) {
	return new Symbol(generateName("tmp"),a->value.real-b->value.integer);
    } else if(a->type==Symbol::INTEGER && b->type==Symbol::REAL) {
	return new Symbol(generateName("tmp"),a->value.integer-b->value.real);
    } else if(a->type==Symbol::FIELD && (b->type==Symbol::REAL||b->type==Symbol::INTEGER)) {
      return createOperation1("Sub_1",a,b);
    } else if(b->type==Symbol::FIELD && a->type==Symbol::FIELD) {
      return createOperation("Sub",a,b);
    } else {
      a->print();b->print();
      printError("Incompatible types in substraction");
    }
    return 0;
}

Symbol *Parser::readSum() { //product + [product]*
  Symbol *a=readProduct();
  Token *t=lookAhead();
  while(t->is('+')||t->is('-')) {
    nextToken();
    Symbol *b=readProduct(); //must delete?
      if(t->is('+')) {
        a=sum(a,b);
      } else if (t->is('-')) {
        a=substract(a,b);
      }
    t=lookAhead();
  }
  return a;
}

Symbol *Parser::multiply(Symbol *a, Symbol *b) {
    if(!a->isInputOrField()&& b->isInputOrField()) {
      return multiply(b,a);
    } else if(a->is(Symbol::INTEGER) && b->is(Symbol::INTEGER)) {
      return new Symbol(generateName("tmp"),a->value.integer*b->value.integer);
    } else if(a->is(Symbol::REAL) && b->is(Symbol::REAL)) {
      return new Symbol(generateName("tmp"),a->value.real*b->value.real);
    } else if(a->is(Symbol::INTEGER) && b->is(Symbol::REAL)) {
      return new Symbol(generateName("tmp"),a->value.integer*b->value.real);
   } else if(a->is(Symbol::REAL) && b->is(Symbol::INTEGER)) {
       return multiply(b,a);
    } else if(a->isInputOrField() && (b->is(Symbol::REAL) || b->is(Symbol::INTEGER))) {
      return createOperation1("Mult_1",a,b);
    } else if(a->isInputOrField() && b->isInputOrField()) {
      return createOperation("Mult",a,b);
    } else if(a->is(Symbol::ARRAY) && b->is(Symbol::INTEGER)) {
      Array *r=new Array(a->value.array->alm.size()*b->value.integer);
      for(size_t i=0;i<a->value.array->alm.size()*b->value.integer;i++) {
        r->alm[i]=a->value.array->alm[i%a->value.array->alm.size()*b->value.integer];
      }
      return new Symbol(generateName("array"),r);
    } else {
      printError("Incompatible types in product");
    }
    return 0;
}

Symbol *Parser::divide(Symbol *a, Symbol *b) {
    if(a->type==Symbol::INTEGER && b->type==Symbol::INTEGER) {
      return new Symbol(generateName("tmp"),a->value.integer/b->value.integer);
    } 
    if(a->type==Symbol::REAL && b->type==Symbol::REAL) {
      return new Symbol(generateName("tmp"),a->value.real/b->value.real);
    } 
    if(a->type==Symbol::INTEGER && b->type==Symbol::REAL) {
      return new Symbol(generateName("tmp"),a->value.integer/b->value.real);
    } 
    if(a->type==Symbol::REAL && b->type==Symbol::INTEGER) {
      return new Symbol(generateName("tmp"),a->value.real/b->value.integer);
      a->value.real/=b->value.integer;
    }
    if(a->type==Symbol::FIELD && (b->type==Symbol::REAL||b->type==Symbol::INTEGER)) {
      return createOperation1("Div_1",a,b);
    } 
    if(a->type==Symbol::FIELD && b->type==Symbol::FIELD) {
      return createOperation("Div",a,b);
    }
    printError("Incompatible types in division");
    return 0;
}

Symbol *Parser::readProduct() { // value * [value]*
  Symbol *a=readValue();
  Token *t=lookAhead();

  while(t->is('*')||t->is('/')) {
    nextToken();
    Symbol *b=readValue();
    if(t->is('*')) {
    a=multiply(a,b);
    } if(t->is('/')) {
    a=divide(a,b);
    }
    t=lookAhead();
  }
  return a;
}

vector<Symbol *> Parser::readCommaSeparatedExp(char end, bool readVars, bool admitNull) {
  vector<Symbol *> a;
  Token *t=lookAhead();
  if(t->is(end)) { nextToken();return a;} ////
  while(!t->is(end)) {
    t=lookAhead();
    if(admitNull && t->is(',')) { //admitNull -> ,, is a null element
        a.push_back(0);
    } else {
        if(readVars) {
	  t=nextToken();
	  if(t->type==IDENTIFIER) {
            a.push_back(new Symbol(t->name,&t->name));
	  } else {
	    printError("Identifier expected");
	  }
	} else {  
            a.push_back(readExpression());
	  }
	}
    t=nextToken();
    if(!t->is(end) && !t->is(',')) {
      char buffer[2]={end,0};
      printError(" ',' or %s expected",buffer);
    }
  }
  return a;
}

Symbol *Parser::readArray(bool readVars) {
  vector<Symbol *> a=readCommaSeparatedExp('}',readVars);
  Array *r=new Array(a);
  return new Symbol(generateName("array"), r);
}

Symbol *Parser::mean(Symbol *a) {
  if(a->type!=Symbol::ARRAY) {
    return a;
  }
  size_t size=a->value.array->alm.size();
  if(size==0) return a;
  Symbol *r=new Symbol(a->value.array->alm[0]);
  for(size_t i=1;i<size;i++) {
    r=sum(r,a->value.array->alm[i]);
  }
  r=divide(r,new Symbol("tmp",(int)size));
  return r;
}

static Symbol *size(Symbol *a) {
  if(a->type!=Symbol::ARRAY) {
    return new Symbol("tmp",1);
  } else {
    return new Symbol("tmp",(int)a->value.array->alm.size());
  }
}

Symbol *Parser::readValue() {
  Token *t=nextToken();
  if(t->type==IDENTIFIER) {
     if(t->name.compare("mean")==0 || t->name.compare("size")==0 ) {
       expect('(');
       Symbol *s=0;
       if(t->name.compare("mean")==0 ) {
         s=mean(readExpression());
       } else if(t->name.compare("size")==0 ) {
         s=size(readExpression());
       }
       expect(')');
       return s;
     } else {
      return readVar(t); 
    }
  } else if(t->type==STRING) {
    return new Symbol(Symbol::STRING,generateName("string"),&t->name);
  } else if(t->type==NUMBER) {
      if(t->name.find(".")!=string::npos) {
        return new Symbol(generateName("real"),(sample)stod(t->name.c_str()));
      } else {
        return new Symbol(generateName("int"),atoi(t->name.c_str()));
      }
  } else if(t->is('(')) {
    Symbol *s=readExpression();
    expect(')');
    return s;
  } else if(t->is('{')) {
    Symbol *s=readArray();
    return s;
  } else if(t->is('-')) {
    return multiply(new Symbol(generateName(" -1 "),-1),readValue());
  } else {
    t->print();
    printError("bad expression");
  }
  return 0;
}

static string getTypeName(const string &id) {
  if(id.length()!=1) return "unknown";
    switch(id[0]) {
      case 'm': return "midi";
      case 'i': return "int";
      case 'r': return "real";
      case 's': return "string";
      case 'd': return "devdef";
      default: return "unknown";
    }
}

vector<Symbol *> Parser::readDeclaration(string type){ //comma separated 
  vector<int> dimensions;
  string name;
  string typeName;
  vector<Symbol *> r;
  Token *t=lookAhead();
  while(t->type!=EOL && t->type!=END) {
    if(t->type==IDENTIFIER) {
      nextToken();
      if(lookAhead()->is(':')) {
        expect(':');
        typeName=getTypeName(t->name);
	t=nextToken();
        if(t->type!=IDENTIFIER) {
	  printError("Identifier expected1");
	}
      } else {
        typeName="real";
      }
      if(type.compare("I/O")!=0){
        typeName=type;
      }
      Symbol *s=new Symbol(t->name);
      name=t->name;
      t=lookAhead();
      vector<Symbol *> init;
      if(t->is('(')){
         nextToken();
         init=readCommaSeparatedExp(')',false,true); //null expr allowed f. ex. 3,,5
      }
      t=lookAhead();
      while(t->is('[')) {
        expect('[');
        t=lookAhead();
	if(type.compare("I/O")==0 && t->is(']')) { // input y output ???
          expect(']');
          //allow [] sintaxis in I/O with dimension that are parameters of the constructor	  
	  dimensions.push_back(0); //array of 0 elements 
	} else {
          Symbol *index=readExpression();
	  if(index->type==Symbol::NEW) {
	    index=new Symbol("temp",1);
	  }
	 //printf("index %s ",name.c_str());index->print();
          expect(']');
          if(index->type!=Symbol::INTEGER) {
            printError("Index must be a integer");
           }
          dimensions.push_back(index->value.integer);
	}
        t=lookAhead();
      }
      s=create(typeName,name,dimensions,init);
      //printf("%s %s\n",typeName.c_str(),name.c_str());
      //s->print();
      r.push_back(s);
      if(t->is('=')) {
        expect('=');
	Symbol *i=readExpression();
	assign(s,i);
	/*
	if(s->type==i->type) {
	  s->value=i->value;
	} else {
	  printError("Init default value of incorrect type");
	}
	*/
	t=lookAhead();
      }
      if(t->is(',')) {
        expect(',');
	dimensions={};
        t=lookAhead();
      } else if(t->type!=EOL && t->type!=END) { 
        t->print();
        printError("Variable1 name expected");
      }
    } else {
      printError("Variable2 name expected");
    }
    t=lookAhead();
  }  
  return r;
}
/*
static string printTypes(Symbol::Type a, Symbol::Type b) {
  return symbolType2string(a) + " and " + symbolType2string(b);
}
*/
void Parser::assign(Symbol *left, Symbol *right) {
      //printf("assign %p %p\n",left,right);
      //printf("assign: ");left->print();right->print();printf("\n\n");
      if(left->type==Symbol::FIELD) {
        Field *leftField=left->value.field;
        if(right->type==Symbol::FIELD){ ///// real o midi??
	  Field *rightField=right->value.field;
          if(leftField->f->type==Symbol::REAL && rightField->f->type==Symbol::REAL) {
            if(rightField->io==1) {//assign input=output
	      Device *leftDev=leftField->devObj->dev;
	      Device *rightDev=rightField->devObj->dev;
              leftDev->connect(leftField->n,rightDev,rightField->n);
	      leftField->devObj->connections[leftField->n]=rightField->devObj; //vemos que esta conectado a ese device (para list)
	      //printf("connect %s %d %s %d\n",leftField->devObj->name.c_str(),leftField->n,rightField->devObj->name.c_str(),rightField->n);
	    } else {  //input=input  left is made to point to the same device than right
	      Device *leftDev=leftField->devObj->dev;
	      Device *rightDev=rightField->devObj->dev;
	      leftDev->inputs[leftField->n]=rightDev->inputs[rightField->n];
	      leftField->devObj->connections[leftField->n]=rightField->devObj->connections[rightField->n];
	    }
	  }  else if (leftField->f->type==Symbol::MIDI && rightField->f->type==Symbol::MIDI) {
            if(rightField->io==1) {//assign input=output
	      Device *leftDev=leftField->devObj->dev;
	      Device *rightDev=rightField->devObj->dev;
	      rightDev->connectM(leftField->n,leftDev,rightField->n);
	    } else {
	      //printf("assign: ");left->print();right->print();printf("\n\n");
	      printError("solo se puede conectar input a output");
	    }
	  } else {
	    left->print();
	    right->print();
            printError("field error1");
	  }
        } else if(right->type==Symbol::REAL){
          if(leftField->io==1) { // output = const
	    Device *leftDev=leftField->devObj->dev;
	    leftDev->setOutput(leftField->n,right->value.real);
	  } else  { 
	    assign(left,createConstant(right->value.real)->value.devObj->outputs[0]);
	  }
        } else if(right->type==Symbol::INTEGER){
          if(leftField->io==1) { // output = const
	    Device *leftDev=leftField->devObj->dev;
	    leftDev->setOutput(leftField->n,right->value.integer);
	  } else  {
	    assign(left,createConstant(right->value.integer)->value.devObj->outputs[0]);
	  }
        } else if(right->type==Symbol::INPUT){
	  //left is a field. We insert left in the set of inputs of right  
	  right->value.input->alm.push_back(left);
	  //left->name=right->name;
	  //right=left;
	  //table.insert(right);
        } else { //error
	  left->print();
	  right->print();
          printError("field error2");
        }
      } else if(left->type==Symbol::ARRAY) {
        if(right->type==Symbol::ARRAY) {
	  if(left->value.array->alm.size()==right->value.array->alm.size()) {
	    for(size_t i=0; i<left->value.array->alm.size();i++) {
	      assign(left->value.array->alm[i],right->value.array->alm[i]);
	    }
	  } else if(left->value.array->alm.size()==0){ //array of size==0 -> init parameter
	    left->type=Symbol::NEW;
	    assign(left,right);
	  } else {
	    left->print();
	    right->print();
	    char buffer[100];
	    sprintf(buffer,"Arrays of different sizes %ld and %ld",left->value.array->alm.size(),right->value.array->alm.size());
	    printError(buffer);
	  }
	} else {
	  //printError("Assigning values of different type. %s",printTypes(left->type,right->type).c_str());
	    for(size_t i=0; i<left->value.array->alm.size();i++) {
	      assign(left->value.array->alm[i],right);
	    }
	}
      } else if(left->type==Symbol::INPUT) {
	for(size_t i=0;i<left->value.input->alm.size();i++) {
	  assign(left->value.input->alm[i],right);
	}
      } else  if(left->type==right->type || left->type==Symbol::NEW) {
	  //printf("normal assign antes ");
	  //left->print();
	  //right->print();
	  //printf("\n");
	  left->value=right->value;
	  left->type=right->type;
	  //right->print();
	  //printf("\n\n\n");
	  table.insert(left);
      } else if(left->type==Symbol::OUTPUT) {
	  left->value=right->value;
	  left->type=right->type;
      } else { 
	  left->print();
	  right->print();
	  printError("Assigning values of different type");
      }
}

void Parser::connect(Symbol *left, Symbol *right) {
  if(left->type!=Symbol::OUTPUT && left->type!=Symbol::FIELD) {
    left=processIO(left,"in");
  }
  if(right->type!=Symbol::INPUT && right->type!=Symbol::FIELD) {
    right=processIO(right,"out");
  }
  assign(left,right);
}

void Parser::nextLine() {
  nLine++;
}

void Parser::jumpBlanks() {
  while(text[textIndex] && isSpace(text[textIndex])) textIndex++;
}

string Parser::readLine() {
  size_t p1=errorIndex; //because of lookAhead
  Token *t=nextToken();
  while(!t->is(EOL)) {
    t=nextToken();
  }
  size_t p2=textIndex;
  return text.substr(p1,p2-p1);
}

string Parser::readIdentifier() {
  size_t i=textIndex;
  while(isalnum(text[textIndex]) || text[textIndex]=='_') textIndex++;
  return text.substr(i,textIndex-i);
}

string Parser::readNumber() {
  size_t i=textIndex;
  while(isdigit(text[textIndex])|| text[textIndex]=='.' ) textIndex++;
  return text.substr(i,textIndex-i);
}

string Parser::readChar() {
  size_t i=textIndex;
  textIndex++;
  return text.substr(i,1);
}

void Parser::readComment(char first) {
  if(first=='/') {
    while(text[textIndex] && text[textIndex]!='\n') textIndex++;
    textIndex++; //go past the '\n'
    nextLine();
  } else if (first=='*') {
    textIndex++;
    do {
      textIndex++;
      if(text[textIndex]=='\n') {
        nextLine();
      }
    } while(text[textIndex]!='*' || text[textIndex+1]!='/'); 
    textIndex+=2;
  }
}

string Parser::readString() {
  char delim=text[textIndex++];
  string s;
  while(text[textIndex]!=delim) {
    if(text[textIndex]=='\\') {
      textIndex++;
    }
    s = s + text[textIndex];
    textIndex++;
  }
  textIndex++;
  return s;
}

Token *Parser::nextToken() {
  Token *r;
  
  bool ok;
    errorIndex=textIndex;
  do {
    ok=true;
    jumpBlanks();
    if(isalpha(text[textIndex])) r=new Token(IDENTIFIER,readIdentifier());
    else if(isdigit(text[textIndex])) r=new Token(NUMBER,readNumber());
    else if(text[textIndex]==0) {r=new Token(END,"");}
    else if(text[textIndex]=='\n') {nextLine();textIndex++;r=new Token(EOL,"\n");}
    else if(text[textIndex]=='\'' || text[textIndex]=='"') {r=new Token(STRING,readString());}
    else if(text[textIndex]==';') {textIndex++;r=new Token(EOL,";");}
    else if(text[textIndex]=='/' && (text[textIndex+1]=='/' || text[textIndex+1]=='*')){readComment(text[textIndex+1]);ok=false;}
    else if(text[textIndex]=='<' && text[textIndex+1]=='<'){textIndex+=2; r=new Token(CONNECT,"<<");}
    else if(isSpecial(text[textIndex])) r=new Token(SPECIAL,readChar());
    else if(text[textIndex]==EOF) r=new Token(END,"");
    else { r=new Token(UNKNOWN,"");printf("UNKNOWN %c\n",text[textIndex]);}
  } while (!ok);
  Token *r1=nextT;
  nextT=r;
  /*
  if(r1){
  printf("line %d ",nLine);r1->print();nextT->print();
  }
*/
  return r1;
}
