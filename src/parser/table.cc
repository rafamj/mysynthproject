//table.cc
#include "table.h"

void DevDef::print() {
  printf("devdef %s\n",name.c_str());
  printf("init %s\n",init.c_str());
  printf("inputs %s\n",inputs.c_str());
  printf("outputs %s\n",outputs.c_str());
  printf("\n");
}


void Symbol::print(){
  printf("symbol  %s ",name.c_str());
  if(type==INTEGER) {
     printf("int %d \n",value.integer);
  } else if(type==REAL) {
     printf("real  %f \n",value.real);
  } else if(type==MIDI) {
     printf("midi   \n");
  } else if(type==OUTPUT) {
     printf("output   \n");
  } else if(type==INPUT) {
     printf("input elementos %ld   \n",value.input->alm.size());
     for(size_t i=0;i<value.input->alm.size();i++) {
       printf("    %ld ",i);value.input->alm[i]->print();
     }
  } else if(type==DEVOBJ){
     if(value.devObj) {
       printf("devobj  %p %s connections:\n",value.devObj,value.devObj->className.c_str());
       for(size_t i=0;i<value.devObj->connections.size();i++) {
         if(value.devObj->connections[i]) printf("   %s\n",value.devObj->connections[i]->name.c_str());
       }
     } else {
       printf("devobj  %p \n",value.devObj);
     }
  } else if(type==DEVDEF){
     if(value.devDef) {
       value.devDef->print();
     } else {
       printf("devdef (null)\n");
     }
  } else if(type==FIELD){
    printf("field   n %d  io %d devObj %p %s className %s f ",value.field->n,value.field->io, value.field->devObj,value.field->devObj->name.c_str(),value.field->devObj->className.c_str());
    if(value.field->f) value.field->f->print();
    else printf("no f\n");
    printf("\n");
  } else if(type==ARRAY){
    printf("array  elements %ld \n",value.array->alm.size());
    for(size_t i=0;i<value.array->alm.size();i++) {
      if(value.array->alm[i]) {printf("%ld ",i);value.array->alm[i]->print();}
    }
  } else if(type==STRING){
    printf(" string '%s'\n",value.str->c_str());
  } else {
    printf(" type %d\n",type);
  }
}

void Table::insert(Symbol *s) {
  //s->print(-1);
  for(int i=0; i<MAX_TABLE; i++) {
    if(alm[i] && alm[i]->name.compare(s->name)==0){
  //printf("redefine %d %s\n",i,s->name.c_str());
      alm[i]=s;
      return;
    } else if(alm[i]==0) {
  //printf("insert %d %s\n",i,s->name.c_str());
      alm[i]=s;
      return;
    }
  }
}

Symbol *Table::search(string n){
  for(int i=0; i<MAX_TABLE; i++) {
    if(alm[i] && alm[i]->name.compare(n)==0) return alm[i];
  }
  return new Symbol(n);
}

vector<Symbol *> Table::searchDevObj(string type) {
  vector<Symbol *> r;
  for(int i=0; i<MAX_TABLE; i++) {
    if(alm[i] && alm[i]->type==Symbol::DEVOBJ && alm[i]->value.devObj->className.compare(type)==0) {
      r.push_back(alm[i]);
    }  
  }
  return r;
}
/*
Symbol *Table::search(DevObj *dev){
  for(int i=0; i<MAX_TABLE; i++) {
    if(alm[i] && alm[i]->type==Symbol::DEVOBJ && alm[i]->value.devObj==dev) return alm[i];
  }
  {
    char buffer[100];
    sprintf(buffer,"%p not in table",dev);
  return new Symbol(buffer,dev);
  }
}
*/
