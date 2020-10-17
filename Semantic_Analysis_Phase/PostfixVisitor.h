//#include "ast.h"
#include <iostream>
using namespace std;

class PostFixVisitor : public ASTvisitor
{
public:
    virtual void visit(ASTProg &node)
    {
        cout << "PostFixVisit traversal invoked" << endl;
        for (auto vardecl : node.variabledeclList)
        {
            vardecl->accept(*this);
        }
        for (auto funcdecl : node.funcdeclList)
        {
            funcdecl->accept(*this);
        }
        node.block->accept(*this);
    }

    virtual void visit(ASTBlock &node)
    {
        int i = 1;
        for (auto statement : node.statementList)
        {
            cout << " line " << i << " : ";
            statement->accept(*this);
            cout << endl;
            i = i + 1;
        }

        ASTExpr *expr = node.expr;
        if(expr){
            cout << "line " << i << " : ";
            expr->accept(*this);
            cout << endl;
            i = i + 1;
        }
    }

    virtual void visit(ASTStatExpr &node)
    {
        node.expr->accept(*this);
    }

    // virtual void visit(ASTStatIf &node)
    // {
    //     cout << "ASTStatIf visit function" << endl;
    // }

    // virtual void visit(ASTStatFor &node)
    // {
    //     cout << "ASTStatFor visit function" << endl;
    // }

    // virtual void visit(ASTStatWhile &node)
    // {
    //     cout << "ASTStatWhile visit function" << endl;
    // }

    virtual void visit(ASTStatCond &node)
    {
        node.exprter->accept(*this);
    }

    virtual void visit(ASTStatBREAK &node)
    {
        cout << " break " << endl;
    }

    virtual void visit(ASTStatCONTINUE &node)
    {
        cout << " continue" << endl;
    }

    virtual void visit(ASTIf &node)
    {
        ASTExpr *expr = node.getexpr();
        ASTBlock *ifblock = node.getifblock();
        ASTBlock *elseblock = node.getelseblock();

        expr->accept(*this);
        ifblock->accept(*this);
        if(elseblock)
            elseblock->accept(*this);
    }

    virtual void visit(ASTFor &node)
    {
        ASTExpr *expr1 = node.getexpr1();
        ASTExpr *expr2 = node.getexpr2();
        ASTExpr *expr3 = node.getexpr3();
        ASTBlock *block = node.getblock();

        expr1->accept(*this);
        expr2->accept(*this);
        expr3->accept(*this);        
        block->accept(*this);
    }

    virtual void visit(ASTWhile &node)
    {
        ASTExpr *expr = node.getexpr();
        ASTBlock *block = node.getblock();

        expr->accept(*this);
        block->accept(*this);
    }

    virtual void visit(ASTFunc &node)
    {
        cout << "ASTFunc visit function" << endl;
    }

    virtual void visit(ASTFuncParam &node)
    {
        cout << "ASTFunc visit function" << endl;
        cout << " " << node.type;
        cout << " " << node.id;
        cout << "(";
        for (auto vardecl : node.variabledeclList)
        {
            vardecl->accept(*this);
        }
        node.vardecl->accept(*this);
        cout << ")";
        node.block->accept(*this);
    }

    virtual void visit(ASTFuncNoparam &node)
    {
        cout << " " << node.type;
        cout << " " << node.id;
        cout << "()";
        node.block->accept(*this);
    }

    virtual void visit(ASTFunccall &node)
    {
        cout << "ASTFunccall visit function" << endl;
    }

    virtual void visit(ASTFunccallParam &node)
    {
        cout << " " << node.id;
        cout << "(";
        for (auto var : node.variableList)
        {
            var->accept(*this);
        }
        node.var->accept(*this);
        cout << ")";
    }

    virtual void visit(ASTFunccallNoparam &node)
    {
        cout << " " << node.id;
        cout << "()";
    }

    virtual void visit(ASTVardecl &node)
    {
        cout << "ASTVardecl visit function" << endl;
    }

    virtual void visit(ASTVarSingle &node)
    {
        cout << " " << node.getTYPE();
        cout << " " << node.getID();
    }

    virtual void visit(ASTVar1darray &node)
    {
        cout << " " << node.getTYPE();
        cout << " " << node.getIDname();
        cout << "[";
        
        int intlit = node.getint();
        string IDvalue = node.getIDvalue();
        ASTExpr *expr = node.getexpr();

        // if(intlit!=NULL)
        //     cout << intlit;
        // if(IDvalue!=NULL)
        //     cout << IDvalue;
        // if(expr!=NULL)
        //     cout << expr;

        cout << "]";
    }

    virtual void visit(ASTVar2darray &node)
    {
        cout << " " << node.getTYPE();
        cout << " " << node.getIDname();
        cout << "[";
        
        // int intlit = node.getint();
        // string IDvalue = node.getIDvalue();
        // ASTExpr *expr = node.getexpr();

        // if(intlit!=NULL)
        //     cout << intlit;
        // if(IDvalue!=NULL)
        //     cout << IDvalue;
        // if(expr!=NULL)
        //     cout << expr;

        cout << "]";
    }

    // virtual void visit(ASTPrint &node)
    // {
    //     ASTExpr *expr = node.getexpr();
    //     expr->accept(*this);
    // }

    // virtual void visit(ASTReadinput &node)
    // {
    //     ASTVar *var = node.getvar();
    //     var->accept(*this);
    // }

    // virtual void visit(ASTReadfile &node)
    // {
    //     ASTVar *var = node.getvar();
    //     ASTVarID *ID = node.getvarId();
    //     ASTVarSTRING *STRING = node.getvarSTRING();
    //     var->accept(*this);
    //     ID->accept(*this);
    //     STRING->accept(*this);
    // }

    virtual void visit(ASTExpr &node)
    {
        cout << "ASTExpr visit function" << endl;
    }

    virtual void visit(ASTExprBinary &node)
    {
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        right->accept(*this);
        cout << " " + node.getBin_operator();
    }

    virtual void visit(ASTExprTernary &node)
    {
        ASTExpr *first = node.getFirst();
        ASTExpr *second = node.getSecond();
        ASTExpr *third = node.getThird();

        first->accept(*this);
        second->accept(*this);
        third->accept(*this);
        cout << " ?:";
    }

    virtual void visit(ASTVar &node)
    {
        cout << "ASTVar visit function" << endl;
    }

    virtual void visit(ASTVarID &node)
    {
        cout << " " << node.getID();
    }

    virtual void visit(ASTVarINT &node)
    {
        cout << " " << node.getIntLit();
    }

    virtual void visit(ASTVarBOOL &node)
    {
        cout << " " << node.getBoolLit();
    }

    virtual void visit(ASTVarCHAR &node)
    {
        cout << " " << node.getCharLit();
    }

    virtual void visit(ASTVarSTRING &node)
    {
        cout << " " << node.getStringLit();
    }
};