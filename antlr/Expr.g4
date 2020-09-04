grammar Expr;

prog: MAIN '{' statements '}' EOF;

statements: (expr ';')+
    ;

expr: expr ('*'|'/'|'%') expr
    | expr ('+'|'-') expr
    | expr rel_op expr
    | '(' expr ')'
    | INT
    | ID
    | FLOAT
    ;

rel_op  : '<'
        | '>'
        | '<='
        | '>='
        ;

/*Tokens*/
MAIN : 'main';
ID  : [a-z][a-zA-Z0-9_]*;
INT : [0-9][0-9]*;
FLOAT : [0-9]+ '.' [0-9]+;
COMMENT : '//' ~[\r\n]*->skip; 
NS : [ \t\n]+ -> skip; 