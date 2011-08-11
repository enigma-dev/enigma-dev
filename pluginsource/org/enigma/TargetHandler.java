/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;
import java.util.regex.Pattern;

import org.enigma.utility.YamlParser;
import org.enigma.utility.YamlParser.YamlContent;
import org.enigma.utility.YamlParser.YamlElement;
import org.enigma.utility.YamlParser.YamlNode;

public final class TargetHandler
	{
	private TargetHandler()
		{
		}

	public static final String COMPILER = "compiler"; //$NON-NLS-1$
	public static final String[] ids = { COMPILER,"windowing","graphics","audio","collision","widget" };
	private static final String[] folders = { null,"Platforms","Graphics_Systems","Audio_Systems",
			"Collision_Systems","Widget_Systems" };
	private static final String COMP_TARG = "target", OS_KEY = "build-platforms"; //$NON-NLS-1$ //$NON-NLS-2$
	private static final Pattern NORMALIZER = Pattern.compile("[\\[\\]\\-\\s_]"); //$NON-NLS-1$
	private static final Pattern SPLITTER = Pattern.compile("\\s*,\\s*"); //$NON-NLS-1$

	public static TargetSelection defCompiler;
	public static Map<String,List<TargetSelection>> targets;
	public static Collection<Combo> combos;
	public static Map<String,TargetSelection> defaults;

	static
		{
		load();
		}

	public static void load()
		{
		targets = new HashMap<String,List<TargetSelection>>();
		targets.put(COMPILER,findCompilers());
		for (int i = 1; i < folders.length; i++)
			targets.put(ids[i],findTargets(folders[i]));

		combos = dependencyResolution();
		findDefaults(combos);

		int scoreIndex = 0;
		int score[] = new int[combos.size()];
		for (Map<String,TargetSelection> i : combos)
			score[scoreIndex++] = scoreCombo(i);
		}

	public static class TargetSelection
		{
		public String name, id; // mandatory
		public Map<String,Set<String>> depends; // mandatory
		public Map<String,Set<String>> defaultOn; // optional
		public String desc, auth, ext; // optional
		public String outputexe;

		public String toString()
			{
			return name;
			}
		}

	public static String getOS()
		{
		String os = normalize(System.getProperty("os.name")); //$NON-NLS-1$
		if (os.contains("nux") || os.contains("nix")) return "Linux"; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
		if (os.contains("win")) return "Windows"; //$NON-NLS-1$ //$NON-NLS-2$
		if (os.contains("mac")) return "MacOSX"; //$NON-NLS-1$ //$NON-NLS-2$
		return os;
		}

	/**
	 * Generates a combination of systems, tests if they work, and returns the
	 * working ones. Inefficient, because it generates a O(N^M) map, but there's
	 * not many systems and it gets the job done. If you can think up a more
	 * efficient solution, please do. For internal workings, see internal
	 * comments.
	 * 
	 * Note: System = Entry<String,Set<TargetSelection>>, or may omit the String
	 */
	private static Collection<Combo> dependencyResolution()
		{
		List<Combo> r = new ArrayList<Combo>();

		// This counter keeps track of which systems we're looking at.
		// Increment one at a time, and roll over into the next system counter.
		int[] systemCounter = new int[targets.size()];
		// Provide a integer pairing for each mapped key, in consistent order.
		String[] systemOrder = targets.keySet().toArray(new String[0]);

		// For efficiency, we could also populate a List of systems.
		for (;;)
			{
			boolean valid = true;
			for (int i = 0; i < systemOrder.length && valid; i++)
				{
				// compiler is already accounted for; why would platform be?
				if (systemOrder[i].equals(COMPILER)) continue;
				List<TargetSelection> system = targets.get(systemOrder[i]);
				// This is usually a sign that something went wrong during system population.
				// A "None" system should have at least been available, but wasn't.
				if (system.isEmpty()) continue; // for now we just ignore it as though the dependencies resolved.

				TargetSelection targ = system.get(systemCounter[i]);
				if (!isResolvable(targ,systemOrder,systemCounter))
					{
					valid = false;
					break;
					}
				}

			if (valid)
				{
				// add to list of acceptable resolutions
				Combo rr = new Combo(targets.size());
				for (int i = 0; i < targets.size(); i++)
					{
					List<TargetSelection> system = targets.get(systemOrder[i]);
					rr.put(systemOrder[i],system.size() <= systemCounter[i] ? null
							: system.get(systemCounter[i]));
					}
				rr.finish();
				r.add(rr);
				}

			// count (little endian)
			int ind = 0;
			while (ind < systemCounter.length
					&& ++systemCounter[ind] >= targets.get(systemOrder[ind]).size())
				systemCounter[ind++] = 0;
			if (ind >= systemCounter.length) break;
			}

		Collections.sort(r);
		return r;
		}

	private static boolean isResolvable(TargetSelection selection, String[] systemOrder,
			int[] systemCounter)
		{
		Set<Entry<String,Set<String>>> entrySet = selection.depends.entrySet();
		depcheck: for (Entry<String,Set<String>> entry : entrySet)
			{
			final String key = entry.getKey().toLowerCase();

			if (key.equalsIgnoreCase(OS_KEY)) // System depends on a certain OS's headers
				{
				List<TargetSelection> allCompilers = targets.get(COMPILER);
				int whichIndex = 0; // This is the index of the compiler we've chosen
				for (int j = 0; j < systemOrder.length; j++)
					if (systemOrder[j].equals(COMPILER))
						{
						whichIndex = systemCounter[j];
						break;
						}
				for (String value : entry.getValue())
					{
					for (String compTargs : allCompilers.get(whichIndex).depends.get(COMP_TARG))
						if (compTargs.equals(value)) continue depcheck;
					}
				return false;
				}

			List<TargetSelection> all = targets.get(key);
			if (all == null) return false; // Invalid dependency system specified.
			// The dependent system has no available targets because dependencies populated wrong.
			// A "None" system should have at least been available, but wasn't.
			if (all.isEmpty()) return false;

			for (int j = 0; j < systemOrder.length; j++)
				if (systemOrder[j].equals(key)
						&& !entry.getValue().contains(targets.get(key).get(systemCounter[j]).id.toLowerCase()))
					return false;
			}
		// All systems pass. If they didn't, they would have failed out early.
		// Also, pass if this doesn't specify any dependencies (loop never executes).
		return true;
		}

	private static int scoreCombo(Map<String,TargetSelection> combo)
		{
		int score = 0;
		for (Entry<String,TargetSelection> pair : combo.entrySet())
			{
			TargetSelection selection = pair.getValue();
			if (selection.defaultOn == null) continue;
			Set<Entry<String,Set<String>>> entrySet = selection.defaultOn.entrySet();

			// For each represented system Value of category Key
			for (Entry<String,Set<String>> rep : entrySet)
				{
				Set<String> acceptable = rep.getValue();
				if (acceptable == null) continue;
				if (rep.getKey().equalsIgnoreCase(OS_KEY))
					{
					if (rep.getValue().contains(combo.get(COMPILER).depends.get(COMP_TARG).iterator().next()))
						score += 2;
					else if (rep.getValue().contains("All") || rep.getValue().contains("all")
							|| rep.getValue().contains("Any") || rep.getValue().contains("any")) score += 1;
					continue;
					}
				TargetSelection cmp = combo.get(rep.getKey().toLowerCase());
				if (acceptable.contains(cmp.id.toLowerCase()))
					score += 2;
				else if (acceptable.contains("all") || acceptable.contains("All"))
					score += 1;
				else if ((acceptable.contains("any") || acceptable.contains("Any"))
						&& !cmp.id.equals("None")) score += 1;
				}
			}
		return score;
		}

	private static class Combo extends HashMap<String,TargetSelection> implements Comparable<Combo>
		{
		private static final long serialVersionUID = 1L;

		//we could add some failsafes in to prevent playing with unset scores, but there's no sense
		public int score = -1;

		public Combo(int size)
			{
			super(size);
			}

		/** Indicates that we are done adding elements so we can calculate its score */
		public void finish()
			{
			score = scoreCombo(this);
			}

		@Override
		public int compareTo(Combo c)
			{
			return c.score - score;
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
				ps.depends = new HashMap<String,Set<String>>();
				Set<String> target = new HashSet<String>();
				String targplat = node.getMC("Target-platform",null); //$NON-NLS-1$
				if (target != null) targplat = normalize(targplat);
				target.add(targplat);
				ps.depends.put("target",target); //$NON-NLS-1$
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
			prop = new File(prop,"About.ey"); //$NON-NLS-1$
			try
				{
				Map<String,Set<String>> depends = new HashMap<String,Set<String>>();
				Map<String,Set<String>> defaultOn = new HashMap<String,Set<String>>();
				YamlNode node = YamlParser.parse(prop);

					/*if (target.equals("Platforms")) //$NON-NLS-1$
					{
						depends = new HashMap<String, Set<String>>();
						String norm = normalize(node.getMC("Build-Platforms")); //$NON-NLS-1$
						if (norm.isEmpty())
							continue;
						Set<String> targs = new HashSet<String>();
						for (String s : SPLITTER.split(norm))
							if (!s.isEmpty())
								targs.add(s);
						depends.put("targets", targs); //$NON-NLS-1$
					} else*/
					{
					try
						{
						populateMap(depends,node,"Depends"); //$NON-NLS-1$
						}
					catch (IndexOutOfBoundsException ee)
						{
						// getM found no "Depends" key, so there are no
						// dependencies.
						}
					try
						{
						populateMap(defaultOn,node,"Represents"); //$NON-NLS-1$
						}
					catch (IndexOutOfBoundsException ee)
						{
						// getM found no "Represents" key, so it's not trying to
						// be a default for anything.
						}
					}

				//				String norm = normalize(node.getMC("Represents","")); //$NON-NLS-1$ //$NON-NLS-2$
				// for (String s : SPLITTER.split(norm))
				// if (!s.isEmpty()) defaultOn.add(s);

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
				// yaml file missing, skip to next file
				}
			catch (IndexOutOfBoundsException e)
				{
				// insufficient yaml, skip to next file
				}
			}
		// if not empty, we may safely assume that the first one is the default
		// selection,
		// or technically, that any of them is the default. The user can/will
		// change it in UI.
		return targets;
		}

	private static void populateMap(Map<String,Set<String>> map, YamlNode node, String key)
		{
		YamlElement e = node.getM(key);
		if (e.isScalar) return; //scalar usually a sign that the map is empty. Treat as empty either way.
		YamlNode n = ((YamlNode) e);
		for (YamlElement el : n.chronos)
			{
			Set<String> dep = new HashSet<String>();
			for (String s : SPLITTER.split(((YamlContent) el).getValue().toLowerCase()))
				if (!s.isEmpty()) dep.add(s);
			map.put(el.name,dep);
			}
		}

	/** get rid of any "[]-_ " (and space), and convert to lowercase. */
	private static String normalize(String s)
		{
		return NORMALIZER.matcher(s.toLowerCase()).replaceAll(""); //$NON-NLS-1$
		}

	private static void findDefaults(Collection<Combo> combos)
		{
		int bestVal = 0;
		for (Combo combo : combos)
			{
			int val = scoreCombo(combo);
			if (val > bestVal)
				{
				bestVal = val;
				defaults = combo;
				}
			}
		}
	}
