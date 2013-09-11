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
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.enigma.TargetHandler.TargetSelection;
import org.enigma.backend.EnigmaCallbacks;
import org.enigma.backend.EnigmaCallbacks.OutputHandler;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.enigma.backend.EnigmaSettings;
import org.enigma.backend.EnigmaStruct;
import org.enigma.file.EgmIO;
import org.enigma.messages.Messages;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.ProjectFile;
import org.lateralgm.file.ProjectFile.ResourceHolder;
import org.lateralgm.file.ProjectFile.SingletonResourceHolder;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.ProjectFormatException;
import org.lateralgm.main.FileChooser;
import org.lateralgm.main.FileChooser.FileReader;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.library.LibManager;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.StringArray;

public final class EnigmaCli
	{
	public static final String prog = "enigma"; //$NON-NLS-1$
	public static EnigmaDriver DRIVER;
	
	private static final String targetsDelimiter = ";";
	private static final String targetsItemDelimiter = ":";
	private static final String targetsStart = "-targets=";
	private static final String outputStart = "-output=";

	private EnigmaCli()
		{
		}

	public static void error(String err)
		{
		System.err.println(prog + ": " + err); //$NON-NLS-1$
		System.exit(-1);
		}
	
	private static enum ArgumentType {
		HELP, SYNTAX, TARGETS, INPUTFILE, OUTPUTNAME, UNKNOWN
	}
	
	private static class ArgumentTypeAndValue {
		public final ArgumentType argType;
		public final String value;
		
		public ArgumentTypeAndValue(ArgumentType _argType, String _value) {
			argType = _argType;
			value = _value;
		}
	}
	
	private static List<ArgumentTypeAndValue> parseCommandLineArguments(String[] args) {
		
		List<ArgumentTypeAndValue> result = new ArrayList<ArgumentTypeAndValue>();
		
		for (String str : args) {
			ArgumentType argType;
			String value = str;
			if (str.equals("-h") || str.equals("--help") || str.equals("-?")) {
				argType = ArgumentType.HELP;
				value = "";
			}
			else if (str.equals("-s") || str.equals("--syntax")) {
				argType = ArgumentType.SYNTAX;
				value = "";
			}
			else if (str.startsWith(targetsStart)) {
				argType = ArgumentType.TARGETS;
				value = str.substring(targetsStart.length());
			}
			else if (str.startsWith(outputStart)) {
				argType = ArgumentType.OUTPUTNAME;
				value = str.substring(outputStart.length());
			}
			else if (!str.startsWith("-")) {
				argType = ArgumentType.INPUTFILE;
				value = str;
			}
			else {
				argType = ArgumentType.UNKNOWN;
				value = str;
			}
			result.add(new ArgumentTypeAndValue(argType, value));
		}
		
		return result;
	}
	
	private static boolean containsArgumentType(List<ArgumentTypeAndValue> parsedArguments, ArgumentType argType) {
		for (ArgumentTypeAndValue argTypeAndValue : parsedArguments) {
			if (argTypeAndValue.argType == argType) {
				return true;
			}
		}
		return false;
	}
	
	private static List<String> getValues(List<ArgumentTypeAndValue> parsedArguments, ArgumentType argType) {
		List<String> results = new ArrayList<String>();
		for (ArgumentTypeAndValue argTypeAndValue : parsedArguments) {
			if (argTypeAndValue.argType == argType) {
				results.add(argTypeAndValue.value);
			}
		}
		return results;
	}
	
	private static class TargetsOrError {
		public final Map<String, TargetSelection> targetToValue;
		public final String error; // If empty, no error. Else, error.
		
		public TargetsOrError(Map<String, TargetSelection> _targetToValue, String _error) {
			targetToValue = _targetToValue;
			error = _error;
		}
	}

	private static TargetsOrError parseTargets(String targetsString) {
		
		final Map<String, TargetSelection> result = new HashMap<String, TargetSelection>();
		
		final String[] targets = targetsString.split(targetsDelimiter);
		
		for (String target : targets) {
			final String[] keyValue = target.split(targetsItemDelimiter);
			if (keyValue.length != 2) { // Ignore ill-formed key-value pairs.
				return new TargetsOrError(result, "Ill-formed key-value pair: " + Arrays.toString(keyValue));
			}
			final String key = keyValue[0];
			final String valueString = keyValue[1];
			if (!TargetHandler.targets.containsKey(key)) {
				return new TargetsOrError(result, "API type not found: " + key);
			}
			final List<TargetSelection> targetSelections = TargetHandler.targets.get(key);
			boolean found = false;
			for (TargetSelection targetSelection : targetSelections) {
				if (targetSelection.id.equals(valueString)) {
					found = true;
					result.put(key, targetSelection);
					break;
				}
			}
			if (!found) {
				return new TargetsOrError(result, "API target not found. API type, target: " + key + ", " + valueString);
			}
		}
		
		return new TargetsOrError(result, "");
	}

	public static void main(String[] args) throws ProjectFormatException
		{
		final String helpString =
			"Name:\n" +
			"  enigma.jar\n" +
			"Synopsis:\n" +
			"  java -jar plugins/enigma.jar PATH_TO_GAME [OPTIONS] [" + targetsStart + "API1" + targetsItemDelimiter +
				"TARGET1[" + targetsDelimiter + "API2" + targetsItemDelimiter + "TARGET2]...]\n" +
			"Description:\n" +
			"  Takes a path to a game source as input, and either compiles it or does\n" +
			"  syntax check on it. Default is compilation. Different API targets than\n" +
			"  in the game source can be passed as parameters.\n" +
			"  \n" +
			"  Note that passing in incompatible API targets may cause undefined\n" +
			"  behaviour.\n" +
			"Options:\n" +
			"  -h, --help, -?\n" +
			"    Shows this help text.\n" +
			"  -s, --syntax\n" +
			"    Performs syntax check instead of compilation.\n" +
			"  " + outputStart + "\n" +
			"    Sets the absolute path and name of the output executable when\n" +
			"    compiling. Default is the path and name of the game source file\n" +
			"    without its extension.\n"
			;
		
		if (args.length < 1) {
			System.out.println(helpString);
			error("No arguments.");
		}
		
		final List<ArgumentTypeAndValue> parsedArguments = parseCommandLineArguments(args);
		
		if (containsArgumentType(parsedArguments, ArgumentType.HELP)) {
			System.out.println(helpString);
			System.exit(0);
		}
		if (containsArgumentType(parsedArguments, ArgumentType.UNKNOWN)) {
			System.out.println(helpString);
			error("Unrecognized argument: " + getValues(parsedArguments, ArgumentType.UNKNOWN).get(0));
		}

		String inputFile;
		{
			final List<String> inputFiles = getValues(parsedArguments, ArgumentType.INPUTFILE);
			if (inputFiles.size() != 1) {
				StringBuilder inputFilesString = new StringBuilder("");
				for (String str : inputFiles) {
					inputFilesString.append(str);
					inputFilesString.append("   ");
				}
				error("Wrong number of input files. Number: " + inputFiles.size() + ", input files: " + inputFilesString.toString());
			}
			inputFile = inputFiles.get(0);
		}
		
		final boolean syntax = containsArgumentType(parsedArguments, ArgumentType.SYNTAX);
		final boolean hasTargets = containsArgumentType(parsedArguments, ArgumentType.TARGETS);
		TargetsOrError parsedTargets;
		{
			final List<String> targetsValues = getValues(parsedArguments, ArgumentType.TARGETS);
			StringBuilder targetsStringBuilder = new StringBuilder();
			for (String targetsValue : targetsValues) {
				targetsStringBuilder.append(targetsValue);
				targetsStringBuilder.append(targetsDelimiter);
			}
			String targetsString = targetsStringBuilder.toString();
			parsedTargets = parseTargets(targetsString);
			
			if (hasTargets && !parsedTargets.error.equals("")) {
				error(parsedTargets.error);
			}
		}
		
		String outname = null;
		{
			List<String> outputNames = getValues(parsedArguments, ArgumentType.OUTPUTNAME);
			if (outputNames.size() == 0) {
				outname = null;
			}
			else if (outputNames.size() == 1) {
				outname = outputNames.get(0);
			}
			else {
				error("Output name indicated multiple times.");
			}
		}
		
		int compilation_status = 0;
		try
			{
			initailize(inputFile,null);
			if (syntax)
				syntaxChecker(LGM.currentFile,LGM.root,parsedTargets.targetToValue);
			else
				compilation_status = compile(LGM.currentFile,LGM.root,outname,parsedTargets.targetToValue);
			}
		catch (IOException e)
			{
			error(e.getMessage());
			}
		catch (GmFormatException e)
			{
			error(e.getMessage());
			}

		System.out.println("CLI Done.");
		System.exit(compilation_status); //FIXME: Find out why it doesn't terminate normally
		}
	
	private static void transformEnigmaSettings(EnigmaSettings enigmaSettings, Map<String, TargetSelection> newTargets) {
		if (newTargets != null) {
			for (Map.Entry<String, TargetSelection> target : newTargets.entrySet()) {
				enigmaSettings.targets.put(target.getKey(), target.getValue());
			}
		}
	}

	private static void addResourceHook()
		{
		EgmIO io = new EgmIO();
		FileChooser.readers.add(io);
		FileChooser.writers.add(io);

		Resource.kinds.add(EnigmaSettings.class);
		Resource.kindsByName3.put("EGS",EnigmaSettings.class);
		String name = Messages.getString("EnigmaRunner.RESNODE_NAME"); //$NON-NLS-1$
		Resource.kindNames.put(EnigmaSettings.class,name);
		Resource.kindNamesPlural.put(EnigmaSettings.class,name);

		LGM.currentFile.resMap.put(EnigmaSettings.class,new SingletonResourceHolder<EnigmaSettings>(
				new EnigmaSettings()));
		}

	public static String initailize(String fn, ResNode root) throws IOException, ProjectFormatException
		{
		File file = new File(fn);
		if (!file.exists()) throw new FileNotFoundException(fn);
		LibManager.autoLoad();

		FileChooser.addDefaultReadersAndWriters();
		addResourceHook();

		if (root == null) root = LGM.newRoot();

		URI uri = file.toURI();
		FileReader reader = FileChooser.findReader(uri);

		LGM.currentFile = reader.read(uri.toURL().openStream(),uri,LGM.newRoot());

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
		return DRIVER.libInit(new EnigmaCallbacks(new CliOutputHandler())); //returns String on toolchain failure
		}
	
	public static void syntaxChecker(ProjectFile f, ResNode root) {
		syntaxChecker(f, root, null);
	}

	private static void syntaxChecker(ProjectFile f, ResNode root, Map<String, TargetSelection> newTargets)
		{
		SyntaxError se = syntaxCheck(f,root,newTargets);
		if (se == null || se.absoluteIndex == -1)
			System.out.println("No syntax errors found.");
		else
			error(se.line + ":" + se.position + "::" + se.errorString);
		}
	
	public static SyntaxError syntaxCheck(ProjectFile f, ResNode root) {
		return syntaxCheck(f, root, null);
	}

	private static SyntaxError syntaxCheck(ProjectFile f, ResNode root, Map<String, TargetSelection> newTargets)
		{
		ResourceHolder<EnigmaSettings> rh = f.resMap.get(EnigmaSettings.class);
		EnigmaSettings ess = rh == null ? new EnigmaSettings() : rh.getResource();
		transformEnigmaSettings(ess, newTargets);
		SyntaxError err = ess.commitToDriver(DRIVER); //returns SyntaxError
		if (err.absoluteIndex != -1) return err;

		int scrNum = LGM.currentFile.resMap.getList(Script.class).size();
		String osl[] = new String[scrNum];
		Script isl[] = LGM.currentFile.resMap.getList(Script.class).toArray(new Script[0]);
		for (int i = 0; i < scrNum; i++)
			osl[i] = isl[i].getName();
		StringArray scripts = new StringArray(osl);

		for (Script s : isl)
			if ((err = DRIVER.syntaxCheck(scrNum,scripts,s.getCode())).absoluteIndex != -1) return err;

		return null;
		}
	
	public static int compile(ProjectFile f, ResNode root, String outname) {
		return compile(f, root, null);
	}

	private static int compile(ProjectFile f, ResNode root, String outname, Map<String, TargetSelection> newTargets) throws FileNotFoundException,
			GmFormatException
		{
		ResourceHolder<EnigmaSettings> rh = f.resMap.get(EnigmaSettings.class);
		EnigmaSettings ess = rh == null ? new EnigmaSettings() : rh.getResource();
		transformEnigmaSettings(ess, newTargets);
		ess.commitToDriver(DRIVER); //returns SyntaxError

		//Generate arguments for compile
		EnigmaStruct es = EnigmaWriter.prepareStruct(f,root);
		
		//XXX: Handle custom modes?
		int mode = EnigmaRunner.MODE_COMPILE;

		if (outname == null)
			{
			outname = new File(f.uri).getAbsolutePath();
			String ext = ess.targets.get(TargetHandler.COMPILER).ext;
			outname = outname.substring(0,outname.lastIndexOf('.'));
			if (ext != null) outname = outname + ext;
			}

		//FIXME: Make compliant with spec2
		TargetSelection compiler = ess.targets.get(TargetHandler.COMPILER);
		if (!compiler.outputexe.equals("$tempfile")) //$NON-NLS-1$
			outname = new File(compiler.outputexe).getAbsolutePath();

		System.out.println("Plugin: Delegating to ENIGMA (out of my hands now)");
		return DRIVER.compileEGMf(es,outname,mode);
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
