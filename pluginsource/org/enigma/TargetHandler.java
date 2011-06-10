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
import java.util.regex.Pattern;

import org.enigma.YamlParser.YamlNode;

public class TargetHandler
	{
	private static final Pattern normalizer = Pattern.compile("[\\[\\]\\-\\s_]"); //$NON-NLS-1$

	public static TargetSelection defCompiler, defPlatform, defGraphics, defAudio, defCollision,
			defWidgets;
	private static List<TargetSelection> tCompilers, tPlatforms, tGraphics, tAudios, tCollisions,
			tWidgets;
	static
		{
		tCompilers = findCompilers();
		tPlatforms = findTargets("Platforms"); //$NON-NLS-1$
		tGraphics = findTargets("Graphics_Systems"); //$NON-NLS-1$
		tAudios = findTargets("Audio_Systems"); //$NON-NLS-1$
		tCollisions = findTargets("Collision_Systems"); //$NON-NLS-1$
		tWidgets = findTargets("Widget_Systems"); //$NON-NLS-1$

		findDefaults();
		}

	private TargetHandler()
		{
		}

	public static String getOS()
		{
		String os = normalize(System.getProperty("os.name")); //$NON-NLS-1$
		if (os.contains("nux") || os.contains("nix")) return "Linux"; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		if (os.contains("win")) return "Windows"; //$NON-NLS-1$ //$NON-NLS-2$
		if (os.contains("mac")) return "MacOSX"; //$NON-NLS-1$ //$NON-NLS-2$
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
		File f = new File(EnigmaRunner.WORKDIR,"Compilers"); //$NON-NLS-1$
		File[] files = new File(f,getOS()).listFiles();
		if (files == null) return tCompilers;
		for (File file : files)
			{
			String ey = file.getName();
			if (!ey.endsWith(".ey")) continue; //$NON-NLS-1$
			try
				{
				YamlNode node = YamlParser.parse(file);

				TargetSelection ps = new TargetSelection();
				ps.id = ey.substring(0,ey.length() - 3);
				ps.name = node.getMC("Name"); //$NON-NLS-1$
				ps.desc = node.getMC("Description",null); //$NON-NLS-1$
				ps.auth = node.getMC("Maintainer",null); //$NON-NLS-1$
				ps.ext = node.getMC("Build-Extension",null); //$NON-NLS-1$
				ps.outputexe = node.getMC("Run-output",null); //$NON-NLS-1$
				ps.depends = new HashSet<String>();
				ps.depends.add(node.getMC("Target-platform",null)); //actually a target, not a dependency //$NON-NLS-1$
				if (node.getBool("Native",false)) defCompiler = ps; //$NON-NLS-1$
				tCompilers.add(ps);
				}
			catch (FileNotFoundException e)
				{
				}
			}
		return tCompilers;
		}

	//target is one of "Platforms","Audio_Systems","Graphics_Systems","Collision_Systems","Widget_Systems"
	private static List<TargetSelection> findTargets(String target)
		{
		ArrayList<TargetSelection> targets = new ArrayList<TargetSelection>();

		File f = new File(EnigmaRunner.WORKDIR,"ENIGMAsystem"); //$NON-NLS-1$
		f = new File(f,"SHELL"); //$NON-NLS-1$
		f = new File(f,target);
		File files[] = f.listFiles();
		if (files == null) return targets;
		for (File dir : files)
			{
			if (!dir.isDirectory()) continue;
			File prop = new File(dir,"Info"); //$NON-NLS-1$
			//technically this could stand to be a .properties file, rather than e-yaml
			prop = new File(prop,"About.ey"); //$NON-NLS-1$
			try
				{
				Set<String> depends = new HashSet<String>();
				Set<String> defaultOn = new HashSet<String>();
				YamlNode node;

				if (target.equals("Platforms")) //$NON-NLS-1$
					{
					node = YamlParser.parse(prop);
					String norm = normalize(node.getMC("Build-Platforms")); //$NON-NLS-1$
					if (norm.isEmpty()) continue;
					for (String s : norm.split(",")) //$NON-NLS-1$
						if (!s.isEmpty()) depends.add(s);
					}
				else if (target.equals("Collision_Systems")) //$NON-NLS-1$
					{
					node = YamlParser.parse(new Scanner(prop));
					depends.add("all"); //$NON-NLS-1$
					}
				else
					{
					if (dir.getName().equals("None")) //$NON-NLS-1$
						depends.add("all"); //$NON-NLS-1$
					else
						{
						String[] configs = new File(dir,"Config").list(); //$NON-NLS-1$
						if (configs == null) continue;
						for (String conf : configs)
							if (conf.endsWith(".ey")) //$NON-NLS-1$
								depends.add(normalize(conf.substring(0,conf.length() - 3)));
						if (depends.isEmpty()) continue;
						}
					node = YamlParser.parse(new Scanner(prop));
					}

				String norm = normalize(node.getMC("Represents","")); //$NON-NLS-1$ //$NON-NLS-2$
				for (String s : norm.split(",")) //$NON-NLS-1$
					if (!s.isEmpty()) defaultOn.add(s);

				TargetSelection ps = new TargetSelection();
				ps.name = node.getMC("Name"); //$NON-NLS-1$
				ps.id = node.getMC("Identifier"); //$NON-NLS-1$
				ps.depends = depends;
				ps.defaultOn = defaultOn;
				ps.desc = node.getMC("Description",null); //$NON-NLS-1$
				ps.auth = node.getMC("Author",null); //$NON-NLS-1$
				ps.ext = node.getMC("Build-Extension",null); //$NON-NLS-1$
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
		return normalizer.matcher(s.toLowerCase()).replaceAll(""); //$NON-NLS-1$
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
			if (ts.depends.contains("all") || ts.depends.contains(depends)) //$NON-NLS-1$
				{
				//specific platforms always take precedence over the "all" keyword
				if (ts.defaultOn.contains(depends)) return ts;
				//to avoid looping again for "all" keyword if no specific platform found, store it
				if (ts.defaultOn.contains("all") && allTs == null) allTs = ts; //$NON-NLS-1$
				}
		if (allTs != null) return allTs;
		//if none are marked as default, just use the first one
		for (TargetSelection ts : tsl)
			if (ts.depends.contains("all") || ts.depends.contains(depends) //$NON-NLS-1$
					&& !ts.defaultOn.contains("none")) return ts; //$NON-NLS-1$
		return null;
		}

	private static List<TargetSelection> getTargetArray(List<TargetSelection> itsl, String depends)
		{
		if (itsl == tCollisions) return itsl;
		List<TargetSelection> otsl = new ArrayList<TargetSelection>();
		for (TargetSelection ts : itsl)
			if (ts.depends.contains(depends.toLowerCase()) || ts.depends.contains("all")) otsl.add(ts); //$NON-NLS-1$
		return otsl;
		}

	public static Object[] getTargetCompilersArray()
		{
		return tCompilers.toArray();
		}

	public static Object[] getTargetPlatformsArray(String depends)
		{
		return getTargetArray(tPlatforms,depends).toArray();
		}

	public static Object[] getTargetGraphicsArray(String depends)
		{
		return getTargetArray(tGraphics,depends).toArray();
		}

	public static Object[] getTargetAudiosArray(String depends)
		{
		return getTargetArray(tAudios,depends).toArray();
		}

	public static Object[] getTargetCollisionsArray()
		{
		return tCollisions.toArray();
		}

	public static Object[] getTargetWidgetsArray(String depends)
		{
		return getTargetArray(tWidgets,depends).toArray();
		}
	}
