#ifndef __KEYE_H__
#define __KEYE_H__

typedef enum {
 TokEOT=0,
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
 TokInc,
 TokSub,
 TokDec,
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

 TokIf,
 TokElse,
 TokWhile,
 TokFor,
 TokFn,
 TokReturn,
} TokType;

typedef struct {
 TokType type;
 int ival;
 char* sval;
} KeyeTok;

void ttprint(KeyeTok token) {
 switch (token.type) {
  case TokEOT: printf("End of token"); break;
  case TokEOL: printf(";"); break;
  case TokSep: printf(","); break;

  case TokStr: printf("String"); break;
  case TokNum: printf("Number"); break;
  case TokName: printf("Name"); break;

  case TokAssign: printf(":="); break;

  case TokSExpr: printf("("); break;
  case TokEExpr: printf(")"); break;

  case TokSBlock: printf("{"); break;
  case TokEBlock: printf("}"); break;

  case TokIf: printf("If"); break;
  case TokElse: printf("Else"); break;
  case TokWhile: printf("While"); break;
  case TokFor: printf("For"); break;
  case TokFn: printf("Fn"); break;
  case TokReturn: printf("Return"); break;

  case TokAdd: printf("+"); break;
  case TokInc: printf("++"); break;
  case TokSub: printf("-"); break;
  case TokDec: printf("--"); break;
  case TokMul: printf("*"); break;
  case TokDiv: printf("/"); break;
  case TokMod: printf("%%"); break;

  case TokEqu: printf("="); break;
  case TokNot: printf("!"); break;
  case TokLess: printf("<"); break;
  case TokMore: printf(">"); break;
  case TokNotEqu: printf("!="); break;
  case TokLessEqu: printf("<="); break;
  case TokMoreEqu: printf(">="); break;

  default: printf("Unknown: %d",token.type); break;
 }
 printf(" ");
 return;
}

unsigned int tokenlen(const KeyeTok* tokens) {
 for (unsigned int i=0;;i++)
  if (tokens[i].type==TokEOT) return i;
}

char streq(const char* a,const char* b) {
 if (strlen(a)!=strlen(b)) return 0;
 for (int i=0;a[i]!=0;i++)
  if (a[i]!=b[i]) return 0;
 return 1;
}

typedef enum {
 AstEOA=0,
 AstStr,
 AstNum,
 AstCall,
} AstType;

typedef struct {
 AstType type;
 unsigned int ival;
 char* sval;
 struct KeyeAst* aval;
} KeyeAst;

#endif
