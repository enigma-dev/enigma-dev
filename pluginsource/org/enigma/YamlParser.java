/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Scanner;

public class YamlParser
	{
	public static class YamlElement
		{
		//Each one has a name, mostly to preserve case
		public String name;
		//Tells whether this contains a single data entry (including [] lists) or more attributes
		public final boolean isScalar;

		public YamlElement(String name, boolean scalar)
			{
			this.name = name;
			isScalar = scalar;
			}
		}

	// Contains multiple members
	public static class YamlNode extends YamlElement
		{
		// Members stored here
		public HashMap<String,YamlElement> values;
		// We link to the start and end of the chrono list
		public LinkedList<YamlElement> chronos;

		private YamlNode(String name)
			{
			super(name,false);
			values = new HashMap<String,YamlElement>();
			chronos = new LinkedList<YamlElement>();
			}

		public YamlElement getM(String key)
			{
			YamlElement r = values.get(key.toLowerCase());
			if (r == null) throw new IndexOutOfBoundsException(key);
			return r;
			}

		public String getMC(String key)
			{
			return ((YamlContent) getM(key)).getValue();
			}

		public String getMC(String key, String def)
			{
			YamlElement e = values.get(key.toLowerCase());
			if (e == null) return def;
			return ((YamlContent) e).getValue();
			}

		public String toString()
			{
			return name;
			}
		}

	//If it doesn't contain more named members, it's a scalar
	public static class YamlContent extends YamlElement
		{
		// Scalars have a single value
		public String rawValue;
		private String escValue = null;

		YamlContent(String name, String value)
			{
			super(name,true);
			rawValue = value;
			}

		// Implicitly behave as a string
		public String toString()
			{
			return rawValue;
			}

		public String getValue()
			{
			if (escValue == null) escValue = escape(rawValue);
			return escValue;
			}

		// This function escapes yaml # and % too: don't just swap it blindly for some Java function
		private String escape(String value)
			{
			if (value == null || value.isEmpty() || value.charAt(0) != '"') return value;

			StringBuilder ret = new StringBuilder(value.substring(1));

			for (int i = 0; i < ret.length(); i++)
				{
				if (ret.charAt(i) == '\\')
					switch (ret.charAt(i + 1))
						{
						case 'r':
							ret.setCharAt(i,'\r');
							ret.delete(i + 1,1);
							break;
						case 'n':
							ret.setCharAt(i,'\n');
							ret.delete(i + 1,1);
							break;
						case 't':
							ret.setCharAt(i,'\t');
							ret.delete(i + 1,1);
							break;
						case '"':
							ret.setCharAt(i,'\"');
							ret.delete(i + 1,1);
							break;
						case '\'':
							ret.setCharAt(i,'\'');
							ret.delete(i + 1,1);
							break;
						case '#':
							ret.setCharAt(i,'#');
							ret.delete(i + 1,1);
							break;
						case '%':
							ret.setCharAt(i,'%');
							ret.delete(i + 1,1);
							break;
						case '\\':
							ret.delete(i + 1,1);
							break;
						}
				else if (ret.charAt(i) == '"')
					{
					ret.delete(i,ret.length() - i);
					break;
					}
				}
			return ret.toString();
			}
		}

	private static class YamlLevel
		{
		YamlLevel prev;
		YamlNode s;
		int i;

		YamlLevel(YamlLevel a, YamlNode b, int c)
			{
			prev = a;
			s = b;
			i = c;
			}
		}

	/** @author Josh Ventura */
	public static YamlNode parse(Scanner file)
		{
		YamlNode res = new YamlNode("Root");
		String line, unlowered = "PROGRAM ERROR!";
		int linenum = 1, multi = 0; // Line number for error reporting, multiline representing not in multi (false), starting multi (-1), or in multi (multiple line count)
		@SuppressWarnings("unused")
		// I never really got around to doing this, but it's not difficult.
		char mchar = 0; // The character that started a multiline entry

		line = file.nextLine();
		if (line.length() < 7 || !line.substring(0,7).toLowerCase().equals("%e-yaml")) return res;

		YamlLevel cur = new YamlLevel(null,res,0);
		String latestkey = null;
		YamlElement latest = res;

		continue_2: while (file.hasNext())
			{
			line = file.nextLine();
			linenum++;
			if (line.length() >= 3
					&& (line.substring(0,3).equals("---") || line.substring(0,3).equals("..."))) continue;
			int inds = 0, pos = 0;

			while (pos < line.length() && Character.isWhitespace(line.charAt(pos)))
				{
				pos++;
				inds++;
				}

			if (multi != 0)
				{
				if (multi == -1) // Determine our block-value's indent (that of first line in it)
					multi = inds;
				else if (inds < multi) // If we've sunk below, that, cancel our multi.
					multi = 0;
				if (multi != 0)
					{ // If our multi isn't canceled, append it.
					if (((YamlContent) latest).rawValue == "")
						((YamlContent) latest).rawValue = line.substring(multi);
					else
						((YamlContent) latest).rawValue += "\n" + line.substring(multi);
					continue;
					}
				}

			if (!(pos < line.length()) || line.charAt(pos) == '#' || line.charAt(pos) == '%') continue;

			if (line.charAt(pos) == '-')
				while (++pos < line.length() && Character.isWhitespace(line.charAt(pos)))
					;

			final int nsp = pos;
			while (pos < line.length() && line.charAt(pos) != ':')
				if (line.charAt(pos++) == '#') continue continue_2;
			final String nname = line.substring(nsp,pos);

			if (inds != cur.i)
				{
				if (inds > cur.i) // This level has more indentation than the last.
					{
					if (cur.s.values.isEmpty())
						cur.i = inds;
					else
						{
						if (latest != null) // If we've already assigned to this key happily
							System.out.println("Indent increased unexpectedly on line " + linenum);
						else
							// There's indeed a key we didn't know what to do with. Now we do.
							{
							latest = new YamlNode(unlowered); // Allocate a new data entry; we'll be populating the latest node
							cur.s.values.put(latestkey,latest);
							cur.s.chronos.removeLast();
							cur.s.chronos.addLast(latest); // Drop our previous entry in the chronos list.
							cur = new YamlLevel(cur,(YamlNode) latest,inds);

							// Act on the key named in this line
							latestkey = nname.toLowerCase();
							cur.s.values.put(latestkey,null);
							cur.s.chronos.addLast(null);
							latest = null; // New node, which we're yet unsure what to do with
							}
						}
					}
				else
					// This level has less indentation than the last; drop.
					{
					if (latest == null)
						{
						latest = new YamlContent(unlowered,"");
						cur.s.values.put(latestkey,latest);
						cur.s.chronos.removeLast();
						cur.s.chronos.addLast(latest);
						}
					while (cur.prev != null && inds < cur.i)
						cur = cur.prev;

					latestkey = nname.toLowerCase();
					cur.s.values.put(latestkey,null);
					cur.s.chronos.addLast(null);
					latest = null;
					}
				}
			else
				// We are at the same indentation as before, and so we will simply add an item to this current scope
				{
				if (latestkey != null && latest == null)
					{
					latest = new YamlContent(unlowered,"");
					cur.s.values.put(latestkey,latest);
					cur.s.chronos.removeLast();
					cur.s.chronos.addLast(latest);
					}
				latestkey = nname.toLowerCase();
				cur.s.values.put(latestkey,null);
				cur.s.chronos.addLast(null);
				latest = null;
				}

			unlowered = nname;
			if (pos >= line.length() || line.charAt(pos) != ':') continue;

			/* Get the value
			*********************/

			while (++pos < line.length() && Character.isWhitespace(line.charAt(pos)))
				; // Skip the whitespace between colon and value

			final int vsp = pos; // Store value start position
			if (pos < line.length())
				{
				if (line.charAt(pos) == '"')
					{
					while (++pos < line.length() && line.charAt(pos) != '"')
						if (line.charAt(pos) == '"') pos++;
					}
				while (pos < line.length() && line.charAt(pos) != '#' && line.charAt(pos) != '%')
					pos++; // Find end of line (or start of comment)
				}
			while (Character.isWhitespace(line.charAt(--pos)))
				; // Trim trailing whitespace
			if (++pos > vsp) // If we have any non-white value after this colon at all...
				if (pos - vsp == 1 && (line.charAt(vsp) == '|' || line.charAt(vsp) == '>'))
					{ // Pipe => Multiline value
					latest = new YamlContent(unlowered,"");
					cur.s.values.put(latestkey,latest);
					cur.s.chronos.removeLast();
					cur.s.chronos.addLast(latest);
					multi = -1;
					mchar = line.charAt(vsp); // Indicate that we are starting a multiline value, and note the character invoking it
					}
				else
					{ // Otherwise, just an ordinary scalar
					latest = new YamlContent(unlowered,line.substring(vsp,pos)); // Store this value as a string
					cur.s.values.put(latestkey,latest);
					cur.s.chronos.removeLast();
					cur.s.chronos.addLast(latest);
					}
			else
				;
			}
		if (latestkey != null && latest == null)
			{
			latest = new YamlContent(unlowered,"");
			cur.s.values.put(latestkey,latest);
			cur.s.chronos.removeLast();
			cur.s.chronos.addLast(latest);
			}

		return res;
		}
	}
