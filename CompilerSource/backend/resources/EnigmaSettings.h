/*
 * Copyright (C) 2010 Josh Ventura
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

//We'll use DWORDS for speed and simplicity
struct ENIGMAsettings
{
  //Compatibility / Progess options
  int cppStrings; // Defines what language strings are inherited from.                0 = GML,               1 = C++
  int cppOperators; // Defines what language operators ++ and -- are inherited from.  0 = GML,               1 = C++
  int literalHandling; // Determines how literals are treated.                        0 = enigma::variant,   1 = C++-scalar
  int structHandling;  // Defines behavior of the closing brace of struct {}.         0 = Implied semicolon, 1 = ISO C
  
  //Advanced options
  int instanceTypes;   // Defines how to represent instances.                         0 = Integer,  1 = Pointer
  int storageClass;    // Determines how instances are stored in memory.              0 = Map,      1 = List,     2 = Array
};
