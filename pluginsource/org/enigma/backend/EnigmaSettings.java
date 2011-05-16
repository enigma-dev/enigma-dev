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
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.enigma.EnigmaRunner;
import org.enigma.TargetHandler;
import org.enigma.TargetHandler.TargetSelection;
import org.enigma.backend.EnigmaDriver.SyntaxError;

public class EnigmaSettings
	{
	public Map<String,String> options = new HashMap<String,String>();

	public String definitions = "", globalLocals = "";
	public String initialization = "", cleanup = "";

	public TargetSelection selCompiler, selPlatform, selGraphics, selAudio, selCollision, selWidgets;

	public EnigmaSettings()
		{
		this(true);
		}

	private EnigmaSettings(boolean load)
		{
		if (!load) return;

		loadDefinitions();

		selCompiler = TargetHandler.defCompiler;
		selPlatform = TargetHandler.defPlatform;
		selGraphics = TargetHandler.defGraphics;
		selAudio = TargetHandler.defAudio;
		selCollision = TargetHandler.defCollision;
		selWidgets = TargetHandler.defWidgets;
		}

	void loadDefinitions()
		{
		definitions = fileToString(new File(EnigmaRunner.WORKDIR,"definitions.h"));
		}

	public void saveDefinitions()
		{
		writeString(new File(EnigmaRunner.WORKDIR,"definitions.h"),definitions);
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

	private String toTargetYaml()
		{
		StringBuilder general = new StringBuilder();
		for (Entry<String,String> entry : options.entrySet())
			general.append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");

		System.out.println(general);

		return "%e-yaml\n---\n"//
				+ general.toString() + "\n"//
				+ "target-compiler: " + (selCompiler == null ? "" : selCompiler.id) + "\n"//
				+ "target-windowing: " + (selPlatform == null ? "" : selPlatform.id) + "\n"//
				+ "target-graphics: " + (selGraphics == null ? "" : selGraphics.id) + "\n"//
				+ "target-audio: " + (selAudio == null ? "" : selAudio.id) + "\n"//
				+ "target-collision: " + (selCollision == null ? "" : selCollision.id) + "\n"//
				+ "target-widget: " + (selWidgets == null ? "" : selWidgets.id) + "\n"//
				+ "target-networking: " + "None" + "\n";//
		}

	public SyntaxError commitToDriver(EnigmaDriver driver)
		{
		return driver.definitionsModified(definitions,toTargetYaml());
		}

	public EnigmaSettings copy()
		{
		EnigmaSettings es = new EnigmaSettings(false);
		copyInto(es);
		return es;
		}

	public void copyInto(EnigmaSettings es)
		{
		es.options.putAll(options);

		es.definitions = definitions;
		es.globalLocals = globalLocals;
		es.initialization = initialization;
		es.cleanup = cleanup;

		es.selCompiler = selCompiler;
		es.selPlatform = selPlatform;
		es.selGraphics = selGraphics;
		es.selAudio = selAudio;
		es.selCollision = selCollision;
		es.selWidgets = selWidgets;
		}
	}
