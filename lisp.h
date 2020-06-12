/*

 "lisp.h" LISP処理系のヘッダファイル
   for CASSIOPEIA A-51

   Cコンパイラの処理系の中核となる

*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "conslib.h"

/* 定数 */
#define TRUE            1
#define FALSE           0
#define NaN             0x80000000

/* トークンID */
#define EOF             -1
#define END             0   /* リストの終わり */
#define XBADTOKEN       1   /* 不正なtoken */
#define LPAREN          2   /* 左カッコ */
#define RPAREN          3   /* 右カッコ */
#define SYMBOL          4
#define NUMBER          5
#define LIST            6
#define ALLTYPE         7
#define REAL            8
#define NEWLIST         9
#define STRING          10

/* 変数の検索モード */
#define NARROW_MODE     0  /* 与えられた名前空間だけを検索する */
#define WIDE_MODE       1  /* 親の名前空間も検索する */

/* 環境のタイプ */
#define TYPE_BLOCK      0 /* ブロック用 */
#define TYPE_FUNCTION   1 /* 関数用 */


/* リスト */
struct _list {
  int type;    /* リストの保持するデータの型： 記号(変数名/関数名) | 数値 | リスト(へのポインタ) */
  union {
    char*  symbol;
    char*  string;
    int    integer;
    float  real;
    struct _list *list;
  } value;
  struct _list *next;
};
typedef struct _list List;
typedef List* ListPtr;


/* 変数 */
struct _var {
  int   type;
  char* symbol;
  union{
    int   number;
    List* list;
  } value;
  struct _var *next;
  struct _var *prev;
};
typedef struct _var Variable;


/* 環境 */
struct _env {
  int         type;
  int         size;
  int         ID;
  Variable*   var_top;
  Variable*   var_end;
  struct _env *parent; /* 親となる環境 */
};
typedef struct _env Environment;


/* プロトタイプ宣言 */
List* createList( List* list );
void deleteList( List* list );
int getListLength( List* list );
List* _concat( List* list, ... );
List* _list( List* list, ... );
List* _symbol( char* symbol );
List* _string( char* string );
List* _integer( int n );
List* _real( double r );
List* list( List* list );
List* addSymbol( List* list, char* symbol );
List* addInteger( List* list, int n );
List* addReal( List* list, double r );
List* addList( List* list1, List* list2 );
List* concatLists( List* list1, List* list2 );
int getInteger( List* list );
char* getSymbol( List* list );
List* getList( List* list );
List* getListEnd( List* list );
void printList( List* list );
void printValue( List* list );
Environment* createEnvironment( Environment* parent, int type );
Environment* createEnvironment2( Environment* parent, int type, int ID );
void deleteEnvironment( Environment* env );
int getVariableType( Environment* env, char* symbol );
int getIntegerVariable( Environment* env, char* symbol, int mode );
List* getListVariable( Environment* env, char* symbol, int mode );
Variable* getVariable( Environment* env, char* symbol, int type, int mode );
Variable* createIntegerVariable( Environment* env, char* symbol, int value );
Variable* createNewListVariable( Environment* env, char* symbol, List* value );
Variable* createListVariable( Environment* env, char* symbol, List* value );
int evalList( Environment* env, List* list );
int evalPlus( Environment* env, List* list );
int evalMinus( Environment* env, List* list );
int evalMul( Environment* env, List* list );
int evalDiv( Environment* env, List* list );
int evalEQ( Environment* env, List* list );
int evalNE( Environment* env, List* list );
int evalLT( Environment* env, List* list );
int evalLE( Environment* env, List* list );
int evalGT( Environment* env, List* list );
int evalGE( Environment* env, List* list );
int evalDefineVariable( Environment* env, List* list );
int evalSetVariable( Environment* env, List* list );
int evalDefineFunction( Environment* env, List* list );
int evalIf( Environment* env, List* list );
int evalFunction( Environment* env, List* list );
int evalOperand( Environment* env, List* operand );
int printOperand( Environment* env, List* operand );
List* inputList();
List* inputList2( List* list);
int newProgram();
int runProgram();
int quitProgram();
int editProgram();
int listProgram();
int loadProgram( List* list );
int saveProgram( List* list );
int getLispToken();
int isTokenMatch( char* str );
void xputc( char c );
char xgetc();
void xungetc( char c );
void xopenInputFile( char* file_name );
void xcloseInputFile();
void xopenOutputFile( char* file_name );
void xcloseOutputFile();
int isZenkaku(char c);


/* 変数宣言 */
TCHAR        AppName[] = TEXT("Console"); /* 必須項目 */
TCHAR        msg[256], tstr[256];
int          QuitFlg = 0, UngetP = 0;
char         TokenTextBuf[1024], UngetBuf[1024];
char*        TokenText = TokenTextBuf;
List*        TokenValue;
HANDLE       OutputFile = INVALID_HANDLE_VALUE;
HANDLE       InputFile  = INVALID_HANDLE_VALUE;
char         LastInputChar = '\0';
List*        Program = NULL;
Environment* RootEnvironment = NULL;
int          Ret;        /* 関数の戻り値を記憶しておく(デバッグ用) */


/* 関数宣言 */


/* パラメータのリストを結合する */
List* _concat( List* list, ... ){
  va_list newlists;
  List*   newlist;

  if( list == NULL ) return NULL;
  va_start( newlists, list );
  while( ( newlist = va_arg( newlists , List* ) ) != NULL ){
    list = concatLists( list, newlist );
  }
  va_end( newlists );
  return list;
}


/* LISPの式を生成する */
List* _list( List* list, ... ){
  va_list newlists;
  List*   newlist;

  if( list == NULL ) return createList(NULL);
  va_start( newlists, list );
  while( ( newlist = va_arg( newlists , List* ) ) != NULL ){
    list = concatLists( list, newlist );
  }
  va_end( newlists );
  return createList( list );
}


/* シンボルを要素とする新しいリストを生成する */
List* _symbol( char* symbol ){
  char* newsymbol;
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = SYMBOL;
  newsymbol = malloc( strlen( symbol ) + 1 );
  strcpy( newsymbol, symbol );
  newlist->value.symbol = newsymbol;
  newlist->next = NULL;
  return newlist;
}


/* 文字列を要素とする新しいリストを生成する */
List* _string( char* string ){
  char* newstring;
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = STRING;
  newstring = malloc( strlen( string ) + 1 );
  strcpy( newstring, string );
  newlist->value.symbol = newstring;
  newlist->next = NULL;
  return newlist;
}


/* 整数を要素とする新しいリストを生成する */
List* _integer( int n ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = NUMBER;
  newlist->value.integer = n;
  newlist->next = NULL;
  return newlist;
}


/* 実数を要素とする新しいリストを生成する */
List* _real( double r ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = REAL;
  newlist->value.real = (float)r;
  newlist->next = NULL;
  return newlist;
}


/* リストを要素とする新しいリストを生成する */
List* list( List* list ){ return createList( list ); }
List* createList( List* list ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = LIST;
  newlist->value.list = list;
  newlist->next = NULL;
  return newlist;
}


/* リストの長さを返す */
int getListLength( List* list ){
int i;

  if(list == NULL) return 0;
  else{
    for( i = 0; list != NULL; i++ ) list = list->next;
    return i;
  }
}


/* リストにシンボルを追加する */
List* addSymbol( List* list, char* symbol ){
  List* newlist;
  List* end;

  newlist = _symbol( symbol );
  end = getListEnd( list );
  if( end == NULL )  return newlist;
  else{
    end->next = newlist;
    return list;
  }
}


/* リストに整数を追加する */
List* addInteger( List* list, int n ){
  List* newlist;
  List* end;

  newlist = _integer( n );
  end = getListEnd( list );
  if( end == NULL ) return newlist;
  else{
    end->next = newlist;
    return list;
  }
}


/* リストに整数を追加する */
List* addReal( List* list, double r ){
  List* newlist;
  List* end;

  newlist = _real( r );
  end = getListEnd( list );
  if( end == NULL ) return newlist;
  else{
    end->next = newlist;
    return list;
  }
}


/* リストにリストを追加する */
List* addList( List* list1, List* list2 ){
  List* newlist;
  List* end;

  newlist = createList( list2 );
  end = getListEnd( list1 );
  if( end == NULL ) return newlist;
  else{
    end->next = newlist;
    return list1;
  }
}


/* リストとリストを連結する */
List* concatLists( List* list1, List* list2 ){
  List* end;

  end = getListEnd( list1 );
  if( end == NULL ) return list2;
  else{
    end->next = list2;
    return list1;
  }
}


/* リストを削除する */
void deleteList( List* list ){
  List* old;

  while( list != NULL ){
    if( list->type == SYMBOL && list->value.symbol != NULL ) free( list->value.symbol );
    if( list->type == STRING && list->value.string != NULL ) free( list->value.string );
    if( list->type == LIST   && list->value.list   != NULL ) deleteList( list->value.list );
    old = list;
    list = list->next;
    free( old );
  }
}


/* リストの保持するデータの値(整数)を返す */
int getInteger( List* list ){
  return list->value.integer;
}


/* リストの保持するデータの値(シンボル)を返す */
char* getSymbol( List* list ){
  return list->value.symbol;
}


/* リストの保持するデータの値(リスト)を返す */
List* getList( List* list ){
  return list->value.list;
}


/* リストの最後尾を返す */
List* getListEnd( List* list ){
  if(list == NULL) return NULL;
  else{
    while(list->next != NULL) list = list->next;
    return list;
  }
}


/* リストを表示する */
void printList( List* list ){
  xputc('('); xputc(' ');
  printValue( list );
  xputc(')'); xputc('\n');
}


/* リストの保持するデータ列を表示する */
void printValue( List* list ){
  static char  *s, sbuf[256];
  static TCHAR tbuf[252];

  if( list == NULL ) return;
  else{
	  switch( list->type ){

      /* シンボルや文字列の場合 */
      case SYMBOL:
	if( list->value.symbol != NULL ){
          for( s = list->value.symbol; *s != '\0'; s++ ) xputc( *s );
        }
        xputc(' ');
        break;

      /* 文字列の場合 */
      case STRING:
	if( list->value.symbol != NULL ){
          xputc('\"');
          for( s = list->value.symbol; *s != '\0'; s++ ){
            if( *s == '\'' ){
              xputc( '\\' );
              xputc( '\'' );
            }
            else if( *s == '\"' ){
              xputc( '\\' );
              xputc( '\"' );
            }
            else if( *s == '\\' ){
              xputc( '\\' );
              xputc( '\\' );
            }
            else if( (unsigned char)*s < ' ' ){
              swprintf( tbuf, TEXT("%02x"), (int)*s );
              WideCharToMultiByte( CP_ACP, 0, tbuf, -1, sbuf, 252, NULL, NULL );
              xputc( '\\' );
              xputc( 'x' );
              xputc( sbuf[0] );
              xputc( sbuf[1] );
            }
            else xputc( *s );
          }
          xputc('\"');
        }
        xputc(' ');
        break;

      /* 整数の場合 */
      case NUMBER:
        for( s = _itoa( list->value.integer, sbuf, 10 ); *s != '\0'; s++ ) xputc( *s );
        xputc(' ');
        break;

      /* 実数の場合 */
      case REAL:
        swprintf( tbuf, TEXT("%f"), list->value.real );
        WideCharToMultiByte( CP_ACP, 0, tbuf, -1, sbuf, 252, NULL, NULL );
	for( s = sbuf; *s != '\0'; s++ ) xputc( *s );
        xputc(' ');
        break;

      /* リストの場合 */
      case NEWLIST:
      case LIST:
        xputc('('); xputc(' ');
        printValue( list->value.list );
        xputc(')'); xputc(' ');
        break;

    }
    printValue( list->next );
  }
}


/* 環境を生成する */
Environment* createEnvironment( Environment* parent, int type ){
  Environment* env;

  env = malloc( sizeof(Environment) );
  env->type = type;
  env->size = 0;
  env->var_top = env->var_end = NULL;
  env->parent = parent;
  return env;
}


/* 環境を生成する(2) */
Environment* createEnvironment2( Environment* parent, int type, int ID ){
  Environment* env;

  env = malloc( sizeof(Environment) );
  env->type = type;
  env->ID = ID;
  env->size = 0;
  env->var_top = env->var_end = NULL;
  env->parent = parent;
  return env;
}


/* 環境を削除する */
void deleteEnvironment( Environment* env ){
  Variable *p, *var;

  var = env->var_top;
  while(  var != NULL ){
    p = var;
   if( var->type == NEWLIST ) deleteList( var->value.list );
    var = var->next;
    free( p );
  }  
  free( env );
}


/* 変数の型を返す */
int getVariableType( Environment* env, char* symbol ){
  Variable* var = getVariable( env, symbol, ALLTYPE, WIDE_MODE );
  return var ==NULL? 0 : var->type;
}


/* 整数型変数の値を返す */
int getIntegerVariable( Environment* env, char* symbol, int mode ){
  Variable* var = getVariable( env, symbol, NUMBER, mode );
  return var == NULL? NaN : var->value.number;
}


/* リスト型変数変数の値を返す */
List* getListVariable( Environment* env, char* symbol, int mode ){
  Variable* var = getVariable( env, symbol, LIST, mode );
  return var == NULL? NULL : var->value.list;
}


/* 変数を返す */
Variable* getVariable( Environment* env, char* symbol, int type, int mode ){
  Variable* var;

  for( var = env->var_top; var != (Variable*)NULL; var = var->next ){
    if( strcmp( var->symbol, symbol ) == 0 ){
      return ( var->type == ALLTYPE || var->type == type )? var : NULL;
    }
  }
  return ( env->parent == NULL || mode == NARROW_MODE )? NULL : getVariable( env->parent, symbol, type, mode );
}


/* 整数型変数を生成する */
Variable* createIntegerVariable( Environment* env, char* symbol, int value ){
  Variable* var;

  var = (Variable*)malloc( sizeof(Variable) );
  var->prev = env->var_end;
  var->next = NULL;
  env->var_end = var;
  if( env->var_top == NULL ) env->var_top = var; else var->prev->next = var;
  var->symbol = symbol;
  var->type = NUMBER;
  var->value.number = value;
  env->size++;
  return var;
}


/* リスト型変数を生成する */
Variable* createListVariable( Environment* env, char* symbol, List* value ){
  Variable* var;

  var = (Variable*)malloc( sizeof(Variable) );
  var->prev = env->var_end;
  var->next = NULL;
  env->var_end = var;
  if( env->var_top == NULL ) env->var_top = var; else var->prev->next = var;
  var->symbol = symbol;
  var->type = LIST;
  var->value.list = value;
  env->size++;
  return var;
}


/* 新規にリストを生成して変数に格納する */
Variable* createNewListVariable( Environment* env, char* symbol, List* value ){
  Variable* var;

//printValue( value);

//Cputs(TEXT("malloc\n"));
  var = (Variable*)malloc( sizeof(Variable) );
//Cputs(TEXT("malloc end\n"));
  var->prev = env->var_end;
  var->next = NULL;
  env->var_end = var;
  if( env->var_top == NULL ) env->var_top = var; else var->prev->next = var;
  var->symbol = symbol;
  var->type = NEWLIST;
  var->value.list = value;
  env->size++;
//Cputs(TEXT("createNewListVariable() end\n"));
  return var;
}


/* リストを評価する */
int evalList( Environment* env, List* list ){
  char* op;

  op = getSymbol( list );
  if( strcmp( op, "+" ) == 0 )           return evalPlus( env, list );
  else if( strcmp( op, "-" ) == 0 )      return evalMinus( env, list );
  else if( strcmp( op, "*" ) == 0 )      return evalMul( env, list );
  else if( strcmp( op, "/" ) == 0 )      return evalDiv( env, list );
  else if( strcmp( op, "==" ) == 0 )     return evalEQ( env, list );
  else if( strcmp( op, "!=" ) == 0 )     return evalNE( env, list );
  else if( strcmp( op, "<" ) == 0 )      return evalLT( env, list );
  else if( strcmp( op, "<=" ) == 0 )     return evalLE( env, list );
  else if( strcmp( op, ">" ) == 0 )      return evalGT( env, list );
  else if( strcmp( op, ">=" ) == 0 )     return evalGE( env, list );
  else if( strcmp( op, "define" ) == 0 ) return evalDefineVariable( env, list );
  else if( strcmp( op, "=" ) == 0 )      return evalSetVariable( env, list );
  else if( strcmp( op, "defun" ) == 0 )  return evalDefineFunction( env, list );
  else if( strcmp( op, "if" ) == 0 )     return evalIf( env, list );
  else if( strcmp( op, "?" ) == 0 )      return printOperand( env, list );
  else if( strcmp( op, "new" ) == 0 )    return newProgram();      
  else if( strcmp( op, "run" ) == 0 )    return runProgram();      
  else if( strcmp( op, "quit" ) == 0 )   return quitProgram();      
  else if( strcmp( op, "edit" ) == 0 )   return editProgram();      
  else if( strcmp( op, "list" ) == 0 )   return listProgram();      
  else if( strcmp( op, "load" ) == 0 )   return loadProgram( list );      
  else if( strcmp( op, "save" ) == 0 )   return saveProgram( list );      
  else                                   return evalFunction( env, list );
}


/* 式の評価：加算("+") */
int evalPlus( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret =x + y;
}


/* 式の評価：減算("-") */
int evalMinus( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x - y;
}


/* 式の評価：乗算("*") */
int evalMul( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x * y;
}


/* 式の評価：除算("/") */
int evalDiv( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x / y;
}


/* 条件式の評価：等しい("==") */
int evalEQ( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x == y );
}


/* 条件式の評価：等しくない("!=") */
int evalNE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x != y );
}


/* 条件式の評価：小さい("<") */
int evalLT( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x < y );
}


/* 条件式の評価：小さい or 等しい("<=") */
int evalLE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x <= y );
}


/* 条件式の評価：大きい(">") */
int evalGT( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x > y );
}


/* 条件式の評価：大きい or 等しい(">=") */
int evalGE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x >= y );
}


/* 式の評価：変数定義("define") */
int evalDefineVariable( Environment* env, List* list ){
  char* symbol;
  int   value;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  symbol = getSymbol( list->next );
  value  = evalOperand( env, list->next->next );
  createIntegerVariable( env, symbol, value );
  return Ret = value;
}


/* 式の評価：変数をセット("=") */
int evalSetVariable( Environment* env, List* list ){
  Variable* var;
  char* symbol;
  int   value;

  if( list == NULL || ( symbol = getSymbol( list->next ) ) == NULL ) return 1;
  var    = getVariable( env, symbol, NUMBER, WIDE_MODE );
  value  = evalOperand( env, list->next->next );
  if( var != (Variable*)NULL ) var->value.number = value;
  else createIntegerVariable( env, symbol, value );
  return Ret = value;
}


/* 式の評価：関数定義("defun") */
int evalDefineFunction( Environment* env, List* list ){
  char* symbol;
  List* function;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  symbol   = getSymbol( list->next );
  function = list->next->next;
  createListVariable( env, symbol, function );
  return Ret;
}


/* 式の評価：条件式("if") */
int evalIf( Environment* env, List* list ){
  int c;
  List *condition, *exec_true, *exec_false;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  condition  = list->next;
  exec_true  = list->next->next;
  exec_false = list->next->next->next;
  if( c = evalOperand( env, condition ) ) evalOperand( env, exec_true );
  else if( exec_false != NULL )           evalOperand( env, exec_false );
  return Ret = c;
}


/* 式の評価：関数評価 */
int evalFunction( Environment* env, List* list ){
  Environment* new_env;
  char  *func_name, *arg_name;
  List  *function, *arg, *body;
  int   value;

  if( list == NULL ) return 1;
  new_env = createEnvironment( env, 0 );
  func_name = getSymbol( list );
  function  = getListVariable( env, func_name, WIDE_MODE );
  if( function == (List*)NULL ) return 0;
  arg  = getList( function );
  body = getList( function->next );
  while( arg != NULL ){
    list = list->next;
    arg_name = getSymbol( arg );
    createIntegerVariable( new_env, arg_name, evalOperand( env, list ) ); 
    arg = arg->next;
  }
  value = evalList( new_env, body );
  deleteEnvironment( new_env );
  return Ret = value;
}


/* オペランドを評価する */
int evalOperand( Environment* env, List* operand ){

  if( operand == NULL ) return 1;
  switch ( operand->type ) {

    /* 整数の場合 */
    case NUMBER :
      return Ret = getInteger( operand );

    /* シンボルの場合 */
    case SYMBOL :
      return Ret = getIntegerVariable( env, getSymbol( operand ), WIDE_MODE );

    /* リストの場合 */
    case LIST :
      return Ret = evalList( env, getList( operand ) );

    /* 上記以外の場合 */
    default :
      Cputs( TEXT("evalOperand(): リストが不正です\n") );
      quitProgram();

  }
}


/* オペランドを表示する */
int printOperand( Environment* env, List* list ){
    List* operand;

    if( list == NULL || ( operand = list->next ) == NULL ) return 1;
    wsprintf( msg, TEXT("%d\n"), Ret = evalOperand( env, operand ) );
    Cputs( msg );
    return Ret;
  }


/* リストを入力する */
List* inputList(){

  if ( getLispToken() == LPAREN ) return inputList2( (List*)NULL );
  else{
    Cputs(TEXT("inputList():リストではありません\n") );
    quitProgram();
  }
}


/* リストを入力する関数のサブ関数 */
List* inputList2( List* list){
  int   token;
  List* newlist;

  while(( token = getLispToken() ) != 0  ){
    switch( token ) {

      /* シンボルの場合 */
      case SYMBOL :

      /* 文字列の場合 */
      case STRING :

      /* 整数の場合 */
      case NUMBER :

      /* 実数の場合 */
      case REAL :

//Cputs(TEXT("inputlist2Value="));
//printValue(TokenValue);
//Cputs(TEXT("\n"));

        list = concatLists( list, TokenValue );

//Cputs(TEXT("inputlist2list="));
//printValue(list);
//Cputs(TEXT("\n"));

        break;

      /* 左括弧の場合 */
      case LPAREN :
        newlist = inputList2( (List*)NULL );
        list = addList( list, newlist );
        break;

      /* 右括弧の場合 */
      case RPAREN :

//Cputs(TEXT("inputlist2="));
//printValue(list);
//Cputs(TEXT("\n"));

        return list;

    }
  }
  Cputs(TEXT("inputList2():リストが不正です\n") );
  quitProgram();
}


/* プログラムと環境を初期化する */
int newProgram(){
  if( RootEnvironment != (Environment*)NULL ) deleteEnvironment( RootEnvironment );
  RootEnvironment = createEnvironment( NULL, 0 );
  if( Program != (List*)NULL ){
    deleteList( Program );
    Program = (List*)NULL;
  }
  return 1;
}


/* プログラムを実行する */
int runProgram(){
  List* list;

  if( Program == (List*)NULL ) return 0;
  if( RootEnvironment != (Environment*)NULL ) deleteEnvironment( RootEnvironment );
  RootEnvironment = createEnvironment( NULL, 0 );
  list = Program;
  while( list != NULL ){
    evalList( RootEnvironment, getList( list ) );
    list = list->next;
  }
  return 1;
}


/* プログラムを終了する */
int quitProgram(){
  if( RootEnvironment != (Environment*)NULL ) deleteEnvironment( RootEnvironment );
  if( Program != (List*)NULL ) deleteList( Program );
  QuitFlg = 1;
  return 1;
}


/* プログラムを入力する */
int editProgram(){
  if( Program != NULL ) deleteList( Program );
  Program = inputList();
  Cputs( TEXT("\n") );
  return 1;
}


/* プログラムを表示する */
int listProgram(){
  printList( Program );
  return 1;
}


/* プログラムをロードする */
int loadProgram( List* list ){
  char* file_name;

  if( list == NULL || list->next == NULL ) return 1;
  file_name = getSymbol( list->next );
  xopenInputFile( file_name );
  Program = inputList();
  xcloseInputFile();
  return 1;
}


/* プログラムをセーブする */
int saveProgram( List* list ){
  char* file_name;

  if( list == NULL || list->next == NULL ) return 1;
  file_name = getSymbol( list->next );
  xopenOutputFile( file_name );
  printList( Program );
  xcloseOutputFile();
  return 1;
}


/* トークンを切り出してTokenValue(List*型)を返す */
int getLispToken(){
  int    c, d, i, t;
  char   cbuf[4];
  TCHAR  tbuf[252];
  double f;

  i = 0;
  while( ( c = xgetc() ) != EOF ){

//Putch(c);
  
    /* 左括弧 */
    if( c == '(' ){
      TokenText[0] = '(';
      TokenText[1] = '\0';
      return LPAREN;
    }

    /* 右括弧 */
    else if( c == ')' ){
      TokenText[0] = ')';
      TokenText[1] = '\0';
      return RPAREN;
    }

    /* 文字定数 */
    else if(  c == '\'' ){
      if( ( c = xgetc() ) == '\\' ){
        c = xgetc();
        d = xgetc();
        if( c == 'a' && d =='\'' )       return  TokenValue = _integer('\a'), NUMBER;
        else if( c == 'b'  && d =='\'' ) return  TokenValue = _integer('\b'), NUMBER;
        else if( c == 'f'  && d =='\'' ) return  TokenValue = _integer('\f'), NUMBER;
        else if( c == 'n'  && d =='\'' ) return  TokenValue = _integer('\n'), NUMBER;
        else if( c == 'r'  && d =='\'' ) return  TokenValue = _integer('\r'), NUMBER;
        else if( c == 't'  && d =='\'' ) return  TokenValue = _integer('\t'), NUMBER;
        else if( c == 'v'  && d =='\'' ) return  TokenValue = _integer('\v'), NUMBER;
        else if( c == '\\' && d =='\'' ) return  TokenValue = _integer('\\'), NUMBER;
        else if( c == '\?' && d =='\'' ) return  TokenValue = _integer('\?'), NUMBER;
        else if( c == '\'' && d =='\'' ) return  TokenValue = _integer('\''), NUMBER;
        else if( c == '\"' && d =='\'' ) return  TokenValue = _integer('\"'), NUMBER;
        else if( c == '0'  && d =='\'' ) return  TokenValue = _integer('\0'), NUMBER;
        else if( c >= '0' && c <= '7' && d >= 0 && d <= 7 ){
          cbuf[0] = c;
          cbuf[1] = d;
          if( ( c = xgetc() ) >= 0 && c <= 7 && xgetc() =='\'' ){
            cbuf[2] = c;
            cbuf[3] = '\0';
            MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            TokenValue = _integer( t );
            return NUMBER;
          }
          else{
            Cputs( TEXT("不正なエスケープ文字です") );
            return XBADTOKEN;
          }
        }
        else if( ( c  == 'x' || c  == 'X') && ( ( d >= '0' && d <= '9' ) || ( d >= 'a' && d <= 'f' ) || ( d >= 'A' && d <= 'F' ) ) ){
          cbuf[0] = d;
          if( ( ( ( c = xgetc() ) >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) ) && xgetc() =='\'' ){
            cbuf[1] = c;
            cbuf[2] = '\0';
            MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%x"), &t );
            TokenValue = _integer( t );
            return NUMBER;
          }
          else{
            Cputs( TEXT("不正なエスケープ文字です") );
            return XBADTOKEN;
          }
        }
        else{
          Cputs( TEXT("不正なエスケープ文字です") );
          return XBADTOKEN;
        }
      }
      else{
        if( xgetc() =='\'' ){
          TokenValue = _integer( c );
          return NUMBER;
        }
        else{
          Cputs( TEXT("文字定数の終端が不正です") );
          return XBADTOKEN;
        }
      }
    }

    /* 文字列定数 */
    else if(  c == '\"' ){
      while( ( c = xgetc() ) != '\"' && c != '\n' && c != EOF ){
        if( c  == '\\' ){
          if( ( c = xgetc() ) == 'a' ) TokenText[ i++ ] = '\a';
          else if( c  == 'b' )  TokenText[ i++ ] = '\b';
          else if( c  == 'f' )  TokenText[ i++ ] = '\f';
          else if( c  == 'n' )  TokenText[ i++ ] = '\n';
          else if( c  == 'r' )  TokenText[ i++ ] = '\r';
          else if( c  == 't' )  TokenText[ i++ ] = '\t';
          else if( c  == 'v' )  TokenText[ i++ ] = '\v';
          else if( c  == '\\' ) TokenText[ i++ ] = '\\';
          else if( c  == '?' )  TokenText[ i++ ] = '\?';
          else if( c  == '\'' ) TokenText[ i++ ] = '\'';
          else if( c  == '\"' ) TokenText[ i++ ] = '\"';
          else if( c  == '0' )  TokenText[ i++ ] = '\0';
          else if( c  == 'x' || c  == 'X'){
            if( ( ( c = xgetc() ) >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) ){
              char cbuf[4];
              cbuf[0] = c;
              if( ( ( c = xgetc() ) >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) ){
                cbuf[1] = c;
                cbuf[2] = '\0';
                MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
                swscanf( tbuf, TEXT("%x"), &t );
                TokenText[ i++ ] = (char)t;
              }
              else{
                Cputs(TEXT("不正なエスケープ文字です\'\\x0*\'\n"));
              }
            }
            else{
              Cputs(TEXT("不正なエスケープ文字です\'\\x*0\'\n"));
            }
          }
          else{
            Cputs(TEXT("不正なエスケープ文字です\'\\*\'\n"));
          }
        }
        else{
          TokenText[ i++ ] = c;
          if( isZenkaku( c ) ) TokenText[ i++ ] = xgetc();
        }
      }
      if( c == '\"' ){
        TokenText[ i++ ] = '\0';
        TokenValue = _string( TokenText );

//printValue( TokenValue );

        return STRING;
      }
      else{
        Cputs(TEXT("文字列定数の終端が不正です\n"));
        return XBADTOKEN;
      }
    }

    /* 整数、実数 */
    if( c =='0' ){
      TokenText[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '7' ){
        do {                 // 8進整数
          TokenText[ i++ ] = c;
          c = xgetc();
        } while( c >= '0' && c <= '7' );
        if( c == 'l' || c == 'L' ){
          if( ( c = xgetc() ) == 'u' || c == 'U' ){
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            TokenValue = _integer( t );

//printValue( TokenValue );

            return NUMBER;
          }
          else{
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            TokenValue = _integer( t );

//printValue( TokenValue );

            return NUMBER;
          }
        }
        else if( c == 'u' || c == 'U' ){
          if( ( c = xgetc() ) == 'l' || c == 'L' ){
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            TokenValue = _integer( t );

//printValue( TokenValue );

            return NUMBER;
          }
          else{
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            TokenValue = _integer( t );

//printValue( TokenValue );

            return NUMBER;
          }
        }
        else{
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText+1, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%o"), &t );
          TokenValue = _integer( t );

//printValue( TokenValue );

          return NUMBER;
        }
      }
      else if( c == 'x' || c == 'X' ){ //16進整数
        TokenText[ i++ ] = c;
        if( ( ( c = xgetc() ) >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) );
          if( c == 'l' || c == 'L' ){
            if( ( c = xgetc() ) == 'u' || c == 'U' ){
              TokenText[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, TokenText+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              TokenValue = _integer( t );

//printValue( TokenValue );

              return NUMBER;
            }
            else{
              xungetc( c );
              TokenText[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, TokenText+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              TokenValue = _integer( t );

//printValue( TokenValue );

              return NUMBER;
            }
          }
          else if( c == 'u' || c == 'U' ){
            if( ( c = xgetc() ) == 'l' || c == 'L' ){
              TokenText[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, TokenText+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              TokenValue = _integer( t );

//printValue( TokenValue );

              return NUMBER;
            }
            else{
              xungetc( c );
              TokenText[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, TokenText+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              TokenValue = _integer( t );

//printValue( TokenValue );

              return NUMBER;
            }
          }
          else{
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText+2, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%x"), &t );
            TokenValue = _integer( t );

//printValue( TokenValue );

            return NUMBER;
          }
        }
        xungetc( c );
        Cputs(TEXT("16進定数の表記が不正です\n"));
        return XBADTOKEN;
      }
      else if( c == '.' ){ //1以下の小数
        TokenText[ i++ ] = c;
        if( ( c = xgetc() ) >= '0' && c <= '9' ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
          if( c == 'f' || c == 'F' ){   // 実数
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
          else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
            TokenText[ i++ ] = c;
            if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
              do {
                TokenText[ i++ ] = c;
                c = xgetc();
              } while( c >= '0' && c <= '9' );
              xungetc( c );
              TokenText[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%lg"), &f );
              TokenValue = _real( f );

//printValue( TokenValue );

              return REAL;
            }
            xungetc( c );
            Cputs(TEXT("実数定数の表記が不正です\n"));
            return XBADTOKEN;
          }
          else{                           // 単純な小数
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
        }
        else if( c == 'f' || c == 'F' ){   // 実数
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          TokenText[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              TokenText[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
          xungetc( c );
          Cputs(TEXT("実数定数の表記が不正です\n"));
          return XBADTOKEN;
        }
        xungetc( c );
        TokenText[ i ] = '\0';
        MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
        swscanf( tbuf, TEXT("%lg"), &f );
        TokenValue = _real( f );

//printValue( TokenValue );

        return REAL;
      }
      else{ // 0の場合
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _integer(0);

//printValue( TokenValue );

        return NUMBER;
      }
    }
    else if( c >= '1' && c <= '9' ){     //10進数
      do {
        TokenText[ i++ ] = c;
        c = xgetc();
      } while( '0' <= c && c <= '9' );
      if( c == '.' ){                    //実数(小数点表示)
        TokenText[ i++ ] = c;
        if( ( c = xgetc() ) >= '1' && c <= '9' ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
        }
        if( c == 'f' || c == 'F' ){   // 実数
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          TokenText[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              TokenText[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
          xungetc( c );
          Cputs(TEXT("実数定数の表記が不正です\n"));
          return XBADTOKEN;
        }
        else{                           // 単純な小数
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
      }
      else if( c == 'f' || c == 'F' ){   // 実数
        TokenText[ i ] = '\0';
        MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
        swscanf( tbuf, TEXT("%lg"), &f );
        TokenValue = _real( f );

//printValue( TokenValue );

        return REAL;
      }
      else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
        TokenText[ i++ ] = c;
        if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( c >= '0' && c <= '9' );
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        xungetc( c );
        Cputs(TEXT("実数定数の表記が不正です\n"));
        return XBADTOKEN;
      }
      else if( c == 'l' || c == 'L' ){  // 整数
        if( ( c = xgetc() ) == 'u' || c == 'U' ){
          TokenText[ i ] = '\0';
          TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

          return NUMBER;
        }
        else{
          xungetc( c );
          TokenText[ i ] = '\0';
          TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

          return NUMBER;
        }
      }
      else if( c == 'u' || c == 'U' ){  // 整数
        if( ( c = xgetc() ) == 'l' || c == 'L' ){
          TokenText[ i ] = '\0';
          TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

          return NUMBER;
        }
        else{
          xungetc( c );
          TokenText[ i ] = '\0';
          TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

          return NUMBER;
        }
      }
      else{  //単純な整数
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

        return NUMBER;
      }
    }
    else if( c == '.' ){  // 1以下の小数
      TokenText[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '9' ){
        do {
          TokenText[ i++ ] = c;
          c = xgetc();
        } while( '0' <= c && c <= '9' );
        if( c == 'f' || c == 'F' ){   // 実数
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          TokenText[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              TokenText[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
          xungetc( c );
          Cputs(TEXT("実数定数の表記が不正です\n"));
          return XBADTOKEN;
        }
        else{                           // 単純な小数
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
      }
      else{ // ピリオド(SYMBOL)として認識
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _symbol( TokenText );

//printValue( TokenValue );

        return SYMBOL;
      }
    }

    /* シンボル */
    else if( c != ' ' && c != '\t' && c != '\n' && c != '\r' ){
      do {
	TokenText[ i++ ] = c;
	c = xgetc();
      } while( c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '(' && c != ')' );
      TokenText[ i ] = '\0';
      xungetc( c );
      TokenValue = _symbol( TokenText );

//printValue( TokenValue );

      return SYMBOL;
    }

  }
  return 0;
}


/* Tokenが一致しているがどうかを返す */
int isTokenMatch( char* token ){
  char c;

  // 比較対象とする文字をとりだす
  switch( token[0] ){

  // 無条件で一致したと判断する
  case '\0':
    return 1;

  // 空白であるなら一致したと判断する
  case ' ':
    if( ( c = xgetc() ) == ' ' || c == '\t' || c == '\r' || c == '\n'  ) return 1;
    xungetc( c );
    return 0;

  // 英字ではない場合は一致したと判断する
  case '\a':
    if( ( c = xgetc() ) == '_' || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ){
      xungetc( c );
      return 0;
    }
    xungetc( c );
    return 1;

  // 英数字ではない場合は一致したと判断する
  case '\b':
    if( ( c = xgetc() ) == '_' || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' )){
      xungetc( c );
      return 0;
    }
    xungetc( c );
    return 1;

  // 文字が一致する場合は直後の文字を解析して一致しているかを判断し、そうでない場合は不一致であると判断する
  default:
    if( token[0] == ( c = xgetc() ) ){ 
      if( isTokenMatch( &token[1] ) ) return 1;
    }
    xungetc( c );
    return 0;

  }
}


/* ファイルまたはコンソールから１文字入力する */
char xgetc(){
  TCHAR tchar[2];
  char  xchar[2], c;
  DWORD size;
  
  /* 入力文字が残っていたらそれを返す */
  if( UngetP > 0 ) return UngetBuf[ --UngetP ];

  /* 入力文字が残っていない場合 */
  else{

    /* コンソールから1文字入力 */
    if( InputFile == INVALID_HANDLE_VALUE ){
      Putch( *tchar = Getch() ); if( *tchar == TEXT('\r') ) Putch( TEXT('\n') );
      tchar[1] = TEXT('\0');
      WideCharToMultiByte( CP_ACP, 0, tchar, -1, xchar, 252, NULL, NULL );
      LastInputChar = '\0';
      return *xchar;
    }

    /* ファイルから1文字入力 */
    else{
      LastInputChar = '\0';
      ReadFile( InputFile, &c, sizeof(char), &size, NULL );
      if( size == 0 ){
        xcloseInputFile();
        return EOF;
      }
      return c ;
    }
  }    
}


/* ファイルまたはコンソールからの１文字入力を元に戻す */
void xungetc( char c ){
  UngetBuf[ UngetP++ ] = c;
}


/* ファイルまたはコンソールに１文字出力 */
void xputc( char c ){
  char  xchar[2];
  TCHAR tchar[2];
  DWORD size;

  /* コンソールに１文字出力 */
  if( OutputFile == INVALID_HANDLE_VALUE ){
    xchar[0] = c;
    xchar[1] = '\0';
    MultiByteToWideChar( CP_ACP, 0, xchar, -1, tchar, 252 );
    Putch( tchar[0] );
  }

  /* ファイルに１文字出力 */
  else WriteFile( OutputFile, (void *)&c, 1, &size, NULL );
}


/* 入力ファイルを開く */
void xopenInputFile( char* file_name ){
  TCHAR tfname[256];

  MultiByteToWideChar( CP_ACP, 0, file_name, -1, tfname, 252 );
  InputFile = CreateFile( tfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( InputFile == INVALID_HANDLE_VALUE ) Cputs( TEXT("ファイルオープン失敗\n") );
}


/* 入力ファイルを閉じる */
void xcloseInputFile(){
  if( InputFile == INVALID_HANDLE_VALUE ) return;
  CloseHandle( InputFile );
  InputFile = INVALID_HANDLE_VALUE;
}


/* 出力ファイルを開く */
void xopenOutputFile( char* file_name ){
  TCHAR tfname[256];

  MultiByteToWideChar( CP_ACP, 0, file_name, -1, tfname, 252 );
  OutputFile = CreateFile( tfname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
  if( OutputFile == INVALID_HANDLE_VALUE ) Cputs( TEXT("ファイルオープン失敗\n") );
}


/* 出力ファイルを閉じる */
void xcloseOutputFile(){
  if( OutputFile == INVALID_HANDLE_VALUE ) return;
  CloseHandle( OutputFile );
  OutputFile = INVALID_HANDLE_VALUE;
}


/* 全角文字かどうかを判定する */
int isZenkaku(char c){
  unsigned char cc = (unsigned char)c;
  return (0x81 <= cc && cc <= 0x9F || 0xE0 <= cc && cc <= 0xFC);
}
