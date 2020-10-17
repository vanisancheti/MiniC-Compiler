#include <iostream>
#include "ast.h"
#include <vector>
#include <string.h>
#include "ExprVisitor.h"

using namespace std;

class ExprBuildASTVisitor : public ExprVisitor
{
public:
    /**
   * Visit parse trees produced by ExprParser.
   */
    virtual antlrcpp::Any visitProg(ExprParser::ProgContext *context)
    {
        cout << "In visitProg" << endl;
        ASTProg *node = new ASTProg();
        ASTVardecl *vardecl;
        ASTFunc *func;
        ASTBlock *block;

        for (auto var : context->var_decl())
        {
            vardecl = visit(var);
            if (vardecl != nullptr)
            {
                node->variabledeclList.push_back(vardecl);
            }
        }

        for (auto fun : context->func_decl())
        {
            func = visit(fun);
            if (func != nullptr)
            {
                node->funcdeclList.push_back(func);
            }
        }

        node->block = visit(context->block());

        return (ASTProg *) node;
    }

    virtual antlrcpp::Any visitBlock(ExprParser::BlockContext *context)
    {
        cout << "In visitBlock" << endl;
        ASTBlock *node = new ASTBlock();
        ASTStat *statementNode;
        ASTExpr *expr = nullptr;

        for (auto statement : context->stat())
        {
            statementNode = visit(statement);
            if (statementNode != nullptr)
            {
                node->statementList.push_back(statementNode);
            }
        }

        if(context->expr())
            expr = visit(context->expr());

        node->expr = expr;

        return (ASTBlock *) node;
    }

    virtual antlrcpp::Any visitStatExpr(ExprParser::StatExprContext *context)
    {
        cout << "In visitStatExpr" << endl;
        ASTStatExpr *node = new ASTStatExpr();
        node->expr = visit(context->expr());
        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitStatIf(ExprParser::StatIfContext *context)
    {
        cout << "In visitStatIf" << endl;
        ASTIf* node = visit(context->if_decl());

        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitIf(ExprParser::IfContext *context)
    {
        cout << "In visitIf" << endl;
        ASTExpr *expr = visit(context->expr());
        ASTBlock *ifblock = visit(context->block(0));
        ASTBlock *elseblock = nullptr;
        if(context->block().size() == 2)
            elseblock = visit(context->block(1));
        ASTIf *node = new ASTIf(expr, ifblock, elseblock);
        return node;
    }

    virtual antlrcpp::Any visitStatFor(ExprParser::StatForContext *context)
    {
        cout << "In visitStatFor" << endl;
        ASTFor* node = visit(context->for_decl());

        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitFor(ExprParser::ForContext *context)
    {
        cout << "In visitFor" << endl;
        ASTExpr *expr1 = visit(context->expr(0));
        ASTExpr *expr2 = visit(context->expr(1));
        ASTExpr *expr3 = visit(context->expr(2));
        ASTBlock *block = visit(context->block());

        ASTFor *node = new ASTFor(expr1, expr2, expr3, block);
        return node;
    }

    virtual antlrcpp::Any visitStatWhile(ExprParser::StatWhileContext *context)
    {
        cout << "In visitStatWhile" << endl;
        ASTWhile* node = visit(context->while_decl());

        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitWhile(ExprParser::WhileContext *context)
    {
        cout << "In visitWhile" << endl;
        ASTExpr *expr = visit(context->expr());
        ASTBlock *block = visit(context->block());

        ASTWhile *node = new ASTWhile(expr, block);
        return node;
    }

    virtual antlrcpp::Any visitStatCond(ExprParser::StatCondContext *context)
    {
        cout << "In visitStatCond" << endl;
        ASTStatCond *node = new ASTStatCond();
        node->exprter = visit(context->cond_decl());
        cout << "Done" << endl;
        return (ASTStat *)node;
    }

    virtual antlrcpp::Any visitCond(ExprParser::CondContext *context)
    {
        cout << "In visitCond" << endl;
        ASTExprTernary *node;
        ASTExpr *first, *second, *third;
         
        first = visit(context->expr(0)); 
        second = visit(context->expr(1)); 
        third = visit(context->expr(2));

        node = new ASTExprTernary(first, second, third);
        return node;
    }

    virtual antlrcpp::Any visitStatBREAK(ExprParser::StatBREAKContext *context)
    {
        cout << "In visitStatBREAK" << endl;
        ASTStatBREAK *node = new ASTStatBREAK();
        return (ASTStat *)node;
    }

    virtual antlrcpp::Any visitStatCONTINUE(ExprParser::StatCONTINUEContext *context)
    {
        cout << "In visitStatCONTINUE" << endl;
        ASTStatCONTINUE *node = new ASTStatCONTINUE();
        return (ASTStat *)node;
    }

    virtual antlrcpp::Any visitStatVardecl(ExprParser::StatVardeclContext *context)
    {
        cout << "In visitStatVardecl" << endl;
        ASTVardecl *node = visit(context->var_decl());
        return (ASTStat *)node;
    }

    virtual antlrcpp::Any visitVarSingle(ExprParser::VarSingleContext *context)
    {
        cout << "In visitVarSingle" << endl;
        string id = context->ID()->getText();
        string type = context->TYPE()->getText();
        ASTVarSingle *node = new ASTVarSingle(type, id);
        return (ASTVardecl* )node;
    }

    virtual antlrcpp::Any visitVar1darray(ExprParser::Var1darrayContext *context)
    {
        cout << "In visitVar1darray" << endl;
        string id = context->ID(0)->getText();
        string type = context->TYPE()->getText();
        ASTVar1darray *node = new ASTVar1darray(type, id);
        return (ASTVardecl* )node;
    }

    virtual antlrcpp::Any visitVar2darray(ExprParser::Var2darrayContext *context)
    {
        cout << "In visitVar2darray" << endl;
        string id = context->ID(0)->getText();
        string type = context->TYPE()->getText();
        ASTVar2darray *node = new ASTVar2darray(type, id);
        return (ASTVardecl* )node;
    }

    virtual antlrcpp::Any visitFuncNoparam(ExprParser::FuncNoparamContext *context)
    {
        cout << "In visitFuncNoparam" << endl;
        string id = context->ID()->getText();
        string type = context->TYPE()->getText();
        ASTFuncNoparam *node = new ASTFuncNoparam();
        node->type = type;
        node->id = id;
        node->block = visit(context->block());
        return (ASTFunc *)node;
    }

    virtual antlrcpp::Any visitFuncParam(ExprParser::FuncParamContext *context)
    {
        cout << "In visitFuncParam" << endl;
        ASTFuncParam *node = new ASTFuncParam();

        string id = context->ID()->getText();
        string type = context->TYPE()->getText();

        node->type = type;
        node->id = id;

        int i=0;
        ASTVardecl *vardecl;

        for (auto statement : context->var_decl())
        {
            if(i == 0)
            {
                node->vardecl = visit(context->var_decl(i));
            }
            else{
                vardecl = visit(statement);
                if (vardecl != nullptr)
                {
                    node->variabledeclList.push_back(vardecl);
                }
            }
            i=i+1;
        }
        node->block = visit(context->block());
        return (ASTFunc *)node;
    }

    // virtual antlrcpp::Any visitStatPrint(ExprParser::StatPrintContext *context)
    // {
    //     cout << "In visitStatPrint" << endl;
    //     ASTPrint *node = visit(context->print_func());
    //     return (ASTStat *)node;
    // }

    // virtual antlrcpp::Any visitPrint(ExprParser::PrintContext *context)
    // {
    //     cout << "In visitPrint" << endl;
    //     ASTPrint *node = new ASTPrint(visit(context->expr()));
    //     return node;
    // }

    // virtual antlrcpp::Any visitStatRead(ExprParser::StatReadContext *context)
    // {
    //     cout << "In visitStatRead" << endl;
    //     ASTStatRead *node = visit(context->read_func());
    //     return (ASTStat *)node;
    // }

    // virtual antlrcpp::Any visitReadinput(ExprParser::ReadinputContext *context)
    // {
    //     cout << "In visitReadinput" << endl;
    //     ASTReadinput *node = new ASTReadinput(visit(context->var()));
    //     return (ASTStatRead *)node;
    // }

    // virtual antlrcpp::Any visitReadfile(ExprParser::ReadfileContext *context)
    // {
    //     cout << "In visitReadFile" << endl;
    //     ASTVar *var = visit(context->var());
    //     ASTVarID *ID = nullptr;
    //     ASTVarSTRING *STRING = nullptr;
    //     if(context->ID())
    //         ID = visit(context->ID());
    //     if(context->STRING())
    //         STRING = visit(context->STRING());
    //     ASTReadfile *node = new ASTReadfile(var, ID, STRING);
    //     return (ASTStatPrint *)node;
    // }

    virtual antlrcpp::Any visitExprAssign(ExprParser::ExprAssignContext *context)
    {
        cout << "In visitExprAssign" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        node = new ASTExprBinary("=", left, right);

        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprVar(ExprParser::ExprVarContext *context)
    {
        cout << "In visitExprVar" << endl;
        ASTVar* node = visit(context->var());
        return (ASTExpr*)node;
    }

    virtual antlrcpp::Any visitVarID(ExprParser::VarIDContext *context)
    {
        cout << "In visitVarID" << endl;
        string id = context->ID()->getText();
        ASTVarID *node = new ASTVarID(id);
        return (ASTVar *) node;
    }

    virtual antlrcpp::Any visitVarINT(ExprParser::VarINTContext *context)
    {
        cout << "In visitVarINT" << endl;
        ASTVarINT *node = new ASTVarINT(stoi(context->INT()->getText()));
        return (ASTVar *) node;
    }

    virtual antlrcpp::Any visitVarBOOL(ExprParser::VarBOOLContext *context)
    {
        cout << "In visitVarBOOL" << endl;
        bool boollit;
        if(context->BOOL()->getText() == "true")
            boollit = true;
        else
            boollit = false;
        cout << boollit << endl;
        ASTVarBOOL *node = new ASTVarBOOL(boollit);
        return (ASTVar *) node;
    }

    virtual antlrcpp::Any visitVarCHAR(ExprParser::VarCHARContext *context)
    {
        cout << "In visitVarCHAR" << endl;
        char charlit = context->CHAR()->getText()[1];
        ASTVarCHAR *node = new ASTVarCHAR(charlit);
        return (ASTVar *) node;
    }

    virtual antlrcpp::Any visitVarSTRING(ExprParser::VarSTRINGContext *context)
    {
        cout << "In visitVarSTRING" << endl;
        string stringlit = context->STRING()->getText();
        ASTVarSTRING *node = new ASTVarSTRING(stringlit);
        return (ASTVar *) node;
    }

    virtual antlrcpp::Any visitExprAddSub(ExprParser::ExprAddSubContext *context)
    {
        cout << "In visitExprAddSub" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string op = context->op->getText();

        if (op[0] == '+')
            node = new ASTExprBinary("+", left, right);
        else
            node = new ASTExprBinary("-", left, right);

        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprRelOp(ExprParser::ExprRelOpContext *context)
    {
        cout << "In visitExprRelOp" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string op = context->op->getText();

        if (op[0] == '<')
            node = new ASTExprBinary("<", left, right);
        if (op[0] == '>')
            node = new ASTExprBinary(">", left, right);
        if (op[0] == '<' && op[1] == '=')
            node = new ASTExprBinary("<=", left, right);
        if (op[0] == '>' && op[1] == '=')
            node = new ASTExprBinary(">=", left, right);

        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprEqOp(ExprParser::ExprEqOpContext *context)
    {
        cout << "In visitExprEqOp" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));
        string op = context->op->getText();

        if (op[0] == '=' && op[1] == '=')
            node = new ASTExprBinary("==", left, right);
        if (op[0] == '!' && op[1] == '=')
            node = new ASTExprBinary("!=", left, right);

        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprBoolOp(ExprParser::ExprBoolOpContext *context)
    {
        cout << "In visitExprBoolOp" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string op = context->op->getText();
        if(op == "AND")
            node = new ASTExprBinary("AND", left, right);
        if(op == "OR")
            node = new ASTExprBinary("OR", left, right);
        if(op == "NOT")
            node = new ASTExprBinary("NOT", left, right);        
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprParenthesis(ExprParser::ExprParenthesisContext *context)
    {
        cout << "In visitExprParenthesis" << endl;
        return visit(context->expr());
    }

    virtual antlrcpp::Any visitExprNot(ExprParser::ExprNotContext *context)
    {
        cout << "In visitExprNot" << endl;
        return visit(context->expr());
    }

    virtual antlrcpp::Any visitExprNegative(ExprParser::ExprNegativeContext *context)
    {
        cout << "In visitExprNegative" << endl;
        return visit(context->expr());
    }

    virtual antlrcpp::Any visitExprMulDivMod(ExprParser::ExprMulDivModContext *context)
    {
        cout << "In visitExprMulDivMod" << endl;
        ASTExpr *left;
        ASTExpr *right;
       
        ASTExprBinary *node;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string op = context->op->getText();

        if (op[0] == '*')
            node = new ASTExprBinary("*", left, right);
        if (op[0] == '/')
            node = new ASTExprBinary("/", left, right);
        if (op[0] == '%')
            node = new ASTExprBinary("%", left, right);

        return (ASTExpr *) node;
    }

  virtual antlrcpp::Any visitExprFunc_call(ExprParser::ExprFunc_callContext *context){
    cout << "In visitExprFunc_call" << endl;
    ASTFunccall* node = visit(context->func_call());
    return (ASTExpr*)node;
  }

  virtual antlrcpp::Any visitFunccallParam(ExprParser::FunccallParamContext *context){
    cout << "In visitFunccallParam" << endl;
    string id = context->ID()->getText();
    ASTFunccallParam *node = new ASTFunccallParam();
    node->id = id;

    int i=0;
    ASTVar *var;

    for (auto tvar : context->var())
    {
        if(i == 0)
        {
            node->var = visit(tvar);
        }
        else{
            var = visit(tvar);
            if (var != nullptr)
            {
                node->variableList.push_back(var);
            }
        }
        i=i+1;
    }

    return (ASTFunccall *) node;
  }

  virtual antlrcpp::Any visitFunccallNoparam(ExprParser::FunccallNoparamContext *context){
    cout << "In visitFunccallNoparam" << endl;
    string id = context->ID()->getText();
    ASTFunccallNoparam *node = new ASTFunccallNoparam();
    node->id = id;
    return (ASTFunccall *) node;
  }

};