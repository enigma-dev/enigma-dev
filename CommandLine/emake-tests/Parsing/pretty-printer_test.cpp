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
  code = "int x = new(int);";

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
  code = "{c++; g++; {int x=12; {for(int i=12;i!=22;i++){g++; c+=23;int cc = new (int);}}}}";

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
      "int x; int y=12,v=2333,dd=33; for(int g=0;g<22;g++){do{c++; "
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
      "int count =0; if(inc) count++; else if(dec) count--; else count=0; int flag=0; flag= (count)? "
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
      "{int x = 5; const int y = 6; float *(*z)[10] = nullptr ;foo(bar); while(i) "
      "for(int p=12;p/2;p++){while(12){if(1)c++; else c--;}}}";

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
      "if(true) for ( int i = 0; i < 12; i++) k++; else switch (i) { case 1 : {k--;} case 2 :{k += 3;} default "
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
      "int strange_name = (3); while(strange_name--){ int xx =12;  foo(12, fo(12), sizeof( int)); "
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
      "switch (5 * 6 +12) { case 1: {while(1){while((f)) do{s--;  float a = new ( float);}while(!(1));} "
      "return 2; break; }case 2:{ return 3;  int x = sizeof 12; break; } default:{ f-=22; break;}}";

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
      "if (cmd == \" add\") { int title, author; int year;getline(infile, title);getline(infile, "
      "author);addBook(title, author, year);} "
      "else if (cmd == \"remove\") { title;getline(infile, title);if (removeBook(title)) {c*=2;} else {c/=2;}} "
      "else if (cmd == \"find\") {int title;getline(infile, title);if (findBook(title)) {c+=1;} else {c--;}} "
      "else if "
      "(cmd == \"findDetailed\") {int title;getline(infile, title);Book* temp = head;found = false;while (temp "
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
      "++a;int postIncrement = b++;int castResult = static_cast<int>(a) / b; ++k;} while (k < 1);++j;}}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

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
      "{ int x = 2;for ( int i = 1; i <= 3; ++i) { int y = i * 2; int z = y - x; int j = "
      "1;while (j <= 2) {z = "
      "static_cast< int>(z * 1.5); int k = 0;do { int result = (y << 1) - (--x) + (z++ * "
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
      "inner = 1;do {inner++;} while (inner <= outer);outer++;} while (!(outer <= 3));result = (c > 10.0) ? ( "
      "int)c "
      ": b; int strange_name = 123; while(strange_name--){int * z = new (int), d = new (int), g; "
      "fn(alignof (int), sizeof 4, 12, x+x+(x++)-x*22); }";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test27) {
  std::string code =
      "{int number = 5; int result = 1;int i = 1;while (i <= number) {result *= i;i++;}int display = result + 1000; "
      "int temp = display;int divisor = 10000;while (divisor > 0) {int digit = temp / divisor;temp = temp % "
      "divisor;divisor = divisor / 10;putchar('0' + digit);}putchar('c');return 0;}";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test28) {
  std::string code =
      "int *num1 = new int(5);int *num2 = new int(3);int *result = new int;int choice = 2;do {switch(choice) {case "
      "1:*result = *num1 + *num2;break;case 2: *result = *num1 - *num2;break;case 3: *result = *num1 * "
      "*num2;break;case 4:if (*num2 != 0) {int tempResult = static_cast<int>(*num1) / "
      "static_cast<int>(*num2);*result = static_cast<int>(tempResult);} else {*result = 0;}break;case 5:*result = "
      "static_cast<int>(sqrt(static_cast<int>(*num1)));break;default:*result = 0;break;}choice++;} while (choice <= "
      "5);";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      " int *num1 = new ( int)(5);  int *num2 = new ( int)(3);  int *result = new "
      "( "
      "int);  int choice = 2; do {switch(choice) {case "
      "1: {*(result) = *(num1) + *(num2);break;} case 2:{ *(result) = *(num1) - *(num2);break;} case 3:{ *(result) = "
      "*(num1) * "
      "*(num2);break;} case 4: {if (*(num2) != 0) { int tempResult = static_cast< int>(*(num1)) / "
      "static_cast< int>(*(num2));*(result) = static_cast< int>(tempResult);} else {*(result) = "
      "0;}break;}case 5:{*(result) = "
      "static_cast< int>(sqrt(static_cast< int>(*(num1))));break;} default: {*(result) = "
      "0;break;}}choice++; }while "
      "(choice <= "
      "5);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test29) {
  std::string code =
      "res = static_cast<int>(sqrt(static_cast<int>(*(num1))));  res = (int) res+2;  res = int(sizeof 12); res = "
      "int(dynamic_cast<int>(*(num1)));  res = reinterpret_cast<long int> (c++); res = static_cast<long int> (flag? "
      "x:foo(12)); ";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test30) {
  std::string code =
      "int* a = new int[3];  int b [3] = {1,2,3};  a[0] = 1;  a[1] = 2;  a[2] = 3;  int c[3]; a = "
      "new (nullptr) int[]{1, 2, 3, 4, 5}; foo([1,2],[]);";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();
  code =
      "int* a = new (int[3]);  int b [3] = {1,2,3};  a[0] = 1;  a[1] = 2;  a[2] = 3;  int c[3]; a = "
      "new (nullptr) (int[]){1, 2, 3, 4, 5}; foo([1,2],[]);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test31) {
  std::string code =
      "const unsigned int n=12; bool x = (n>12); signed char c='s'; volatile int v=12; const volatile unsigned long "
      "long int f=12; const double l = 123; unsigned int u = 123; int *p = new (int)(22+3); const int * q ; ";

  ParserTester test{code};
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::Visitor v;
  v.VisitCode(*block);
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}
