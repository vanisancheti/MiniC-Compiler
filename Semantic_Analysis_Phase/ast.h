#include <string>
#include <vector>

using namespace std;

class ASTProg;
class ASTBlock;

class ASTStatExpr;
class ASTStatCond;
class ASTStatBREAK;
class ASTStatCONTINUE;

class ASTIf;
class ASTFor;
class ASTWhile;

class ASTFunc;
class ASTFuncParam;
class ASTFuncNoparam;

class ASTFunccall;
class ASTFunccallParam;
class ASTFunccallNoparam;

class ASTVardecl;
class ASTVarSingle;
class ASTVar1darray;
class ASTVar2darray;

// class ASTPrint;
// class ASTReadinput;
// class ASTReadfile;

class ASTExpr;
class ASTExprBinary;
class ASTExprTernary;

class ASTVar;
class ASTVarID;
class ASTVarINT;
class ASTVarBOOL;
class ASTVarCHAR;
class ASTVarSTRING;

class ASTvisitor
{
public:
    virtual void visit(ASTProg &node) = 0;
    virtual void visit(ASTBlock &node) = 0;

    virtual void visit(ASTStatExpr &node) = 0;
    virtual void visit(ASTStatCond &node) = 0;
    virtual void visit(ASTStatBREAK &node) = 0;
    virtual void visit(ASTStatCONTINUE &node) = 0;

    virtual void visit(ASTIf &node) = 0;
    virtual void visit(ASTFor &node) = 0;
    virtual void visit(ASTWhile &node) = 0;

    virtual void visit(ASTFunc &node) = 0;
    virtual void visit(ASTFuncParam &node) = 0;
    virtual void visit(ASTFuncNoparam &node) = 0;

    virtual void visit(ASTFunccall &node) = 0;
    virtual void visit(ASTFunccallParam &node) = 0;
    virtual void visit(ASTFunccallNoparam &node) = 0;

    virtual void visit(ASTVardecl &node) = 0;
    virtual void visit(ASTVar1darray &node) = 0;
    virtual void visit(ASTVar2darray &node) = 0;
    // virtual void visit(ASTPrint &node) = 0;
    // virtual void visit(ASTReadinput &node) = 0;
    // virtual void visit(ASTReadfile &node) = 0;

    virtual void visit(ASTExpr &node) = 0;
    virtual void visit(ASTExprBinary &node) = 0;
    virtual void visit(ASTExprTernary &node) = 0;

    virtual void visit(ASTVar &node) = 0;
    virtual void visit(ASTVarID &node) = 0;
    virtual void visit(ASTVarINT &node) = 0;
    virtual void visit(ASTVarBOOL &node) = 0;
    virtual void visit(ASTVarCHAR &node) = 0;
    virtual void visit(ASTVarSTRING &node) = 0;
};

class ASTnode
{
public:
    virtual ~ASTnode()
    {
    }

    //  virtual void printPostFix() const = 0;

    virtual void accept(ASTvisitor &V) = 0;
};

class ASTStat : public ASTnode
{
public:
    virtual ~ASTStat()
    {
    }

    //  virtual void printPostFix() const = 0;

    virtual void accept(ASTvisitor &V) = 0;
};

class ASTProg : public ASTnode
{
public:
    ASTBlock *block;
    std::vector<ASTVardecl *> variabledeclList;
    std::vector<ASTFunc *> funcdeclList;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTBlock : public ASTnode
{
public:
    std::vector<ASTStat *> statementList;
    ASTExpr *expr;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTExpr : public ASTnode
{
public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatExpr : public ASTStat
{
public:
    ASTExpr *expr;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

// class ASTStatIf : public ASTStat
// {
// public:
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

class ASTIf : public ASTStat
{
    ASTExpr *expr;
    ASTBlock *ifblock;
    ASTBlock *elseblock;
    
public:
    ASTIf(ASTExpr *_expr, ASTBlock *_ifblock, ASTBlock *_elseblock = nullptr) : expr(_expr), ifblock(_ifblock), elseblock(_elseblock) {}
    ASTExpr *getexpr()
    {
        return expr;
    }

    ASTBlock *getifblock()
    {
        return ifblock;
    }

    ASTBlock *getelseblock()
    {
        return elseblock;
    }
    
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

// class ASTStatFor : public ASTStat
// {
// public:
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

class ASTFor : public ASTStat
{
    ASTExpr *expr1;
    ASTExpr *expr2;
    ASTExpr *expr3;
    ASTBlock *block;
    
public:
    ASTFor(ASTExpr *_expr1, ASTExpr *_expr2, ASTExpr *_expr3, ASTBlock *_block) : expr1(_expr1), expr2(_expr2), expr3(_expr3), block(_block) {}
    ASTExpr *getexpr1()
    {
        return expr1;
    }

    ASTExpr *getexpr2()
    {
        return expr2;
    }

    ASTExpr *getexpr3()
    {
        return expr3;
    }

    ASTBlock *getblock()
    {
        return block;
    }
    
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

// class ASTStatWhile : public ASTStat
// {
// public:
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

class ASTWhile : public ASTStat
{
    ASTExpr *expr;
    ASTBlock *block;
    
public:
    ASTWhile(ASTExpr *_expr, ASTBlock *_block) : expr(_expr), block(_block) {}
    ASTExpr *getexpr()
    {
        return expr;
    }

    ASTBlock *getblock()
    {
        return block;
    }
    
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatCond : public ASTStat
{
public:
    ASTExprTernary *exprter;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatBREAK : public ASTStat
{
public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatCONTINUE : public ASTStat
{
public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTFunc : public ASTnode
{
    public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTFuncParam : public ASTFunc
{
    public:
    std::vector<ASTVardecl *> variabledeclList;
    ASTVardecl *vardecl;
    string type;
    string id;
    ASTBlock *block;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTFuncNoparam : public ASTFunc
{
    public:
    string type;
    string id;
    ASTBlock *block;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTFunccall : public ASTExpr
{
    public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTFunccallParam : public ASTFunccall
{
    public:
    std::vector<ASTVar *> variableList;
    ASTVar *var;
    string id;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTFunccallNoparam : public ASTFunccall
{
    public:
    string id;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTVardecl : public ASTStat
{
public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarSingle : public ASTVardecl
{
    string type;
    string id;
public:
    ASTVarSingle(string type, string id) : type(type), id(id) {}
    string getTYPE()
    {
        return type;
    }
    
    string getID()
    {
        return id;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVar1darray : public ASTVardecl
{
    string type;
    string id_name;
    
    int intlit;
    string id_value;
    ASTExpr *expr;

public:
    ASTVar1darray(string type, string id_name, int intlit=0, string id_value="", ASTExpr *expr = nullptr) : type(type), id_name(id_name), intlit(intlit), id_value(id_value), expr(expr) {}
    string getTYPE()
    {
        return type;
    }
    
    string getIDname()
    {
        return id_name;
    }

    int getint()
    {
        return intlit;
    }

    string getIDvalue()
    {
        return id_value;
    }

    ASTExpr* getexpr()
    {
        return expr;
    } 

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVar2darray : public ASTVardecl
{
    string type;
    string id_name;
    
    int intlit1, intlit2;
    string id_value1, id_value2;
    ASTExpr *expr1, *expr2;

public:
    ASTVar2darray(string type, string id_name, int intlit1=0, int intlit2=0, string id_value1="", string id_value2="", ASTExpr *expr1 = nullptr, ASTExpr *expr2 = nullptr) : type(type), id_name(id_name), intlit1(intlit1), intlit2(intlit2), id_value1(id_value1), id_value2(id_value2), expr1(expr1), expr2(expr2) {}
    string getTYPE()
    {
        return type;
    }
    
    string getIDname()
    {
        return id_name;
    }

    int getint1()
    {
        return intlit1;
    }

    int getint2()
    {
        return intlit2;
    }

    string getIDvalue1()
    {
        return id_value1;
    }

    string getIDvalue2()
    {
        return id_value2;
    }

    ASTExpr* getexpr1()
    {
        return expr1;
    }

    ASTExpr* getexpr2()
    {
        return expr2;
    } 

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

// class ASTPrint : public ASTStat
// {
//     ASTExpr *expr;
// public:
//     ASTPrint(ASTExpr *_expr) : expr(_expr) {}
//     ASTExpr *getexpr()
//     {
//         return expr;
//     }
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

// class ASTReadinput : public ASTStat
// {
//     ASTVar *var;
// public:
//     ASTReadinput(ASTVar *_var) : var(_var) {}
//     ASTVar *getvar()
//     {
//         return var;
//     }
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

// class ASTReadfile : public ASTStatRead
// {
//     ASTVar *var;
//     ASTVarID *ID;
//     ASTVarSTRING *STRING;
// public:
//     ASTReadfile(ASTVar *_var, ASTVarID *_ID = nullptr, ASTVarSTRING *_STRING = nullptr) : var(_var), ID(_ID), STRING(_STRING) {}
//     ASTVar *getvar()
//     {
//         return var;
//     }
//     ASTVarID *getvarId()
//     {
//         return ID;
//     }
//     ASTVarSTRING *getvarSTRING()
//     {
//         return STRING;
//     }
//     virtual void accept(ASTvisitor &v)
//     {
//         v.visit(*this);
//     }
// };

class ASTExprBinary : public ASTExpr
{
    std::string bin_operator;

    // lhs and rhs can be of any type.
    // So we need to use BaseAST
    ASTExpr *left;
    ASTExpr *right;

public:
    // Constructor to initialize binary operator,
    // lhs and rhs of the binary expression.
    ASTExprBinary(std::string op, ASTExpr *_left, ASTExpr *_right) : bin_operator(op), left(_left), right(_right) {}

    /*  virtual void printPostFix() const 
     {
     	lhs->printPostFix();
     	rhs->printPostFix();
     	std::cout << bin_operator << " "; 
     } */

    ASTExpr *getLeft()
    {
        return left;
    }

    ASTExpr *getRight()
    {
        return right;
    }

    std::string getBin_operator()
    {
        return bin_operator;
    }
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTExprTernary : public ASTExpr
{

    ASTExpr *first;
    ASTExpr *second;
    ASTExpr *third;

public:
    ASTExprTernary(ASTExpr *first, ASTExpr *second, ASTExpr *third) : first(first), second(second), third(third) {}

    /*   virtual void printPostFix() const
     {
     	first->printPostFix();
     	second->printPostFix();
     	third->printPostFix();
     	std::cout << "? " << std::endl; 
     }  */

    ASTExpr *getFirst()
    {
        return first;
    }

    ASTExpr *getSecond()
    {
        return second;
    }

    ASTExpr *getThird()
    {
        return third;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVar : public ASTExpr
{
public:
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarID : public ASTVar
{

    string id;

public:
    ASTVarID(string id) : id(id) {}

    string getID()
    {
        return id;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarINT : public ASTVar
{

    int intlit;

public:
    ASTVarINT(int intlit) : intlit(intlit) {}

    int getIntLit()
    {
        return intlit;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarBOOL : public ASTVar
{

    bool boollit;

public:
    ASTVarBOOL(bool boollit) : boollit(boollit) {}

    bool getBoolLit()
    {
        return boollit;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarCHAR : public ASTVar
{
    char charlit;
public:
    ASTVarCHAR(char charlit) : charlit(charlit) {}

    char getCharLit()
    {
        return charlit;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarSTRING : public ASTVar
{

    string stringlit;

public:
    ASTVarSTRING(string stringlit) : stringlit(stringlit) {}

    string getStringLit()
    {
        return stringlit;
    }

    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTContext
{
public:
    ASTnode *root;

    ~ASTContext()
    {
        clearAST();
    }

    /// free all saved expression trees
    void clearAST()
    {
        delete root;
    }
};
