/*
   sh3ansic.y	-- a parser of ANSI C
   SH-3 C KMYACCパーサ
      for CASSIOPEIA A-51


    コマンド:
      kmyacc -m \Windows\kmyacc.par -dlv sh3ansic.y

*/


%{

#define  YYSTYPE  ListPtr
#define  FNAME_MAX 80
#include "lisp.h"
#include "YTAB.H"
#define VAL if(pass == 2) 

/* 入出力ファイル */
#define INPUT_FILE  "\\Storage Card\\__tmp2"
#define OUTPUT_FILE "\\Storage Card\\__tmp1"

/* 変数の記憶クラス */
#define CLASS_EXTERN 1
#define CLASS_STATIC 2
#define CLASS_AUTO   3


/* プロトタイプ宣言 */
List* def_type( char* attr, List* type_name, List* pointer, List* def_vars, List* def_name );
List* def_func( List* func_name, char* attr, List* type_name, List* pointer, List* args, List* func_body );
List* set_context( List* content );
List* cat_string( List* str1, List* str2 );
int   is_type_name( char* name );


extern   YYSTYPE yylval;
char     YYTEXT_BUF[256];
char*    yytext;
int      yynerrs;
List*    type_names;
int      pass;
int      LineNo = 1;
char     FileName[FNAME_MAX] = {0};
char     PrevName[FNAME_MAX] = {0};

%} 


/* 以下の記述によりYACCプログラムでyyparse()関数を生成する */


/* トークンの宣言 */

%token    ';' '{' '}' ',' ':' '=' '(' ')' '[' ']' '<' '>' '.' '&' '!' '~' '-' '+' '*' '/' '%' '^' '|' '?' '@'

%token    IDR
%token    TYPE_NAME
%token    CONST_INT
%token    CONST_REAL
%token    CONST_STRING

%token    KAKERU_EQUAL
%token    WARU_EQUAL
%token    PERCENT_EQUAL
%token    PLUS_EQUAL
%token    MINUS_EQUAL
%token    LEFT_SHIFT_EQUAL
%token    RIGHT_SHIFT_EQUAL
%token    AND_EQUAL
%token    EOR_EQUAL
%token    OR_EQUAL
%token    ARROW
%token    PLUSPLUS
%token    MINUSMINUS
%token    SIZEOF
%token    LEFT_SHIFT
%token    RIGHT_SHIFT
%token    LESS_THAN_EQUAL
%token    GREATER_THAN_EQUAL
%token    EQUAL_EQUAL
%token    NOT_EQUAL
%token    ANDAND
%token    OROR
%token    AUTO
%token    REGISTER
%token    STATIC
%token    EXTERN
%token    TYPEDEF
%token    xVOID
%token    CHAR
%token    SHORT
%token    INT
%token    LONG
%token    FLOAT
%token    DOUBLE
%token    SIGNED
%token    UNSIGNED
%token    xCONST
%token    VOLATILE
%token    TENTENTEN
%token    ENUM
%token    STRUCT
%token    UNION
%token    GOTO
%token    CONTINUE
%token    BREAK
%token    RETURN
%token    WHILE
%token    DO
%token    FOR
%token    IF
%token    ELSE
%token    SWITCH
%token    CASE
%token    DEFAULT
%token    BADTOKEN
%token    ASM

%start program

%%


/* 文法の定義 */
program
    : declare                                           { $$ = set_context( $1 ); }
    | program declare                                   { $$ = concatLists( $1, set_context( $2 ) ); }
    | error '}'                                         { $$ = $1; }

declare
    : def_var                                           { $$ = $1; }
    | def_struct                                        { $$ = $1; }
    | def_union                                         { $$ = $1; }
    | def_type                                          { $$ = $1; }
    | proto_type                                        { $$ = $1; }
    | def_function                                      { $$ = $1; }

def_var
    : STRUCT IDR var_list ';'                           { $$ = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("struct"), $2, list( $3 ), NULL ); }
    | UNION  IDR var_list ';'                           { $$ = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("union"),  $2, list( $3 ), NULL ); }
    | type   var_list ';'                               { $$ = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("elem"),   $1, list( $2 ), NULL ); }
    | var_class STRUCT IDR var_list ';'                 { $$ = _list( _symbol("var"),       $1,             _symbol("struct"), $3, list( $3 ), NULL ); }
    | var_class UNION  IDR var_list ';'                 { $$ = _list( _symbol("var"),       $1,             _symbol("union"),  $3, list( $3 ), NULL ); }
    | var_class type   var_list ';'                     { $$ = _list( _symbol("var"),       $1,             _symbol("elem"),   $2, list( $3 ), NULL ); }
    | STRUCT IDR '{' def_vars '}' var_list ';'          { $$ = _list( _symbol("struct"),    $2,              list( $4 ), list( $6 ), NULL ); }
    | STRUCT     '{' def_vars '}' var_list ';'          { $$ = _list( _symbol("struct"), _symbol("*"),       list( $3 ), list( $5 ), NULL ); }
    | UNION IDR  '{' def_vars '}' var_list ';'          { $$ = _list( _symbol("union"),     $2,              list( $4 ), list( $6 ), NULL ); }
    | UNION      '{' def_vars '}' var_list ';'          { $$ = _list( _symbol("union"),  _symbol("*"),       list( $3 ), list( $5 ), NULL ); }

def_struct
    : STRUCT IDR '{' def_vars '}' ';'                   { $$ = _list( _symbol("struct"),      $2,       list( $4 ), list(NULL), NULL ); }

def_union
    : UNION  IDR '{' def_vars '}' ';'                   { $$ = _list( _symbol("union"),       $2,       list( $4 ), list(NULL), NULL ); }

def_type
    : TYPEDEF type       IDR ';'                        { $$ = def_type( "elem",       $2,       _integer(0), NULL, $3 ); }
    | TYPEDEF type       TYPE_NAME ';'                  { $$ = def_type( "elem",       $2,       _integer(0), NULL, $3 ); }
    | TYPEDEF type   ptr IDR ';'                        { $$ = def_type( "elem",       $2,           $3,      NULL, $4 ); }
    | TYPEDEF type   ptr TYPE_NAME ';'                  { $$ = def_type( "elem",       $2,           $3,      NULL, $4 ); }
    | TYPEDEF STRUCT IDR     IDR ';'                    { $$ = def_type( "struct",     $3,       _integer(0), NULL, $4 ); }
    | TYPEDEF STRUCT IDR     TYPE_NAME ';'              { $$ = def_type( "struct",     $3,       _integer(0), NULL, $4 ); }
    | TYPEDEF STRUCT IDR ptr IDR ';'                    { $$ = def_type( "struct",     $3,           $4,      NULL, $5 ); }
    | TYPEDEF STRUCT IDR ptr TYPE_NAME ';'              { $$ = def_type( "struct",     $3,           $4,      NULL, $5 ); }
    | TYPEDEF UNION  IDR     IDR ';'                    { $$ = def_type( "union",      $3,       _integer(0), NULL, $4 ); }
    | TYPEDEF UNION  IDR     TYPE_NAME ';'              { $$ = def_type( "union",      $3,       _integer(0), NULL, $4 ); }
    | TYPEDEF UNION  IDR ptr IDR ';'                    { $$ = def_type( "union",      $5,           $4,      NULL, $5 ); }
    | TYPEDEF UNION  IDR ptr TYPE_NAME ';'              { $$ = def_type( "union",      $5,           $4,      NULL, $5 ); }
    | TYPEDEF STRUCT '{' def_vars '}' IDR ';'           { $$ = def_type( "struct", _symbol("*"), _integer(0),  $4 , $6 ); }
    | TYPEDEF STRUCT '{' def_vars '}' TYPE_NAME ';'     { $$ = def_type( "struct", _symbol("*"), _integer(0),  $4 , $6 ); }
    | TYPEDEF UNION  '{' def_vars '}' IDR ';'           { $$ = def_type( "union",  _symbol("*"), _integer(0),  $4 , $6 ); }
    | TYPEDEF UNION  '{' def_vars '}' TYPE_NAME ';'     { $$ = def_type( "union",  _symbol("*"), _integer(0),  $4 , $6 ); }
    | TYPEDEF STRUCT IDR '{' def_vars '}' IDR ';'       { $$ = def_type( "struct",     $3,       _integer(0),  $5 , $7 ); }
    | TYPEDEF STRUCT IDR '{' def_vars '}' TYPE_NAME ';' { $$ = def_type( "struct",     $3,       _integer(0),  $5 , $7 ); }
    | TYPEDEF UNION  IDR '{' def_vars '}' IDR ';'       { $$ = def_type( "union",      $3,       _integer(0),  $5 , $7 ); }
    | TYPEDEF UNION  IDR '{' def_vars '}' TYPE_NAME ';' { $$ = def_type( "union",      $3,       _integer(0),  $5 , $7 ); }

proto_type
    : type IDR '(' formal_arguments ')' ';'             { $$ = def_func( $2, "elem",   $1, _integer(0),    $4,      list(NULL) ); } 
    | type ptr IDR '(' formal_arguments ')' ';'         { $$ = def_func( $3, "elem",   $1,      $2,        $5,      list(NULL) ); } 
    | STRUCT IDR ptr IDR '(' formal_arguments ')' ';'   { $$ = def_func( $4, "struct", $2,      $3,        $6,      list(NULL) ); }
    | UNION  IDR ptr IDR '(' formal_arguments ')' ';'   { $$ = def_func( $4, "union",  $2,      $3,        $6,      list(NULL) ); }

type
    : INT                                               { $$ = _symbol("long");   } // 暫定的にint/uintはlong/ulongと同じものとしておく
    | SIGNED                                            { $$ = _symbol("long");   }
    | UNSIGNED                                          { $$ = _symbol("ulong");  }
    | SIGNED INT                                        { $$ = _symbol("long");   }
    | UNSIGNED INT                                      { $$ = _symbol("ulong");  }
    | LONG                                              { $$ = _symbol("long");   }
    | SIGNED LONG                                       { $$ = _symbol("long");   }
    | UNSIGNED LONG                                     { $$ = _symbol("ulong");  }
    | SHORT                                             { $$ = _symbol("short");  }
    | SIGNED SHORT                                      { $$ = _symbol("short");  }
    | UNSIGNED SHORT                                    { $$ = _symbol("ushort"); }
    | CHAR                                              { $$ = _symbol("char");   }
    | SIGNED CHAR                                       { $$ = _symbol("char");   }
    | UNSIGNED CHAR                                     { $$ = _symbol("uchar");  }
    | FLOAT                                             { $$ = _symbol("float");  }
    | DOUBLE                                            { $$ = _symbol("float");  }  // 暫定的にdoubleとfloatは同じものとしておく
    | xVOID                                             { $$ = _symbol("void");   }
    | TYPE_NAME                                         { $$ = $1; }

ptr
    : '*'                                                              { $$ = _integer(1); }
    | ptr '*'                                                          { ( $$ = $1 )->value.integer++; }

def_vars
    : def_var                                                          { $$ = $1; }
    | def_vars def_var                                                 { $$ = concatLists( $1, $2 ); }

var_class
    : AUTO                                                             { $$ = _integer(CLASS_AUTO);     }
    | STATIC                                                           { $$ = _integer(CLASS_STATIC);   }
    | EXTERN                                                           { $$ = _integer(CLASS_EXTERN);   }
    | REGISTER                                                         { $$ = _integer(CLASS_AUTO);     } // 暫定的にauto変数と解釈する
    | VOLATILE                                                         { $$ = _integer(CLASS_AUTO);     } // 暫定的にauto変数と解釈する
    | xCONST                                                           { $$ = _integer(CLASS_AUTO);     } // 暫定的にauto変数と解釈する

var_list
    : var                                                              { $$ = $1; }
    | var_list ',' var                                                 { $$ = concatLists( $1, $3 ); }

var
    : IDR                                                              { $$ = _list( $1, _integer(0), list(NULL), list(NULL), list(NULL), NULL ); }
    | IDR '=' initialiser                                              { $$ = _list( $1, _integer(0), list(NULL),     $3,     list(NULL), NULL ); }
    | IDR array                                                        { $$ = _list( $1, _integer(0), list( $2 ), list(NULL), list(NULL), NULL ); }
    | IDR array '=' initialiser                                        { $$ = _list( $1, _integer(0), list( $2 ),     $4,     list(NULL), NULL ); }
    | ptr IDR                                                          { $$ = _list( $2,     $1,      list(NULL), list(NULL), list(NULL), NULL ); }
    | ptr IDR '=' initialiser                                          { $$ = _list( $2,     $1,      list(NULL),     $4,     list(NULL), NULL ); }
    | ptr IDR array                                                    { $$ = _list( $2,     $1,      list( $3 ), list(NULL), list(NULL), NULL ); }
    | ptr IDR array '=' initialiser                                    { $$ = _list( $2,     $1,      list( $3 ),     $5,     list(NULL), NULL ); }
    | '(' ptr IDR ')' '(' formal_arguments ')'     /* 関数ポインタ */  { $$ = _list( $3,     $2,      list(NULL), list(NULL), list( $6 ), NULL ); }
    | '(' ptr IDR ')' '(' formal_arguments ')' '=' initialiser         { $$ = _list( $3,     $2,      list(NULL),     $9,     list( $6 ), NULL ); }
    | '(' ptr IDR array ')' '(' formal_arguments ')'                   { $$ = _list( $3,     $2,      list( $4 ), list(NULL), list( $7 ), NULL ); }
    | '(' ptr IDR array ')' '(' formal_arguments ')' '=' initialiser   { $$ = _list( $3,     $2,      list( $4 ),     $10,    list( $7 ), NULL ); }

array
    : '[' ']'                                                          { $$ = _integer(0x7fffffff); }
    | '[' CONST_INT ']'                                                { $$ = $2; }
    | array '[' CONST_INT ']'                                          { $$ = concatLists( $1, $3 ); }

initialisers
    : initialiser                                                      { $$ = $1; }
    | initialisers ',' initialiser                                     { $$ = concatLists( $1, $3 ); }

initialiser
    : assign_expression                                                { $$ = $1; }
    | '{' initialisers '}'                                             { $$ = list( $2 ); }

def_function
    : type IDR '(' formal_arguments ')' function_body                  { $$ = def_func( $2, "elem",   $1, _integer(0),    $4,      $6 ); } 
    | type ptr IDR '(' formal_arguments ')' function_body              { $$ = def_func( $3, "elem",   $1,      $2,        $5,      $7 ); } 
    | STRUCT IDR ptr IDR '(' formal_arguments ')' function_body        { $$ = def_func( $4, "struct", $2,      $3,        $6,      $8 ); }
    | UNION  IDR ptr IDR '(' formal_arguments ')' function_body        { $$ = def_func( $4, "union",  $2,      $3,        $6,      $8 ); }

formal_arguments
    :                                                                  { $$ = list(NULL); }
    | formal_argument_list                                             { $$ = list( $1 ); } 

formal_argument_list
    : formal_argument                                                  { $$ = $1; }
    | formal_argument_list ',' formal_argument                         { $$ = concatLists( $1, $3 ); }
    | formal_argument_list ',' TENTENTEN                               { $$ = $1; }  /* とりあえず無視する */
formal_argument
    : type IDR                                                         { $$ = _list( _symbol("arg"),     $2,       _symbol("elem"),   $1, _integer(0), NULL ); }
    | type                                                             { $$ = _list( _symbol("arg"), _symbol("*"), _symbol("elem"),   $1, _integer(0), NULL ); }

    | type ptr IDR                                                     { $$ = _list( _symbol("arg"),     $3,       _symbol("elem"),   $1,     $2,      NULL ); }
    | type ptr                                                         { $$ = _list( _symbol("arg"), _symbol("*"), _symbol("elem"),   $1,     $2,      NULL ); }
    | STRUCT IDR  ptr IDR                                              { $$ = _list( _symbol("arg"),     $4,       _symbol("struct"), $2,     $3,      NULL ); }  
    | STRUCT IDR  ptr                                                  { $$ = _list( _symbol("arg"), _symbol("*"), _symbol("struct"), $2,     $3,      NULL ); }  
    | UNION  IDR  ptr IDR                                              { $$ = _list( _symbol("arg"),     $4,       _symbol("union"),  $2,     $3,      NULL ); } 
    | UNION  IDR  ptr                                                  { $$ = _list( _symbol("arg"), _symbol("*"), _symbol("union"),  $2,     $3,      NULL ); } 

function_body
    : '{' '}'                                                          { $$ = _list( _symbol("{}"), NULL );}
    | '{' statements '}'                                               { $$ = _list( _symbol("{}"), $2, NULL ); }

statements
    : statement                                                        { $$ = set_context( $1 ); }
    | statements statement                                             { $$ = concatLists( $1, set_context( $2 ) ); }

statement
    : ';'                                                              { $$ = _list( _symbol("nop"), NULL ); }
    | '{' '}'                                                          { $$ = _list( _symbol("nop"), NULL ); }
    | '{' statements '}'                                               { $$ = _list( _symbol("{}"), $2, NULL ); }
    | IF '(' expression ')' statement                                  { $$ = _list( _symbol("if"), $3, $5, NULL );     }
    | IF '(' expression ')' statement ELSE statement                   { $$ = _list( _symbol("if"), $3, $5, $7, NULL ); }
    | WHILE '(' expression ')' statement                               { $$ = _list( _symbol("while"), $3, $5, NULL ); }
    | DO statement WHILE '(' expression ')' ';'                        { $$ = _list( _symbol("do"), $2, $5, NULL ); }
    | FOR '(' ';' ';' ')' statement                                    { $$ = _list( _symbol("for"), list(NULL), list(NULL), list(NULL), $6, NULL ); }
    | FOR '(' ';' ';' expression ')' statement                         { $$ = _list( _symbol("for"), list(NULL), list(NULL),     $5,     $7, NULL ); }
    | FOR '(' ';' expression ';' ')' statement                         { $$ = _list( _symbol("for"), list(NULL),     $4,     list(NULL), $7, NULL ); }
    | FOR '(' ';' expression ';' expression ')' statement              { $$ = _list( _symbol("for"), list(NULL),     $4,         $6,     $8, NULL ); }
    | FOR '(' expression ';' ';' ')' statement                         { $$ = _list( _symbol("for"),     $3,     list(NULL), list(NULL), $7, NULL ); }
    | FOR '(' expression ';' ';' expression ')' statement              { $$ = _list( _symbol("for"),     $3,     list(NULL),     $6,     $8, NULL ); }
    | FOR '(' expression ';' expression ';' ')' statement              { $$ = _list( _symbol("for"),     $3,         $5,     list(NULL), $8, NULL ); }
    | FOR '(' expression ';' expression ';' expression ')' statement   { $$ = _list( _symbol("for"),     $3,         $5,         $7,     $9, NULL ); }
    | SWITCH '(' expression ')' statement                              { $$ = _list( _symbol("switch"), $3, $5, NULL ); }
    | GOTO IDR ';'                                                     { $$ = _list( _symbol("goto"),     $2, NULL ); }
    | BREAK ';'                                                        { $$ = _list( _symbol("break"),    NULL );     }
    | CONTINUE ';'                                                     { $$ = _list( _symbol("continue"), NULL );     }
    | RETURN expression ';'                                            { $$ = _list( _symbol("return"), $2, NULL ); }
    | RETURN ';'                                                       { $$ = _list( _symbol("return"), NULL ); }
    | IDR ':'                                                          { $$ = _list( _symbol("label"),    $1, NULL ); }
    | CASE constant ':'                                                { $$ = _list( _symbol("case"),     $2, NULL ); }
    | DEFAULT ':'                                                      { $$ = _list( _symbol("default"),  NULL );     }
    | def_var                                                          { $$ = $1; }
    | expression ';'                                                   { $$ = $1; }
    | ASM '(' CONST_STRING ')' ';'                                     { $$ = _list( _symbol("asm"),     $3, NULL ); }

expression
    : assign_expression                                                { $$ = $1; }
    | expression ',' assign_expression                                 { $$ = _list( _symbol(","), $1, $3, NULL ); }

assign_expression
    : condition_expression                                             { $$ = $1; }
    | lvalue '=' assign_expression                                     { $$ = _list( _symbol("="),   $1, $3, NULL ); }
    | lvalue KAKERU_EQUAL assign_expression                            { $$ = _list( _symbol("*="),  $1, $3, NULL ); }
    | lvalue WARU_EQUAL assign_expression                              { $$ = _list( _symbol("/="),  $1, $3, NULL ); }
    | lvalue PERCENT_EQUAL assign_expression                           { $$ = _list( _symbol("%="),  $1, $3, NULL ); }
    | lvalue PLUS_EQUAL assign_expression                              { $$ = _list( _symbol("+="),  $1, $3, NULL ); }
    | lvalue MINUS_EQUAL assign_expression                             { $$ = _list( _symbol("-="),  $1, $3, NULL ); }
    | lvalue LEFT_SHIFT_EQUAL assign_expression                        { $$ = _list( _symbol("<<="), $1, $3, NULL ); }
    | lvalue RIGHT_SHIFT_EQUAL assign_expression                       { $$ = _list( _symbol(">>="), $1, $3, NULL ); }
    | lvalue AND_EQUAL assign_expression                               { $$ = _list( _symbol("&="),  $1, $3, NULL ); }
    | lvalue EOR_EQUAL assign_expression                               { $$ = _list( _symbol("^="),  $1, $3, NULL ); }
    | lvalue OR_EQUAL assign_expression                                { $$ = _list( _symbol("|="),  $1, $3, NULL ); }

condition_expression
    : oror_expression                                                  { $$ = $1; }
    | oror_expression '?' expression ':' condition_expression          { $$ = _list( _symbol("?"),  $1, $3, $5, NULL ); }

oror_expression
    : andand_expression                                                { $$ = $1; }
    | oror_expression OROR andand_expression                           { $$ = _list( _symbol("||"),   $1, $3, NULL ); }

andand_expression
    : or_expression                                                    { $$ = $1; }
    | andand_expression ANDAND or_expression                           { $$ = _list( _symbol("&&"),   $1, $3, NULL ); }

or_expression
    : eor_expression                                                   { $$ = $1; }
    | or_expression '|' eor_expression                                 { $$ = _list( _symbol("|"),   $1, $3, NULL ); }

eor_expression
    : and_expression                                                   { $$ = $1; }
    | eor_expression '^' and_expression                                { $$ = _list( _symbol("^"),   $1, $3, NULL ); }

and_expression
    : equal_expression                                                 { $$ = $1; }
    | and_expression '&' equal_expression                              { $$ = _list( _symbol("&"),   $1, $3, NULL ); }

equal_expression
    : relation_expression                                              { $$ = $1; }
    | equal_expression EQUAL_EQUAL relation_expression                 { $$ = _list( _symbol("=="),   $1, $3, NULL ); }
    | equal_expression NOT_EQUAL relation_expression                   { $$ = _list( _symbol("!="),   $1, $3, NULL ); }

relation_expression
    : shift_expression                                                 { $$ = $1; }
    | relation_expression '<' shift_expression                         { $$ = _list( _symbol("<"),   $1, $3, NULL ); }
    | relation_expression '>' shift_expression                         { $$ = _list( _symbol(">"),   $1, $3, NULL ); }
    | relation_expression LESS_THAN_EQUAL shift_expression             { $$ = _list( _symbol("<="),  $1, $3, NULL ); }
    | relation_expression GREATER_THAN_EQUAL shift_expression          { $$ = _list( _symbol(">="),  $1, $3, NULL ); }

shift_expression
    : factor                                                           { $$ = $1; }
    | shift_expression LEFT_SHIFT factor                               { $$ = _list( _symbol("<<"),   $1, $3, NULL ); }
    | shift_expression RIGHT_SHIFT factor                              { $$ = _list( _symbol(">>"),   $1, $3, NULL ); }

factor
    : term                                                             { $$ = $1; }
    | factor '+' term                                                  { $$ = _list( _symbol("+"),   $1, $3, NULL ); }
    | factor '-' term                                                  { $$ = _list( _symbol("-"),   $1, $3, NULL ); }

term
    : cast_expression                                                  { $$ = $1; }
    | term '*' cast_expression                                         { $$ = _list( _symbol("*"),   $1, $3, NULL ); }
    | term '/' cast_expression                                         { $$ = _list( _symbol("/"),   $1, $3, NULL ); }
    | term '%' cast_expression                                         { $$ = _list( _symbol("%"),   $1, $3, NULL ); }

cast_expression
    : lvalue                                                { $$ = $1; }
    | '(' type ')' cast_expression                          { $$ = _list( _symbol("cast"), _symbol("elem"),   $2, _integer(0), $4, NULL ); }
    | '(' type ptr ')' cast_expression                      { $$ = _list( _symbol("cast"), _symbol("elem"),   $2,      $3,     $5, NULL ); }
    | '(' STRUCT IDR ptr ')' cast_expression                { $$ = _list( _symbol("cast"), _symbol("struct"), $3,      $4,     $6, NULL ); }
    | '(' UNION  IDR ptr ')' cast_expression                { $$ = _list( _symbol("cast"), _symbol("union"),  $3,      $4,     $6, NULL ); }

lvalue
    : lvalue2                                               { $$ = $1; }
    | PLUSPLUS lvalue                                       { $$ = _list( _symbol("++x"), $2, NULL );    }
    | MINUSMINUS lvalue                                     { $$ = _list( _symbol("--x"), $2, NULL );    }
    | '&' cast_expression                                   { $$ = _list( _symbol("&x"), $2, NULL );     }
    | '*' cast_expression                                   { $$ = _list( _symbol("*x"), $2, NULL );     }
    | '+' cast_expression                                   { $$ = _list( _symbol("+x"), $2, NULL );     }
    | '-' cast_expression                                   { $$ = _list( _symbol("-x"), $2, NULL );     }
    | '~' cast_expression                                   { $$ = _list( _symbol("~x"), $2, NULL );     }
    | '!' cast_expression                                   { $$ = _list( _symbol("!x"), $2, NULL );     }
    | SIZEOF '(' type ')'                                   { $$ = _list( _symbol("sztype"), _symbol("elem"),   _integer(0), $3, NULL ); } 
    | SIZEOF '(' type ptr ')'                               { $$ = _list( _symbol("sztype"), _symbol("elem"),        $4,     $3, NULL ); } 
    | SIZEOF '(' STRUCT IDR ')'                             { $$ = _list( _symbol("sztype"), _symbol("struct"), _integer(0), $4, NULL ); } 
    | SIZEOF '(' STRUCT IDR ptr ')'                         { $$ = _list( _symbol("sztype"), _symbol("struct"),      $5,     $4, NULL ); } 
    | SIZEOF '(' UNION  IDR ')'                             { $$ = _list( _symbol("sztype"), _symbol("union"),  _integer(0), $4, NULL ); } 
    | SIZEOF '(' UNION  IDR ptr ')'                         { $$ = _list( _symbol("sztype"), _symbol("union"),       $5,     $4, NULL ); } 
    | SIZEOF '(' IDR ')'                                    { $$ = _list( _symbol("szvar"), $3, NULL ); } 

lvalue2
    : primitive                                             { $$ = $1; }
    | lvalue2 array_argument                                { $$ = _list( _symbol("[]"),    $1,     $2,     NULL ); }
    | lvalue2 '(' actual_arguments ')'                      { $$ = _list( _symbol("call"),  $1,     $3,     NULL ); }
    | '(' lvalue2 ')' '(' actual_arguments ')'              { $$ = _list( _symbol("call&"), $2,     $5,     NULL ); }
    | lvalue2 '.' IDR                                       { $$ = _list( _symbol("."),     $1,     $3,     NULL ); }
    | lvalue2 ARROW IDR                                     { $$ = _list( _symbol("->"),    $1,     $3,     NULL ); }
    | lvalue2 PLUSPLUS                                      { $$ = _list( _symbol("x++"),   $1, NULL ); }
    | lvalue2 MINUSMINUS                                    { $$ = _list( _symbol("x--"),   $1, NULL ); }

primitive
    : IDR                          /* 単純変数参照 */       { $$ = _list( _symbol("@var"), $1, NULL );   }
    | constant                                              { $$ = _list( _symbol("@const"), $1, NULL ); }
    | '(' expression ')'                                    { $$ = $2; }

constant
    : CONST_INT                                             { $$ = $1; }
    | CONST_REAL                                            { $$ = $1; }
    | string                                                { $$ = $1; }
 
string
    : CONST_STRING                                          { $$ = $1; }
    | string CONST_STRING                                   { $$ = cat_string( $1, $2 ); }
 
array_argument
    : '[' expression ']'                                    { $$ = $2; }
    | array_argument '[' expression ']'                     { $$ = _concat( $1, $3, NULL );}

actual_arguments
    :                                                       { $$ = list(NULL); }
    | actual_argument_list                                  { $$ = list( $1 ); }

actual_argument_list
    : assign_expression                                     { $$ = $1; }
    | actual_argument_list ',' assign_expression            { $$ = concatLists( $1, $3 ); }

%%


/* トークン解析関数 */
int yylex(){
  int    c, d, e, i, t, size;
  char   cbuf[4];
  TCHAR  tbuf[252];
  double f;
  
  i = 0;
  yytext = YYTEXT_BUF;
  while( ( c = xgetc() ) != EOF ){
    if( c == ' ' || c == '\t' || c == '\r' || c == '\n' ) continue;
    if( c == '#' ){
      char cc;
      for( d = LineNo = 0; d < 8; d++ ){
        ReadFile( InputFile, (void*)&cc, 1, &size, NULL );
        LineNo = LineNo * 10 + (int)( cc - '0' );
      }
      d = 0;
      while( ReadFile( InputFile, (void*)&cc, 1, &size, NULL ), cc != '\n'  ) FileName[ d++ ] = cc;
      FileName[ d ] = '\0';
      continue;
    }

    xungetc( c );
    if( isTokenMatch("*=") )		return KAKERU_EQUAL;
    if( isTokenMatch("/=") )		return WARU_EQUAL;
    if( isTokenMatch("%=") )		return PERCENT_EQUAL;
    if( isTokenMatch("+=") )		return PLUS_EQUAL;
    if( isTokenMatch("-=") )		return MINUS_EQUAL;
    if( isTokenMatch("<<=") )		return LEFT_SHIFT_EQUAL;
    if( isTokenMatch(">>=") )		return RIGHT_SHIFT_EQUAL;
    if( isTokenMatch("&=") )		return AND_EQUAL;
    if( isTokenMatch("^=") )		return EOR_EQUAL;
    if( isTokenMatch("|=") )		return OR_EQUAL;
    if( isTokenMatch("->") )		return ARROW;
    if( isTokenMatch("++") )		return PLUSPLUS;
    if( isTokenMatch("--") )		return MINUSMINUS;
    if( isTokenMatch("sizeof\a") )		return SIZEOF;
    if( isTokenMatch("<<") )		return LEFT_SHIFT;
    if( isTokenMatch(">>") )		return RIGHT_SHIFT;
    if( isTokenMatch("<=") )		return LESS_THAN_EQUAL;
    if( isTokenMatch(">=") )		return GREATER_THAN_EQUAL;
    if( isTokenMatch("==") )		return EQUAL_EQUAL;
    if( isTokenMatch("!=") )		return NOT_EQUAL;
    if( isTokenMatch("&&") )		return ANDAND;
    if( isTokenMatch("||") )		return OROR;
    if( isTokenMatch("auto\a") )		return AUTO;
    if( isTokenMatch("register\a") )	return REGISTER;
    if( isTokenMatch("static\a") )		return STATIC;
    if( isTokenMatch("extern\a") )		return EXTERN;
    if( isTokenMatch("typedef\a") )	return TYPEDEF;
    if( isTokenMatch("void\a") )		return xVOID;
    if( isTokenMatch("char\a") )		return CHAR;
    if( isTokenMatch("short\a") )		return SHORT;
    if( isTokenMatch("int\a") )		return INT;
    if( isTokenMatch("long\a") )		return LONG;
    if( isTokenMatch("float\a") )		return FLOAT;
    if( isTokenMatch("double\a") )		return DOUBLE;
    if( isTokenMatch("signed\a") )		return SIGNED;
    if( isTokenMatch("unsigned\a") )	return UNSIGNED;
    if( isTokenMatch("const\a") )		return xCONST;
    if( isTokenMatch("volatile\a") )	return VOLATILE;
    if( isTokenMatch("...") )		return TENTENTEN;
//  if( isTokenMatch("enum\a") )		return ENUM;
    if( isTokenMatch("struct\a") )		return STRUCT;
    if( isTokenMatch("union\a") )		return UNION;
    if( isTokenMatch("goto\a") )		return GOTO;
    if( isTokenMatch("continue\a") )	return CONTINUE;
    if( isTokenMatch("break\a") )		return BREAK;
    if( isTokenMatch("return\a") )		return RETURN;
    if( isTokenMatch("while\a") )		return WHILE;
    if( isTokenMatch("do\a") )		return DO;
    if( isTokenMatch("for\a") )		return FOR;
    if( isTokenMatch("if\a") )		return IF;
    if( isTokenMatch("else\a") )		return ELSE;
    if( isTokenMatch("switch\a") )		return SWITCH;
    if( isTokenMatch("case\a") )		return CASE;
    if( isTokenMatch("default\a") )	return DEFAULT;
    if( isTokenMatch("asm\a") )		return ASM;
    if( isTokenMatch(";") )			return(';');
    if( isTokenMatch("{") )			return('{');
    if( isTokenMatch("}") )			return('}');
    if( isTokenMatch(",") )			return(',');
    if( isTokenMatch(":") )			return(':');
    if( isTokenMatch("=") )			return('=');
    if( isTokenMatch("(") )			return('(');
    if( isTokenMatch(")") )			return(')');
    if( isTokenMatch("[") )			return('[');
    if( isTokenMatch("]") )			return(']');
    if( isTokenMatch("<") )			return('<');
    if( isTokenMatch(">") )			return('>');
    if( isTokenMatch("&") )			return('&');
    if( isTokenMatch("!") )			return('!');
    if( isTokenMatch("~") )			return('~');
    if( isTokenMatch("-") )			return('-');
    if( isTokenMatch("+") )			return('+');
    if( isTokenMatch("*") )			return('*');
    if( isTokenMatch("/") )			return('/');
    if( isTokenMatch("%") )			return('%');
    if( isTokenMatch("^") )			return('^');
    if( isTokenMatch("|") )			return('|');
    if( isTokenMatch("?") )			return('?');
    if( isTokenMatch("@") )			return('@');

    c = xgetc();

    /* 定義名, 変数名 */
    if( ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ) || c == '_' ){
      do {
        yytext[ i++ ] = c;
        c = xgetc();
      } while( ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ) || ( '0' <= c && c <= '9' ) || c == '_' );
      yytext[ i ] = '\0';
      xungetc( c );
      yylval = _symbol( yytext );
      if( is_type_name( yytext ) ) return TYPE_NAME; // 型名
      return IDR;                                    // 変数名など
    }

    /* 文字定数 */
    else if(  c == '\'' ){
      if( ( c = xgetc() ) == '\\' ){
        c = xgetc();
        d = xgetc();
        if( c == 'a' && d =='\'' )       return  yylval = _integer('\a'), CONST_INT;
        else if( c == 'b'  && d =='\'' ) return  yylval = _integer('\b'), CONST_INT;
        else if( c == 'f'  && d =='\'' ) return  yylval = _integer('\f'), CONST_INT;
        else if( c == 'n'  && d =='\'' ) return  yylval = _integer('\n'), CONST_INT;
        else if( c == 'r'  && d =='\'' ) return  yylval = _integer('\r'), CONST_INT;
        else if( c == 't'  && d =='\'' ) return  yylval = _integer('\t'), CONST_INT;
        else if( c == 'v'  && d =='\'' ) return  yylval = _integer('\v'), CONST_INT;
        else if( c == '\\' && d =='\'' ) return  yylval = _integer('\\'), CONST_INT;
        else if( c == '\?' && d =='\'' ) return  yylval = _integer('\?'), CONST_INT;
        else if( c == '\'' && d =='\'' ) return  yylval = _integer('\''), CONST_INT;
        else if( c == '\"' && d =='\'' ) return  yylval = _integer('\"'), CONST_INT;
        else if( c == '0'  && d =='\'' ) return  yylval = _integer('\0'), CONST_INT;
        else if( c >= '0' && c <= '7' && d >= 0 && d <= 7 ){
          if( ( e = xgetc() ) >= 0 && e <= 7 && xgetc() =='\'' ){
            cbuf[0] = c;
            cbuf[1] = d;
            cbuf[2] = e;
            cbuf[3] = '\0';
            MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
          else{
            yyerror("不正なエスケープ文字です\\0cc");
            return BADTOKEN;
          }
        }
        else if( ( c  == 'x' || c  == 'X') && ( ( d >= '0' && d <= '9' ) || ( d >= 'a' && d <= 'f' ) || ( d >= 'A' && d <= 'F' ) ) ){
          if( ( ( ( e = xgetc() ) >= '0' && e <= '9' ) || ( e >= 'a' && e <= 'f' ) || ( e >= 'A' && e <= 'F' ) ) && xgetc() =='\'' ){
            cbuf[0] = d;
            cbuf[1] = e;
            cbuf[2] = '\0';
            MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%x"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
          else{
            yyerror("不正なエスケープ文字です\\xcc");
            return BADTOKEN;
          }
        }
        else{
          yyerror("不正なエスケープ文字です\\c");
          return BADTOKEN;
        }
      }
      else{
        if( xgetc() =='\'' ){
          yylval = _integer( c );
          return CONST_INT;
        }
        else{
          yyerror("文字定数の終端が不正です");
          return BADTOKEN;
        }
      }
    }

    /* 文字列定数 */
    else if(  c == '\"' ){
      while( ( c = xgetc() ) != '\"' && c != '\n' && c != EOF ){
        if( c  == '\\' ){
          if( ( c = xgetc() ) == 'a' ) yytext[ i++ ] = '\a';
          else if( c  == 'b'  ) yytext[ i++ ] = '\b';
          else if( c  == 'f'  ) yytext[ i++ ] = '\f';
          else if( c  == 'n'  ) yytext[ i++ ] = '\n';
          else if( c  == 'r'  ) yytext[ i++ ] = '\r';
          else if( c  == 't'  ) yytext[ i++ ] = '\t';
          else if( c  == 'v'  ) yytext[ i++ ] = '\v';
          else if( c  == '\\' ) yytext[ i++ ] = '\\';
          else if( c  == '?'  ) yytext[ i++ ] = '\?';
          else if( c  == '\'' ) yytext[ i++ ] = '\'';
          else if( c  == '\"' ) yytext[ i++ ] = '\"';
          else if( c >= '0' && c <= '7' ){
            if( ( d = xgetc() ) >= '0' && d <= '7' && ( e = xgetc() ) >= '0' && e <= '7' ){
              cbuf[0] = c;
              cbuf[1] = d;
              cbuf[2] = e;
              cbuf[3] = '\0';
              MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%o"), &t );
              yytext[ i++ ] = (char)t;
            }
            else if( c == '0' ){
              yytext[ i++ ] = '\0';
              xungetc( e );
              xungetc( d );
            }
            else{
              yyerror("不正なエスケープ文字列です\\0cc");
            }
          }
          else if( c  == 'x' || c  == 'X'){
            if( ( ( ( d = xgetc() ) >= '0' && d <= '9' ) || ( d >= 'a' && d <= 'f' ) || ( d >= 'A' && d <= 'F' ) ) &&
                ( ( ( e = xgetc() ) >= '0' && e <= '9' ) || ( e >= 'a' && e <= 'f' ) || ( e >= 'A' && e <= 'F' ) ) ){
              cbuf[0] = d;
              cbuf[1] = e;
              cbuf[2] = '\0';
              MultiByteToWideChar( CP_ACP, 0, cbuf, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              yytext[ i++ ] = (char)t;
            }
            else{
              yyerror("不正なエスケープ文字列です\\xcc");
            }
          }
          else{
            yyerror("不正なエスケープ文字列です\\c");
          }
        }
        else{
          yytext[ i++ ] = c;
          if( isZenkaku( c ) ){ yytext[ i++ ] = xgetc(); }
        }
      }
      if( c == '\"' ){
        yytext[ i++ ] = '\0';
        yylval = _string( yytext );
        return CONST_STRING;
      }
      else{
        yyerror("文字列定数の終端が不正です");
        return BADTOKEN;
      }
    }

    /* 整数、実数 */
    if( c =='0' ){
      yytext[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '7' ){
        do {                 // 8進整数
          yytext[ i++ ] = c;
          c = xgetc();
        } while( c >= '0' && c <= '7' );
        if( c == 'l' || c == 'L' ){
          if( ( c = xgetc() ) == 'u' || c == 'U' ){
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
          else{
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
        }
        else if( c == 'u' || c == 'U' ){
          if( ( c = xgetc() ) == 'l' || c == 'L' ){
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
          else{
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext+1, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%o"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
        }
        else{
          xungetc( c );
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext+1, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%o"), &t );
          yylval = _integer( t );
          return CONST_INT;
        }
      }
      else if( c == 'x' || c == 'X' ){ //16進整数
        yytext[ i++ ] = c;
        if( ( ( c = xgetc() ) >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) ){
          do {
            yytext[ i++ ] = c;
            c = xgetc();
          } while( ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) );
          if( c == 'l' || c == 'L' ){
            if( ( c = xgetc() ) == 'u' || c == 'U' ){
              yytext[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, yytext+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              yylval = _integer( t );
              return CONST_INT;
            }
            else{
              xungetc( c );
              yytext[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, yytext+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              yylval = _integer( t );
              return CONST_INT;
            }
          }
          else if( c == 'u' || c == 'U' ){
            if( ( c = xgetc() ) == 'l' || c == 'L' ){
              yytext[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, yytext+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              yylval = _integer( t );
              return CONST_INT;
            }
            else{
              xungetc( c );
              yytext[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, yytext+2, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%x"), &t );
              yylval = _integer( t );
              return CONST_INT;
            }
          }
          else{
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext+2, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%x"), &t );
            yylval = _integer( t );
            return CONST_INT;
          }
        }
        xungetc( c );
        yyerror("16進定数の表記が不正です");
        return BADTOKEN;
      }
      else if( c == '.' ){ //1以下の小数
        yytext[ i++ ] = c;
        if( ( c = xgetc() ) >= '0' && c <= '9' ){
          do {
            yytext[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
          if( c == 'f' || c == 'F' ){   // 実数
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            yylval = _real( f );
            return CONST_REAL;
          }
          else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
            yytext[ i++ ] = c;
            if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
              do {
                yytext[ i++ ] = c;
                c = xgetc();
              } while( c >= '0' && c <= '9' );
              xungetc( c );
              yytext[ i ] = '\0';
              MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
              swscanf( tbuf, TEXT("%lg"), &f );
              yylval = _real( f );
              return CONST_REAL;
            }
            xungetc( c );
            yyerror("実数定数の表記が不正です");
            return BADTOKEN;
          }
          else{                           // 単純な小数
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            yylval = _real( f );
            return CONST_REAL;
          }
        }
        else if( c == 'f' || c == 'F' ){   // 実数
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          yytext[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              yytext[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            yylval = _real( f );
            return CONST_REAL;
          }
          xungetc( c );
          yyerror("実数定数の表記が不正です");
          return BADTOKEN;
        }
        xungetc( c );
        yytext[ i ] = '\0';
        MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
        swscanf( tbuf, TEXT("%lg"), &f );
        yylval = _real( f );
        return CONST_REAL;
      }
      else{ // 0の場合
        xungetc( c );
        yytext[ i ] = '\0';
        yylval = _integer(0);
        return CONST_INT;
      }
    }
    else if( c >= '1' && c <= '9' ){     //10進数
      do {
        yytext[ i++ ] = c;
        c = xgetc();
      } while( '0' <= c && c <= '9' );
      if( c == '.' ){                    //実数(小数点表示)
        yytext[ i++ ] = c;
        if( ( c = xgetc() ) >= '0' && c <= '9' ){
          do {
            yytext[ i++ ] = c;
            c = xgetc();
          } while( '0' <= c && c <= '9' );
        }
        if( c == 'f' || c == 'F' ){   // 実数
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          yytext[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              yytext[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            yylval = _real( f );
            return CONST_REAL;
          }
          xungetc( c );
          yyerror("実数定数の表記が不正です");
          return BADTOKEN;
        }
        else{                           // 単純な小数
          xungetc( c );
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
      }
      else if( c == 'f' || c == 'F' ){   // 実数
        yytext[ i ] = '\0';
        MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
        swscanf( tbuf, TEXT("%lg"), &f );
        yylval = _real( f );
        return CONST_REAL;
      }
      else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
        yytext[ i++ ] = c;
        if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
          do {
            yytext[ i++ ] = c;
            c = xgetc();
          } while( c >= '0' && c <= '9' );
          xungetc( c );
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
        xungetc( c );
        yyerror("実数定数の表記が不正です");
        return BADTOKEN;
      }
      else if( c == 'l' || c == 'L' ){  // 整数
        if( ( c = xgetc() ) == 'u' || c == 'U' ){
          yytext[ i ] = '\0';
          yylval = _integer( atoi( yytext ) );
          return CONST_INT;
        }
        else{
          xungetc( c );
          yytext[ i ] = '\0';
          yylval = _integer( atoi( yytext ) );
          return CONST_INT;
        }
      }
      else if( c == 'u' || c == 'U' ){  // 整数
        if( ( c = xgetc() ) == 'l' || c == 'L' ){
          yytext[ i ] = '\0';
          yylval = _integer( atoi( yytext ) );
          return CONST_INT;
        }
        else{
          xungetc( c );
          yytext[ i ] = '\0';
          yylval = _integer( atoi( yytext ) );
          return CONST_INT;
        }
      }
      else{  //単純な整数
        xungetc( c );
        yytext[ i ] = '\0';
        yylval = _integer( atoi( yytext ) );
        return CONST_INT;
      }
    }
    else if( c == '.' ){  // 1以下の小数
      yytext[ i++ ] = c;
      if( ( c = xgetc() ) >= '0' && c <= '9' ){
        do {
          yytext[ i++ ] = c;
          c = xgetc();
        } while( '0' <= c && c <= '9' );
        if( c == 'f' || c == 'F' ){   // 実数
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
        else if( c == 'e' || c == 'E' ){   // 実数(指数表示)
          yytext[ i++ ] = c;
          if( ( c = xgetc() ) == '+' || c == '-' || ( c >= '0' && c <= '9' ) ){
            do {
              yytext[ i++ ] = c;
              c = xgetc();
            } while( c >= '0' && c <= '9' );
            xungetc( c );
            yytext[ i ] = '\0';
            MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
            swscanf( tbuf, TEXT("%lg"), &f );
            yylval = _real( f );
            return CONST_REAL;
          }
          xungetc( c );
          yyerror("実数定数の表記が不正です");
          return BADTOKEN;
        }
        else{                           // 単純な小数
          xungetc( c );
          yytext[ i ] = '\0';
          MultiByteToWideChar( CP_ACP, 0, yytext, -1, tbuf, 252 );
          swscanf( tbuf, TEXT("%lg"), &f );
          yylval = _real( f );
          return CONST_REAL;
        }
      }
      else{ // ピリオドとして認識
        xungetc( c );
        yytext[ i ] = '\0';
        return('.');
      }
    }
    else return BADTOKEN;
  }
  return 0;
}


/* エラー表示 */
int yyerror( char* mesg ){
  TCHAR tmesg[256], fname[256], buf[256];

  if( pass == 2 ){
    MultiByteToWideChar( CP_ACP, 0, mesg, -1, tmesg, 252 );
    MultiByteToWideChar( CP_ACP, 0, FileName, -1, fname, 252 );
    swprintf( buf, TEXT("\"%s\" Line %d error:%s\n"), fname,  LineNo, tmesg );
    Cputs( buf );
    yynerrs++;
  }
  return 0;
}


/* 型を定義する */
List* def_type( char* attr, List* type_name, List* pointer, List* def_vars, List* def_name ){
  char *name, *newname;

  if( pass == 1 ){
    name = getSymbol( def_name );
    newname = malloc( strlen( name ) );
    strcpy( newname, name ); 
    if( type_names == NULL ) type_names = _symbol( newname ); else concatLists( type_names, _symbol( newname ) );
  }
  return _list( _symbol("typdef"), _symbol( attr ), type_name, pointer, list( def_vars ), def_name, NULL );
}


/* 関数を定義する */
List* def_func( List* func_name, char* attr, List* type_name, List* pointer, List* args, List* func_body ){
  return _list( _symbol("func"), func_name, _symbol( attr ), type_name, pointer, args, func_body, NULL );
}


/* コンテキスト情報を付加して返す */
List* set_context( List* content ){
  List *fname, *line;

  line = _list( _symbol("line"), _integer( LineNo ), NULL );
  if( strcmp( FileName, PrevName ) != 0 ){
    fname = _list( _symbol("file"), _string( FileName ), NULL );
    strncpy( PrevName, FileName, FNAME_MAX );
    return _concat( fname, line, content, NULL );
  }
  return _concat( line, content, NULL );
}


/* 文字列を結合したリストを求める */
List* cat_string( List* str1, List* str2 ){
  char* s = malloc( strlen( str1->value.string ) + strlen( str2->value.string ) + 1 );
  strcpy( s, str1->value.string );
  strcat( s, str2->value.string );
  deleteList( str1 );
  deleteList( str2 );
  return _string( s );
}


/* 与えられた文字列が型名として使用されているかどうかを返す */
int is_type_name( char* name ){
  List* list;

  for( list = type_names; list != NULL; list = list->next ){
    if( strcmp( name, getSymbol( list ) ) == 0 ) return 1;
  }
  return 0;
}


/* メイン関数 */ 
int Main( DWORD targc, LPTSTR targv[] ){
  type_names = NULL;

  InputFile = CreateFile( TEXT( INPUT_FILE ), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( InputFile == INVALID_HANDLE_VALUE ){
    Cputs( TEXT("ファイルオープン失敗(pass1)\n") );
    return 0;
  }
  pass = 1;
  yynerrs = 0;
  LineNo = 1;
  FileName[0] = PrevName[0] = '\0';
  yyparse();
  xcloseInputFile();
  deleteList( yyval );

  InputFile = CreateFile( TEXT( INPUT_FILE ), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( InputFile == INVALID_HANDLE_VALUE ){
    Cputs( TEXT("ファイルオープン失敗(pass2)\n") );
    if( type_names != NULL ) deleteList( type_names );
    return 0;
  }
  pass = 2;
  yynerrs = 0;
  LineNo = 1;
  FileName[0] = PrevName[0] = '\0';
  yyparse();
  xcloseInputFile();

//printList( type_names );

  swprintf( msg, TEXT("total error %d\n"), yynerrs );
  Cputs(msg);
  xopenOutputFile( OUTPUT_FILE );
  printList( yyval );
  xcloseOutputFile();
  if( type_names != NULL ) deleteList( type_names );
  deleteList( yyval );
  return 0;
}

