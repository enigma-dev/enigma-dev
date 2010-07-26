package org.enigma.backend;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public class EnigmaDriver
	{
	static
		{
		String lib = "compileEGMf";
		NativeLibrary.addSearchPath(lib,".");
		Native.register(lib);
		}

	public static class SyntaxError extends Structure
		{
		public String errorString;
		public int line, position;
		public int absoluteIndex;
		}

	public static native int libInit(EnigmaCallbacks ef);

	public static native int gccDefinePath(String path);

	public static native SyntaxError whitespaceModified(String wscode);

	/** @param scriptNames (StringArray) */
	public static native SyntaxError syntaxCheck(int scriptCount, Pointer scriptNames, String code);

	public static native int compileEGMf(EnigmaStruct es, String outname, int mode);

	/** Returns the name of the first resource on the list, or null if there is no first element. */
	public static native String first_available_resource();

	/** Returns the name of the next resource on the list, or null if there is no first element. */
	public static native String next_available_resource();

	/** Returns the minimum number of arguments to the function; the names cannot be fetched. */
	public static native int resource_argCountMin();

	/** Returns the maximum number of arguments to the function; the names cannot be fetched. */
	public static native int resource_argCountMax();

	/** Returns whether the resource can be called as a function */
	public static native boolean resource_isFunction();

	/** Returns whether the resource is nothing but a global variable. */
	public static native boolean resource_isGlobal();

	/** Returns whether the resource can be used as a typename. */
	public static native boolean resource_isTypeName();
	}
