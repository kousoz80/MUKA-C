/* -*-c-*-  KM-yacc parser model file (standard version) */

/* -p option renames following identifiers */

/* User declarations and token definitions */


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


#ifndef YYSTYPE
#define YYSTYPE int
#endif

#define YYERRTOK 256
#define IDR 257
#define TYPE_NAME 258
#define CONST_INT 259
#define CONST_REAL 260
#define CONST_STRING 261
#define KAKERU_EQUAL 262
#define WARU_EQUAL 263
#define PERCENT_EQUAL 264
#define PLUS_EQUAL 265
#define MINUS_EQUAL 266
#define LEFT_SHIFT_EQUAL 267
#define RIGHT_SHIFT_EQUAL 268
#define AND_EQUAL 269
#define EOR_EQUAL 270
#define OR_EQUAL 271
#define ARROW 272
#define PLUSPLUS 273
#define MINUSMINUS 274
#define SIZEOF 275
#define LEFT_SHIFT 276
#define RIGHT_SHIFT 277
#define LESS_THAN_EQUAL 278
#define GREATER_THAN_EQUAL 279
#define EQUAL_EQUAL 280
#define NOT_EQUAL 281
#define ANDAND 282
#define OROR 283
#define AUTO 284
#define REGISTER 285
#define STATIC 286
#define EXTERN 287
#define TYPEDEF 288
#define xVOID 289
#define CHAR 290
#define SHORT 291
#define INT 292
#define LONG 293
#define FLOAT 294
#define DOUBLE 295
#define SIGNED 296
#define UNSIGNED 297
#define xCONST 298
#define VOLATILE 299
#define TENTENTEN 300
#define ENUM 301
#define STRUCT 302
#define UNION 303
#define GOTO 304
#define CONTINUE 305
#define BREAK 306
#define RETURN 307
#define WHILE 308
#define DO 309
#define FOR 310
#define IF 311
#define ELSE 312
#define SWITCH 313
#define CASE 314
#define DEFAULT 315
#define BADTOKEN 316
#define ASM 317


#ifndef YYMAXDEPTH
# define YYMAXDEPTH 200
#endif /* !YYMAXDEPTH */

#ifndef YYASP
# define YYASP(n) yyasp[-n]
#endif /* !YYASP */

#define yyclearin (yychar = -1)
#define yyerrok (yyerrflag = 0)
#define YYRECOVERING (yyerrflag != 0)

#if YYACTFUN
# include <setjmp.h>
  static jmp_buf yyjmp;
# define YYACCEPT  longjmp(yyjmp, 1)
# define YYABORT  longjmp(yyjmp, 2)
#endif /* YYACTFUN */

#if !YYACTFUN
# define YYACCEPT  return (0)
# define YYABORT  return (1)
# define YYERROR  goto yyerrlab
#endif /* !YYACTFUN */


int yychar;
static int yyerrflag;

#if YYDEBUG
int yydebug;
#endif /* YYDEBUG */

int yylex();
int yyparse();
int yyerror();

YYSTYPE yylval;

static YYSTYPE *yyasp, yyval;

#if YYACTFUN

static void yydef()
{
}
#endif /* YYACTFUN */


/* Following line will be replaced by parser tables */



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
        if( ( c = xgetc() ) >= '1' && c <= '9' ){
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


#ifdef YYDEBUG
#define YYMINTOK 256
#define YYMAXTOK 317
static char *yytnames[] = {
 "error",
 "IDR",
 "TYPE_NAME",
 "CONST_INT",
 "CONST_REAL",
 "CONST_STRING",
 "KAKERU_EQUAL",
 "WARU_EQUAL",
 "PERCENT_EQUAL",
 "PLUS_EQUAL",
 "MINUS_EQUAL",
 "LEFT_SHIFT_EQUAL",
 "RIGHT_SHIFT_EQUAL",
 "AND_EQUAL",
 "EOR_EQUAL",
 "OR_EQUAL",
 "ARROW",
 "PLUSPLUS",
 "MINUSMINUS",
 "SIZEOF",
 "LEFT_SHIFT",
 "RIGHT_SHIFT",
 "LESS_THAN_EQUAL",
 "GREATER_THAN_EQUAL",
 "EQUAL_EQUAL",
 "NOT_EQUAL",
 "ANDAND",
 "OROR",
 "AUTO",
 "REGISTER",
 "STATIC",
 "EXTERN",
 "TYPEDEF",
 "xVOID",
 "CHAR",
 "SHORT",
 "INT",
 "LONG",
 "FLOAT",
 "DOUBLE",
 "SIGNED",
 "UNSIGNED",
 "xCONST",
 "VOLATILE",
 "TENTENTEN",
 "ENUM",
 "STRUCT",
 "UNION",
 "GOTO",
 "CONTINUE",
 "BREAK",
 "RETURN",
 "WHILE",
 "DO",
 "FOR",
 "IF",
 "ELSE",
 "SWITCH",
 "CASE",
 "DEFAULT",
 "BADTOKEN",
 "ASM",
  0 };

static char *yyntnames[] = {
  "$start",
  "program",
  "declare",
  "def_var",
  "def_struct",
  "def_union",
  "def_type",
  "proto_type",
  "def_function",
  "var_list",
  "type",
  "var_class",
  "def_vars",
  "ptr",
  "formal_arguments",
  "var",
  "initialiser",
  "array",
  "initialisers",
  "assign_expression",
  "function_body",
  "formal_argument_list",
  "formal_argument",
  "statements",
  "statement",
  "expression",
  "constant",
  "condition_expression",
  "lvalue",
  "oror_expression",
  "andand_expression",
  "or_expression",
  "eor_expression",
  "and_expression",
  "equal_expression",
  "relation_expression",
  "shift_expression",
  "factor",
  "term",
  "cast_expression",
  "lvalue2",
  "primitive",
  "array_argument",
  "actual_arguments",
  "string",
  "actual_argument_list",
};

static short yyrstates[] = {
     7,    21,    29,    30,    43,    44,    45,    46,    56,    57,
    59,    67,    68,    69,    70,    73,    74,    78,    79,    83,
    85,    86,    87,    90,    91,    95,    96,   104,   106,   108,
   110,   111,   112,   116,   118,   119,   120,   121,   122,   123,
   128,   129,   147,   158,   161,   162,   167,   169,   175,   182,
   183,   185,   186,   189,   207,   210,   211,   212,   213,   214,
   215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
   225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
   235,   236,   237,   238,   239,   240,   241,   242,   260,   273,
   283,   286,   289,   291,   334,   339,   354,   355,   361,   366,
   368,   374,   376,   382,   388,   390,   398,   413,   414,   433,
   435,   436,   437,   444,   456,   463,   464,   465,   466,   469,
   470,   475,   477,   478,   485,   486,   488,   491,   495,   497,
   498,   504,     0,     1,    15,    16,    19,    20,    32,    33,
    34,    48,    50,    51,    52,    54,    55,    58,    60,    61,
    62,    63,    64,    65,    71,    72,    75,    76,    77,    80,
    81,    88,    89,    92,    93,    94,    97,    98,    99,   102,
   103,   105,   114,   115,   117,   130,   135,   136,   137,   138,
   139,   140,   141,   142,   143,   144,   145,   149,   150,   151,
   152,   153,   154,   157,   159,   163,   164,   165,   168,   172,
   173,   174,   178,   179,   184,   187,   188,   190,   191,   193,
   194,   195,   196,   198,   199,   243,   244,   247,   249,   250,
   252,   253,   254,   257,   259,   263,   264,   265,   268,   269,
   270,   274,   277,   278,   281,   282,   284,   287,   288,   290,
   293,   294,   295,   296,   297,   309,   310,   311,   312,   313,
   314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
   324,   329,   330,   331,   335,   336,   344,   348,   349,   352,
   353,   357,   359,   360,   363,   364,   370,   371,   373,   378,
   384,   385,   386,   387,   389,   391,   392,   393,   394,   395,
   396,   400,   411,   412,   416,   418,   420,   426,   428,   432,
   434,   438,   440,   445,   454,   455,   457,   458,   459,   461,
   467,   468,   471,   474,   476,   479,   480,   483,   487,   489,
   490,   499,     2,     3,     4,     5,     6,     8,     9,    10,
    11,    12,    13,    14,    17,    18,    22,    23,    24,    25,
    26,    27,    28,    31,    35,    36,    37,    38,    39,    40,
    41,    42,    47,    49,    53,    66,    82,    84,   100,   101,
   107,   109,   113,   124,   125,   126,   127,   131,   132,   133,
   134,   146,   148,   155,   156,   160,   166,   170,   171,   176,
   177,   180,   181,   192,   197,   200,   201,   202,   203,   204,
   205,   206,   208,   209,   245,   246,   248,   251,   255,   256,
   258,   261,   262,   266,   267,   271,   272,   275,   276,   279,
   280,   285,   292,   298,   299,   300,   301,   302,   303,   304,
   305,   306,   307,   308,   325,   326,   327,   328,   332,   333,
   337,   338,   340,   341,   342,   343,   345,   346,   347,   350,
   351,   356,   358,   362,   365,   367,   369,   372,   375,   377,
   379,   380,   381,   383,   397,   399,   401,   402,   403,   404,
   405,   406,   407,   408,   409,   410,   415,   417,   419,   421,
   422,   423,   424,   425,   427,   429,   430,   431,   439,   441,
   442,   443,   446,   447,   448,   449,   450,   451,   452,   453,
   460,   462,   472,   473,   481,   482,   484,   492,   493,   494,
   496,   500,   501,   502,   503,   505,
};
#endif /* YYDEBUG */

#define YYPASS 16384
#define YYLAST 727
static short yynext[] = {
    35,   322,    75,   357,    76,    34,   141,   174,   353,    39,
    38,   506,    37,   385,   385,   385,   385,   385,   385,   392,
   393,   253,   254,   255,   256,   410,   452,    19,   160,    56,
   357,   294,   327,   328,   329,   330,   331,   332,   333,   134,
   135,   412,   356,   433,    93,   188,   189,   385,   385,   385,
   385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
   385,   385,   385,   385,   385,    35,   259,   260,   368,    91,
    34,   385,   174,   357,    39,    38,    67,    37,   385,   353,
   385,   224,   357,     4,   368,   353,   385,   385,   233,   235,
   103,    35,   487,    36,   368,   368,    34,   357,   174,   130,
    39,    38,   154,    37,   156,   161,    18,   413,   414,   415,
   416,   417,   418,   419,   420,   421,   422,   423,   196,   197,
   344,   345,   346,   347,   154,   204,   273,    17,   205,   348,
   349,   350,   351,   250,   293,    85,   141,   427,   353,   368,
    83,   457,   368,   374,   445,    84,   463,   465,   232,   249,
   411,   427,    53,   471,    19,    54,   160,   159,    36, 16684,
    24,   138,   139,   353,   368,   357,   368,   337,   357,   361,
   355,   353,   355,    44, 16443,   355,   355,   357,   360,   355,
   357,   360,   355,   357,    36,   300,   480,   208,    93,   360,
   355,   360,   355,   232,   234,   360,   493,    97,   360,   316,
   494,   496,   101,   159,   498,   499,   500,   501,   502,   503,
   504,   505,   357,   360,   360,   355,   355,     5,    20,    50,
    77,    78,   248,    14,   280,   322,   364,   365,   366,   247,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    40,    41,   175,   144,    26,    30,   155, 16684,   157,   145,
   146,   323,   324,   325,   326,   360,   327,   328,   329,   330,
   331,   332,   333,   134,   135,   334,   335, 16640,   156,   152,
   153,   281,   282,   283,   104,   284,   105,   285,   286,     6,
   287,   288,   289,   246,   290,   364,   365,   366,   172,   363,
   322,   364,   365,   366,   163,   164,    68,   173,   337,   214,
   149,   150,    82,   234,    81,    40,    41,   175,   133,   193,
   322,     9,   169,   170,   470,   363,   301,   364,   365,   366,
   297,   327,   328,   329,   330,   331,   332,   333,   134,   135,
   322,    40,    41,   175,   210,   211,   323,   324,   325,   326,
     0,   327,   328,   329,   330,   331,   332,   333,   134,   135,
   334,   335,    10,    14,   136,   137,   323,   324,   325,   326,
   213,   327,   328,   329,   330,   331,   332,   333,   134,   135,
   334,   335,     4,   245,   152,   153,     6,   480,   166,   167,
    46,   151,   158,   199,   200,   207,   397,   241,   165,   168,
   236, 16507,   430,   263,   431,   450,    35,   360,   202,   203,
   279,    34,   456,   174,   434,    39,    38,   375,    37,   462,
   299,   464, 16443,     7,    93,   304,   306,   307,   308,   214,
   310,    93,   123,   313,   314,   257,   258,   451,   318,   319,
 16684,   220,   242,   243,   352,   159,    86,   240,   140,     2,
   194,     8,   215,    21,   383,   147,   386,   387,   388,   389,
   390,   391,   343,   239,   399,   400,   190,    19,    95,   264,
   265,   353,    43,   449,    15,    22,    95,    23,    25,   358,
   472,    29,   362,    95,    32,    95,    33,   359,   274,   275,
   213,   377,    47,   190,    48,   278,   148,    87,    19,    36,
   190,   190,   194,   244,    19,   424,   425,   426,    19,   436,
    88,    19,   443,   376,   444,   378,    27,    89,   231,   401,
   171,   466,    19,   402,    19,   379,   407,   295,   296,   482,
   483,   380,    19,    93,   491,   142,   492,   409,   190,   441,
   194,    93,    93,    93,    93,   381,   190,   442,   481,   116,
   119,   120,    93,    93,    93,   382,   489,   311,   122,   320,
   317,   124,   128,    93,    93,    93,   129,   131,    55,    93,
    93,   251,   252,    73,    74,    79,    80,   225,   226,    69,
     2,   228,   229,    92,   353,   267,   268,   269,   270,    99,
   357,   447,   353,   107,   357,   108,   357,   467,   353,   468,
   353,   469,   357,   485,   357,   486,   357,    70,    71,    72,
   396,   221,   218,   219,   403,   398,   222,   223,   404,    90,
   405,   237,   212,   238,   406,   428,   429,   315,    94,   435,
   363,   266,   364,   365,   366,   437,   321,   438,   271,   439,
   440,   272,    98,   100,   458,   446,    40,    41,   175,   276,
   277,   448,   102,   459,   460,   461,   292,   474,   298,   109,
   475,   110,   111,   112,   476,   478,   302,   113,   303,   484,
   190,   488,   490,   305,   309,   117,   495,   115,   216,   394,
   395,   312,   127,   497, 16424, 16424, 16507,    45,    52,    13,
    16,    49,   408, 16425, 16684, 16443,   477,   114,   118,   121,
   125,   126, 16509, 32767, 32767, 32767, 32767,    28,    31,   198,
 32767,   201,    51,   206,   227, 32767,   230,    96, 16509, 32767,
 32767, 32767, 32767,   453, 32767, 32767, 32767, 32767, 32767, 32767,
 32767, 32767, 32767, 32767,   473, 32767,   479,
};

#define YY2TABSTATES 132
#define YYLR1STATES 322
#define YYNSTATES 506
#define YYNXR 507
#define YYSTATE0 132
static short yybase[] = {
  -141,    11,   268,   -53,   567,   556,   567,   557,   553,   567,
   567,   553,   572,   567,   634,   573,   567,   353,   384,   553,
   637,   553,   553,   574,   567,   576,   567,   558,   553,   577,
   638,   553,   578,   353,   626,   626,   626,   626,   626,   626,
   115,   115,   642,   353,   353,   384,   635,   579,   581,   567,
   567,   623,   384,   384,   626,   130,   626,   626,   626,   626,
   626,   626,   626,   626,   626,   626,   626,   626,   626,   626,
   626,   626,   626,   626,   626,   626,   626,   626,   626,   626,
   626,   626,   626,   626,   626,   626,   626,   626,   353,   582,
   384,   353,   626,   626,   626,   588,   553,   553,   384,   626,
   626,   626,   626,   599,   627,   583,   601,   626,   626,   626,
   628,   626,   626,   353,   629,   353,   583,   626,   648,   583,
   583,   583,   649,   650,   583,   583,   583,   583,   583,   583,
   583,   583,    52,   327,  -170,  -161,   -40,   156,   188,   229,
    43,   419,    66,   -17,   -39,   186,   208,    37,   121,   410,
   418,    55,   249,   253,   110,   -12,   413,    31,    40,    50,
   401,   444,   123,   422,   446,   126,   456,   462,   141,   476,
   486,    96,   112,   344,    32,   518,   -32,    13,   287,   473,
   504,   561,   283,   -58,   289,   259,    98,   339,   345,   346,
   129,   560,   562,   512,   348,   439,   450,   454,   310,   545,
   549,   314,   551,   555,   457,   468,   -34,   569,   349,    25,
   354,   356,   532,     0,   396,   358,   359,   527,   419,   419,
   135,   335,  -257,   526,   580,   566,   568,   318,   570,   571,
   320,    72,   470,   587,   478,   590,   592,   419,   419,   538,
   593,   594,   382,   383,   540,   144,   287,   473,   504,   561,
   283,   -58,   -58,   289,   289,   289,   289,   259,   259,    98,
    98,   600,   598,   370,   138,   170,   343,   575,   584,   585,
   586,   350,   352,   605,   542,   544,   546,   548,   550,   377,
   589,   391,   591,   595,   609,   611,   612,   613,    26,   597,
   616,   479,   596,   617,   618,   552,   554,   -33,   602,   487,
   355,   604,   403,   606,   498,   625,   488,   499,   500,   630,
   489,   363,   607,   509,   510,   490,   360,   614,   511,   515,
    58,   516,
};

static short yybase2[] = {
  -257,    52,    96,  -257,    72,    96,    72,    96,    96,    72,
    72,    96,    72,    72,    66,    72,    72,   363,  -257,    96,
   112,    96,    96,    72,    72,    72,    72,    96,    96,    72,
   112,    96,    72,   363,   363,   363,   363,   363,   363,   363,
   363,   363,   396,   363,   363,  -257,   112,    72,    72,    72,
    72,   -34,  -257,  -257,   363,  -257,   363,   363,   363,   363,
   363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
   363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
   363,   363,   363,   363,   363,   363,   363,   363,   363,    72,
  -257,   363,   363,   363,   363,   -33,    96,    96,  -257,   363,
   363,   363,   363,   -33,   363,   -33,   -33,   363,   363,   363,
   363,   363,   363,   363,   363,   363,   -33,   363,    58,   -33,
   -33,   -33,    58,    58,   -33,   -33,   -33,   -33,   -33,   -33,
   -33,   -33,
};

static short yydefault[] = {
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    99,    -1,
    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,   192,    -1,    -1,    99,    80,    -1,    -1,    -1,
    -1,    -1,    99,    99,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   226,    -1,    -1,    -1,
    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
   226,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,
    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    80,    -1,    -1,    -1,   187,   157,   159,   161,
   163,   165,   167,   169,   172,   177,   180,   221,    -1,    -1,
   105,    -1,   100,    -1,    -1,    82,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,   192,    -1,    -1,   209,    -1,    -1,
   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   160,   162,   164,   166,
   168,   170,   171,   173,   174,   175,   176,   178,   179,   182,
   181,    -1,   227,    -1,   109,   111,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
    -1,    -1,    63,    68,    71,    69,    70,    62,    57,    54,
    46,    51,    60,    61,    73,    72,     1,     4,     5,     6,
     7,     8,     9,     3,    58,    55,    49,    52,    59,    56,
    50,    53,     2,    64,    74,    66,    12,    65,    22,    23,
    67,    10,    11,   216,   219,   220,   222,    77,    93,   217,
   145,   183,   208,   101,    88,    75,    15,    26,    27,    30,
    31,    24,    25,    91,   143,   187,   195,   200,   199,   198,
   197,   196,   193,   194,   214,   215,   223,   104,    89,    79,
    81,    13,    14,    28,    29,    32,    33,    17,    20,    19,
    21,    94,   218,   146,   147,   148,   149,   150,   151,   152,
   153,   154,   155,   156,   184,   185,   186,   228,   212,   213,
   106,    42,    95,   103,   102,    90,    83,    34,    35,    36,
    37,    16,    18,    92,   188,   144,   207,   201,   210,   224,
   108,   110,   116,   112,   140,   114,    43,    96,    38,    39,
    40,    41,    44,    97,    45,    98,   189,   203,   205,   202,
   158,   229,   225,   117,   137,   134,   133,   136,   139,   113,
   115,   141,   190,   191,   211,   204,   206,   118,   132,   135,
   138,    85,    87,   121,   131,   142,   123,   122,   124,   125,
   127,   120,   126,   128,   129,   130,
};

static short yycheck[] = {
   288,    -1,   -10,   -11,   123,   257,   123,   257,   -10,   123,
   123,   -10,   -12,   123,   257,   -12,   123,    61,    40,    44,
   257,   257,   257,   -12,   123,   -12,   123,   257,   125,   -12,
   257,   125,   -12,   123,    38,    33,   126,    45,    43,    42,
   273,   274,   -40,    61,    61,    40,   257,   -12,   -12,   123,
   123,   125,    40,    40,    40,    40,    61,   262,   263,   264,
   265,   266,   267,   268,   269,   270,   271,    63,   283,   282,
   124,    94,    38,   280,   281,    60,    62,   278,   279,   276,
   277,    45,    43,    42,    47,    37,    40,    91,    61,   -12,
    40,    44,    41,    44,    91,   123,   125,   125,    40,    41,
    40,    58,    44,   123,   307,   309,   -23,    41,    41,    40,
    40,    40,    40,    61,    59,    61,    41,    40,    59,    41,
    41,    41,    59,    59,    41,    41,    41,   312,    41,    41,
    41,    41, 32767,   256,   296,   297,   302,   303,   302,   303,
   -10,    40,   257,    -9,   -13,   302,   303,   257,   257,   257,
   258,   -13,   302,   303,    -9,   -13,    -9,   -13,   -13,    91,
   -17,    -9,   -13,   257,   258,   -13,   257,   258,   -13,   257,
   258,   257,   257,   257,    40,   275,   -28,   -29,   -30,   -31,
   -32,   -33,   -34,   -35,   -36,   -37,   -38,   -44,   302,   303,
   -10,   -14,   -21,   259,    91,   -17,    -9,    -9,   125,   257,
   258,   125,   257,   258,    -9,    -9,   125,    41,   -17,   -18,
   302,   303,   -10,   -25,   -40,    46,   272,   -42,   257,   257,
   -13,    41,    44,   259,   -14,   257,   258,   125,   257,   258,
   125,   -12,    -9,   -14,    -9,   -14,    41,   257,   257,   -13,
    41,   257,   302,   303,   -10,   -25,   -30,   -31,   -32,   -33,
   -34,   -35,   -35,   -36,   -36,   -36,   -36,   -37,   -37,   -38,
   -38,   -43,   -45,   -25,   -13,   -13,    41,   257,   258,   257,
   258,    41,    41,   -14,   -13,   -13,   257,   257,   -13,   -25,
   257,   304,   305,   306,   308,   310,   311,   313,   314,   315,
   317,   -25,    41,   -14,   -43,   -13,   -13,   -23,   257,   -25,
   -24,   -26,    40,    41,   -25,   308,   -25,   -25,   -25,   261,
   -25,    59,    41,   -25,   -25,   -25,   -24,    41,   -25,   -25,
    59,   -25,   258,   284,   285,   286,   287,   289,   290,   291,
   292,   293,   294,   295,   298,   299,    -2,    -3,    -4,    -5,
    -6,    -7,    -8,   125,   290,   291,   292,   293,   290,   291,
   292,   293,    -2,    42,   -15,    -3,    59,    42,    59,    59,
    -3,    59,    59,   257,   259,   260,   261,   -16,   -19,   -26,
   -27,   -39,   -41,   -22,    93,   -15,    59,    59,    59,    59,
    59,    59,    59,   -16,   -19,   -28,   -39,   -39,   -39,   -39,
   -39,   -39,   -28,   -28,   273,   274,   261,   257,    93,   -16,
   -16,    59,    59,    59,    59,    59,    59,    59,    59,    59,
    59,   125,    41,   -19,   -19,   -19,   -19,   -19,   -19,   -19,
   -19,   -19,   -19,   -19,   -39,   -39,   -39,   -19,   257,   257,
   257,    59,   -20,   300,   -22,    93,   -16,    59,    59,    59,
    59,    59,    59,   -16,   -39,   -19,    41,    41,    41,    93,
   257,   257,    59,   125,    -3,   -24,    59,   -20,    59,    59,
    59,    59,    59,   -20,    59,   -20,   -39,    41,    41,    41,
   -27,   -19,    93,   125,    58,    59,    59,    59,    58,   125,
   -24,    59,   -39,   -39,    41,    41,    41,   125,    59,    59,
    58,   -16,   -16,   -24,   -24,    59,   -24,    59,   -24,   -24,
   -24,   -24,   -24,   -24,   -24,   -24,
    11,
};

static short yyrederr[] = {
     0,
};

static short yygbase[] = {
 32767, 32767,   433,   166, 32767, 32767, 32767, 32767, 32767,    97,
   438, 32767,   458,   241,    36,   388,   411,    14, 32767,    51,
  -125, 32767,   182,   217,    80,   306,    28,   213,   -21, 32767,
   215,   160,   152,    78,    61,   488,   -54,   346,   -15,   412,
   245, 32767, 32767,   -69, 32767, 32767,
};

static short yydefgoto[] = {
     0,     1,   336,   454,   338,   339,   340,   341,   342,   143,
    11,     3,    12,   162,   191,   354,   367,   195,   209,   384,
   432,   192,   373,   106,   455,   291,   369,   370,   176,   177,
   178,   179,   180,   181,   182,   183,   184,   185,   186,   371,
    42,   372,   217,   261,   187,   262,
};

static char yyleft[] = {
     0,     1,     1,     1,     2,     2,     2,     2,     2,     2,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     4,     5,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
     6,     6,     7,     7,     7,     7,    10,    10,    10,    10,
    10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
    10,    10,    10,    10,    13,    13,    12,    12,    11,    11,
    11,    11,    11,    11,     9,     9,    15,    15,    15,    15,
    15,    15,    15,    15,    15,    15,    15,    15,    17,    17,
    17,    18,    18,    16,    16,     8,     8,     8,     8,    14,
    14,    21,    21,    21,    22,    22,    22,    22,    22,    22,
    22,    22,    20,    20,    23,    23,    24,    24,    24,    24,
    24,    24,    24,    24,    24,    24,    24,    24,    24,    24,
    24,    24,    24,    24,    24,    24,    24,    24,    24,    24,
    24,    24,    24,    25,    25,    19,    19,    19,    19,    19,
    19,    19,    19,    19,    19,    19,    19,    27,    27,    29,
    29,    30,    30,    31,    31,    32,    32,    33,    33,    34,
    34,    34,    35,    35,    35,    35,    35,    36,    36,    36,
    37,    37,    37,    38,    38,    38,    38,    39,    39,    39,
    39,    39,    28,    28,    28,    28,    28,    28,    28,    28,
    28,    28,    28,    28,    28,    28,    28,    28,    40,    40,
    40,    40,    40,    40,    40,    40,    41,    41,    41,    26,
    26,    26,    44,    44,    42,    42,    43,    43,    45,    45,
};

static char yylen[] = {
     1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
     4,     4,     3,     5,     5,     4,     7,     6,     7,     6,
     6,     6,     4,     4,     5,     5,     5,     5,     6,     6,
     5,     5,     6,     6,     7,     7,     7,     7,     8,     8,
     8,     8,     6,     7,     8,     8,     1,     1,     1,     2,
     2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
     1,     1,     1,     1,     1,     2,     1,     2,     1,     1,
     1,     1,     1,     1,     1,     3,     1,     3,     2,     4,
     2,     4,     3,     5,     7,     9,     8,    10,     2,     3,
     4,     1,     3,     1,     3,     6,     7,     8,     8,     0,
     1,     1,     3,     3,     2,     1,     3,     2,     4,     3,
     4,     3,     2,     3,     1,     2,     1,     2,     3,     5,
     7,     5,     7,     6,     7,     7,     8,     7,     8,     8,
     9,     5,     3,     2,     2,     3,     2,     2,     3,     2,
     1,     2,     5,     1,     3,     1,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     1,     5,     1,
     3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
     3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
     1,     3,     3,     1,     3,     3,     3,     1,     4,     5,
     6,     6,     1,     2,     2,     2,     2,     2,     2,     2,
     2,     4,     5,     5,     6,     5,     6,     4,     1,     2,
     4,     6,     3,     3,     2,     2,     1,     1,     3,     1,
     1,     1,     1,     2,     3,     4,     0,     1,     1,     3,
};




#if !YYDEBUG

#define YYTRACE_DCL
#define YYTRACE_NEWSTATE()
#define YYTRACE_READ()
#define YYTRACE_SHIFT(tok)
#define YYTRACE_ACCEPT()
#define YYTRACE_REDUCE()
#define YYTRACE_POP()
#define YYTRACE_DISCARD()

#endif /* !YYDEBUG */


#if YYDEBUG

#ifndef EOF
#include <stdio.h>
#endif

#ifndef YYTRACEF
#define YYTRACEF stderr
#endif /* !YYTRACEF */

static char *yytokname(n)
    int n;
{
    static char buf[10];

    if (YYMINTOK <= n && n <= YYMAXTOK && yytnames[n - YYMINTOK])
	return yytnames[n - YYMINTOK];
    switch (n) {
    case 0:
	return ("end-of-file");
    case '\n':
	return ("'\\n'");
    case '\t':
	return ("'\\t'");
    default:
	if (n < 0x20 || n > 0x7e) {
	    sprintf(buf, "<%d>", n);
	} else {
	    buf[0] = '\'';
	    buf[1] = n;
	    buf[2] = '\'';
	    buf[3] = '\0';
	}
	return (buf);
    }
}


#if YYDEBUG == 2

/* Traditional Debug Mode */

#define YYTRACE_DCL /* empty */

#define YYTRACE_NEWSTATE() \
    if (yydebug) \
	fprintf(YYTRACEF, "%%%% State %d, Lookahead %s\n", \
		yyrstates[yystate], yychar < 0 ? "--none--" : yytokname(yychar))

#define YYTRACE_READ() \
    if (yydebug) \
	fprintf(YYTRACEF, "%%%% Reading %s\n", yytokname(yychar))

#define YYTRACE_SHIFT(tok) \
    if (yydebug) \
	fprintf(YYTRACEF, "%%%% Shift %s\n", yytokname(yychar))

#define YYTRACE_ACCEPT() \
    if (yydebug) fprintf(YYTRACEF, "%%%% Accepted.\n")

#define YYTRACE_REDUCE() \
    if (yydebug) { \
	int sym; \
	short *p; \
	fprintf(YYTRACEF, "%%%% Reduce by (%d) %s :", \
		yyn, yyntnames[yyleft[yyn]]); \
	if (yyl == 0) \
	    fprintf(YYTRACEF, " /* empty */"); \
	for (p = yyssp - yyl + 1; p <= yyssp; p++) { \
	    sym = yycheck[*p]; \
	    fprintf(YYTRACEF, " %s", sym >= 0 ? yytokname(sym) : yyntnames[-sym]); \
	} \
	fputc('\n', YYTRACEF); \
    }

#define YYTRACE_POP() \
    if (yydebug) \
	fprintf(YYTRACEF, "%%%% Recovering, uncovers state %d\n", \
		yyrstates[yystate])

#define YYTRACE_DISCARD() \
    if (yydebug) fprintf(YYTRACEF, "%%%% Discard %s\n", yytokname(yychar))

#endif /* YYDEBUG == 2 */

#if YYDEBUG != 2
/* Messy Debug Mode */

#ifndef YYMAXSTKSHOWN
#define YYMAXSTKSHOWN 50
#endif /* YYMAXSTKSHOWN */

static void yyshowstack(sstk, ssp, mark)
    short *sstk;
    short *ssp;
    short *mark;
{
    short *p;
    int sym, len;
    int strlen();

    len = 1;
    for (p = ssp; p >= sstk; --p) {
	sym = yycheck[*p];
	len += 1 + strlen(sym >= 0 ? yytokname(sym) : yyntnames[-sym]);
	if (len > YYMAXSTKSHOWN)
	    break;
    }
    p++;
    if (mark && p > mark) p = mark;
    if (p != sstk) fprintf(YYTRACEF, "...");
    for (; p <= ssp; p++) {
	if (p == mark)
	    fputc('[', YYTRACEF);
	else
	    fputc(' ', YYTRACEF);
	sym = yycheck[*p];
	fprintf(YYTRACEF, "%s", sym >= 0 ? yytokname(sym) : yyntnames[-sym]);
    }
    if (mark) {
	if (p == mark)
	    fputc('[', YYTRACEF);
	fputc(']', YYTRACEF);
    } else
	fputc(' ', YYTRACEF);
}

#define YYTRACE_DCL /* empty */

#define YYTRACE_NEWSTATE() \
    if (yydebug) { \
	fprintf(YYTRACEF, "%%%%%3d:", yyrstates[yystate]); \
	yyshowstack(yysstk + 1, yyssp, 0); \
	if (yychar >= 0) \
	    fprintf(YYTRACEF, "  <<< %s", yytokname(yychar)); \
	fprintf(YYTRACEF, "\n"); \
    }

#define YYTRACE_READ() \
    if (yydebug) \
	fprintf(YYTRACEF, "%%%% Reading %s\n", yytokname(yychar))

#define YYTRACE_SHIFT(tok)

#define YYTRACE_ACCEPT() \
    if (yydebug) fprintf(YYTRACEF, "%%%% Accepted.\n")

#define YYTRACE_REDUCE() \
    if (yydebug) { \
	fprintf(YYTRACEF, "%%%%%3d:", yyrstates[yystate]); \
	yyshowstack(yysstk + 1, yyssp, yyssp - yyl + 1); \
	fprintf(YYTRACEF, " --> %s, by (%d)\n", yyntnames[yyleft[yyn]], yyn); \
    }

#define YYTRACE_POP() \
    if (yydebug) { \
	fprintf(YYTRACEF, "%%%%%3d:", yyrstates[yystate]); \
	yyshowstack(yysstk + 1, yyssp, 0); \
	fprintf(YYTRACEF, "  (Recovering)\n", yyrstates[yystate]); \
    }

#define YYTRACE_DISCARD() \
    if (yydebug) fprintf(YYTRACEF, "%%%% Discard %s\n", yytokname(yychar))

#endif /* YYDEBUG == 2 */

#endif /* YYDEBUG */



int yyparse()
{
    YYSTYPE yyastk[YYMAXDEPTH];
    short yysstk[YYMAXDEPTH];
    short *yyssp;
    int yystate;
    int yyn;
    int yyl;
    YYTRACE_DCL

#if YYACTFUN    
    if ((yyn = setjmp(yyjmp)) != 0)
	return (yyn - 1);
#endif /* YYACTFUN */
    yyssp = yysstk;
    yyasp = yyastk - 1;
    *yyssp = yystate = YYSTATE0;
    yychar = -1;
    yyerrflag = 0;
    for (;;) {
	YYTRACE_NEWSTATE();

	if (yystate >= YYLR1STATES)
	    goto yydeflt;

	if (yychar < 0) {
	    if ((yychar = yylex()) <= 0) yychar = 0;
	    YYTRACE_READ();
	}

	if ((yyn = yybase[yystate] + yychar) >= 0 && yyn < YYLAST) {
	    if ((yyn = yynext[yyn]) < YYNSTATES) {
		if (yycheck[yyn] == yychar)
		    goto yyshift;
	    } else if (yyn < YYNXR) {
		if (yycheck[yyn] == yybase[yystate]) {
		    yyn = yyrederr[yyn - YYNSTATES];
		    goto yyaction;
		}
	    } else if (yyn - YYPASS == yychar)
		goto yydeflt;
	}

	if (yystate >= YY2TABSTATES
	    || (yyn = yybase2[yystate] + yychar) < 0 || YYLAST <= yyn
	    || (yyn = yynext[yyn]) >= YYNSTATES || yycheck[yyn] != yychar)
	    goto yydeflt;

    yyshift:
	YYTRACE_SHIFT(yychar);
	if (yyssp >= yysstk + YYMAXDEPTH) {
	    yyerror("parser stack overflow");
	    return (2);
	}
	*++yyssp = yystate = yyn;
	*++yyasp = yylval;
	yychar = -1;
	if (yyerrflag > 0)
	    yyerrflag--;
	continue;

    yydeflt:
	yyn = yydefault[yystate];

    yyaction:
	/* reduce/error */
	if (yyn == 0) {
	    /* accept */
	    YYTRACE_ACCEPT();
	    return (0);
	} else if (yyn > 0) {
	    /* reduce */
	    yyl = yylen[yyn];
	    YYTRACE_REDUCE();
	    yyval = yyasp[1 - yyl]; /* default value */
#if YYACTFUN
	    (*yya[yyn])();
#endif /* YYACTFUN */

#if !YYACTFUN
	    switch (yyn) {
		/* Following line will be replaced by actions */
case 1:
{ yyval = set_context( YYASP(0) ); } break;
case 2:
{ yyval = concatLists( YYASP(1), set_context( YYASP(0) ) ); } break;
case 3:
{ yyval = YYASP(1); } break;
case 4:
{ yyval = YYASP(0); } break;
case 5:
{ yyval = YYASP(0); } break;
case 6:
{ yyval = YYASP(0); } break;
case 7:
{ yyval = YYASP(0); } break;
case 8:
{ yyval = YYASP(0); } break;
case 9:
{ yyval = YYASP(0); } break;
case 10:
{ yyval = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("struct"), YYASP(2), list( YYASP(1) ), NULL ); } break;
case 11:
{ yyval = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("union"),  YYASP(2), list( YYASP(1) ), NULL ); } break;
case 12:
{ yyval = _list( _symbol("var"), _integer(CLASS_AUTO), _symbol("elem"),   YYASP(2), list( YYASP(1) ), NULL ); } break;
case 13:
{ yyval = _list( _symbol("var"),       YYASP(4),             _symbol("struct"), YYASP(2), list( YYASP(2) ), NULL ); } break;
case 14:
{ yyval = _list( _symbol("var"),       YYASP(4),             _symbol("union"),  YYASP(2), list( YYASP(2) ), NULL ); } break;
case 15:
{ yyval = _list( _symbol("var"),       YYASP(3),             _symbol("elem"),   YYASP(2), list( YYASP(1) ), NULL ); } break;
case 16:
{ yyval = _list( _symbol("struct"),    YYASP(5),              list( YYASP(3) ), list( YYASP(1) ), NULL ); } break;
case 17:
{ yyval = _list( _symbol("struct"), _symbol("*"),       list( YYASP(3) ), list( YYASP(1) ), NULL ); } break;
case 18:
{ yyval = _list( _symbol("union"),     YYASP(5),              list( YYASP(3) ), list( YYASP(1) ), NULL ); } break;
case 19:
{ yyval = _list( _symbol("union"),  _symbol("*"),       list( YYASP(3) ), list( YYASP(1) ), NULL ); } break;
case 20:
{ yyval = _list( _symbol("struct"),      YYASP(4),       list( YYASP(2) ), list(NULL), NULL ); } break;
case 21:
{ yyval = _list( _symbol("union"),       YYASP(4),       list( YYASP(2) ), list(NULL), NULL ); } break;
case 22:
{ yyval = def_type( "elem",       YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 23:
{ yyval = def_type( "elem",       YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 24:
{ yyval = def_type( "elem",       YYASP(3),           YYASP(2),      NULL, YYASP(1) ); } break;
case 25:
{ yyval = def_type( "elem",       YYASP(3),           YYASP(2),      NULL, YYASP(1) ); } break;
case 26:
{ yyval = def_type( "struct",     YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 27:
{ yyval = def_type( "struct",     YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 28:
{ yyval = def_type( "struct",     YYASP(3),           YYASP(2),      NULL, YYASP(1) ); } break;
case 29:
{ yyval = def_type( "struct",     YYASP(3),           YYASP(2),      NULL, YYASP(1) ); } break;
case 30:
{ yyval = def_type( "union",      YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 31:
{ yyval = def_type( "union",      YYASP(2),       _integer(0), NULL, YYASP(1) ); } break;
case 32:
{ yyval = def_type( "union",      YYASP(1),           YYASP(2),      NULL, YYASP(1) ); } break;
case 33:
{ yyval = def_type( "union",      YYASP(1),           YYASP(2),      NULL, YYASP(1) ); } break;
case 34:
{ yyval = def_type( "struct", _symbol("*"), _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 35:
{ yyval = def_type( "struct", _symbol("*"), _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 36:
{ yyval = def_type( "union",  _symbol("*"), _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 37:
{ yyval = def_type( "union",  _symbol("*"), _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 38:
{ yyval = def_type( "struct",     YYASP(5),       _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 39:
{ yyval = def_type( "struct",     YYASP(5),       _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 40:
{ yyval = def_type( "union",      YYASP(5),       _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 41:
{ yyval = def_type( "union",      YYASP(5),       _integer(0),  YYASP(3) , YYASP(1) ); } break;
case 42:
{ yyval = def_func( YYASP(4), "elem",   YYASP(5), _integer(0),    YYASP(2),      list(NULL) ); } break;
case 43:
{ yyval = def_func( YYASP(4), "elem",   YYASP(6),      YYASP(5),        YYASP(2),      list(NULL) ); } break;
case 44:
{ yyval = def_func( YYASP(4), "struct", YYASP(6),      YYASP(5),        YYASP(2),      list(NULL) ); } break;
case 45:
{ yyval = def_func( YYASP(4), "union",  YYASP(6),      YYASP(5),        YYASP(2),      list(NULL) ); } break;
case 46:
{ yyval = _symbol("long");   } break;
case 47:
{ yyval = _symbol("long");   } break;
case 48:
{ yyval = _symbol("ulong");  } break;
case 49:
{ yyval = _symbol("long");   } break;
case 50:
{ yyval = _symbol("ulong");  } break;
case 51:
{ yyval = _symbol("long");   } break;
case 52:
{ yyval = _symbol("long");   } break;
case 53:
{ yyval = _symbol("ulong");  } break;
case 54:
{ yyval = _symbol("short");  } break;
case 55:
{ yyval = _symbol("short");  } break;
case 56:
{ yyval = _symbol("ushort"); } break;
case 57:
{ yyval = _symbol("char");   } break;
case 58:
{ yyval = _symbol("char");   } break;
case 59:
{ yyval = _symbol("uchar");  } break;
case 60:
{ yyval = _symbol("float");  } break;
case 61:
{ yyval = _symbol("float");  } break;
case 62:
{ yyval = _symbol("void");   } break;
case 63:
{ yyval = YYASP(0); } break;
case 64:
{ yyval = _integer(1); } break;
case 65:
{ ( yyval = YYASP(1) )->value.integer++; } break;
case 66:
{ yyval = YYASP(0); } break;
case 67:
{ yyval = concatLists( YYASP(1), YYASP(0) ); } break;
case 68:
{ yyval = _integer(CLASS_AUTO);     } break;
case 69:
{ yyval = _integer(CLASS_STATIC);   } break;
case 70:
{ yyval = _integer(CLASS_EXTERN);   } break;
case 71:
{ yyval = _integer(CLASS_AUTO);     } break;
case 72:
{ yyval = _integer(CLASS_AUTO);     } break;
case 73:
{ yyval = _integer(CLASS_AUTO);     } break;
case 74:
{ yyval = YYASP(0); } break;
case 75:
{ yyval = concatLists( YYASP(2), YYASP(0) ); } break;
case 76:
{ yyval = _list( YYASP(0), _integer(0), list(NULL), list(NULL), list(NULL), NULL ); } break;
case 77:
{ yyval = _list( YYASP(2), _integer(0), list(NULL),     YYASP(0),     list(NULL), NULL ); } break;
case 78:
{ yyval = _list( YYASP(1), _integer(0), list( YYASP(0) ), list(NULL), list(NULL), NULL ); } break;
case 79:
{ yyval = _list( YYASP(3), _integer(0), list( YYASP(2) ),     YYASP(0),     list(NULL), NULL ); } break;
case 80:
{ yyval = _list( YYASP(0),     YYASP(1),      list(NULL), list(NULL), list(NULL), NULL ); } break;
case 81:
{ yyval = _list( YYASP(2),     YYASP(3),      list(NULL),     YYASP(0),     list(NULL), NULL ); } break;
case 82:
{ yyval = _list( YYASP(1),     YYASP(2),      list( YYASP(0) ), list(NULL), list(NULL), NULL ); } break;
case 83:
{ yyval = _list( YYASP(3),     YYASP(4),      list( YYASP(2) ),     YYASP(0),     list(NULL), NULL ); } break;
case 84:
{ yyval = _list( YYASP(4),     YYASP(5),      list(NULL), list(NULL), list( YYASP(1) ), NULL ); } break;
case 85:
{ yyval = _list( YYASP(6),     YYASP(7),      list(NULL),     YYASP(0),     list( YYASP(3) ), NULL ); } break;
case 86:
{ yyval = _list( YYASP(5),     YYASP(6),      list( YYASP(4) ), list(NULL), list( YYASP(1) ), NULL ); } break;
case 87:
{ yyval = _list( YYASP(7),     YYASP(8),      list( YYASP(6) ),     YYASP(0),    list( YYASP(3) ), NULL ); } break;
case 88:
{ yyval = _integer(0x7fffffff); } break;
case 89:
{ yyval = YYASP(1); } break;
case 90:
{ yyval = concatLists( YYASP(3), YYASP(1) ); } break;
case 91:
{ yyval = YYASP(0); } break;
case 92:
{ yyval = concatLists( YYASP(2), YYASP(0) ); } break;
case 93:
{ yyval = YYASP(0); } break;
case 94:
{ yyval = list( YYASP(1) ); } break;
case 95:
{ yyval = def_func( YYASP(4), "elem",   YYASP(5), _integer(0),    YYASP(2),      YYASP(0) ); } break;
case 96:
{ yyval = def_func( YYASP(4), "elem",   YYASP(6),      YYASP(5),        YYASP(2),      YYASP(0) ); } break;
case 97:
{ yyval = def_func( YYASP(4), "struct", YYASP(6),      YYASP(5),        YYASP(2),      YYASP(0) ); } break;
case 98:
{ yyval = def_func( YYASP(4), "union",  YYASP(6),      YYASP(5),        YYASP(2),      YYASP(0) ); } break;
case 99:
{ yyval = list(NULL); } break;
case 100:
{ yyval = list( YYASP(0) ); } break;
case 101:
{ yyval = YYASP(0); } break;
case 102:
{ yyval = concatLists( YYASP(2), YYASP(0) ); } break;
case 103:
{ yyval = YYASP(2); } break;
case 104:
{ yyval = _list( _symbol("arg"),     YYASP(0),       _symbol("elem"),   YYASP(1), _integer(0), NULL ); } break;
case 105:
{ yyval = _list( _symbol("arg"), _symbol("*"), _symbol("elem"),   YYASP(0), _integer(0), NULL ); } break;
case 106:
{ yyval = _list( _symbol("arg"),     YYASP(0),       _symbol("elem"),   YYASP(2),     YYASP(1),      NULL ); } break;
case 107:
{ yyval = _list( _symbol("arg"), _symbol("*"), _symbol("elem"),   YYASP(1),     YYASP(0),      NULL ); } break;
case 108:
{ yyval = _list( _symbol("arg"),     YYASP(0),       _symbol("struct"), YYASP(2),     YYASP(1),      NULL ); } break;
case 109:
{ yyval = _list( _symbol("arg"), _symbol("*"), _symbol("struct"), YYASP(1),     YYASP(0),      NULL ); } break;
case 110:
{ yyval = _list( _symbol("arg"),     YYASP(0),       _symbol("union"),  YYASP(2),     YYASP(1),      NULL ); } break;
case 111:
{ yyval = _list( _symbol("arg"), _symbol("*"), _symbol("union"),  YYASP(1),     YYASP(0),      NULL ); } break;
case 112:
{ yyval = _list( _symbol("{}"), NULL );} break;
case 113:
{ yyval = _list( _symbol("{}"), YYASP(1), NULL ); } break;
case 114:
{ yyval = set_context( YYASP(0) ); } break;
case 115:
{ yyval = concatLists( YYASP(1), set_context( YYASP(0) ) ); } break;
case 116:
{ yyval = _list( _symbol("nop"), NULL ); } break;
case 117:
{ yyval = _list( _symbol("nop"), NULL ); } break;
case 118:
{ yyval = _list( _symbol("{}"), YYASP(1), NULL ); } break;
case 119:
{ yyval = _list( _symbol("if"), YYASP(2), YYASP(0), NULL );     } break;
case 120:
{ yyval = _list( _symbol("if"), YYASP(4), YYASP(2), YYASP(0), NULL ); } break;
case 121:
{ yyval = _list( _symbol("while"), YYASP(2), YYASP(0), NULL ); } break;
case 122:
{ yyval = _list( _symbol("do"), YYASP(5), YYASP(2), NULL ); } break;
case 123:
{ yyval = _list( _symbol("for"), list(NULL), list(NULL), list(NULL), YYASP(0), NULL ); } break;
case 124:
{ yyval = _list( _symbol("for"), list(NULL), list(NULL),     YYASP(2),     YYASP(0), NULL ); } break;
case 125:
{ yyval = _list( _symbol("for"), list(NULL),     YYASP(3),     list(NULL), YYASP(0), NULL ); } break;
case 126:
{ yyval = _list( _symbol("for"), list(NULL),     YYASP(4),         YYASP(2),     YYASP(0), NULL ); } break;
case 127:
{ yyval = _list( _symbol("for"),     YYASP(4),     list(NULL), list(NULL), YYASP(0), NULL ); } break;
case 128:
{ yyval = _list( _symbol("for"),     YYASP(5),     list(NULL),     YYASP(2),     YYASP(0), NULL ); } break;
case 129:
{ yyval = _list( _symbol("for"),     YYASP(5),         YYASP(3),     list(NULL), YYASP(0), NULL ); } break;
case 130:
{ yyval = _list( _symbol("for"),     YYASP(6),         YYASP(4),         YYASP(2),     YYASP(0), NULL ); } break;
case 131:
{ yyval = _list( _symbol("switch"), YYASP(2), YYASP(0), NULL ); } break;
case 132:
{ yyval = _list( _symbol("goto"),     YYASP(1), NULL ); } break;
case 133:
{ yyval = _list( _symbol("break"),    NULL );     } break;
case 134:
{ yyval = _list( _symbol("continue"), NULL );     } break;
case 135:
{ yyval = _list( _symbol("return"), YYASP(1), NULL ); } break;
case 136:
{ yyval = _list( _symbol("return"), NULL ); } break;
case 137:
{ yyval = _list( _symbol("label"),    YYASP(1), NULL ); } break;
case 138:
{ yyval = _list( _symbol("case"),     YYASP(1), NULL ); } break;
case 139:
{ yyval = _list( _symbol("default"),  NULL );     } break;
case 140:
{ yyval = YYASP(0); } break;
case 141:
{ yyval = YYASP(1); } break;
case 142:
{ yyval = _list( _symbol("asm"),     YYASP(2), NULL ); } break;
case 143:
{ yyval = YYASP(0); } break;
case 144:
{ yyval = _list( _symbol(","), YYASP(2), YYASP(0), NULL ); } break;
case 145:
{ yyval = YYASP(0); } break;
case 146:
{ yyval = _list( _symbol("="),   YYASP(2), YYASP(0), NULL ); } break;
case 147:
{ yyval = _list( _symbol("*="),  YYASP(2), YYASP(0), NULL ); } break;
case 148:
{ yyval = _list( _symbol("/="),  YYASP(2), YYASP(0), NULL ); } break;
case 149:
{ yyval = _list( _symbol("%="),  YYASP(2), YYASP(0), NULL ); } break;
case 150:
{ yyval = _list( _symbol("+="),  YYASP(2), YYASP(0), NULL ); } break;
case 151:
{ yyval = _list( _symbol("-="),  YYASP(2), YYASP(0), NULL ); } break;
case 152:
{ yyval = _list( _symbol("<<="), YYASP(2), YYASP(0), NULL ); } break;
case 153:
{ yyval = _list( _symbol(">>="), YYASP(2), YYASP(0), NULL ); } break;
case 154:
{ yyval = _list( _symbol("&="),  YYASP(2), YYASP(0), NULL ); } break;
case 155:
{ yyval = _list( _symbol("^="),  YYASP(2), YYASP(0), NULL ); } break;
case 156:
{ yyval = _list( _symbol("|="),  YYASP(2), YYASP(0), NULL ); } break;
case 157:
{ yyval = YYASP(0); } break;
case 158:
{ yyval = _list( _symbol("?"),  YYASP(4), YYASP(2), YYASP(0), NULL ); } break;
case 159:
{ yyval = YYASP(0); } break;
case 160:
{ yyval = _list( _symbol("||"),   YYASP(2), YYASP(0), NULL ); } break;
case 161:
{ yyval = YYASP(0); } break;
case 162:
{ yyval = _list( _symbol("&&"),   YYASP(2), YYASP(0), NULL ); } break;
case 163:
{ yyval = YYASP(0); } break;
case 164:
{ yyval = _list( _symbol("|"),   YYASP(2), YYASP(0), NULL ); } break;
case 165:
{ yyval = YYASP(0); } break;
case 166:
{ yyval = _list( _symbol("^"),   YYASP(2), YYASP(0), NULL ); } break;
case 167:
{ yyval = YYASP(0); } break;
case 168:
{ yyval = _list( _symbol("&"),   YYASP(2), YYASP(0), NULL ); } break;
case 169:
{ yyval = YYASP(0); } break;
case 170:
{ yyval = _list( _symbol("=="),   YYASP(2), YYASP(0), NULL ); } break;
case 171:
{ yyval = _list( _symbol("!="),   YYASP(2), YYASP(0), NULL ); } break;
case 172:
{ yyval = YYASP(0); } break;
case 173:
{ yyval = _list( _symbol("<"),   YYASP(2), YYASP(0), NULL ); } break;
case 174:
{ yyval = _list( _symbol(">"),   YYASP(2), YYASP(0), NULL ); } break;
case 175:
{ yyval = _list( _symbol("<="),  YYASP(2), YYASP(0), NULL ); } break;
case 176:
{ yyval = _list( _symbol(">="),  YYASP(2), YYASP(0), NULL ); } break;
case 177:
{ yyval = YYASP(0); } break;
case 178:
{ yyval = _list( _symbol("<<"),   YYASP(2), YYASP(0), NULL ); } break;
case 179:
{ yyval = _list( _symbol(">>"),   YYASP(2), YYASP(0), NULL ); } break;
case 180:
{ yyval = YYASP(0); } break;
case 181:
{ yyval = _list( _symbol("+"),   YYASP(2), YYASP(0), NULL ); } break;
case 182:
{ yyval = _list( _symbol("-"),   YYASP(2), YYASP(0), NULL ); } break;
case 183:
{ yyval = YYASP(0); } break;
case 184:
{ yyval = _list( _symbol("*"),   YYASP(2), YYASP(0), NULL ); } break;
case 185:
{ yyval = _list( _symbol("/"),   YYASP(2), YYASP(0), NULL ); } break;
case 186:
{ yyval = _list( _symbol("%"),   YYASP(2), YYASP(0), NULL ); } break;
case 187:
{ yyval = YYASP(0); } break;
case 188:
{ yyval = _list( _symbol("cast"), _symbol("elem"),   YYASP(2), _integer(0), YYASP(0), NULL ); } break;
case 189:
{ yyval = _list( _symbol("cast"), _symbol("elem"),   YYASP(3),      YYASP(2),     YYASP(0), NULL ); } break;
case 190:
{ yyval = _list( _symbol("cast"), _symbol("struct"), YYASP(3),      YYASP(2),     YYASP(0), NULL ); } break;
case 191:
{ yyval = _list( _symbol("cast"), _symbol("union"),  YYASP(3),      YYASP(2),     YYASP(0), NULL ); } break;
case 192:
{ yyval = YYASP(0); } break;
case 193:
{ yyval = _list( _symbol("++x"), YYASP(0), NULL );    } break;
case 194:
{ yyval = _list( _symbol("--x"), YYASP(0), NULL );    } break;
case 195:
{ yyval = _list( _symbol("&x"), YYASP(0), NULL );     } break;
case 196:
{ yyval = _list( _symbol("*x"), YYASP(0), NULL );     } break;
case 197:
{ yyval = _list( _symbol("+x"), YYASP(0), NULL );     } break;
case 198:
{ yyval = _list( _symbol("-x"), YYASP(0), NULL );     } break;
case 199:
{ yyval = _list( _symbol("~x"), YYASP(0), NULL );     } break;
case 200:
{ yyval = _list( _symbol("!x"), YYASP(0), NULL );     } break;
case 201:
{ yyval = _list( _symbol("sztype"), _symbol("elem"),   _integer(0), YYASP(1), NULL ); } break;
case 202:
{ yyval = _list( _symbol("sztype"), _symbol("elem"),        YYASP(1),     YYASP(2), NULL ); } break;
case 203:
{ yyval = _list( _symbol("sztype"), _symbol("struct"), _integer(0), YYASP(1), NULL ); } break;
case 204:
{ yyval = _list( _symbol("sztype"), _symbol("struct"),      YYASP(1),     YYASP(2), NULL ); } break;
case 205:
{ yyval = _list( _symbol("sztype"), _symbol("union"),  _integer(0), YYASP(1), NULL ); } break;
case 206:
{ yyval = _list( _symbol("sztype"), _symbol("union"),       YYASP(1),     YYASP(2), NULL ); } break;
case 207:
{ yyval = _list( _symbol("szvar"), YYASP(1), NULL ); } break;
case 208:
{ yyval = YYASP(0); } break;
case 209:
{ yyval = _list( _symbol("[]"),    YYASP(1),     YYASP(0),     NULL ); } break;
case 210:
{ yyval = _list( _symbol("call"),  YYASP(3),     YYASP(1),     NULL ); } break;
case 211:
{ yyval = _list( _symbol("call&"), YYASP(4),     YYASP(1),     NULL ); } break;
case 212:
{ yyval = _list( _symbol("."),     YYASP(2),     YYASP(0),     NULL ); } break;
case 213:
{ yyval = _list( _symbol("->"),    YYASP(2),     YYASP(0),     NULL ); } break;
case 214:
{ yyval = _list( _symbol("x++"),   YYASP(1), NULL ); } break;
case 215:
{ yyval = _list( _symbol("x--"),   YYASP(1), NULL ); } break;
case 216:
{ yyval = _list( _symbol("@var"), YYASP(0), NULL );   } break;
case 217:
{ yyval = _list( _symbol("@const"), YYASP(0), NULL ); } break;
case 218:
{ yyval = YYASP(1); } break;
case 219:
{ yyval = YYASP(0); } break;
case 220:
{ yyval = YYASP(0); } break;
case 221:
{ yyval = YYASP(0); } break;
case 222:
{ yyval = YYASP(0); } break;
case 223:
{ yyval = cat_string( YYASP(1), YYASP(0) ); } break;
case 224:
{ yyval = YYASP(1); } break;
case 225:
{ yyval = _concat( YYASP(3), YYASP(1), NULL );} break;
case 226:
{ yyval = list(NULL); } break;
case 227:
{ yyval = list( YYASP(0) ); } break;
case 228:
{ yyval = YYASP(0); } break;
case 229:
{ yyval = concatLists( YYASP(2), YYASP(0) ); } break;

	    }
#endif /* !YYACTFUN */
	    if (yyssp >= yysstk + YYMAXDEPTH) {
		yyerror("parser stack overflow");
		return (2);
	    }
	    yyasp -= yyl;
	    yyssp -= yyl;
	    yystate = yygbase[yyn = yyleft[yyn]] + *yyssp;
	    if (yystate < 0 || YYLAST <= yystate
		|| (yystate = yynext[yystate]) >= YYNSTATES
		|| -yycheck[yystate] != yyn)
		yystate = yydefgoto[yyn];
	    *++yyssp = yystate;
	    *++yyasp = yyval;
	} else {
	    /* error */
	    switch (yyerrflag) {
	    case 0:
	    yyerrlab:
		yyerror("syntax error");
	    case 1:
	    case 2:
		yyerrflag = 3;
		for (;;) {
		    yyn = yybase[yystate] + YYERRTOK;
		    if (0 <= yyn && yyn < YYLAST) {
			if ((yyn = yynext[yyn]) < YYNSTATES
			    && yycheck[yyn] == YYERRTOK)
			    break;
			if (yyn - YYPASS == YYERRTOK)
			    goto yyskip;
		    }
		    if (yystate < YY2TABSTATES
			&& (yyn = yybase2[yystate] + YYERRTOK) >= 0
			&& yyn < YYLAST
			&& (yyn = yynext[yyn]) < YYNSTATES
			&& yycheck[yyn] == YYERRTOK)
			break;
		yyskip:
		    if (yyssp <= yysstk)
			return (1);
		    yystate = *--yyssp;
		    yyasp--;
		    YYTRACE_POP();
		}
		YYTRACE_SHIFT(YYERRTOK);
		*++yyssp = yystate = yyn;
		++yyasp;
		break;

	    case 3:
		YYTRACE_DISCARD();
		if (yychar == 0)
		    return (1);
		yychar = -1;
		break;
	    }
	}
    }
}
