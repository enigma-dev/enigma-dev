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

namespace jdip {
  struct ASTOperator {
    virtual void operate(AST_Node* x, void *param) = 0;
    virtual void operate_Definition(AST_Node_Definition* x, void *param) = 0;
    virtual void operate_Scope(AST_Node_Scope* x, void *param) = 0;
    virtual void operate_Type(AST_Node_Type* x, void *param) = 0;
    virtual void operate_Unary(AST_Node_Unary* x, void *param) = 0;
    virtual void operate_sizeof(AST_Node_sizeof* x, void *param) = 0;
    virtual void operate_Cast(AST_Node_Cast* x, void *param) = 0;
    virtual void operate_Binary(AST_Node_Binary* x, void *param) = 0;
    virtual void operate_Ternary(AST_Node_Ternary* x, void *param) = 0;
    virtual void operate_Parameters(AST_Node_Parameters* x, void *param) = 0;
    virtual void operate_Array(AST_Node_Array* x, void *param) = 0;
    virtual void operate_new(AST_Node_new* x, void *param) = 0;
    virtual void operate_delete(AST_Node_delete* x, void *param) = 0;
    virtual void operate_Subscript(AST_Node_Subscript* x, void *param) = 0;
    virtual void operate_TempInst(AST_Node_TempInst* x, void *param) = 0;
    virtual void operate_TempKeyInst(AST_Node_TempKeyInst* x, void *param) = 0;
    virtual ~ASTOperator();
  };

  struct ConstASTOperator {
    virtual void operate(const AST_Node* x, void *param) = 0;
    virtual void operate_Definition(const AST_Node_Definition* x, void *param) = 0;
    virtual void operate_Scope(const AST_Node_Scope* x, void *param) = 0;
    virtual void operate_Type(const AST_Node_Type* x, void *param) = 0;
    virtual void operate_Unary(const AST_Node_Unary* x, void *param) = 0;
    virtual void operate_sizeof(const AST_Node_sizeof* x, void *param) = 0;
    virtual void operate_Cast(const AST_Node_Cast* x, void *param) = 0;
    virtual void operate_Binary(const AST_Node_Binary* x, void *param) = 0;
    virtual void operate_Ternary(const AST_Node_Ternary* x, void *param) = 0;
    virtual void operate_Parameters(const AST_Node_Parameters* x, void *param) = 0;
    virtual void operate_Array(const AST_Node_Array* x, void *param) = 0;
    virtual void operate_new(const AST_Node_new* x, void *param) = 0;
    virtual void operate_delete(const AST_Node_delete* x, void *param) = 0;
    virtual void operate_Subscript(const AST_Node_Subscript* x, void *param) = 0;
    virtual void operate_TempInst(const AST_Node_TempInst* x, void *param) = 0;
    virtual void operate_TempKeyInst(const AST_Node_TempKeyInst* x, void *param) = 0;
    virtual ~ConstASTOperator();
  };
}

#endif
