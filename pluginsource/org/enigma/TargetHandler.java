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
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

import org.enigma.YamlParser.YamlNode;

public class TargetHandler
	{
	public static TargetSelection defCompiler, defPlatform, defGraphics, defAudio, defCollision,
			defWidgets;
	private static List<TargetSelection> tCompilers, tPlatforms, tGraphics, tAudios, tCollisions,
			tWidgets;

	static
		{
		tCompilers = findCompilers();
		tPlatforms = findTargets("Platforms");
		tGraphics = findTargets("Graphics_Systems");
		tAudios = findTargets("Audio_Systems");
		tCollisions = findTargets("Collision_Systems");
		tWidgets = findTargets("Widget_Systems");

		findDefaults();
		}

	private TargetHandler()
		{
		}

	public static String getOS()
		{
		String os = normalize(System.getProperty("os.name"));
		if (os.contains("nux") || os.contains("nix")) return "Linux";
		if (os.contains("win")) return "Windows";
		if (os.contains("mac")) return "MacOSX";
		return os;
		}

	public static class TargetSelection
		{
		public String name, id; //mandatory
		public Set<String> depends; //mandatory, non-empty
		public Set<String> defaultOn; //optional
		public String desc, auth, ext; //optional
		public String outputexe;

		public String toString()
			{
			return name;
			}
		}

	private static ArrayList<TargetSelection> findCompilers()
		{
		ArrayList<TargetSelection> tCompilers = new ArrayList<TargetSelection>();
		File f = new File(EnigmaRunner.WORKDIR,"Compilers");
		File[] files = new File(f,getOS()).listFiles();
		if (files == null) return tCompilers;
		for (File file : files)
			{
			String ey = file.getName();
			if (!ey.endsWith(".ey")) continue;
			try
				{
				YamlNode node = YamlParser.parse(new Scanner(file));

				TargetSelection ps = new TargetSelection();
				ps.id = ey.substring(0,ey.length() - 3);
				ps.name = node.getMC("Name");
				ps.desc = node.getMC("Description",null);
				ps.auth = node.getMC("Maintainer",null);
				ps.ext = node.getMC("Build-Extension",null);
				ps.outputexe = node.getMC("Run-output",null);
				String nat = node.getMC("Native","No").toLowerCase(); //native is a keyword
				if (nat.equals("yes") || nat.equals("true")) defCompiler = ps;
				tCompilers.add(ps);
				}
			catch (FileNotFoundException e)
				{
				}
			}
		return tCompilers;
		}

	//target is one of ""Platforms","Audio_Systems","Graphics_Systems","Collision_Systems"
	private static List<TargetSelection> findTargets(String target)
		{
		ArrayList<TargetSelection> targets = new ArrayList<TargetSelection>();

		File f = new File(EnigmaRunner.WORKDIR,"ENIGMAsystem");
		f = new File(f,"SHELL");
		f = new File(f,target);
		File files[] = f.listFiles();
		if (files == null) return targets;
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
					node = YamlParser.parse(new Scanner(prop));
					String norm = normalize(node.getMC("Build-Platforms"));
					if (norm.isEmpty()) continue;
					for (String s : norm.split(","))
						if (!s.isEmpty()) depends.add(s);
					}
				else if (target.equals("Collision_Systems"))
					{
					node = YamlParser.parse(new Scanner(prop));
					depends.add("all");
					}
				else
					{
					if (dir.getName().equals("None"))
						depends.add("all");
					else
						{
						String[] configs = new File(dir,"Config").list();
						if (configs == null) continue;
						for (String conf : configs)
							if (conf.endsWith(".ey"))
								depends.add(normalize(conf.substring(0,conf.length() - 3)));
						if (depends.isEmpty()) continue;
						}
					node = YamlParser.parse(new Scanner(prop));
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

	private static void findDefaults()
		{
		defPlatform = findDefault(tPlatforms,getOS());
		if (defPlatform != null)
			{
			defGraphics = findDefault(tGraphics,defPlatform.id);
			defAudio = findDefault(tAudios,defPlatform.id);
			defCollision = findDefault(tCollisions,defPlatform.id);
			defWidgets = findDefault(tWidgets,defPlatform.id);
			}
		}

	private static TargetSelection findDefault(List<TargetSelection> tsl, String depends)
		{
		depends = depends.toLowerCase();
		TargetSelection allTs = null;

		for (TargetSelection ts : tsl)
			if (ts.depends.contains("all") || ts.depends.contains(depends))
				{
				//specific platforms always take precedence over the "all" keyword
				if (ts.defaultOn.contains(depends)) return ts;
				//to avoid looping again for "all" keyword if no specific platform found, store it
				if (ts.defaultOn.contains("all") && allTs == null) allTs = ts;
				}
		if (allTs != null) return allTs;
		//if none are marked as default, just use the first one
		for (TargetSelection ts : tsl)
			if (ts.depends.contains("all") || ts.depends.contains(depends)
					&& !ts.defaultOn.contains("none")) return ts;
		return null;
		}

	private static List<TargetSelection> getTargetArray(List<TargetSelection> itsl)
		{
		List<TargetSelection> otsl = new ArrayList<TargetSelection>();
		String depends;
		if (itsl == tPlatforms)
			depends = getOS();
		else if (itsl == tCollisions)
			return itsl;
		else
			{
			if (defPlatform == null) return otsl;
			depends = defPlatform.id;
			}

		for (TargetSelection ts : itsl)
			if (ts.depends.contains(depends.toLowerCase()) || ts.depends.contains("all")) otsl.add(ts);

		return otsl;
		}

	public static Object[] getTargetCompilersArray()
		{
		return tCompilers.toArray();
		}

	public static Object[] getTargetPlatformsArray()
		{
		return getTargetArray(tPlatforms).toArray();
		}

	public static Object[] getTargetGraphicsArray()
		{
		return getTargetArray(tGraphics).toArray();
		}

	public static Object[] getTargetAudiosArray()
		{
		return getTargetArray(tAudios).toArray();
		}

	public static Object[] getTargetCollisionsArray()
		{
		return getTargetArray(tCollisions).toArray();
		}

	public static Object[] getTargetWidgetsArray()
		{
		return getTargetArray(tWidgets).toArray();
		}
	}
