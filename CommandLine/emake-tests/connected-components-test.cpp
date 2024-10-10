#include "algorithm/connected-components.h"

#include <gtest/gtest.h>

using namespace egm::algorithm;

Coords A { 0, 0, 0 };
Coords B { 0, 0, 1 };
Coords C { 0, 1, 0 };
Coords D { 0, 1, 1 };
Coords E { 1, 0, 0 };
Coords F { 1, 0, 1 };
Coords G { 1, 1, 0 };
Coords H { 1, 1, 1 };

#include <iostream>
using namespace std;

TEST(ConnectedComponentsTest, OneComponentConnection) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 2);
}

TEST(ConnectedComponentsTest, BasicComponentConnections) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  comp.Connect(B, C);
  comp.Connect(C, D);
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 4);
  ASSERT_EQ(comp.components.begin()->second, (std::set<Coords> {A, B, C, D}));
}

TEST(ConnectedComponentsTest, BridgingComponentConnections) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  comp.Connect(B, C);
  
  comp.Connect(D, E);
  comp.Connect(E, F);
  
  comp.Connect(C, D);
  
  cout << "Components (" << comp.components.size() << "):" << endl;
  for (const auto &s : comp.components) {
    cout << " - Component " << s.first << " (" << s.second.size() << " items)\n";
    for (const Coords &c : s.second) {
      cout << "    * (" << c.x << ", " << c.y << ", " << c.z << ")" << endl;
    }
  }
  
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 6);
  ASSERT_EQ(comp.components.begin()->second, (std::set<Coords> {A, B, C, D, E, F}));
}

TEST(ConnectedComponentsTest, DoubleConnectComponentsAA) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  comp.Connect(A, B);
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 2);
}

TEST(ConnectedComponentsTest, DoubleConnectComponentsAB) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  comp.Connect(B, A);
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 2);
}

TEST(ConnectedComponentsTest, TripleConnectComponents) {
  ConnectedComponents comp;
  comp.Connect(A, B);
  comp.Connect(B, A);
  comp.Connect(A, B);
  ASSERT_EQ(comp.components.size(), 1);
  ASSERT_EQ(comp.components.begin()->second.size(), 2);
}
