/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SIMPLE_TAB_HPP_INCLUDED
# define YY_YY_SIMPLE_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "Simple.ypp"

    #include <string>
    #include <vector>

#line 53 "Simple.tab.hpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    num = 258,
    pidentifier = 259,
    FOR = 260,
    IF = 261,
    WHILE = 262,
    REPEAT = 263,
    DO = 264,
    VAR = 265,
    BEGINNER = 266,
    END = 267,
    THEN = 268,
    ELSE = 269,
    ENDIF = 270,
    FROM = 271,
    TO = 272,
    DOWNTO = 273,
    ENDFOR = 274,
    ENDWHILE = 275,
    ENDDO = 276,
    UNTIL = 277,
    READ = 278,
    WRITE = 279,
    LE = 280,
    GE = 281,
    LEQ = 282,
    GEQ = 283,
    EQ = 284,
    NEQ = 285,
    PLUS = 286,
    MINUS = 287,
    TIMES = 288,
    DIV = 289,
    MOD = 290,
    ASSIGN = 291,
    ERROR = 292,
    ADD = 293,
    SUB = 294,
    MUL = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 20 "Simple.ypp"
union sem_rec
{
#line 21 "Simple.ypp"

	std::string *pidentifier;
    long long int num;
    struct variable *variable;
    struct condition *cond;
    std::vector<std::string> *command;

#line 114 "Simple.tab.hpp"

};
#line 20 "Simple.ypp"
typedef union sem_rec YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SIMPLE_TAB_HPP_INCLUDED  */
