/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public interface EnigmaDriver extends Library
	{
	public static class SyntaxError extends Structure
		{
		public String errorString;
		public int line, position;
		public int absoluteIndex;
		}

	public String libInit(EnigmaCallbacks ef);

	public SyntaxError definitionsModified(String wscode, String yaml);

	/** @param scriptNames (StringArray) */
	public SyntaxError syntaxCheck(int scriptCount, Pointer scriptNames, String code);

	public int compileEGMf(EnigmaStruct es, String outname, int mode);

	// Keyword and Function Names //

	/** Returns the name of the first resource on the list, or null if there is no first element. */
	public String first_available_resource();

	/** Returns the name of the next resource on the list, or null if there is no first element. */
	public String next_available_resource();

	/** Returns whether the resource is nothing but a global variable. */
	public boolean resource_isGlobal();

	/** Returns whether the resource can be used as a typename. */
	public boolean resource_isTypeName();

	/** Returns whether the resource can be called as a function */
	public boolean resource_isFunction();

	/** Returns the minimum number of arguments to the function. */
	public int resource_argCountMin();

	/** Returns the maximum number of arguments to the function. */
	public int resource_argCountMax();

	/** Returns the number of times the function was declared in the parsed sources. */
	public int resource_overloadCount();

	/**
	 * Returns a simple string of parameters and defaults that would
	 * serve as the prototype of this function. Includes braces.
	 * WARNING: Returned string invalidated upon next call to definitionsModified().
	 * @param i The number of the overload.
	 */
	public String resource_parameters(int i);
	}
