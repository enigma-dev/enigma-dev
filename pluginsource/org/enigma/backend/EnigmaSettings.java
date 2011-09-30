/*
 * Copyright (C) 2010, 2011 IsmAvatar <IsmAvatar@gmail.com>
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
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import org.enigma.EnigmaRunner;
import org.enigma.SettingsHandler;
import org.enigma.TargetHandler;
import org.enigma.SettingsHandler.ExtensionSetting;
import org.enigma.SettingsHandler.OptionGroupSetting;
import org.enigma.SettingsHandler.OptionSetting;
import org.enigma.TargetHandler.TargetSelection;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.enigma.utility.YamlParser.YamlContent;
import org.enigma.utility.YamlParser.YamlElement;
import org.enigma.utility.YamlParser.YamlNode;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.util.PropertyMap;

public class EnigmaSettings extends Resource<EnigmaSettings,EnigmaSettings.PEnigmaSettings>
	{
	public String definitions = "", globalLocals = ""; //$NON-NLS-1$ //$NON-NLS-2$
	public String initialization = "", cleanup = ""; //$NON-NLS-1$ //$NON-NLS-2$

	public Map<String,String> options = new HashMap<String,String>();
	//Strings one of: "compiler","windowing","graphics","audio","collision","widget"
	public Map<String,TargetSelection> targets = new HashMap<String,TargetSelection>();
	public Set<String> extensions = new HashSet<String>();

	public enum PEnigmaSettings
		{
		}

	public EnigmaSettings()
		{
		this(true);
		}

	private EnigmaSettings(boolean load)
		{
		super();
		if (!load) return;

		loadDefinitions();
		targets.putAll(TargetHandler.defaults);

		for (OptionGroupSetting ogs : SettingsHandler.optionGroups)
			for (OptionSetting os : ogs.opts)
				options.put(os.id,os.def);

		for (ExtensionSetting es : SettingsHandler.extensions)
			if (es.def) extensions.add(es.path);
		}

	void loadDefinitions()
		{
		definitions = fileToString(new File(EnigmaRunner.WORKDIR,"definitions.h")); //$NON-NLS-1$
		}

	public void saveDefinitions()
		{
		writeString(new File(EnigmaRunner.WORKDIR,"definitions.h"),definitions); //$NON-NLS-1$
		}

	static String fileToString(File f)
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

	static void writeString(File f, String s)
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

	public void fromYaml(YamlNode n)
		{
		for (YamlElement e : n.chronos)
			{
			if (!e.isScalar) continue;
			String val = ((YamlContent) e).getValue();
			options.put(e.name,val);
			}
		}

	public void toYaml(PrintWriter out, boolean includeHeader)
		{
		if (includeHeader)
			{
			out.println("%e-yaml"); //$NON-NLS-1$
			out.println("---"); //$NON-NLS-1$
			}

		for (Entry<String,String> entry : options.entrySet())
			out.append(entry.getKey()).append(": ").append(entry.getValue()).println(); //$NON-NLS-1$

		out.println();

		for (Entry<String,TargetSelection> entry : targets.entrySet())
			{
			if (entry.getValue() == null) continue;
			out.append("target-").append(entry.getKey()).append(": ").append(entry.getValue().id).println(); //$NON-NLS-1$ //$NON-NLS-2$
			}
		out.println("target-networking: None"); //$NON-NLS-1$

		if (extensions.size() > 0)
			{
			out.println();
			out.append("extensions: "); //$NON-NLS-1$

			String[] exts = extensions.toArray(new String[0]);
			out.append(exts[0]);
			for (int i = 1; i < exts.length; i++)
				out.append(',').append(exts[i]);

			out.println();
			}
		out.flush();
		}

	public SyntaxError commitToDriver(EnigmaDriver driver)
		{
		StringWriter sw = new StringWriter();
		toYaml(new PrintWriter(sw),true);
		return driver.definitionsModified(definitions,sw.toString());
		}

	public EnigmaSettings copy()
		{
		EnigmaSettings es = new EnigmaSettings(false);
		copyInto(es);
		return es;
		}

	public void copyInto(EnigmaSettings es)
		{
		es.definitions = definitions;
		es.globalLocals = globalLocals;
		es.initialization = initialization;
		es.cleanup = cleanup;

		es.options.clear();
		es.options.putAll(options);
		es.targets.clear();
		es.targets.putAll(targets);
		es.extensions.clear();
		es.extensions.addAll(extensions);
		}

	@Override
	public EnigmaSettings makeInstance(ResourceReference<EnigmaSettings> ref)
		{
		return new EnigmaSettings(false);
		}

	@Override
	protected PropertyMap<PEnigmaSettings> makePropertyMap()
		{
		return new PropertyMap<PEnigmaSettings>(PEnigmaSettings.class,this,null);
		}

	@Override
	protected void postCopy(EnigmaSettings dest)
		{
		copyInto(dest);
		}

	public boolean equals(Object o)
		{
		if (this == o) return true;
		if (o == null || !(o instanceof EnigmaSettings)) return false;
		EnigmaSettings es = (EnigmaSettings) o;
		if (!es.definitions.equals(definitions) || !es.globalLocals.equals(globalLocals)
				|| !es.initialization.equals(initialization) || !es.cleanup.equals(cleanup)) return false;
		return es.options.equals(options) && es.targets.equals(targets)
				&& es.extensions.equals(extensions);
		}
	}
