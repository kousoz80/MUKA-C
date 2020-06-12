/*

 "lisp.h" LISP�����n�̃w�b�_�t�@�C��
   for CASSIOPEIA A-51

   C�R���p�C���̏����n�̒��j�ƂȂ�

*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "conslib.h"

/* �萔 */
#define TRUE            1
#define FALSE           0
#define NaN             0x80000000

/* �g�[�N��ID */
#define EOF             -1
#define END             0   /* ���X�g�̏I��� */
#define XBADTOKEN       1   /* �s����token */
#define LPAREN          2   /* ���J�b�R */
#define RPAREN          3   /* �E�J�b�R */
#define SYMBOL          4
#define NUMBER          5
#define LIST            6
#define ALLTYPE         7
#define REAL            8
#define NEWLIST         9
#define STRING          10

/* �ϐ��̌������[�h */
#define NARROW_MODE     0  /* �^����ꂽ���O��Ԃ������������� */
#define WIDE_MODE       1  /* �e�̖��O��Ԃ��������� */

/* ���̃^�C�v */
#define TYPE_BLOCK      0 /* �u���b�N�p */
#define TYPE_FUNCTION   1 /* �֐��p */


/* ���X�g */
struct _list {
  int type;    /* ���X�g�̕ێ�����f�[�^�̌^�F �L��(�ϐ���/�֐���) | ���l | ���X�g(�ւ̃|�C���^) */
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


/* �ϐ� */
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


/* �� */
struct _env {
  int         type;
  int         size;
  int         ID;
  Variable*   var_top;
  Variable*   var_end;
  struct _env *parent; /* �e�ƂȂ�� */
};
typedef struct _env Environment;


/* �v���g�^�C�v�錾 */
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


/* �ϐ��錾 */
TCHAR        AppName[] = TEXT("Console"); /* �K�{���� */
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
int          Ret;        /* �֐��̖߂�l���L�����Ă���(�f�o�b�O�p) */


/* �֐��錾 */


/* �p�����[�^�̃��X�g���������� */
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


/* LISP�̎��𐶐����� */
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


/* �V���{����v�f�Ƃ���V�������X�g�𐶐����� */
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


/* �������v�f�Ƃ���V�������X�g�𐶐����� */
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


/* ������v�f�Ƃ���V�������X�g�𐶐����� */
List* _integer( int n ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = NUMBER;
  newlist->value.integer = n;
  newlist->next = NULL;
  return newlist;
}


/* ������v�f�Ƃ���V�������X�g�𐶐����� */
List* _real( double r ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = REAL;
  newlist->value.real = (float)r;
  newlist->next = NULL;
  return newlist;
}


/* ���X�g��v�f�Ƃ���V�������X�g�𐶐����� */
List* list( List* list ){ return createList( list ); }
List* createList( List* list ){
  List* newlist;

  newlist = malloc( sizeof(List) );
  newlist->type = LIST;
  newlist->value.list = list;
  newlist->next = NULL;
  return newlist;
}


/* ���X�g�̒�����Ԃ� */
int getListLength( List* list ){
int i;

  if(list == NULL) return 0;
  else{
    for( i = 0; list != NULL; i++ ) list = list->next;
    return i;
  }
}


/* ���X�g�ɃV���{����ǉ����� */
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


/* ���X�g�ɐ�����ǉ����� */
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


/* ���X�g�ɐ�����ǉ����� */
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


/* ���X�g�Ƀ��X�g��ǉ����� */
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


/* ���X�g�ƃ��X�g��A������ */
List* concatLists( List* list1, List* list2 ){
  List* end;

  end = getListEnd( list1 );
  if( end == NULL ) return list2;
  else{
    end->next = list2;
    return list1;
  }
}


/* ���X�g���폜���� */
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


/* ���X�g�̕ێ�����f�[�^�̒l(����)��Ԃ� */
int getInteger( List* list ){
  return list->value.integer;
}


/* ���X�g�̕ێ�����f�[�^�̒l(�V���{��)��Ԃ� */
char* getSymbol( List* list ){
  return list->value.symbol;
}


/* ���X�g�̕ێ�����f�[�^�̒l(���X�g)��Ԃ� */
List* getList( List* list ){
  return list->value.list;
}


/* ���X�g�̍Ō����Ԃ� */
List* getListEnd( List* list ){
  if(list == NULL) return NULL;
  else{
    while(list->next != NULL) list = list->next;
    return list;
  }
}


/* ���X�g��\������ */
void printList( List* list ){
  xputc('('); xputc(' ');
  printValue( list );
  xputc(')'); xputc('\n');
}


/* ���X�g�̕ێ�����f�[�^���\������ */
void printValue( List* list ){
  static char  *s, sbuf[256];
  static TCHAR tbuf[252];

  if( list == NULL ) return;
  else{
	  switch( list->type ){

      /* �V���{���╶����̏ꍇ */
      case SYMBOL:
	if( list->value.symbol != NULL ){
          for( s = list->value.symbol; *s != '\0'; s++ ) xputc( *s );
        }
        xputc(' ');
        break;

      /* ������̏ꍇ */
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

      /* �����̏ꍇ */
      case NUMBER:
        for( s = _itoa( list->value.integer, sbuf, 10 ); *s != '\0'; s++ ) xputc( *s );
        xputc(' ');
        break;

      /* �����̏ꍇ */
      case REAL:
        swprintf( tbuf, TEXT("%f"), list->value.real );
        WideCharToMultiByte( CP_ACP, 0, tbuf, -1, sbuf, 252, NULL, NULL );
	for( s = sbuf; *s != '\0'; s++ ) xputc( *s );
        xputc(' ');
        break;

      /* ���X�g�̏ꍇ */
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


/* ���𐶐����� */
Environment* createEnvironment( Environment* parent, int type ){
  Environment* env;

  env = malloc( sizeof(Environment) );
  env->type = type;
  env->size = 0;
  env->var_top = env->var_end = NULL;
  env->parent = parent;
  return env;
}


/* ���𐶐�����(2) */
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


/* �����폜���� */
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


/* �ϐ��̌^��Ԃ� */
int getVariableType( Environment* env, char* symbol ){
  Variable* var = getVariable( env, symbol, ALLTYPE, WIDE_MODE );
  return var ==NULL? 0 : var->type;
}


/* �����^�ϐ��̒l��Ԃ� */
int getIntegerVariable( Environment* env, char* symbol, int mode ){
  Variable* var = getVariable( env, symbol, NUMBER, mode );
  return var == NULL? NaN : var->value.number;
}


/* ���X�g�^�ϐ��ϐ��̒l��Ԃ� */
List* getListVariable( Environment* env, char* symbol, int mode ){
  Variable* var = getVariable( env, symbol, LIST, mode );
  return var == NULL? NULL : var->value.list;
}


/* �ϐ���Ԃ� */
Variable* getVariable( Environment* env, char* symbol, int type, int mode ){
  Variable* var;

  for( var = env->var_top; var != (Variable*)NULL; var = var->next ){
    if( strcmp( var->symbol, symbol ) == 0 ){
      return ( var->type == ALLTYPE || var->type == type )? var : NULL;
    }
  }
  return ( env->parent == NULL || mode == NARROW_MODE )? NULL : getVariable( env->parent, symbol, type, mode );
}


/* �����^�ϐ��𐶐����� */
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


/* ���X�g�^�ϐ��𐶐����� */
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


/* �V�K�Ƀ��X�g�𐶐����ĕϐ��Ɋi�[���� */
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


/* ���X�g��]������ */
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


/* ���̕]���F���Z("+") */
int evalPlus( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret =x + y;
}


/* ���̕]���F���Z("-") */
int evalMinus( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x - y;
}


/* ���̕]���F��Z("*") */
int evalMul( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x * y;
}


/* ���̕]���F���Z("/") */
int evalDiv( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = x / y;
}


/* �������̕]���F������("==") */
int evalEQ( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x == y );
}


/* �������̕]���F�������Ȃ�("!=") */
int evalNE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x != y );
}


/* �������̕]���F������("<") */
int evalLT( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x < y );
}


/* �������̕]���F������ or ������("<=") */
int evalLE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x <= y );
}


/* �������̕]���F�傫��(">") */
int evalGT( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x > y );
}


/* �������̕]���F�傫�� or ������(">=") */
int evalGE( Environment* env, List* list ){
  int x, y;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  x = evalOperand( env, list->next );
  y = evalOperand( env, list->next->next );
  return Ret = ( x >= y );
}


/* ���̕]���F�ϐ���`("define") */
int evalDefineVariable( Environment* env, List* list ){
  char* symbol;
  int   value;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  symbol = getSymbol( list->next );
  value  = evalOperand( env, list->next->next );
  createIntegerVariable( env, symbol, value );
  return Ret = value;
}


/* ���̕]���F�ϐ����Z�b�g("=") */
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


/* ���̕]���F�֐���`("defun") */
int evalDefineFunction( Environment* env, List* list ){
  char* symbol;
  List* function;

  if( list == NULL || list->next == NULL || list->next->next == NULL ) return 1;
  symbol   = getSymbol( list->next );
  function = list->next->next;
  createListVariable( env, symbol, function );
  return Ret;
}


/* ���̕]���F������("if") */
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


/* ���̕]���F�֐��]�� */
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


/* �I�y�����h��]������ */
int evalOperand( Environment* env, List* operand ){

  if( operand == NULL ) return 1;
  switch ( operand->type ) {

    /* �����̏ꍇ */
    case NUMBER :
      return Ret = getInteger( operand );

    /* �V���{���̏ꍇ */
    case SYMBOL :
      return Ret = getIntegerVariable( env, getSymbol( operand ), WIDE_MODE );

    /* ���X�g�̏ꍇ */
    case LIST :
      return Ret = evalList( env, getList( operand ) );

    /* ��L�ȊO�̏ꍇ */
    default :
      Cputs( TEXT("evalOperand(): ���X�g���s���ł�\n") );
      quitProgram();

  }
}


/* �I�y�����h��\������ */
int printOperand( Environment* env, List* list ){
    List* operand;

    if( list == NULL || ( operand = list->next ) == NULL ) return 1;
    wsprintf( msg, TEXT("%d\n"), Ret = evalOperand( env, operand ) );
    Cputs( msg );
    return Ret;
  }


/* ���X�g����͂��� */
List* inputList(){

  if ( getLispToken() == LPAREN ) return inputList2( (List*)NULL );
  else{
    Cputs(TEXT("inputList():���X�g�ł͂���܂���\n") );
    quitProgram();
  }
}


/* ���X�g����͂���֐��̃T�u�֐� */
List* inputList2( List* list){
  int   token;
  List* newlist;

  while(( token = getLispToken() ) != 0  ){
    switch( token ) {

      /* �V���{���̏ꍇ */
      case SYMBOL :

      /* ������̏ꍇ */
      case STRING :

      /* �����̏ꍇ */
      case NUMBER :

      /* �����̏ꍇ */
      case REAL :

//Cputs(TEXT("inputlist2Value="));
//printValue(TokenValue);
//Cputs(TEXT("\n"));

        list = concatLists( list, TokenValue );

//Cputs(TEXT("inputlist2list="));
//printValue(list);
//Cputs(TEXT("\n"));

        break;

      /* �����ʂ̏ꍇ */
      case LPAREN :
        newlist = inputList2( (List*)NULL );
        list = addList( list, newlist );
        break;

      /* �E���ʂ̏ꍇ */
      case RPAREN :

//Cputs(TEXT("inputlist2="));
//printValue(list);
//Cputs(TEXT("\n"));

        return list;

    }
  }
  Cputs(TEXT("inputList2():���X�g���s���ł�\n") );
  quitProgram();
}


/* �v���O�����Ɗ������������� */
int newProgram(){
  if( RootEnvironment != (Environment*)NULL ) deleteEnvironment( RootEnvironment );
  RootEnvironment = createEnvironment( NULL, 0 );
  if( Program != (List*)NULL ){
    deleteList( Program );
    Program = (List*)NULL;
  }
  return 1;
}


/* �v���O���������s���� */
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


/* �v���O�������I������ */
int quitProgram(){
  if( RootEnvironment != (Environment*)NULL ) deleteEnvironment( RootEnvironment );
  if( Program != (List*)NULL ) deleteList( Program );
  QuitFlg = 1;
  return 1;
}


/* �v���O��������͂��� */
int editProgram(){
  if( Program != NULL ) deleteList( Program );
  Program = inputList();
  Cputs( TEXT("\n") );
  return 1;
}


/* �v���O������\������ */
int listProgram(){
  printList( Program );
  return 1;
}


/* �v���O���������[�h���� */
int loadProgram( List* list ){
  char* file_name;

  if( list == NULL || list->next == NULL ) return 1;
  file_name = getSymbol( list->next );
  xopenInputFile( file_name );
  Program = inputList();
  xcloseInputFile();
  return 1;
}


/* �v���O�������Z�[�u���� */
int saveProgram( List* list ){
  char* file_name;

  if( list == NULL || list->next == NULL ) return 1;
  file_name = getSymbol( list->next );
  xopenOutputFile( file_name );
  printList( Program );
  xcloseOutputFile();
  return 1;
}


/* �g�[�N����؂�o����TokenValue(List*�^)��Ԃ� */
int getLispToken(){
  int    c, d, i, t;
  char   cbuf[4];
  TCHAR  tbuf[252];
  double f;

  i = 0;
  while( ( c = xgetc() ) != EOF ){

//Putch(c);
  
    /* ������ */
    if( c == '(' ){
      TokenText[0] = '(';
      TokenText[1] = '\0';
      return LPAREN;
    }

    /* �E���� */
    else if( c == ')' ){
      TokenText[0] = ')';
      TokenText[1] = '\0';
      return RPAREN;
    }

    /* �����萔 */
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
            Cputs( TEXT("�s���ȃG�X�P�[�v�����ł�") );
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
            Cputs( TEXT("�s���ȃG�X�P�[�v�����ł�") );
            return XBADTOKEN;
          }
        }
        else{
          Cputs( TEXT("�s���ȃG�X�P�[�v�����ł�") );
          return XBADTOKEN;
        }
      }
      else{
        if( xgetc() =='\'' ){
          TokenValue = _integer( c );
          return NUMBER;
        }
        else{
          Cputs( TEXT("�����萔�̏I�[���s���ł�") );
          return XBADTOKEN;
        }
      }
    }

    /* ������萔 */
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
                Cputs(TEXT("�s���ȃG�X�P�[�v�����ł�\'\\x0*\'\n"));
              }
            }
            else{
              Cputs(TEXT("�s���ȃG�X�P�[�v�����ł�\'\\x*0\'\n"));
            }
          }
          else{
            Cputs(TEXT("�s���ȃG�X�P�[�v�����ł�\'\\*\'\n"));
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
        Cputs(TEXT("������萔�̏I�[���s���ł�\n"));
        return XBADTOKEN;
      }
    }

    /* �����A���� */
    if( c =='0' ){
      TokenText[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '7' ){
        do {                 // 8�i����
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
      else if( c == 'x' || c == 'X' ){ //16�i����
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
        Cputs(TEXT("16�i�萔�̕\�L���s���ł�\n"));
        return XBADTOKEN;
      }
      else if( c == '.' ){ //1�ȉ��̏���
        TokenText[ i++ ] = c;
        if( ( c = xgetc() ) >= '0' && c <= '9' ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
          if( c == 'f' || c == 'F' ){   // ����
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
          else if( c == 'e' || c == 'E' ){   // ����(�w���\��)
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
            Cputs(TEXT("�����萔�̕\�L���s���ł�\n"));
            return XBADTOKEN;
          }
          else{                           // �P���ȏ���
            xungetc( c );
            TokenText[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            TokenValue = _real( f );

//printValue( TokenValue );

            return REAL;
          }
        }
        else if( c == 'f' || c == 'F' ){   // ����
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // ����(�w���\��)
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
          Cputs(TEXT("�����萔�̕\�L���s���ł�\n"));
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
      else{ // 0�̏ꍇ
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _integer(0);

//printValue( TokenValue );

        return NUMBER;
      }
    }
    else if( c >= '1' && c <= '9' ){     //10�i��
      do {
        TokenText[ i++ ] = c;
        c = xgetc();
      } while( '0' <= c && c <= '9' );
      if( c == '.' ){                    //����(�����_�\��)
        TokenText[ i++ ] = c;
        if( ( c = xgetc() ) >= '1' && c <= '9' ){
          do {
            TokenText[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
        }
        if( c == 'f' || c == 'F' ){   // ����
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // ����(�w���\��)
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
          Cputs(TEXT("�����萔�̕\�L���s���ł�\n"));
          return XBADTOKEN;
        }
        else{                           // �P���ȏ���
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
      }
      else if( c == 'f' || c == 'F' ){   // ����
        TokenText[ i ] = '\0';
        MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
        swscanf( tbuf, TEXT("%lg"), &f );
        TokenValue = _real( f );

//printValue( TokenValue );

        return REAL;
      }
      else if( c == 'e' || c == 'E' ){   // ����(�w���\��)
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
        Cputs(TEXT("�����萔�̕\�L���s���ł�\n"));
        return XBADTOKEN;
      }
      else if( c == 'l' || c == 'L' ){  // ����
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
      else if( c == 'u' || c == 'U' ){  // ����
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
      else{  //�P���Ȑ���
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _integer( atoi( TokenText ) );

//printValue( TokenValue );

        return NUMBER;
      }
    }
    else if( c == '.' ){  // 1�ȉ��̏���
      TokenText[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '9' ){
        do {
          TokenText[ i++ ] = c;
          c = xgetc();
        } while( '0' <= c && c <= '9' );
        if( c == 'f' || c == 'F' ){   // ����
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
        else if( c == 'e' || c == 'E' ){   // ����(�w���\��)
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
          Cputs(TEXT("�����萔�̕\�L���s���ł�\n"));
          return XBADTOKEN;
        }
        else{                           // �P���ȏ���
          xungetc( c );
          TokenText[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, TokenText, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          TokenValue = _real( f );

//printValue( TokenValue );

          return REAL;
        }
      }
      else{ // �s���I�h(SYMBOL)�Ƃ��ĔF��
        xungetc( c );
        TokenText[ i ] = '\0';
        TokenValue = _symbol( TokenText );

//printValue( TokenValue );

        return SYMBOL;
      }
    }

    /* �V���{�� */
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


/* Token����v���Ă��邪�ǂ�����Ԃ� */
int isTokenMatch( char* token ){
  char c;

  // ��r�ΏۂƂ��镶�����Ƃ肾��
  switch( token[0] ){

  // �������ň�v�����Ɣ��f����
  case '\0':
    return 1;

  // �󔒂ł���Ȃ��v�����Ɣ��f����
  case ' ':
    if( ( c = xgetc() ) == ' ' || c == '\t' || c == '\r' || c == '\n'  ) return 1;
    xungetc( c );
    return 0;

  // �p���ł͂Ȃ��ꍇ�͈�v�����Ɣ��f����
  case '\a':
    if( ( c = xgetc() ) == '_' || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ){
      xungetc( c );
      return 0;
    }
    xungetc( c );
    return 1;

  // �p�����ł͂Ȃ��ꍇ�͈�v�����Ɣ��f����
  case '\b':
    if( ( c = xgetc() ) == '_' || ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) || ( c >= '0' && c <= '9' )){
      xungetc( c );
      return 0;
    }
    xungetc( c );
    return 1;

  // ��������v����ꍇ�͒���̕�������͂��Ĉ�v���Ă��邩�𔻒f���A�����łȂ��ꍇ�͕s��v�ł���Ɣ��f����
  default:
    if( token[0] == ( c = xgetc() ) ){ 
      if( isTokenMatch( &token[1] ) ) return 1;
    }
    xungetc( c );
    return 0;

  }
}


/* �t�@�C���܂��̓R���\�[������P�������͂��� */
char xgetc(){
  TCHAR tchar[2];
  char  xchar[2], c;
  DWORD size;
  
  /* ���͕������c���Ă����炻���Ԃ� */
  if( UngetP > 0 ) return UngetBuf[ --UngetP ];

  /* ���͕������c���Ă��Ȃ��ꍇ */
  else{

    /* �R���\�[������1�������� */
    if( InputFile == INVALID_HANDLE_VALUE ){
      Putch( *tchar = Getch() ); if( *tchar == TEXT('\r') ) Putch( TEXT('\n') );
      tchar[1] = TEXT('\0');
      WideCharToMultiByte( CP_ACP, 0, tchar, -1, xchar, 252, NULL, NULL );
      LastInputChar = '\0';
      return *xchar;
    }

    /* �t�@�C������1�������� */
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


/* �t�@�C���܂��̓R���\�[������̂P�������͂����ɖ߂� */
void xungetc( char c ){
  UngetBuf[ UngetP++ ] = c;
}


/* �t�@�C���܂��̓R���\�[���ɂP�����o�� */
void xputc( char c ){
  char  xchar[2];
  TCHAR tchar[2];
  DWORD size;

  /* �R���\�[���ɂP�����o�� */
  if( OutputFile == INVALID_HANDLE_VALUE ){
    xchar[0] = c;
    xchar[1] = '\0';
    MultiByteToWideChar( CP_ACP, 0, xchar, -1, tchar, 252 );
    Putch( tchar[0] );
  }

  /* �t�@�C���ɂP�����o�� */
  else WriteFile( OutputFile, (void *)&c, 1, &size, NULL );
}


/* ���̓t�@�C�����J�� */
void xopenInputFile( char* file_name ){
  TCHAR tfname[256];

  MultiByteToWideChar( CP_ACP, 0, file_name, -1, tfname, 252 );
  InputFile = CreateFile( tfname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( InputFile == INVALID_HANDLE_VALUE ) Cputs( TEXT("�t�@�C���I�[�v�����s\n") );
}


/* ���̓t�@�C������� */
void xcloseInputFile(){
  if( InputFile == INVALID_HANDLE_VALUE ) return;
  CloseHandle( InputFile );
  InputFile = INVALID_HANDLE_VALUE;
}


/* �o�̓t�@�C�����J�� */
void xopenOutputFile( char* file_name ){
  TCHAR tfname[256];

  MultiByteToWideChar( CP_ACP, 0, file_name, -1, tfname, 252 );
  OutputFile = CreateFile( tfname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
  if( OutputFile == INVALID_HANDLE_VALUE ) Cputs( TEXT("�t�@�C���I�[�v�����s\n") );
}


/* �o�̓t�@�C������� */
void xcloseOutputFile(){
  if( OutputFile == INVALID_HANDLE_VALUE ) return;
  CloseHandle( OutputFile );
  OutputFile = INVALID_HANDLE_VALUE;
}


/* �S�p�������ǂ����𔻒肷�� */
int isZenkaku(char c){
  unsigned char cc = (unsigned char)c;
  return (0x81 <= cc && cc <= 0x9F || 0xE0 <= cc && cc <= 0xFC);
}