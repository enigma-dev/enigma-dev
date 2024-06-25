#include "parser-test-classes.h"

void reach_nonspace(std::string &s, std::size_t &i) {
  while (i < s.size() && s[i] == ' ') {
    i++;
  }
}

bool compare(std::string code, std::string printed) {
  if (printed.size() > code.size()) {
    swap(printed, code);
  }

  std::size_t ind = 0;
  for (std::size_t i = 0; i < code.size(); i++) {
    reach_nonspace(printed, ind);
    if (code[i] == ' ') {
      continue;
    }
    if (code[i] != printed[ind]) {
      return false;
    }
    ind++;
  }

  if (ind != printed.size()) {
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

TEST(PrinterTest, test8) {
  std::string code =
      "if(true) for (int i = 0; i < 12; i++) k++ else switch (i) { case 1 : k-- case 2 :k += 3 default : k = 0 }";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "if(true) for (signed int i = 0; i < 12; i++) k++; else switch (i) { case 1 : {k--;} case 2 :{k += 3;} default "
      ":{ k = "
      "0;} }";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test9) {
  std::string code = "repeat(3){int xx =12;  foo(12, fo(12), sizeof(int)) while((2)){c-- c++ c*=2}}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "int strange_name = (3); while(strange_name){signed int xx =12;  foo(12, fo(12), sizeof(signed int)); "
      "while((2)){c--; "
      "c++; c+=2;}}";

  ASSERT_TRUE(compare(code, printed));
}
