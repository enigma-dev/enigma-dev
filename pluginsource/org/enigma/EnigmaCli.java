/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.io.File;
import java.io.FileNotFoundException;

import org.enigma.backend.EnigmaCallbacks;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaSettings;
import org.enigma.backend.EnigmaStruct;
import org.enigma.backend.EnigmaCallbacks.OutputHandler;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFileReader;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.main.LGM;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;

public class EnigmaCli
	{
	public static final String prog = "enigma"; //$NON-NLS-1$
	public static EnigmaDriver DRIVER;

	public static void error(String err)
		{
		System.err.println(prog + ": " + err); //$NON-NLS-1$
		System.exit(-1);
		}

	public static void main(String[] args)
		{
		if (args.length != 1) error("no input file");

		if (args[0].equals("-?") || args[0].equals("--help"))
			{
			System.out.println("Fuck you too");
			System.exit(-1);
			}

		try
			{
			compile(args[0]);
			}
		catch (FileNotFoundException e)
			{
			error(e.getMessage());
			}
		catch (GmFormatException e)
			{
			error(e.getMessage());
			}
		}

	public static void compile(String fn) throws FileNotFoundException,GmFormatException
		{
		compile(fn,null);
		}

	public static void compile(File file) throws FileNotFoundException,GmFormatException
		{
		compile(file.getPath(),null);
		}

	public static void compile(String fn, ResNode root) throws FileNotFoundException,
			GmFormatException
		{
		if (!new File(fn).exists()) throw new FileNotFoundException(fn);
		if (root == null) root = new ResNode("Root",(byte) 0,null,null); //$NON-NLS-1$;
		GmFile f = GmFileReader.readGmFile(fn,root);
		try
			{
			attemptLib();
			}
		catch (UnsatisfiedLinkError e)
			{
			error("Unable to communicate with the library,\n"
					+ "either because it could not be found or uses methods different from those expected.\n"
					+ "The exact error is:\n" + e.getMessage());
			}
		DRIVER.libInit(new EnigmaCallbacks(new CliOutputHandler())); //returns String on toolchain failure
		EnigmaSettings ess = new EnigmaSettings();
		//TODO: Handle custom settings
		ess.commitToDriver(DRIVER); //returns SyntaxError

		//Generate arguments for compile
		EnigmaStruct es = EnigmaWriter.prepareStruct(f,root);

		//XXX: Handle custom modes?
		int mode = EnigmaRunner.MODE_COMPILE;

		//TODO: Handle custom outname
		//FIXME: Make compliant with spec2
		File outname = new File(fn.substring(0,fn.lastIndexOf('.')) + ess.selPlatform.ext);
		if (!ess.selCompiler.outputexe.equals("$tempfile")) //$NON-NLS-1$
			outname = new File(ess.selCompiler.outputexe);

		DRIVER.compileEGMf(es, outname.getAbsolutePath(), mode);
		}

	private static UnsatisfiedLinkError attemptLib()
		{
		try
			{
			String lib = "compileEGMf"; //$NON-NLS-1$
			NativeLibrary.addSearchPath(lib,"."); //$NON-NLS-1$
			NativeLibrary.addSearchPath(lib,LGM.workDir.getParent());
			DRIVER = (EnigmaDriver) Native.loadLibrary(lib,EnigmaDriver.class);
			return null;
			}
		catch (UnsatisfiedLinkError e)
			{
			return e;
			}
		}

	public static class CliOutputHandler implements OutputHandler
		{
		@Override
		public void append(String s)
			{
			System.out.print(s);
			}

		@Override
		public void clear()
			{
			//clearing the terminal doesn't make sense
			}

		@Override
		public void open()
			{
			//terminal already open
			}

		@Override
		public void progress(int i)
			{
			//progress/tip not handled at this time
			}

		@Override
		public void tip(String s)
			{
			//progress/tip not handled at this time
			}
		}
	}
