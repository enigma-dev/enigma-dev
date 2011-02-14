package org.enigma;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;

import org.enigma.EYamlParser.YamlNode;

public class TargetHandler
	{
	public TargetSelection targetPlatform, targetGraphics, targetAudio, targetCollision;
	static final List<TargetSelection> tPlatforms, tGraphics, tAudios, tCollisions;

	static
		{
		tPlatforms = findTargets("Platforms");
		tGraphics = findTargets("Graphics_Systems");
		tAudios = findTargets("Audio_Systems");
		tCollisions = findTargets("Collision_Systems");
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

		public String toString()
			{
			return name;
			}
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

	private void setup()
		{
		targetPlatform = findDefault(tPlatforms,getOS());
		if (targetPlatform != null)
			{
			targetGraphics = findDefault(tGraphics,targetPlatform.id);
			targetAudio = findDefault(tAudios,targetPlatform.id);
			targetCollision = findDefault(tCollisions,targetPlatform.id);
			}
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
	}
