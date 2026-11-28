#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<malloc.h>
#include"keye.h"

unsigned char err=0;

KeyeTok* lex(const char* code) {
 const int len=strlen(code);
 unsigned int ti=0;
 unsigned int cap=16;
 KeyeTok* res=malloc(cap*sizeof(KeyeTok));
 char* word=alloca((len+1)*sizeof(char));
 unsigned int wi=0;
 for (int i=0;i<len;i++) {
  if (isalpha(code[i]) || code[i]=='_') {
   for (;i<len && (isalnum(code[i]) || code[i]=='_');i++) {
    word[wi]=code[i];
    wi++;
   }
   word[wi]=0;
   if (streq(word,"let")) {
    res[ti]=(KeyeTok){.type=TokLet,.sval=0,.ival=0};
   } else if (streq(word,"if")) {
    res[ti]=(KeyeTok){.type=TokIf,.sval=0,.ival=0};
   } else if (streq(word,"else")) {
    res[ti]=(KeyeTok){.type=TokElse,.sval=0,.ival=0};
   } else if (streq(word,"while")) {
    res[ti]=(KeyeTok){.type=TokWhile,.sval=0,.ival=0};
   } else if (streq(word,"for")) {
    res[ti]=(KeyeTok){.type=TokFor,.sval=0,.ival=0};
   } else if (streq(word,"fn")) {
    res[ti]=(KeyeTok){.type=TokFn,.sval=0,.ival=0};
   } else if (streq(word,"return")) {
    res[ti]=(KeyeTok){.type=TokReturn,.sval=0,.ival=0};
   } else {
    res[ti]=(KeyeTok){.type=TokName,.sval=strdup(word),.ival=0};
   }
   ti++;
   if (ti>=cap) {
    cap*=2;
    res=realloc(res,cap*sizeof(KeyeTok));
   }
   word[0]=0;
   wi=0;
  } else if (isdigit(code[i])) {
   for (;i<len && isdigit(code[i]);i++) {
    word[wi]=code[i];
    wi++;
   }
   word[wi]=0;
   res[ti]=(KeyeTok){.type=TokNum,.sval=0,.ival=atoi(word)};
   ti++;
   if (ti>=cap) {
    cap*=2;
    res=realloc(res,cap*sizeof(KeyeTok));
   }
   word[0]=0;
   wi=0;
  } else if (code[i]=='"') {
   i++;
   char c=1;
   for (;i<len;i++) {
    if (code[i]=='"') c--;
    if (c==0) break;
    word[wi]=code[i];
    wi++;
   }
   if (c!=0) {
    printf("Error while reading string");
    err=1;
    break;
   }
   word[wi]=0;
   res[ti]=(KeyeTok){.type=TokNum,.sval=strdup(word),.ival=0};
   ti++;
   if (ti>=cap) {
    cap*=2;
    res=realloc(res,cap*sizeof(KeyeTok));
   }
   word[0]=0;
   wi=0;
  }

  if (i<len) {
   if (code[i]==';') {
    res[ti]=(KeyeTok){.type=TokEOL,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='(') {
    res[ti]=(KeyeTok){.type=TokSExpr,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]==')') {
    res[ti]=(KeyeTok){.type=TokEExpr,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='{') {
    res[ti]=(KeyeTok){.type=TokSBlock,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='}') {
    res[ti]=(KeyeTok){.type=TokEBlock,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]==':') {
    i++;
    if (i>=len) {
     printf("Unexpected end of file\n");
     err=1;
     break;
    }
    if (code[i]=='=') {
     res[ti]=(KeyeTok){.type=TokAssign,.sval=0,.ival=0};
     ti++;
     if (ti>=cap) {
      cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
     }
    } else {
     printf("Expected '='\n");
     err=1;
     break;
    }
   } else if (code[i]=='+') {
    if (i+1<len && code[i+1]=='+') {
     i++;
     res[ti]=(KeyeTok){.type=TokInc,.sval=0,.ival=0};
    }
    else res[ti]=(KeyeTok){.type=TokAdd,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='-') {
    if (i+1<len && code[i+1]=='-') {
     i++;
     res[ti]=(KeyeTok){.type=TokDec,.sval=0,.ival=0};
    } else if (i+1<len && isdigit(code[i+1])) {
     i++;
     word[0]='-';
     wi=1;
     for (;i<len && isdigit(code[i]);i++) {
      word[wi]=code[i];
      wi++;
     }
     word[wi]=0;
     res[ti]=(KeyeTok){.type=TokNum,.sval=0,.ival=atoi(word)};
     ti++;
     if (ti>=cap) {
      cap*=2;
      res=realloc(res,cap*sizeof(KeyeTok));
     }
     wi=0;
     word[wi]=0;
    }
    else res[ti]=(KeyeTok){.type=TokSub,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='*') {
    if (i+1<len && code[i+1]=='/') {
     printf("Unexpected '*/'\n");
     err=1;
     break;
    }
    res[ti]=(KeyeTok){.type=TokMul,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='/') {
    if (i+1<len && code[i+1]=='/') {
     for (;i<len && code[i]!='\n';i++);
    } else if (i+1<len && code[i+1]=='*') {
     i++;
     for (int c=0;i<len;i++) {
      if (code[i]=='"') {
       if (c==0) c++;
       else c--;
      }
      if (code[i]=='*')
       if (i+1<len && code[i+1]=='/' && c==0) break;
     }
     i++;
    } else {
     res[ti]=(KeyeTok){.type=TokDiv,.sval=0,.ival=0};
     ti++;
     if (ti>=cap) {
      cap*=2;
      res=realloc(res,cap*sizeof(KeyeTok));
     }
    }
   } else if (code[i]=='%') {
    res[ti]=(KeyeTok){.type=TokMod,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='=') {
    res[ti]=(KeyeTok){.type=TokEqu,.sval=0,.ival=0};
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='!') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     res[ti]=(KeyeTok){.type=TokNotEqu,.sval=0,.ival=0};
    } else {
     res[ti]=(KeyeTok){.type=TokNot,.sval=0,.ival=0};
    }
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='<') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     res[ti]=(KeyeTok){.type=TokLessEqu,.sval=0,.ival=0};
    } else {
     res[ti]=(KeyeTok){.type=TokLess,.sval=0,.ival=0};
    }
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   } else if (code[i]=='>') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     res[ti]=(KeyeTok){.type=TokMoreEqu,.sval=0,.ival=0};
    } else {
     res[ti]=(KeyeTok){.type=TokMore,.sval=0,.ival=0};
    }
    ti++;
    if (ti>=cap) {
     cap*=2;
     res=realloc(res,cap*sizeof(KeyeTok));
    }
   }
  }
 }
 res=realloc(res,(ti+1)*sizeof(KeyeTok));
 res[ti]=(KeyeTok){.type=TokEOT,.sval=0,.ival=0};
 return res;
}

KeyeAst* parse(const KeyeTok* tokens) {
 unsigned int cap=8;
 KeyeAst* res=malloc(cap*sizeof(KeyeAst));
 unsigned int ri=0;
 unsigned int len=tokenlen(tokens);
 for (int i=0;i<len;i++) {
  ttprint(tokens[i]);
  if (tokens[i].sval!=0) {
   printf("%s ",tokens[i].sval);
   free(tokens[i].sval);
  }
  printf("%d\n",tokens[i].ival);
 }
 res=realloc(res,(ri+1)*sizeof(KeyeAst));
 res[ri]=(KeyeAst){.type=AstEOA,.sval=0,.ival=0,.aval=0};
 return res;
}

int main() {
 FILE* fin=fopen("main.keye","r");
 if (fin==NULL) return -1;
 fseek(fin,0,2);
 unsigned int fsize=ftell(fin);
 fseek(fin,0,0);
 char buf[fsize+1];
 fread(buf,fsize,sizeof(char),fin);
 buf[fsize]=0;
 fclose(fin);
 KeyeTok *tokens=lex(buf);
 if (err) {
  for (unsigned int i=0;tokens[i].type!=TokEOT;i++) {
   free(tokens[i].sval);
  }
  free(tokens);
  return err;
 }
 err=0;
 KeyeAst* ast=parse(tokens);
 free(tokens);
 free(ast);
 return err;
}
