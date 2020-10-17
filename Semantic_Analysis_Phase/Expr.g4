grammar Expr;

/** The start rule; begin parsing here. */
prog: (var_decl ';')* func_decl* VOID MAIN '('')' block EOF;

block: '{' ( stat+ | stat*  'return' ( |expr) ';' )'}';

stat:   var_decl ';'        #statVardecl
        |expr ';'           #statExpr
        | if_decl           #statIf
        | for_decl          #statFor
        | while_decl        #statWhile
        | cond_decl         #statCond
        | BREAK ';'         #statBREAK
        | CONTINUE ';'      #statCONTINUE
        ;

func_decl:   TYPE ID '(' (var_decl ',')* var_decl ')' block   #   funcParam
            |TYPE ID '(' ')' block                            #   funcNoparam
            ;

var_decl: TYPE ID                                               #varSingle
        | TYPE ID'[' (INT | ID | expr)']'                     #var1darray
        | TYPE ID'[' (INT | ID | expr)',' (INT | ID | expr)']'  #var2darray
        ;

if_decl : 'if''(' expr ')' block ( | 'else' block) #if
        ;

for_decl: 'for''(' expr ';' expr ';' expr ')' block #for
        ;

while_decl: 'while''(' expr ')' block #while
        ;

cond_decl: 'cond''(' expr ')' expr ':' expr ';' #cond
        ;

expr:   expr op=('*'|'/'|'%') expr                  #exprMulDivMod
    |   expr op=('+'|'-') expr                      #exprAddSub
    |   expr op=('<' | '>' | '<=' | '>=') expr      #exprRelOp
    |   expr op=('==' | '!=') expr                  #exprEqOp
    |   expr op='=' expr                            #exprAssign
    |   expr op=('AND' | 'OR' | 'NOT') expr         #exprBoolOp
    |   var                                         #exprVar
    |   '!' expr                                    #exprNot
    |   '-' expr                                    #exprNegative
    |   '(' expr ')'                                #exprParenthesis
    |   func_call                                   #exprFunc_call
    ;

var:  ID            #varID
    | INT           #varINT
    | BOOL          #varBOOL
    | CHAR          #varCHAR
    | STRING        #varSTRING
    ;

func_call:  ID '(' (var ',')* var ')'      #funccallParam
            | ID '('')'                    #funccallNoparam
            ;

/*Tokens*/
MAIN : 'main';
VOID : 'void';
BOOL : 'true' | 'false';
BREAK : 'break';
CONTINUE : 'continue';
PRINT : 'print';
READ : 'read';
READFILE : 'readfile';
TYPE: ('int' | 'uint' | 'bool' | 'char'| 'void');

ID  :   [a-zA-Z_]+ ;         // match identifiers <label id="code.tour.expr.3"/>
INT :   [0-9]+ ;                // match integers
CHAR : '\'' [a-zA-Z] '\'';      // match char
STRING : '"'.*?'"';             // match string

NS  :   [ \t\n]+ -> skip ; // toss out whitespace and newlines
