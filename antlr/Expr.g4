grammar Expr;

prog: (variable_decl ';')* functions_decl* VOID MAIN '('')' block EOF;

functions_decl: type ID '(' (variable_decl ',')* variable_decl ')' block
                |type ID '(' ')' block
                ;

block: '{' statements+ '}'
        | '{' statements*  'return' ';' '}'
        | '{' statements*  'return' expr ';' '}'
        ;

statements: variable_decl ';'
            | expr ';'
            | if_decl
            | for_decl
            | while_decl
            | cond_decl
            | BREAK ';'
            | CONTINUE ';'
            | print_functions ';'
            | read_functions ';'
            ;

function_call: ID '(' (parameter_decl ',')* parameter_decl ')'
                | ID '('')'
                ;

parameter_decl: ID
                | INT
                | CHAR
                | BOOL
                ;

print_functions : PRINT '(' expr ')' ;

read_functions  : variables '=' READ'('')' 
                | variables '=' READFILE'(' (ID |STRING) ')'
                ;

variable_decl: type ID
        | type ID'[' ( |INT | ID | expr)']'
        | type ID'[' (INT | ID | expr)',' (INT | ID | expr)']' ;

type: 'int' 
    | 'uint' 
    | 'bool' 
    | 'char'
    | 'void'
    ;

variables:ID
         | ID'[' (INT | ID | expr)']'
         | ID'[' (INT | ID | expr)',' (INT | ID | expr)']'
         | INT
         | BOOL
         | CHAR
         | STRING
         ;

if_decl : 'if''(' expr ')' block
        | 'if''(' expr ')' block 'else' block
        ;

for_decl: 'for''(' expr ';' expr ';' expr ')' block;

while_decl: 'while''(' expr ')' block;

cond_decl: 'cond''(' expr ')' expr ':' expr;

expr: expr ('*'|'/'|'%') expr
    | expr ('+'|'-') expr
    | expr rel_op expr
    | expr eq_op expr
    | expr bool_op expr
    | expr '=' expr
    | '!' expr
    | '-' expr
    | '(' expr ')'
    | variables
    | function_call
    ;

rel_op  : '<'
        | '>'
        | '<='
        | '>='
        ;

eq_op   : '=='
        |  '!='
        ;

bool_op: AND
        | OR
        | NOT
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
AND : 'and';
OR : 'or';
NOT : 'not';

ID  : [a-zA-Z0-9_]+;
INT : [0-9][0-9]*;
CHAR : '\'' [a-zA-Z] '\'';
STRING : '"'.*?'"';

FLOAT : [0-9]+ '.' [0-9]+;
MULTIPLECOMMENTS : '/*' .*? '*/' ->skip; 
SINGLECOMMENT : '//' ~[\r\n]* ->skip; 
NS : [ \t\n]+ -> skip; 