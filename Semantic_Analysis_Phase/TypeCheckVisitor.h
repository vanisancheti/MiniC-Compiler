//#include "ast.h"
#include <iostream>
#define ASSERT(cond, message) if(!(cond)) \
    throw message
using namespace std;

enum Type {DEFAULT_TYPE, INT, BOOL, CHAR, VOID};

class TypeCheckVisitor : public ASTvisitor
{
private:
    Type type = Type::DEFAULT_TYPE;
    Type return_type = Type::DEFAULT_TYPE;
public:
    virtual void visit(ASTProg &node)
    {
        int i=0;
        for (auto vardecl : node.variabledeclList)
        {
            vardecl->accept(*this);
            i = i+1;
        }
        for (auto funcdecl : node.funcdeclList)
        {
            funcdecl->accept(*this);
            i = i+1;
        }
        node.block->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTBlock &node)
    {
        int i = 1;
        for (auto statement : node.statementList)
        {
            statement->accept(*this);
            i = i + 1;
        }

        ASTExpr *expr = node.expr;
        if(expr){
            expr->accept(*this);
            cout << " return" << endl;
        }

        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTStatExpr &node)
    {
        node.expr->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTStatCond &node)
    {
        node.exprter->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    // virtual void visit(ASTRead &node)
    // {
    //     cout << " ASTRead visit function " << endl;
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

    virtual void visit(ASTStatBREAK &node)
    {
        cout << " break ";
    }

    virtual void visit(ASTStatCONTINUE &node)
    {
        cout << " continue ";
    }

    virtual void visit(ASTIf &node)
    {
        ASTExpr *expr = node.getexpr();
        ASTBlock *ifblock = node.getifblock();
        ASTBlock *elseblock = node.getelseblock();

        expr->accept(*this);
        ASSERT(type == Type::BOOL, "If statement Condition must be boolean");
        cout << endl;
        cout << "IF BLOCK" << endl;
        ifblock->accept(*this);
        type = Type::DEFAULT_TYPE;
        if(elseblock){
            cout << "ELSE BLOCK" << endl;
            elseblock->accept(*this);
        }
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTFor &node)
    {
        ASTExpr *expr1 = node.getexpr1();
        ASTExpr *expr2 = node.getexpr2();
        ASTExpr *expr3 = node.getexpr3();
        ASTBlock *block = node.getblock();

        expr1->accept(*this);
        Type it_type = type;
        ASSERT(type == Type::INT, "For loop iterator must be int");

        expr2->accept(*this);
        ASSERT(type == it_type, "For loop expr2 must be int");

        expr3->accept(*this);
        ASSERT(type == it_type, "For loop expr3 must be int");

        type = Type::DEFAULT_TYPE;
        block->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTWhile &node)
    {
        ASTExpr *expr = node.getexpr();
        ASTBlock *block = node.getblock();

        expr->accept(*this);
        ASSERT(type == Type::BOOL, "While loop iterator must be bool");

        type = Type::DEFAULT_TYPE;
        block->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTPrint &node)
    {
        cout << "print ";
        ASTExpr *expr = node.getexpr();
        expr->accept(*this);
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTFunc &node)
    {
        cout << "ASTFunc visit function" << endl;
    }

    virtual void visit(ASTFuncParam &node)
    {
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
        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTFuncNoparam &node)
    {
        cout << " " << node.type;
        cout << " " << node.id;
        cout << "()";
        node.block->accept(*this);
        type = Type::DEFAULT_TYPE;
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
        type = Type::DEFAULT_TYPE;
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

    virtual void visit(ASTExpr &node)
    {
        cout << "ASTExpr visit function" << endl;
    }

    virtual void visit(ASTExprBinary &node)
    {
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        Type left_type = type;

        right->accept(*this);
        Type right_type = type;

        string op = node.getBin_operator();
        ASSERT(left_type == right_type, "Binary Expression Operator and Operands have incompatible type");

        if(op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" || op == "AND" || op == "OR" || op == "NOT")
            type = Type::BOOL;
        else if(left_type == Type::CHAR or right_type == Type::CHAR)
            type = Type::CHAR;
        else if(left_type == Type::INT or right_type == Type::INT)
            type = Type::INT;

        type = Type::DEFAULT_TYPE;
    }

    virtual void visit(ASTExprTernary &node)
    {
        ASTExpr *first = node.getFirst();
        ASTExpr *second = node.getSecond();
        ASTExpr *third = node.getThird();

        first->accept(*this);
        
        second->accept(*this);
        third->accept(*this);

        type = Type::DEFAULT_TYPE;
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
        type = Type::INT;
        cout << " " << node.getIntLit();
    }

    virtual void visit(ASTVarBOOL &node)
    {
        type = Type::BOOL;
        cout << " " << node.getBoolLit();
    }

    virtual void visit(ASTVarCHAR &node)
    {
        type = Type::CHAR;
        cout << " " << node.getCharLit();
    }

    virtual void visit(ASTVarSTRING &node)
    {
        cout << " " << node.getStringLit();
    }
};