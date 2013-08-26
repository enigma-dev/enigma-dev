/*
 * Copyright (C) 2013 Robert B. Colton
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef _SHADER__H
#define _SHADER__H

#include "backend/JavaStruct.h"

struct Shader
{
	String name;
	int id;

	String vertex;
	String fragment;
	String type;
	boolean precompile;
};

#endif
