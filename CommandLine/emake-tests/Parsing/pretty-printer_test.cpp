#include "parser-test-classes.h"

void reach_nonspace(std::string &s, std::size_t &i) {
  while (s[i] == ' ') {
    i++;
  }
}

bool compare(std::string code, std::string printed) {
  if (printed.size() > code.size()) {
    swap(printed, code);
  }

  std::size_t count = 0;
  for (std::size_t i = 0; i < code.size(); i++) {
    reach_nonspace(printed, count);
    if (code[i] == ' ') {
      continue;
    }
    if (code[i] != printed[count]) {
      return false;
    }
    count++;
  }

  if (count != printed.size()) {
    return false;
  }
  return true;
}

TEST(PrinterTest, test1) {
  std::string code = "{{{}}}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test2) {
  std::string code = "int x = new int";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code = "signed int x = new(signed int);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test3) {
  std::string code = "{c++; g++; {int x=12; {for(int i=12;i!=22;i++){g++; c+=23; int cc = new int}}}}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "{c++; g++; {signed int x=12; {for(signed int i=12;i!=22;i++){g++; c+=23; signed int cc = new (signed int);}}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test4) {
  std::string code = "int x;  int y=12,v=2333,dd=33; for(int g=0;g<22;g++){do{c++; v=cc+12+fn(12,22);}while(true)}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "signed int x;  signed int y=12,v=2333,dd=33; for(signed int g=0;g<22;g++){do{c++; "
      "v=cc+12+fn(12,22);}while(true);}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test5) {
  std::string code = "while(1){while(1){while(0) c++;}}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test6) {
  std::string code = "int count =0; if(inc) count++; else if(dec) count--; else count=0 int flag=0; flag= (count)? 1:0";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "signed int count =0; if(inc) count++; else if(dec) count--; else count=0; signed int flag=0; flag= (count)? "
      "1:0;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test7) {
  std::string code =
      "{ int x = 5 const int y = 6 float *(*z)[10] = nullptr foo(bar) while(i) for(int "
      "p=12;p/2;p++){while(12){if(1)c++ else c--;}}}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "{ signed int x = 5; signed int y = 6; signed float *(*z)[18446744073709551615] = nullptr ;foo(bar); while(i) "
      "for(signed int p=12;p/2;p++){while(12){if(1)c++; else c--;}}}";

  ASSERT_TRUE(compare(code, printed));
}
