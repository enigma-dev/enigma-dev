#include "parse_edl.h"
#include "parser/lex_edl.h"
#include "languages/language_adapter.h"

using namespace jdip;

struct EDL_AST: AST {
  /// General parent
  struct AST_Node_Code: AST::AST_Node {
    virtual string toString() const = 0; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const = 0; ///< Renders this node and its children as a string, recursively.
    
    AST_Node_Code();
    virtual ~AST_Node_Code();
  };
  
  /// Represents a block of code
  struct AST_Node_block: AST_Node_Code {
    vector<AST_Node_Code*> operations; ///< Statements and operations in the order they are to be executed.
    
    AST_Node_block(); ///< Default constructor.
    ~AST_Node_block(); ///< Default destructor. Frees children recursively.
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };

  struct AST_Node_Statement: AST_Node_Code {
    AST::AST_Node *operand;
    
    AST_Node_Statement(AST_Node* left=NULL, AST::AST_Node* right=NULL); ///< Default constructor. Sets children to NULL.
    AST_Node_Statement(AST_Node* left, AST::AST_Node* right, string op); ///< Default constructor. Sets children to NULL.
    ~AST_Node_Statement(); ///< Default destructor. Frees children recursively.
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };
  
  struct AST_Node_Statement_if: AST_Node_Statement {
    AST_Node_block *do_if;
    AST_Node_block *do_else;
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };
  
  struct AST_Node_Statement_for: AST_Node_Statement {
    AST_Node *condition;
    AST_Node *operand_post;
    AST_Node_block *do_for;
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };
  
  struct AST_Node_Statement_while: AST_Node_Statement {
    AST_Node_Statement *condition;
    bool negate; ///< True if this is actually an `until' statement
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };
  
  struct AST_Node_Statement_do: AST_Node_Statement {
    AST_Node_Statement *condition;
    
    virtual string toString() const; ///< Renders this node and its children as a string, recursively.
    virtual string toString(int indent) const; ///< Renders this node and its children as a string, recursively.
    virtual void toSVG(int x, int y, SVGrenderInfo* svg); ///< Renders this node and its children as an SVG.
    virtual int width(); ///< Returns the width which will be used to render this node and all its children.
    virtual int height(); ///< Returns the height which will be used to render this node and all its children.
  };
  
  struct AST_Node_Statement_return {
    //virtual 
  };
};

EDL_AST::AST_Node_Code::AST_Node_Code(): AST_Node() {}
EDL_AST::AST_Node_Code::~AST_Node_Code() {}

string EDL_AST::AST_Node_block::toString() const {
  return toString(0);
}
string EDL_AST::AST_Node_block::toString(int indent) const {
  if (operations.size() > 1) {
    string res = "{\n";
    string indstr(indent + 2, ' ');
    for (size_t i = 0; i < operations.size(); ++i)
      res += indstr + operations[i]->toString(indent + 2) + "\n";
    return res + string(indent, ' ') + "}";
  }
  else {
    if (operations.empty())
      return "{}";
    return string(indent, ' ') + operations[0]->toString(indent + 2) + ";";
  }
}


struct definition_code: definition {
  EDL_AST::AST_Node_block *code;
};
struct definition_object: definition_scope {
  
};


void parse_edl(string code) {
  llreader codereader;
  codereader.encapsulate(code);
  lexer_edl lex(codereader, (macro_map&)main_context->get_macros(), "Code");
  token_t token = lex.get_token(def_error_handler);
  while (token.type != TT_ENDOFCODE) {
    token.report_errorf(def_error_handler, "read %s");
    token = lex.get_token(def_error_handler);
  }
}
