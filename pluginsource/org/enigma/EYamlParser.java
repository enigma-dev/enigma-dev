/*
 * Copyright (C) 2010 Matt Shaffer
 * Copyright (C) 2010 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * 
 * Enigma Plugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Enigma Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

package org.enigma;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class EYamlParser
	{
	static YamlNode parse(Scanner sc)
		{
		Deque<YamlNode> v = new ArrayDeque<YamlNode>();
		YamlNode b = new YamlNode(); // base ymlnode from which all life springs forth
		boolean sblock = false; // whether or not we're parsing a block of text
		int blocklvl = -1; // -1 = tbd, first run it'll be set to the indentation
		int blocktype = 0; // 0 = relaxed, 1 = literal

		if (!sc.nextLine().trim().equals("%e-yaml")) System.out.println("[Insert not e-yaml error]");
		while (sc.hasNext())
			{
			String s = sc.nextLine();
			String st = s.trim();

			int cur_indent = 0;
			while (cur_indent < s.length() && s.charAt(cur_indent) == ' ')
				cur_indent++;
			String cur_line = s.substring(cur_indent);

			if (s.startsWith("---") || st.startsWith("#")) continue;

			if (!sblock && st.indexOf(":") == -1)
				{
				// I couldn't think of any more reasonable way to determine if it was a non-literal text block...
				sblock = true;
				blocktype = 0;
				}
			if (sblock)
				{ // Determine if block continues
				if (cur_indent < blocklvl && !st.isEmpty())
					{
					sblock = false;
					blocklvl = -1;
					v.getFirst().content = v.getFirst().content.trim();
					}
				}
			if (sblock)
				{
				if (blocklvl == -1) blocklvl = cur_indent;
				if (blocktype == 0)
					{
					if (!st.isEmpty()) v.getFirst().content = v.getFirst().content + st + " ";
					}
				else
					{
					if (st.isEmpty())
						v.getFirst().content = v.getFirst().content + "\n";
					else
						v.getFirst().content = v.getFirst().content + cur_line + "\n";
					}

				}
			else
				{
				if (st.startsWith("- "))
					{
					v.addFirst(new YamlNode());

					if (!cur_line.endsWith(":"))
						{
						String name = cur_line.trim();
						// Also for some odd reason here, I just left this variable
						// named "name" even though it's the sequence value.
						name = name.substring(name.indexOf(":") + 2,name.length());
						// This is a sequence so we better have a parent...

						Iterator<YamlNode> it = v.iterator();
						it.next();
						while (it.hasNext())
							{
							YamlNode cur = it.next();
							if (cur.lvl >= cur_indent || !cur.isMap()) continue;

							YamlNode child = v.getFirst();
							child.set(cur_indent,1);
							cur.seqs.add(child);
							if (name.equals("|"))
								{
								sblock = true;
								blocktype = 1;
								}
							else if (name.startsWith("[") && name.endsWith("]"))
								{
								name = name.substring(1,name.length() - 1);
								String[] listelmts = name.split(",");
								for (int i2 = 0; i2 < listelmts.length; i2++)
									{
									YamlNode node = new YamlNode(cur_indent + 1,1);
									node.content = listelmts[i2];
									v.getFirst().seqs.add(node);
									}
								}
							else
								{
								// Inline Comments...
								if (name.indexOf("#") > -1)
									{
									name = name.substring(0,name.indexOf("#") - 1);
									}
								// If it's empty, there's no value given = just leave the object null
								if (!name.equals(""))
									{
									v.getFirst().content = name;
									}
								}
							} //iterate for parent
						} //not dangling map
					} //sequence
				else
					{
					v.addFirst(new YamlNode());
					String name = cur_line;
					name = name.substring(0,name.indexOf(":"));
					if (cur_indent == 0)
						{
						// It's in the topmost node level, so we have to add it differently
						YamlNode node = v.getFirst();
						node.set(0,0);
						b.maps.put(name.toLowerCase(),node);
						}
					else
						{
						// Determining parent node
						Iterator<YamlNode> it = v.iterator();
						it.next();
						while (it.hasNext())
							{
							YamlNode cur = it.next();
							if (cur.lvl >= cur_indent || !cur.isMap()) continue;
							YamlNode node = v.getFirst();
							node.set(cur_indent,0);
							cur.maps.put(name.toLowerCase(),node);
							break;
							}
						}
					if (!st.endsWith(":"))
						{
						// Ohey a value
						String val = st;
						val = val.substring(val.indexOf(":") + 2,val.length());
						if (val.equals("|"))
							{
							sblock = true;
							blocktype = 1;
							}
						else if (val.startsWith("[") && val.endsWith("]"))
							{ // [lists]
							val = val.substring(1,val.length() - 1);
							String[] listelmts = val.split(",");
							for (int i2 = 0; i2 < listelmts.length; i2++)
								{
								YamlNode node = new YamlNode(cur_indent + 1,1);
								node.content = listelmts[i2];
								v.getFirst().seqs.add(node);
								}
							}
						else
							{
							// Inline Comments...
							if (val.indexOf("#") > -1) val = val.substring(0,val.indexOf("#") - 1);
							// If it's empty, there's no value given = just leave the string empty.
							// It really seams pointless because if val is "", might as well save
							// the overhead of the if statement. Maybe I'll remove this if statement
							// some time, you won't even notice.
							if (!val.isEmpty()) v.getFirst().content = val;
							} //modifier [] |
						} //not dangling map
					} //non-sequence

				} // BLAH
			} //more lines

		return b;
		}

	public static void main(String[] args)
		{
		try
			{
			//			String loc = "C:\\Users\\Owner\\Documents\\NetBeansProjects\\byamltest\\build\\classes\\test.yml";
			YamlNode myyml = EYamlParser.parse(new Scanner(System.in));
			System.out.println("-----");
			System.out.println(myyml.getM("Universe").getM("Galaxies").getS(1));

			if (true) return;
			System.out.println(myyml.getM("Universe").getM("Galaxies").getM("Solar Systems").getM("Ours").getS(
					1).getS(2).content);
			System.out.println(myyml.getM("Universe").content);
			System.out.println(myyml.getM("Universe").getM("Ethereal").getS(3).content);
			System.out.println(myyml.getM("Universe").getM("Galaxies").getM("Solar Systems").getM("Ours").content);
			System.out.println(myyml.getM("Universe").getS(0).content);
			System.out.println(myyml.getM("Universe").getM("Bio").content);
			System.out.println(myyml.getM("Universe").getM("Ethereal").getS(5).content);
			}
		catch (Exception e)
			{
			//System.out.println("I failed.");
			e.printStackTrace();
			}
		}
	}

class YamlNode
	{
	public Map<String,YamlNode> maps = new HashMap<String,YamlNode>();
	public List<YamlNode> seqs = new ArrayList<YamlNode>();
	public String content = "";
	public int lvl = -1; // indentation
	private int type = -1; // whether it's a map/seq node

	public YamlNode()
		{
		}

	public YamlNode(int lvl, int type)
		{
		set(lvl,type);
		}

	void set(int lvl, int type)
		{
		this.lvl = lvl;
		this.type = type;
		}

	boolean isMap()
		{
		return type == 0;
		}

	public YamlNode getM(String s) throws IndexOutOfBoundsException
		{
		YamlNode r = maps.get(s.toLowerCase());
		if (r == null) throw new IndexOutOfBoundsException(s);
		return r;
		}

	public String getMC(String s) throws IndexOutOfBoundsException
		{
		return getM(s).content;
		}

	public String getMC(String s, String def)
		{
		YamlNode r = maps.get(s.toLowerCase());
		if (r == null) return def;
		return r.content;
		}

	public YamlNode getS(int k) throws IndexOutOfBoundsException
		{
		return seqs.get(k);
		}

	// There is no reason to use this. I included it just in case you forgot content was public and
	// were just going along with the general naming convention.
	public String getC()
		{
		return content;
		}

	public String toString()
		{
		return "{" + lvl + ",content=" + content + ",seq[" + seqs.size() + "],map:" + maps.keySet()
				+ "}";
		}
	}
