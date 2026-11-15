#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX_LENGTH 1024
#define MAX_STRING_LENGTH 256

typedef enum {
 TokEOT,
 TokEOL,
 TokSep,

 TokStr,
 TokNum,
 TokName,

 TokAssign,

 TokSExpr,
 TokEExpr,
 TokSBlock,
 TokEBlock,

 TokAdd,
 TokSub,
 TokMul,
 TokDiv,
 TokMod,

 TokEqu,
 TokNot,
 TokNotEqu,
 TokLess,
 TokLessEqu,
 TokMore,
 TokMoreEqu,

 TokVar,
 TokConst,
 TokIf,
 TokElse,
 TokWhile,
 TokFn,
} TokType;

typedef struct {
 TokType type;
 unsigned int ival;
 char sval[MAX_STRING_LENGTH];
} KeyeTok;

void print_toktype(TokType tt) {
 switch (tt) {
  //Misc
  case TokEOT: printf("End of token"); break;
  case TokEOL: printf("End of line"); break;
  case TokSep: printf(";"); break;

  // Data
  case TokStr: printf("String"); break;
  case TokNum: printf("Number"); break;
  case TokName: printf("Name"); break;

  // Assign
  case TokAssign: printf(":="); break;

  // Expr
  case TokSExpr: printf("("); break;
  case TokEExpr: printf(")"); break;

  // Block
  case TokSBlock: printf("{"); break;
  case TokEBlock: printf("}"); break;

  // Keyword
  case TokVar: printf("Var"); break;
  case TokConst: printf("Const"); break;
  case TokIf: printf("If"); break;
  case TokElse: printf("Else"); break;
  case TokWhile: printf("While"); break;
  case TokFn: printf("Fn"); break;

  // Operation
  case TokAdd: printf("+"); break;
  case TokSub: printf("-"); break;
  case TokMul: printf("*"); break;
  case TokDiv: printf("/"); break;
  case TokMod: printf("%%"); break;

  // Comparison
  case TokEqu: printf("="); break;
  case TokNot: printf("!"); break;
  case TokLess: printf("<"); break;
  case TokMore: printf(">"); break;
  case TokNotEqu: printf("!="); break;
  case TokLessEqu: printf("<="); break;
  case TokMoreEqu: printf(">="); break;

  // Error
  default: printf("Unknown: %d",tt); break;
 }
 printf(" ");
 return;
}

unsigned int atoui(const char* a) {
 unsigned int ui=0;
 for (int i=0;a[i]!=0;i++) {
  ui*=10;
  ui+=a[i]-'0';
 }
 return ui;
}

char streq(const char* a,const char* b) {
 if (strlen(a)!=strlen(b)) return 0;
 for (int i=0;a[i]!=0;i++)
  if (a[i]!=b[i]) return 0;
 return 1;
}

void lex(const char* code,KeyeTok* tokens) {
 const int len=strlen(code);
 unsigned int ti=0;
 char word[MAX_STRING_LENGTH]={0};
 unsigned int wi=0;
 for (int i=0;i<len;i++) {
  if (isalpha(code[i]) || code[i]=='_') {
   for (;i<len && (isalnum(code[i]) || code[i]=='_');i++) {
    word[wi]=code[i];
    wi++;
   }
   word[wi]=0;
   if (streq(word,"var")) {
    tokens[ti]=(KeyeTok){.type=TokVar,.sval=0,.ival=0};
   } else if (streq(word,"const")) {
    tokens[ti]=(KeyeTok){.type=TokConst,.sval=0,.ival=0};
   } else if (streq(word,"if")) {
    tokens[ti]=(KeyeTok){.type=TokIf,.sval=0,.ival=0};
   } else if (streq(word,"else")) {
    tokens[ti]=(KeyeTok){.type=TokElse,.sval=0,.ival=0};
   } else if (streq(word,"while")) {
    tokens[ti]=(KeyeTok){.type=TokWhile,.sval=0,.ival=0};
   } else if (streq(word,"fn")) {
    tokens[ti]=(KeyeTok){.type=TokFn,.sval=0,.ival=0};
   } else {
    tokens[ti]=(KeyeTok){.type=TokName,.sval=0,.ival=0};
    strcpy(tokens[ti].sval,word);
   }
   ti++;
   word[0]=0;
   wi=0;
  } else if (isdigit(code[i])) {
   for (;i<len && isdigit(code[i]);i++) {
    word[wi]=code[i];
    wi++;
   }
   word[wi]=0;
   tokens[ti]=(KeyeTok){.type=TokNum,.sval=0,.ival=atoui(word)};
   ti++;
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
    tokens[0]=(KeyeTok){.type=TokEOT,.sval=0,.ival=0};
    break;
   }
   word[wi]=0;
   tokens[ti]=(KeyeTok){.type=TokNum,.sval=0,.ival=0};
   strcpy(tokens[ti].sval,word);
   ti++;
   word[0]=0;
   wi=0;
  }

  if (i<len) {
   if (code[i]==';') {
    tokens[ti]=(KeyeTok){.type=TokEOL,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='(') {
    tokens[ti]=(KeyeTok){.type=TokSExpr,.sval=0,.ival=0};
    ti++;
   } else if (code[i]==')') {
    tokens[ti]=(KeyeTok){.type=TokEExpr,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='{') {
    tokens[ti]=(KeyeTok){.type=TokSBlock,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='}') {
    tokens[ti]=(KeyeTok){.type=TokEBlock,.sval=0,.ival=0};
    ti++;
   } else if (code[i]==',') {
    tokens[ti]=(KeyeTok){.type=TokSep,.sval=0,.ival=0};
    ti++;
   } else if (code[i]==':') {
    i++;
    if (i>=len) {
     printf("Unexpected end of file\n");
     ti=0;
     break;
    }
    if (code[i]=='=') {
     tokens[ti]=(KeyeTok){.type=TokAssign,.sval=0,.ival=0};
     ti++;
    } else {
     printf("Expected '=', got '%c'\n",code[i]);
     ti=0;
     break;
    }
   } else if (code[i]=='+') {
    tokens[ti]=(KeyeTok){.type=TokAdd,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='-') {
    tokens[ti]=(KeyeTok){.type=TokSub,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='*') {
    if (i+1<len && code[i+1]=='/') {
     printf("Unexpected '*/'\n");
     ti=0;
     break;
    }
    tokens[ti]=(KeyeTok){.type=TokMul,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='/') {
    if (i+1<len && code[i+1]=='/') {
     for (;i<len && code[i]!='\n';i++);
    } else if (i+1<len && code[i+1]=='*') {
     i++;
     for (int c=1;i<len;i++) {
      if (code[i]=='"') {
       if (c==0) c++;
       else c--;
      }
      if (code[i]=='*')
       if (i+1<len && code[i+1]=='/' && c==0) break;
     }
    } else {
     tokens[ti]=(KeyeTok){.type=TokDiv,.sval=0,.ival=0};
     ti++;
    }
   } else if (code[i]=='%') {
    tokens[ti]=(KeyeTok){.type=TokMod,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='=') {
    tokens[ti]=(KeyeTok){.type=TokEqu,.sval=0,.ival=0};
    ti++;
   } else if (code[i]=='!') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     tokens[ti]=(KeyeTok){.type=TokNotEqu,.sval=0,.ival=0};
    } else {
     tokens[ti]=(KeyeTok){.type=TokNot,.sval=0,.ival=0};
    }
    ti++;
   } else if (code[i]=='<') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     tokens[ti]=(KeyeTok){.type=TokLessEqu,.sval=0,.ival=0};
    } else {
     tokens[ti]=(KeyeTok){.type=TokLess,.sval=0,.ival=0};
    }
    ti++;
   } else if (code[i]=='>') {
    if (i+1<len && code[i+1]=='=') {
     i++;
     tokens[ti]=(KeyeTok){.type=TokMoreEqu,.sval=0,.ival=0};
    } else {
     tokens[ti]=(KeyeTok){.type=TokMore,.sval=0,.ival=0};
    }
    ti++;
   }
  }
 }
 tokens[ti]=(KeyeTok){.type=TokEOT,.sval=0,.ival=0};
 return;
}

void parse(const KeyeTok* tokens) {
 for (int i=0;i<MAX_LENGTH;i++) {
  if (tokens[i].type==TokEOT) break;
  print_toktype(tokens[i].type);
  if (tokens[i].sval[0]!=0)
   printf("%s ",tokens[i].sval);
  printf("%u\n",tokens[i].ival);
 }
 return;
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
 KeyeTok tokens[MAX_LENGTH];
 lex(buf,tokens);
 parse(tokens);
 return 0;
}
