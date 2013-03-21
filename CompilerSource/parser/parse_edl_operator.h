
/**
  @file  parse_edl_operator.h
  @brief Header declaring a class that can operate recursively on an EDL_AST.

  @section License

    Copyright (C) 2013 forthevin
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
 *
**/

#ifndef _EDL_AST_OPERATOR__H
#define _EDL_AST_OPERATOR__H

#include "JDI/src/API/AST_operator.h"
#include "parser/parse_edl.h"

struct EDL_ASTOperator: public jdi::ASTOperator {
  virtual void operate_Statement(EDL_AST::AST_Node_Statement* x, void *param) = 0;
  virtual void operate_Statement_Standard(EDL_AST::AST_Node_Statement_Standard* x, void *param) = 0;
  virtual void operate_Block(EDL_AST::AST_Node_Block* x, void *param) = 0;
  virtual void operate_Declaration(EDL_AST::AST_Node_Declaration* x, void *param) = 0;
  virtual void operate_Structdef(EDL_AST::AST_Node_Structdef* x, void *param) = 0;
  virtual void operate_Enumdef(EDL_AST::AST_Node_Enumdef* x, void *param) = 0;
  virtual void operate_Statement_if(EDL_AST::AST_Node_Statement_if* x, void *param) = 0;
  virtual void operate_Statement_for(EDL_AST::AST_Node_Statement_for* x, void *param) = 0;
  virtual void operate_Statement_repeat(EDL_AST::AST_Node_Statement_repeat* x, void *param) = 0;
  virtual void operate_Statement_while(EDL_AST::AST_Node_Statement_while* x, void *param) = 0;
  virtual void operate_Statement_with(EDL_AST::AST_Node_Statement_with* x, void *param) = 0;
  virtual void operate_Statement_switch(EDL_AST::AST_Node_Statement_switch* x, void *param) = 0;
  virtual void operate_Statement_trycatch(EDL_AST::AST_Node_Statement_trycatch* x, void *param) = 0;
  virtual void operate_Statement_do(EDL_AST::AST_Node_Statement_do* x, void *param) = 0;
  virtual void operate_Statement_return(EDL_AST::AST_Node_Statement_return* x, void *param) = 0;
  virtual void operate_Statement_default(EDL_AST::AST_Node_Statement_default* x, void *param) = 0;
  virtual void operate_Statement_case(EDL_AST::AST_Node_Statement_case* x, void *param) = 0;
  virtual void operate_Statement_break(EDL_AST::AST_Node_Statement_break* x, void *param) = 0;
  virtual void operate_Statement_continue(EDL_AST::AST_Node_Statement_continue* x, void *param) = 0;
  virtual ~EDL_ASTOperator();
};

struct EDL_ConstASTOperator: public jdi::ConstASTOperator {
  virtual void operate_Statement(const EDL_AST::AST_Node_Statement* x, void *param) = 0;
  virtual void operate_Statement_Standard(const EDL_AST::AST_Node_Statement_Standard* x, void *param) = 0;
  virtual void operate_Block(const EDL_AST::AST_Node_Block* x, void *param) = 0;
  virtual void operate_Declaration(const EDL_AST::AST_Node_Declaration* x, void *param) = 0;
  virtual void operate_Structdef(const EDL_AST::AST_Node_Structdef* x, void *param) = 0;
  virtual void operate_Enumdef(const EDL_AST::AST_Node_Enumdef* x, void *param) = 0;
  virtual void operate_Statement_if(const EDL_AST::AST_Node_Statement_if* x, void *param) = 0;
  virtual void operate_Statement_for(const EDL_AST::AST_Node_Statement_for* x, void *param) = 0;
  virtual void operate_Statement_repeat(const EDL_AST::AST_Node_Statement_repeat* x, void *param) = 0;
  virtual void operate_Statement_while(const EDL_AST::AST_Node_Statement_while* x, void *param) = 0;
  virtual void operate_Statement_with(const EDL_AST::AST_Node_Statement_with* x, void *param) = 0;
  virtual void operate_Statement_switch(const EDL_AST::AST_Node_Statement_switch* x, void *param) = 0;
  virtual void operate_Statement_trycatch(const EDL_AST::AST_Node_Statement_trycatch* x, void *param) = 0;
  virtual void operate_Statement_do(const EDL_AST::AST_Node_Statement_do* x, void *param) = 0;
  virtual void operate_Statement_return(const EDL_AST::AST_Node_Statement_return* x, void *param) = 0;
  virtual void operate_Statement_default(const EDL_AST::AST_Node_Statement_default* x, void *param) = 0;
  virtual void operate_Statement_case(const EDL_AST::AST_Node_Statement_case* x, void *param) = 0;
  virtual void operate_Statement_break(const EDL_AST::AST_Node_Statement_break* x, void *param) = 0;
  virtual void operate_Statement_continue(const EDL_AST::AST_Node_Statement_continue* x, void *param) = 0;
  virtual ~EDL_ConstASTOperator();
};

#endif
