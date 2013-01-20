/**
 * @file  AST_operator.h
 * @brief Header declaring a class that can operate recursively on an AST.
 * 
 * @section License
 * 
 * Copyright (C) 2012-2013 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
 * 
**/

#ifndef _AST_OPERATOR__H
#define _AST_OPERATOR__H

#include "AST.h"

namespace jdi {
  struct ASTOperator {
    virtual void operate(AST::AST_Node* x, void *param) = 0;
    virtual void operate_Definition(AST::AST_Node_Definition* x, void *param) = 0;
    virtual void operate_Scope(AST::AST_Node_Scope* x, void *param) = 0;
    virtual void operate_Type(AST::AST_Node_Type* x, void *param) = 0;
    virtual void operate_Unary(AST::AST_Node_Unary* x, void *param) = 0;
    virtual void operate_sizeof(AST::AST_Node_sizeof* x, void *param) = 0;
    virtual void operate_Cast(AST::AST_Node_Cast* x, void *param) = 0;
    virtual void operate_Binary(AST::AST_Node_Binary* x, void *param) = 0;
    virtual void operate_Ternary(AST::AST_Node_Ternary* x, void *param) = 0;
    virtual void operate_Parameters(AST::AST_Node_Parameters* x, void *param) = 0;
    virtual void operate_Array(AST::AST_Node_Array* x, void *param) = 0;
    virtual void operate_new(AST::AST_Node_new* x, void *param) = 0;
    virtual void operate_delete(AST::AST_Node_delete* x, void *param) = 0;
    virtual void operate_Subscript(AST::AST_Node_Subscript* x, void *param) = 0;
    virtual ~ASTOperator();
  };

  struct ConstASTOperator {
    virtual void operate(const AST::AST_Node* x, void *param) = 0;
    virtual void operate_Definition(const AST::AST_Node_Definition* x, void *param) = 0;
    virtual void operate_Scope(const AST::AST_Node_Scope* x, void *param) = 0;
    virtual void operate_Type(const AST::AST_Node_Type* x, void *param) = 0;
    virtual void operate_Unary(const AST::AST_Node_Unary* x, void *param) = 0;
    virtual void operate_sizeof(const AST::AST_Node_sizeof* x, void *param) = 0;
    virtual void operate_Cast(const AST::AST_Node_Cast* x, void *param) = 0;
    virtual void operate_Binary(const AST::AST_Node_Binary* x, void *param) = 0;
    virtual void operate_Ternary(const AST::AST_Node_Ternary* x, void *param) = 0;
    virtual void operate_Parameters(const AST::AST_Node_Parameters* x, void *param) = 0;
    virtual void operate_Array(const AST::AST_Node_Array* x, void *param) = 0;
    virtual void operate_new(const AST::AST_Node_new* x, void *param) = 0;
    virtual void operate_delete(const AST::AST_Node_delete* x, void *param) = 0;
    virtual void operate_Subscript(const AST::AST_Node_Subscript* x, void *param) = 0;
    virtual ~ConstASTOperator();
  };
}

#endif
