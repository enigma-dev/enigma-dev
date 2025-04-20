/*
 * Copyright (C) 2013 Robert B. Colton
 *
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

#ifndef ENIGMA_SHADER_H
#define ENIGMA_SHADER_H

#include "backend/JavaStruct.h"

namespace deprecated {
namespace JavaStruct {

struct Shader
{
	String name;
	int id;

	String vertex;
	String fragment;
	String type;
	boolean precompile;
};

}  // namespace JavaStruct
}  // namespace deprecated

#endif
