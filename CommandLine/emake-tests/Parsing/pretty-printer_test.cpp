#include "parser-test-classes.h"

TEST(PrinterTest, test1) {
  ParserTester test{"{{{}}}"};
  auto node = test->ParseCode();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  ASSERT_EQ(block->statements.size(), 1);
  ASSERT_EQ(block->statements[0]->type, AST::NodeType::BLOCK);

  auto *inner_block = block->statements[0]->As<AST::CodeBlock>();
  ASSERT_EQ(inner_block->statements.size(), 1);
  ASSERT_EQ(inner_block->statements[0]->type, AST::NodeType::BLOCK);
}
 