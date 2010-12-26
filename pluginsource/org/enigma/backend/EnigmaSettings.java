/*
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.backend;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

import org.enigma.EYamlParser;
import org.enigma.EYamlParser.YamlNode;
import org.lateralgm.main.LGM;

public class EnigmaSettings
	{
	//Compatibility / Progress options
	public int cppStrings = 0; // Defines what language strings are inherited from.               0 = GML,               1 = C
	public int cppOperators = 0; // Defines what language operators ++ and -- are inherited from. 0 = GML,               1 = C
	public int cppEquals = 0; // Defines whether = should be exclusively treated as a setter.     0 = GML (= or ==)      1 = C (= only)
	public int literalHandling = 0; // Determines how literals are treated.                       0 = enigma::variant,   1 = C-scalar
	public int structHandling = 0; // Defines behavior of the closing brace of struct {}.         0 = Implied semicolon, 1 = ISO C

	//Advanced options
	public int instanceTypes = 0; // Defines how to represent instances.           0 = Integer, 1 = Pointer
	public int storageClass = 0; // Determines how instances are stored in memory. 0 = Map,     1 = List,    2 = Array

	public String definitions = "", globalLocals = "";
	public String initialization = "", cleanup = "";

	public TargetSelection targetPlatform, targetGraphics, targetAudio, targetCollision;
	static final List<TargetSelection> tPlatforms, tGraphics, tAudios, tCollisions;

	static
		{
		tPlatforms = findTargets("Platforms");
		tGraphics = findTargets("Graphics_Systems");
		tAudios = findTargets("Audio_Systems");
		tCollisions = findTargets("Collision_Systems");
		}

	public static class TargetSelection
		{
		public String name, id; //mandatory
		public Set<String> depends; //mandatory, non-empty
		public Set<String> defaultOn; //optional
		public String desc, auth, ext; //optional

		public String toString()
			{
			return name;
			}
		}

	//target is one of ""Platforms","Audio_Systems","Graphics_Systems","Collision_Systems"
	private static List<TargetSelection> findTargets(String target)
		{
		ArrayList<TargetSelection> targets = new ArrayList<TargetSelection>();

		File f = new File(LGM.workDir.getParentFile(),"ENIGMAsystem");
		f = new File(f,"SHELL");
		f = new File(f,target);
		File files[] = f.listFiles();
		for (File dir : files)
			{
			if (!dir.isDirectory()) continue;
			//technically this could stand to be a .properties file, rather than e-yaml
			File prop = new File(dir,"About.ey");
			try
				{
				Set<String> depends = new HashSet<String>();
				Set<String> defaultOn = new HashSet<String>();
				YamlNode node;

				if (target.equals("Platforms"))
					{
					node = EYamlParser.parse(new Scanner(prop));
					String norm = normalize(node.getMC("Build-Platforms"));
					if (norm.isEmpty()) continue;
					for (String s : norm.split(","))
						if (!s.isEmpty()) depends.add(s);
					}
				else if (target.equals("Collision_Systems"))
					{
					node = EYamlParser.parse(new Scanner(prop));
					depends.add("all");
					}
				else
					{
					String[] configs = new File(dir,"Config").list();
					if (configs == null) continue;
					for (String conf : configs)
						if (conf.endsWith(".ey")) depends.add(normalize(conf.substring(0,conf.length() - 3)));
					if (depends.isEmpty()) continue;
					node = EYamlParser.parse(new Scanner(prop));
					}

				String norm = normalize(node.getMC("Represents",""));
				for (String s : norm.split(","))
					if (!s.isEmpty()) defaultOn.add(s);

				TargetSelection ps = new TargetSelection();
				ps.name = node.getMC("Name");
				ps.id = node.getMC("Identifier");
				ps.depends = depends;
				ps.defaultOn = defaultOn;
				ps.desc = node.getMC("Description",null);
				ps.auth = node.getMC("Author",null);
				ps.ext = node.getMC("Build-Extension",null);
				targets.add(ps);
				}
			catch (FileNotFoundException e)
				{
				//yaml file missing, skip to next file
				}
			catch (IndexOutOfBoundsException e)
				{
				//insufficient yaml, skip to next file
				}
			}
		//if not empty, we may safely assume that the first one is the default selection,
		//or technically, that any of them is the default. The user can/will change it in UI.
		return targets;
		}

	//get rid of any "[]-_ " (and space), and convert to lowercase.
	private static String normalize(String s)
		{
		return s.toLowerCase().replaceAll("[\\[\\]\\-\\s_]","");
		}

	public EnigmaSettings()
		{
		this(true);
		}

	private EnigmaSettings(boolean load)
		{
		if (!load) return;

		loadDefinitions();

		targetPlatform = findDefault(tPlatforms,getOS());
		if (targetPlatform != null)
			{
			targetGraphics = findDefault(tGraphics,targetPlatform.id);
			targetAudio = findDefault(tAudios,targetPlatform.id);
			targetCollision = findDefault(tCollisions,targetPlatform.id);
			}
		}

	private static String getOS()
		{
		String os = normalize(System.getProperty("os.name"));
		if (os.contains("nux") || os.contains("nix")) return "linux";
		if (os.contains("win")) return "windows";
		if (os.contains("mac")) return "macosx";
		return os;
		}

	private static TargetSelection findDefault(List<TargetSelection> tsl, String depends)
		{
		depends = depends.toLowerCase();
		for (TargetSelection ts : tsl)
			if ((ts.depends.contains("all") || ts.depends.contains(depends))
					&& (ts.defaultOn.contains("all") || ts.defaultOn.contains(depends))) return ts;
		//if none are marked as default, just use the first one
		for (TargetSelection ts : tsl)
			if (ts.depends.contains("all") || ts.depends.contains(depends)
					&& !ts.defaultOn.contains("none")) return ts;
		return null;
		}

	private List<TargetSelection> getTargetArray(List<TargetSelection> itsl)
		{
		List<TargetSelection> otsl = new ArrayList<TargetSelection>();
		String depends;
		if (itsl == tPlatforms)
			depends = getOS();
		else if (itsl == tCollisions)
			return itsl;
		else
			{
			if (targetPlatform == null) return otsl;
			depends = targetPlatform.id;
			}

		for (TargetSelection ts : itsl)
			if (ts.depends.contains(depends.toLowerCase())) otsl.add(ts);

		return otsl;
		}

	public Object[] getTargetPlatformsArray()
		{
		return getTargetArray(tPlatforms).toArray();
		}

	public Object[] getTargetGraphicsArray()
		{
		return getTargetArray(tGraphics).toArray();
		}

	public Object[] getTargetAudiosArray()
		{
		return getTargetArray(tAudios).toArray();
		}

	public Object[] getTargetCollisionsArray()
		{
		return getTargetArray(tCollisions).toArray();
		}

	void loadDefinitions()
		{
		definitions = fileToString(new File(LGM.workDir.getParentFile(),"definitions.h"));
		}

	public void saveDefinitions()
		{
		writeString(new File(LGM.workDir.getParentFile(),"definitions.h"),definitions);
		}

	String fileToString(File f)
		{
		StringBuffer sb = new StringBuffer(1024);
		try
			{
			FileReader in = new FileReader(f);
			char[] cbuf = new char[1024];
			int size = 0;
			while ((size = in.read(cbuf)) > -1)
				sb.append(cbuf,0,size);
			in.close();
			}
		catch (IOException e)
			{
			}
		return sb.toString();
		}

	void writeString(File f, String s)
		{
		try
			{
			PrintStream ps = new PrintStream(f);
			ps.print(s);
			ps.close();
			}
		catch (FileNotFoundException e)
			{
			}
		}

	public String toTargetYaml()
		{
		return "%e-yaml\n---\n"//
				+ "target-windowing: " + (targetPlatform == null ? "" : targetPlatform.id) + "\n"//
				+ "target-graphics: " + (targetGraphics == null ? "" : targetGraphics.id) + "\n"//
				+ "target-audio: " + (targetAudio == null ? "" : targetAudio.id) + "\n"//
				+ "target-collision: " + (targetCollision == null ? "" : targetCollision.id) + "\n";//
		}

	public EnigmaSettings copy()
		{
		EnigmaSettings es = new EnigmaSettings(false);
		es.cppStrings = cppStrings;
		es.cppOperators = cppOperators;
		es.literalHandling = literalHandling;
		es.structHandling = structHandling;

		es.instanceTypes = instanceTypes;
		es.storageClass = storageClass;

		es.definitions = definitions;
		es.globalLocals = globalLocals;
		es.initialization = initialization;
		es.cleanup = cleanup;

		es.targetPlatform = targetPlatform;
		es.targetGraphics = targetGraphics;
		es.targetAudio = targetAudio;
		es.targetCollision = targetCollision;
		return es;
		}
	}
