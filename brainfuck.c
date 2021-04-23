#include<stdio.h>
#include<stdlib.h>
#define MEM_LEN 3000

void except(char* source,int e){
  free(source);
  if(e>=0) exit(e);
}

int interpret(FILE* file){
  char ch;
  int mem=0;
  long unsigned int code=0;
  char memory[MEM_LEN+1];
  fseek(file,0,SEEK_END);
  long unsigned int source_len=ftell(file);
  char* source=(char*)malloc(source_len);
  rewind(file);
  fread(source, 1, source_len, file);
  fclose(file);
  while(code<source_len){
    char c=source[code];
    if(c=='>' && (++mem)==MEM_LEN) mem=0;
    else if(c=='<' && !(mem--)) mem=MEM_LEN-1;
    else if(c=='.') putc(memory[mem], stdout);
    else if(c==',') memory[mem]=fgetc(stdin);
    else if(c=='+') memory[mem]++;
    else if(c=='-') memory[mem]--;
    else if(c=='[' && !memory[mem]){
      int open=1;
      while(open){
        if(++code==source_len) except(source,0);
        if(source[code]=='[') open++;
        if(source[code]==']') open--;
      }
    }else if(c==']' && memory[mem]){
      int close=1;
      while(close){
        if(--code==-1) except(source,0);
        if(source[code]==']') close++;
        if(source[code]=='[') close--;
      }
    }else if(!(c=='\t' || c=='\n' || c==' ' || c=='<' || c=='>' || c=='[' || c==']')){
      fprintf(stderr,"Error on %lu: Unexpected Character (%c)\n",code, c);
      except(source,1);
    }
    code++;
  }
  return 0;
}

int main(int argc,char** argv){
  if(argc!=2){
    fputs("Usage: brainfuck file\n", stderr);
    exit(1);
  }
  FILE* file=fopen(argv[1], "r");

  if(!file){
    fputs("Could not open file! Check if the file exists\n", stderr);
    exit(1);
  }
  return interpret(file);
}