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
      "int strange_name = (3); while(strange_name--){signed int xx =12;  foo(12, fo(12), sizeof(signed int)); "
      "while((2)){c--; "
      "c++; c*=2;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test10) {
  std::string code = "alignof(const volatile unsigned long long *)";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code = "alignof(const volatile unsigned long long int*);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test11) {
  std::string code =
      "switch (5 * 6 +12) { case 1:while(1){while((f)) do{s-- float a = new float}until(1)} return 2 break case 2: "
      "return 3 int x = sizeof 12 break default: f-=22 break}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "switch (5 * 6 +12) { case 1: {while(1){while((f)) do{s--; signed float a = new (signed float);}while(!(1));} "
      "return 2; break; }case 2:{ return 3; signed int x = sizeof 12; break; } default:{ f-=22; break;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test12) {
  std::string code =
      "if (head == nullptr) return false;if (head->data == val) {Node* temp = head; head = head->next; delete temp; "
      "return true;}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test13) {
  std::string code =
      "Node* current = head;while (current->next != nullptr && current->next->data != val) {current = "
      "current->next;}if (current->next == nullptr) return false;Node* temp = current->next; current->next = "
      "current->next->next; delete temp; return true;";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test14) {
  std::string code =
      "{Node* temp = head;while (temp != nullptr) {if (temp->data == val) return true;temp = temp->next;}return "
      "false;}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test15) {
  std::string code =
      "int count = 0;Node* temp = head;while (temp != nullptr) {count++;temp = temp->next;}return count;";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code = "signed int count = 0;Node* temp = head;while (temp != nullptr) {count++;temp = temp->next;}return count;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test16) {
  std::string code =
      "{Book* current = head;Book* next;while (current != nullptr) {next = current->next;delete current;current = "
      "next;}head = nullptr;}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test17) {
  std::string code =
      "if (head == nullptr) {head = newBook;} else {Book* temp = head;while (temp->next != nullptr) {temp = "
      "temp->next;}temp->next = newBook;}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test18) {
  std::string code =
      "{if (head == nullptr) return;bool swapped;do {swapped = false;Book* temp = head;while (temp->next != nullptr) "
      "{if (temp->title > temp->next->title) {swap(temp, temp->next);swapped = true;}temp = temp->next;}} while "
      "(swapped);}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "{if (head == nullptr) return;signed bool swapped;do {swapped = false;Book* temp = head;while (temp->next != "
      "nullptr) "
      "{if (temp->title > temp->next->title) {swap(temp, temp->next);swapped = true;}temp = temp->next;}} while "
      "(swapped);}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test19) {
  std::string code =
      "{if (head == nullptr) return false;if (head->title == title) {Book* temp = head;head = head->next;delete "
      "temp;return true;}Book* current = head;while (current->next != nullptr && current->next->title != title) "
      "{current = current->next;}if (current->next == nullptr) return false;Book* temp = current->next;current->next = "
      "current->next->next;delete temp;return true;}";
  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test20) {
  std::string code =
      "if (cmd == \" add\") {int title, author;int year;getline(infile, title);getline(infile, "
      "author);addBook(title, author, year);} "
      "else if (cmd == \"remove\") { title;getline(infile, title);if (removeBook(title)) {c*=2;} else {c/=2;}} "
      "else if (cmd == \"find\") {int title;getline(infile, title);if (findBook(title)) {c+=1} else {c--;}} else if "
      "(cmd == \"findDetailed\") {int title;getline(infile, title);Book* temp = head;found = false;while (temp != "
      "nullptr) {if (temp->title == title) {detailedBookInfo(temp, outfile);found = true;break;}temp = temp->next;}if "
      "(!found) {c++;}}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "if (cmd == \" add\") {signed int title, author;signed int year;getline(infile, title);getline(infile, "
      "author);addBook(title, author, year);} "
      "else if (cmd == \"remove\") { title;getline(infile, title);if (removeBook(title)) {c*=2;} else {c/=2;}} "
      "else if (cmd == \"find\") {signed int title;getline(infile, title);if (findBook(title)) {c+=1;} else {c--;}} "
      "else if "
      "(cmd == \"findDetailed\") {signed int title;getline(infile, title);Book* temp = head;found = false;while (temp "
      "!= "
      "nullptr) {if (temp->title == title) {detailedBookInfo(temp, outfile);found = true;break;}temp = temp->next;}if "
      "(!found) {c++;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test21) {
  std::string code =
      "for (char i = 'A'; i <= 'B'; ++i) {for (char j = '1'; j <= '2'; ++j) {for (char k = 'a'; k <= 'b'; ++k) {for "
      "(char l = 'X'; l <= 'Y'; ++l) {c++;}}}}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "for (signed char i = 'A'; i <= 'B'; ++i) {for (signed char j = '1'; j <= '2'; ++j) {for (signed char k = 'a'; k "
      "<= 'b'; ++k) {for "
      "(signed char l = 'X'; l <= 'Y'; ++l) {c++;}}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test22) {
  std::string code =
      "char i = 'A';do {char j = '1';while (j <= '2') {for (char k = 'a'; k <= 'b'; ++k) {char l = 'X';do {++l;} while "
      "(l <= 'Y');}++j;}++i;} while (i <= 'B');";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "signed char i = 'A';do {signed char j = '1';while (j <= '2') {for (signed char k = 'a'; k <= 'b'; ++k) {signed "
      "char l = 'X';do {++l;} while "
      "(l <= 'Y');}++j;}++i;} while (i <= 'B');";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test23) {
  std::string code =
      "do {char j = '1';while (j <= '2') {for (char k = 'a'; k <= 'b'; ++k) {char l = 'X';do {switch (i) {case "
      "'A':break;case 'B':break;default:break;}condition = (i == 'A' && j == '1') || (k == 'b' && l == 'Y');if "
      "(condition) {c++;}++l;} while (l <= 'Y');k--;k++;}++j;}++i;} while (i <= 'B');";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "do {signed char j = '1';while (j <= '2') {for (signed char k = 'a'; k <= 'b'; ++k) {signed char l = 'X';do "
      "{switch (i) {case "
      "'A':{break;}case 'B':{break;}default:{break;}}condition = (i == 'A' && j == '1') || (k == 'b' && l == 'Y');if "
      "(condition) {c++;}++l;} while (l <= 'Y');k--;k++;}++j;}++i;} while (i <= 'B');";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test24) {
  std::string code =
      "int outerLoopLimit = 3;int innerLoopLimit = 2;for (int i = 0; i < outerLoopLimit; ++i) {int j = 0;while (j < "
      "innerLoopLimit) {int k = 0;do {int a = 5;int b = 3;int result;result = a + b;result = a * b;int preIncrement = "
      "++a;int postIncrement = b++;double castResult = static_cast<double>(a) / b; ++k;} while (k < 1);++j;}}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "signed int outerLoopLimit = 3; signed int innerLoopLimit = 2;for (signed int i = 0; i < outerLoopLimit; ++i) "
      "{signed int j = 0;while (j < "
      "innerLoopLimit) {signed int k = 0;do {signed int a = 5; signed int b = 3;signed int result;result = a + "
      "b;result = a * b;signed int preIncrement = "
      "++a;signed int postIncrement = b++;signed double castResult = static_cast<signed double>(a) / b; ++k;} while (k "
      "< "
      "1);++j;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test25) {
  std::string code =
      "{int x = 2;for (int i = 1; i <= 3; ++i) {int y = i * 2;int z = y - x;int j = 1;while (j <= 2) {z = "
      "static_cast<int>(z * 1.5);int k = 0;do {int result = (y << 1) - (--x) + (z++ * (k++));switch (result % 5) {case "
      "0:break;case 1:result += x;break;case 2:result -= y;break;case 3:result *= z;break;case 4:result = (result / 2) "
      "+ 1;break;default:break;}} while (k < 1);++j;}}return 0;}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "{signed int x = 2;for (signed int i = 1; i <= 3; ++i) {signed int y = i * 2;signed int z = y - x;signed int j = "
      "1;while (j <= 2) {z = "
      "static_cast<signed int>(z * 1.5);signed int k = 0;do {signed int result = (y << 1) - (--x) + (z++ * "
      "(k++));switch (result % 5) {case "
      "0:{break;}case 1:{result += x;break;}case 2:{result -= y;break;}case 3:{result *= z;break;}case 4:{result = "
      "(result / 2) "
      "+ 1;break;}default:{break;}}} while (k < 1);++j;}}return 0;}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test26) {
  std::string code =
      "condition = (size_a > size_b) ? 1 : 0;result = (condition) ? size_a : size_b;outer = 1;do {"
      "inner = 1;do {inner++;} while (inner <= outer);outer++;} until (outer <= 3);result = (c > 10.0) ? (int)c "
      ": b; repeat 123 {int * z = new int, d = new int, g; fn(alignof (int), sizeof 4, 12, x+x+(x++)-x*22); }";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "condition = (size_a > size_b) ? 1 : 0;result = (condition) ? size_a : size_b;outer = 1;do {"
      "inner = 1;do {inner++;} while (inner <= outer);outer++;} while (!(outer <= 3));result = (c > 10.0) ? (signed "
      "int)c "
      ": b; int strange_name = 123; while(strange_name--){signed int * z = new (signed int), d = new (signed int), g; "
      "fn(alignof (signed int), sizeof 4, 12, x+x+(x++)-x*22); }";

  ASSERT_TRUE(compare(code, printed));
}
