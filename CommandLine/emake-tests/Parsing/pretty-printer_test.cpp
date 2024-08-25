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

  if (ind != printed.size() && printed[ind] != ' ') {
    return false;
  }
  return true;
}

TEST(PrinterTest, test1) {
  std::string code = "{{{}}}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test2) {
  std::string code = "int x = new int";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "int x = new(int);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test3) {
  std::string code = "{c++; g++; {int x=12; {for(int i=12;i!=22;i++){g++; c+=23; int cc = new int}}}}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "{c++; g++; {int x=12; {for(int i=12;i!=22;i++){g++; c+=23;int cc = new (int);}}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test4) {
  std::string code = "int x;  int y=12,v=2333,dd=33; for(int g=0;g<22;g++){do{c++; v=cc+12+fn(12,22);}while(true)}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "int x; int y=12,v=2333,dd=33; for(int g=0;g<22;g++){do{c++; "
      "v=cc+12+fn(12,22);}while(true);}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test5) {
  std::string code = "while(1){while(1){while(0) c++;}}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test6) {
  std::string code = "int count =0; if(inc) count++; else if(dec) count--; else count=0 int flag=0; flag= (count)? 1:0";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "{int x = 5; const int y = 6; float *(*z)[10] = nullptr ;foo(bar); while(i) "
      "for(int p=12;p/2;p++){while(12){if(1)c++; else c--;}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test8) {
  std::string code =
      "if(true) for (int i = 0; i < 12; i++) k++ else switch (i) { case 1 : k-- case 2 :k += 3 default : k = 0 }";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "if(true) for ( int i = 0; i < 12; i++) k++; else switch (int((i))) { case 1 : {k--;} case 2 :{k += 3;} default "
      ":{ k = "
      "0;} }";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test9) {
  std::string code = "repeat(3){int xx =12;  foo(12, fo(12), sizeof(int)) while((2)){c-- c++ c*=2}}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "int strange_name = (3); while(strange_name--){ int xx =12;  foo(12, fo(12), sizeof( int)); "
      "while((2)){c--; "
      "c++; c*=2;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test10) {
  std::string code = "alignof(const volatile unsigned long long *)";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "alignof(const volatile unsigned long long int*);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test11) {
  std::string code =
      "switch (5 * 6 +12) { case 1:while(1){while((f)) do{s-- float a = new float}until(1)} return 2 break case 2: "
      "return 3 int x = sizeof 12 break default: f-=22 break}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "switch (int((5 * 6 +12))) { case 1: {while(1){while((f)) do{s--;  float a = new ( float);}while(!(1));} "
      "return 2; break; }case 2:{ return 3;  int x = sizeof 12; break; } default:{ f-=22; break;}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test12) {
  std::string code =
      "if (head == nullptr) return false;if (head->data == val) {Node* temp = head; head = head->next; delete temp; "
      "return true;}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test13) {
  std::string code =
      "Node* current = head;while (current->next != nullptr && current->next->data != val) {current = "
      "current->next;}if (current->next == nullptr) return false;Node* temp = current->next; current->next = "
      "current->next->next; delete temp; return true;";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test14) {
  std::string code =
      "{Node* temp = head;while (temp != nullptr) {if (temp->data == val) return true;temp = temp->next;}return "
      "false;}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test15) {
  std::string code =
      "int count = 0;Node* temp = head;while (temp != nullptr) {count++;temp = temp->next;}return count;";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test16) {
  std::string code =
      "{Book* current = head;Book* next;while (current != nullptr) {next = current->next;delete current;current = "
      "next;}head = nullptr;}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test17) {
  std::string code =
      "if (head == nullptr) {head = newBook;} else {Book* temp = head;while (temp->next != nullptr) {temp = "
      "temp->next;}temp->next = newBook;}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test18) {
  std::string code =
      "{if (head == nullptr) return;bool swapped;do {swapped = false;Book* temp = head;while (temp->next != nullptr) "
      "{if (temp->title > temp->next->title) {swap(temp, temp->next);swapped = true;}temp = temp->next;}} while "
      "(swapped);}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test19) {
  std::string code =
      "{if (head == nullptr) return false;if (head->title == title) {Book* temp = head;head = head->next;delete "
      "temp;return true;}Book* current = head;while (current->next != nullptr && current->next->title != title) "
      "{current = current->next;}if (current->next == nullptr) return false;Book* temp = current->next;current->next = "
      "current->next->next;delete temp;return true;}";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithoutCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithoutCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithoutCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "do {signed char j = '1';while (j <= '2') {for (signed char k = 'a'; k <= 'b'; ++k) {signed char l = 'X';do "
      "{switch (int((i))) {case "
      "'A':{break;}case 'B':{break;}default:{break;}}condition = (i == 'A' && j == '1') || (k == 'b' && l == 'Y');if "
      "(condition) {c++;}++l;} while (l <= 'Y');k--;k++;}++j;}++i;} while (i <= 'B');";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test24) {
  std::string code =
      "int outerLoopLimit = 3;int innerLoopLimit = 2;for (int i = 0; i < outerLoopLimit; ++i) {int j = 0;while (j < "
      "innerLoopLimit) {int k = 0;do {int a = 5;int b = 3;int result;result = a + b;result = a * b;int preIncrement = "
      "++a;int postIncrement = b++;int castResult = static_cast<int>(a) / b; ++k;} while (k < 1);++j;}}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test25) {
  std::string code =
      "{int x = 2;for (int i = 1; i <= 3; ++i) {int y = i * 2;int z = y - x;int j = 1;while (j <= 2) {z = "
      "static_cast<int>(z * 1.5);int k = 0;do {int result = (y << 1) - (--x) + (z++ * (k++));switch (result % 5) {case "
      "0:break;case 1:result += x;break;case 2:result -= y;break;case 3:result *= z;break;case 4:result = (result / 2) "
      "+ 1;break;default:break;}} while (k < 1);++j;}}return 0;}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "{ int x = 2;for ( int i = 1; i <= 3; ++i) { int y = i * 2; int z = y - x; int j = "
      "1;while (j <= 2) {z = "
      "static_cast< int>(z * 1.5); int k = 0;do { int result = (y << 1) - (--x) + (z++ * "
      "(k++));switch (int((result % 5))) {case "
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

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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
      "divisor;divisor = divisor / 10;putchar('0' + digit);}putchar(   'c');return 0;}";

  ParserTester test = ParserTester::CreateWithoutCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      " int *num1 = new ( int)(5);  int *num2 = new ( int)(3);  int *result = new "
      "( "
      "int);  int choice = 2; do {switch(int((choice))) {case "
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

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test30) {
  std::string code =
      "int* a = new int[3];  int b [3] = {1,2,3};  a[0] = 1;  a[1] = 2;  a[2] = 3;  int c[3]; a = "
      "new (nullptr) int[]{1, 2, 3, 4, 5}; foo([1,2],[]);";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
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

  ParserTester test = ParserTester::CreateWithoutCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test32) {
  std::string code =
      "int x [2]; int* y = new int[2]; int z[2] = {1,2}; int* p = new int(12); float* q = new float(2+3); int* r = new "
      "int(sizeof 12); bool* b = new bool; char* c = new char; double* d = new double;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "int x [2]; int* y = new (int[2]); int z[2] = {1,2}; int* p = new (int)(12); float* q = new (float)(2+3); int* r "
      "= new "
      "(int)(sizeof 12); bool* b = new (bool); signed char* c = new (signed char); double* d = new (double);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test33) {
  std::string code =
      "int result = 0;for (int i = 0; i < n; ++i) {for (int j = 0; j < n; ++j) {int k = 0;while (k < n) {int m = 0;do "
      "{int value = arr[i][j][k];value = -value;value = value * 2 + 3; double sqrtValue = "
      "static_cast<double>(value);sqrtValue = sqrt(sqrtValue);switch (m) {case 0:result += (value << 1) - 3;result = "
      "~result;break;case 1:result += static_cast<int>(sqrtValue) * 2;result = result ^ 1;break;default:result += "
      "(value & 2) | 1;result = ++result;break;}++m;} while (m < 3);++k;}}}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "int result = 0; for (int i = 0; i < n; ++i) {for (int j = 0; j < n; ++j) {int k = 0; while (k < n) {int m = "
      "0;do "
      "{int value = arr[i][j][k];value = -value; value = value * 2 + 3; double sqrtValue = "
      "static_cast<double>(value); sqrtValue = sqrt(sqrtValue); switch (int((m))) {case 0:{result += (value << 1) - "
      "3;result "
      "= "
      "~result;break;} case 1:{result += static_cast<int>(sqrtValue) * 2;result = result ^ 1;break;}default:{result "
      "+= "
      "(value & 2) | 1;result = ++result;break;}}++m;} while (m < 3);++k;}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test34) {
  std::string code =
      "int *(*(*a)[10][12])[15];  s = (x ? y : z ? a : (z[5](6))); ss = sizeof...(ident); int x = sizeof(const "
      "volatile unsigned long long int **(*)[10]); int size = alignof(const volatile unsigned long long int*);";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test35) {
  std::string code =
      "s = new (nullptr) int[]{1, 2, 3, 4, 5}; a=::new int[][15]{1, 2, 3, 4, 5}; b=::new (nullptr) (int *(**)[10])(1, "
      "2, 3, 4, 5); c=new (int *(**)[10]); d=new (nullptr) (int[]){.x=1, .y=2, .z=3, .u=4, .v=5}; e=new (nullptr) "
      "int[]{args...};";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "s = new (nullptr) (int[]){1, 2, 3, 4, 5}; a=::new (int[][15]){1, 2, 3, 4, 5}; b=::new (nullptr) (int "
      "*(**)[10])(1, 2, 3, 4, 5); c=new (int *(**)[10]); d=new (nullptr) (int[]){.x=1, .y=2, .z=3, .u=4, .v=5}; e=new "
      "(nullptr) (int[]){args...};";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test36) {
  std::string code = "delete x; ::delete x; delete[] x; ::delete[] x;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test37) {
  std::string code =
      "switch (5 * 6) { case 1: return 2; break 13; case 2: return 3; break; default: break;} switch (1) { case 1: "
      "return 2; default: return \"test\";} switch (1) { default: continue 12;} switch (1) { default: continue 12} "
      "switch (1) { default: delete [] x; return \"new test\";} switch (1) { default: new (nullptr) int[]{1, 2, 3, 4, "
      "5}; return \"new test\";}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "switch (int((5 * 6))) { case 1:{ return 2; break 13; }case 2: {return 3; break; } default:{ break;}} switch "
      "(int((1))) { case "
      "1: "
      "{return 2; }default:{ return \"test\";}} switch (int((1))) { default:{ continue 12;}} switch (int((1))) { "
      "default: {continue "
      "12;}} "
      "switch (int((1))) { default:{ delete [] x; return \"new test\";}} switch (int((1))) { default: {new (nullptr) "
      "(int[]){1, 2, "
      "3, 4, "
      "5}; return \"new test\";}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test38) {
  std::string code = "{ int x = 5 const int y = 6 float *(*z)[10] = nullptr foo(bar) } {{{}}}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "{ int x = 5; const int y = 6; float *(*z)[10] = nullptr; foo(bar); } {{{}}}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test39) {
  std::string code =
      "if(3>2) j++; else --k; if k k++; if (true) { return 1; } else { return 2; } if (false) for(int i=0;i<12;i++) "
      "{k++;} else switch(i){ case 1 : k--; case 2 : k+=3; default : k=0; }";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "if(3>2) j++; else --k; if (k) k++; if (true) { return 1; } else { return 2; } if (false) for(int "
      "i=0;i<12;i++) "
      "{k++;} else switch(int((i))){ case 1 :{ k--;} case 2 : {k+=3; }default :{ k=0;} }";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test40) {
  std::string code = "int((*x)[5] + 6); int(*(*a)[10]) = nullptr;  int(*(*a)[10] + b); int(*(*(*(*x + 4))));";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "int((*(x))[5] + 6); int *(*a)[10] = nullptr;  int(*((*(a))[10]) + b); int(*(*(*(*(x) + 4))));";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test41) {
  std::string code =
      "for (int i = 0; i < 5; i++) {} for int i = 0, j=1; i >= 12; --i {} for int i = 0, j=1, k=133 ;i != 12; --i {j "
      "++;} for int i = 0, j=1, k=133, w=-99 ;w % 22; j++ {if(l) break; else continue;} for int i = 0, j=1, k=133, "
      "w=44, u=-77 ;w % 22; w++ {f++; if(i) x = new int; else delete y;} for int(i = 5); i < 5; i++ {} for (int)(i = "
      "0); i < 5; i++ {} for static_cast<int>(i = 10); i / 3; i-- {k++; return;} for static_cast<int>(i = 10, j=12); i "
      "mod 3; i-- {k--; return 12;} for dynamic_cast<int>(i = 10); i / 3; i-- {} for dynamic_cast<int>(i); i / 3; i-- "
      "{} for dynamic_cast<int>((i)); i / 3; i-- {} for const_cast<int>(i = 10); i / 3; i-- {} for const_cast<int>(i); "
      "i / 3; i-- {} for const_cast<int>((i)); i / 3; i-- {} for reinterpret_cast<int>(i = 10); i / 3; i-- {} for "
      "reinterpret_cast<int>(i); i / 3; i-- {} for reinterpret_cast<int>((i)); i / 3; i-- {}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "for (int i = 0; i < 5; i++) {} for( int i = 0, j=1; i >= 12; --i) {} for (int i = 0, j=1, k=133 ;i != 12; --i) "
      "{j "
      "++;} for( int i = 0, j=1, k=133, w=-99 ;w % 22; j++) {if(l) break; else continue;} for (int i = 0, j=1, k=133, "
      "w=44, u=-77 ;w % 22; w++) {f++; if(i) x = new (int); else delete y;} for (int(i = 5); i < 5; i++ ){} for( "
      "(int)(i = "
      "0); i < 5; i++ ){} for( static_cast<int>(i = 10); i / 3; i-- ){k++; return;} for (static_cast<int>(i = 10, "
      "j=12); i "
      "mod 3; i--) {k--; return 12;} for( dynamic_cast<int>(i = 10); i / 3; i--) {} for (dynamic_cast<int>(i); i / 3; "
      "i--) "
      "{} for (dynamic_cast<int>((i)); i / 3; i--) {} for (const_cast<int>(i = 10); i / 3; i--) {} for "
      "(const_cast<int>(i); "
      "i / 3; i--) {} for( const_cast<int>((i)); i / 3; i-- ){} for (reinterpret_cast<int>(i = 10); i / 3; i--) {} for "
      "("
      "reinterpret_cast<int>(i); i / 3; i--) {} for (reinterpret_cast<int>((i)); i / 3; i--) {}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test42) {
  std::string code = "while(i==1){i++} until(i==1) {i++} repeat(4){i++} ";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "while(i==1){i++;} while(!(i==1)) {i++;} int strange_name =(4) ; while(strange_name--){i++;}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test43) {
  std::string code = "do{c++}while(i) do c++ until i ";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "do{c++;}while(i); do{ c++;} while(!(i)); ";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test44) {
  std::string code =
      "a = [1,2,3] a = [1] a = [2+3] a = [x] a = [2+3, 4*6, 5/2] a = [(12)] a = [x++] a = [--x] a = [foo(12)] a = "
      "[sizeof 12] a = [reinterpret_cast<int>(i)] x++; if(true) l--";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code =
      "a = [1,2,3]; a = [1]; a = [2+3]; a = [x]; a = [2+3, 4*6, 5/2]; a = [(12)]; a = [x++]; a = [--x]; a = [foo(12)]; "
      "a = "
      "[sizeof 12]; a = [reinterpret_cast<int>(i)]; x++; if(true) l--;";

  ASSERT_TRUE(compare(code, printed));
}

// TEST(PrinterTest, test45) {
//   std::string code =
//       "char y = '\\n'; y='\\t'; y='\\v'; y = '\\b'; y='\\r'; y='\\f'; y = '\\a'; y='\\t'; y='\\v'; y='\\';y='\\?';";

//   ParserTester test=ParserTester::CreateWithCpp(code);
//   auto node = test->ParseCode();

//   ASSERT_EQ(node->type, AST::NodeType::BLOCK);
//   auto *block = node->As<AST::CodeBlock>();

//   AST::CppPrettyPrinter v;
//   ASSERT_TRUE(v.VisitCode(*block));
//   std::string printed = v.GetPrintedCode();
//   code =
//       "signed char y  = '\\n'; y='\\t'; y='\\v'; y = '\\b'; y='\\r'; y='\\f'; y = '\\a'; y='\\t'; y='\\v'; y='\\'; y='\\?'; ";

//   ASSERT_TRUE(compare(code, printed));
// }

TEST(PrinterTest, test46) {
  std::string code = "if (x * 2)> s(12) --l";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if ((x * 2)> s(12)) --l;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test47) {
  std::string code = "if c++ --l";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if (c++) --l;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test48) {
  std::string code = "foo(12)--x;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "foo(12); --x;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test49) {
  std::string code = "if((x * 2)> s(12)) --l;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test50) {
  std::string code = "if (x * 2)> s(12) --l";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if ((x * 2)> s(12)) --l;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test51) {
  std::string code = "if a.b --l";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if (enigma::varaccess_b(a) --) l;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test52) {
  std::string code = "if a->b --l";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if (a->b --) l;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test53) {
  std::string code = "if(1);else x++";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if(1); else x++;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test54) {
  std::string code = "y = x=> x+10;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "y = [&] (auto x) {x+10;};";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test55) {
  std::string code = "y = (x)=> x+10;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "y = [&](auto x) {x+10;};";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test56) {
  std::string code = "y = ()=> x+10;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "y = [&]() {x+10;};";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test57) {
  std::string code = "y = (x,c,z)=> v= c++ + ++x;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "y = [&](auto x, auto c, auto z) { v= c++ + ++x; };";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test58) {
  std::string code = "y = (x,c,z)=> {v= c++ + ++x;}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "y = [&](auto x, auto c, auto z) { v= c++ + ++x; };";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test59) {
  std::string code = "if (power_up_collected) { with (obj_enemy) { image_blend = c_red; } }";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test60) {
  std::string code = "if (power_up_collected)  with (obj_enemy)  image_blend = c_red; ";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test61) {
  std::string code = "{if(y<(room_height / 2-350));}";

  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test62) {
  std::string code = "global.x=1";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "enigma::varaccess_x(int(global))=1;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test63) {
  std::string code = "var gg=1;";

  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test64) {
  std::string code = "instance_create((choose(x+0, x+96, x+192, x+288)), 576, obj_blue)";

  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "instance_create((choose((enigma::varargs(), x + 0, x + 96, x + 192, x + 288))), 576, obj_blue);";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test65) {
  std::string code = "int global foo;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = ";";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test66) {
  std::string code = "global int foo;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = ";";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test67) {
  std::string code = "long long foo;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "long long int foo;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test68) {
  std::string code = "long foo;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "long int foo;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test69) {
  std::string code = "short foo;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "short int foo;";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test70) {
  std::string code = "if not grabbed=true then {x=mouse_x-xpart;y=mouse_y-ypart;friction=0;}else{friction=0.5;}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  code = "if (!(grabbed=true)) {x=mouse_x-xpart;y=mouse_y-ypart;friction=0;}else{friction=0.5;}";

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test71) {
  std::string code = "for (b = 1; b < 11; b += 1) {game_field[a, b] = -1;}";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test72) {
  std::string code = "pf_phi_shield ^= 1;";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}

TEST(PrinterTest, test73) {
  std::string code = "for(int i=0;i<n;i++);";

  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();

  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();

  ASSERT_TRUE(compare(code, printed));
}
