/*
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.utility;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Map.Entry;

public class EEFReader
	{
	static final String commentSymbols = "/'%;#"; //$NON-NLS-1$

	public static class EEFNode
		{
		public String blockName;
		public Map<String,String[]> namedAttributes;
		public ArrayList<String> lineAttribs;
		public ArrayList<EEFNode> children;
		public String[] id;

		protected EEFNode()
			{
			blockName = "";
			namedAttributes = new HashMap<String,String[]>();
			lineAttribs = new ArrayList<String>();
			children = new ArrayList<EEFNode>();
			id = new String[0];
			}

		protected static int tostring_calls = 0;

		public String toString()
			{
			tostring_calls++;
			String indent = "";
			for (int i = 1; i < tostring_calls; i++)
				indent += "  ";

			String res = indent;
			if (blockName == null)
				res += "EEF Root";
			else if (blockName.equals(""))
				res += "Instance";
			else
				res += blockName;
			if (id != null && id.length != 0)
				{
				res += "(";
				for (String i : id)
					res += i;
				res += ")";
				}
			for (Entry<String,String[]> i : namedAttributes.entrySet())
				{
				res += " " + i.getKey() + "(";
				boolean wrote = false;
				for (String ii : i.getValue())
					{
					res += wrote ? ", " + ii : ii;
					wrote = true;
					}
				res += ")";
				}
			res += (children.size() > 0 ? "{" + children.size() + "}" : "") + "\n";
			for (String i : lineAttribs)
				res += indent + "  " + i.trim() + "\n";
			for (EEFNode child : children)
				{
				res += child;
				res += res.endsWith("\n") ? "" : "\n";
				}

			tostring_calls--;
			return res;
			}
		}

	private static Scanner file;

	protected static String comment_str;
	protected static String separator_str;

	public static EEFNode parse(InputStream is)
		{
		return parse(is,","); //$NON-NLS-1$
		}

	public static EEFNode parse(InputStream is, String sepchar)
		{
		EEFNode rootNode = new EEFNode();
		rootNode.blockName = null;
		separator_str = sepchar;
		file = new Scanner(is);
		String firstLine = nextLine();
		int lenComment = 0;
		for (lenComment = 0; commentSymbols.contains(firstLine.subSequence(lenComment,lenComment + 1)); lenComment++)
			;
		if (lenComment > 0)
			{
			comment_str = firstLine.substring(0,lenComment);
			while (file.hasNextLine())
				{
				String str = nextLine();
				if (str.isEmpty()) continue;
				readItem(rootNode,str);
				}
			}
		else
			{
			comment_str = null;
			readItem(rootNode,firstLine);
			while (file.hasNextLine())
				{
				String str = nextLine();
				if (str.isEmpty()) continue;
				readItem(rootNode,str);
				}
			}
		return rootNode;
		}

	protected static boolean isWordChar(char charAt)
		{
		return Character.isLetterOrDigit(charAt) | charAt == '_';
		}

	protected static class MetaNode
		{
		int children;
		int lineAttrs;
		String[] ids;
		Map<String,String[]> other_attrs;
		public String name;

		public MetaNode()
			{
			name = null;
			ids = new String[0];
			children = lineAttrs = 0;
			other_attrs = new HashMap<String,String[]>();
			}
		}

	protected static MetaNode readAttrs(String line, String blockName)
		{
		MetaNode res = new MetaNode();
		int pos = 0;
		if (blockName != null)
			for (; pos < line.length(); pos++)
				{
				if (Character.isWhitespace(line.charAt(pos))) continue;
				if (!isWordChar(line.charAt(pos)))
					{
					System.err.println("Child data must begin with identifier.");
					break;
					}
				final int spos = pos;
				while (++pos < line.length() && isWordChar(line.charAt(pos)))
					;
				final String elementName = line.substring(spos,pos);
				if (blockName != null && !blockName.matches(elementName + "e?s"))
					{
					System.err.println(LINES + " Warning: instance `" + elementName
							+ "' Does not appear to be a member of `" + blockName + "'");
					}
				while (Character.isWhitespace(line.charAt(pos)))
					++pos;
				if (line.charAt(pos) == '(')
					{
					int[] rpos = new int[] { pos };
					res.ids = parseParenths(rpos,line,separator_str).toArray(res.ids);
					pos = rpos[0] + 1;
					}
				if (line.charAt(pos) != ':')
					{
					System.err.println(LINES + " Warning: Missing colon");
					System.err.println("in `" + line + "` at " + pos);
					}
				else
					pos++;
				break;
				}
		for (; pos < line.length(); pos++)
			{
			if (Character.isWhitespace(line.charAt(pos))) continue;
			if (comment_str != null && line.substring(pos,pos + comment_str.length()).equals(comment_str))
				break;
			if (line.charAt(pos) == ',') continue;

			String attrn;
			if (line.charAt(pos) == '"')
				{
				final int spos = pos + 1;
				while (line.charAt(++pos) != '"')
					if (line.charAt(pos) == '\\') ++pos;
				attrn = eefEsc(line.substring(spos,pos));
				pos++;
				}
			else if (line.charAt(pos) == '\'')
				{
				final int spos = pos + 1;
				while (line.charAt(++pos) != '\'')
					if (line.charAt(pos) == '\\') ++pos;
				attrn = eefEsc(line.substring(spos,pos));
				pos++;
				}
			else if (isWordChar(line.charAt(pos)))
				{
				final int spos = pos;
				while (++pos < line.length() && isWordChar(line.charAt(pos)))
					;
				attrn = line.substring(spos,pos);
				}
			else
				{
				System.err.println(LINES + " Unexpected symbol '" + line.charAt(pos) + "' encountered");
				continue;
				}

			if (pos >= line.length()) continue;

			// Find next non-white char
			while (Character.isWhitespace(line.charAt(pos)))
				++pos;

			if (line.charAt(pos) == '(')
				{
				int[] rpos = new int[] { pos };
				ArrayList<String> values = parseParenths(rpos,line,separator_str);
				res.other_attrs.put(attrn,values.toArray(new String[values.size()]));
				pos = rpos[0];
				continue;
				}
			if (line.charAt(pos) == '[')
				{
				while (Character.isWhitespace(line.charAt(++pos)))
					;
				if (!Character.isDigit(line.charAt(pos)))
					{
					System.err.println(LINES + " Expected number in []");
					}
				final int spos = pos;
				while (Character.isDigit(line.charAt(++pos)))
					;
				res.lineAttrs = Integer.parseInt(line.substring(spos,pos));
				while (pos < line.length() && line.charAt(pos) != ']')
					pos++;
				continue;
				}
			if (line.charAt(pos) == '{')
				{
				while (Character.isWhitespace(line.charAt(++pos)))
					;
				if (!Character.isDigit(line.charAt(pos)))
					{
					System.err.println(LINES + " Expected number in {}");
					}
				final int spos = pos;
				while (Character.isDigit(line.charAt(++pos)))
					;
				res.children = Integer.parseInt(line.substring(spos,pos));
				res.name = attrn;
				while (pos < line.length() && line.charAt(pos) != '}')
					pos++;
				continue;
				}
			res.other_attrs.put(attrn,new String[] {});
			pos--;
			}
		return res;
		}

	private static ArrayList<String> parseParenths(int[] pos, String line, String sep)
		{
		ArrayList<String> values = new ArrayList<String>();
		while (Character.isWhitespace(line.charAt(++pos[0])))
			;
		int spos = pos[0];
		while (line.charAt(pos[0]) != ')')
			{
			if (line.charAt(pos[0]) == '\'')
				{
				final int sspos = pos[0] + 1;
				while (line.charAt(++pos[0]) != '\'')
					if (line.charAt(pos[0]) == '\\') ++pos[0];
				values.add(eefEsc(line.substring(sspos,pos[0])));
				while (Character.isWhitespace(line.charAt(++pos[0])))
					;
				spos = pos[0];
				}
			else if (line.charAt(pos[0]) == '"')
				{
				final int sspos = pos[0] + 1;
				while (line.charAt(++pos[0]) != '"')
					if (line.charAt(pos[0]) == '\\') ++pos[0];
				values.add(eefEsc(line.substring(sspos,pos[0])));
				while (Character.isWhitespace(line.charAt(++pos[0])))
					;
				spos = pos[0];
				}
			else
				{
				if (line.substring(pos[0],pos[0] + sep.length()).equals(sep))
					{
					values.add(line.substring(spos,pos[0]).trim());
					spos = pos[0] + 1;
					}
				pos[0]++;
				}
			}
		if (pos[0] - spos > 0) values.add(line.substring(spos,pos[0]));
		return values;
		}

	protected static String eefEsc(String str)
		{
		return str.replace("\\\\","\\"); //$NON-NLS-1$
		}

	protected static void readItem(EEFNode parent, String line)
		{
		MetaNode read_attrs = readAttrs(line,parent.blockName);
		EEFNode e = new EEFNode();
		if (read_attrs.ids != null) e.id = read_attrs.ids;
		e.namedAttributes = read_attrs.other_attrs;
		if (read_attrs.name != null) e.blockName = read_attrs.name;

		for (int i = 0; i < read_attrs.lineAttrs; i++)
			e.lineAttribs.add(nextLine());
		for (int i = 0; i < read_attrs.children; i++)
			{
			String str = nextLine();
			while (str.isEmpty())
				str = nextLine();
			readItem(e,str);
			}

		parent.children.add(e);
		}

	protected static int LINES = 0;

	protected static String nextLine()
		{
		LINES++;
		return file.nextLine();
		}
	}
