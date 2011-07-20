/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.imageio.ImageIO;

import org.enigma.messages.Messages;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFileReader;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.main.Util;
import org.lateralgm.resources.Background;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Sound;
import org.lateralgm.resources.Sprite;
import org.lateralgm.resources.Resource.Kind;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.util.PropertyMap;

public class EFileWriter
	{
	public static final String EY = ".ey"; //$NON-NLS-1$

	// Modularity Classes
	public static abstract class EGMOutputStream
		{
		protected PrintStream last;

		public PrintStream next(final List<String> directory, String filename) throws IOException
			{
			if (last != null) _finishLast();
			return last = _next(directory,filename);
			}

		protected abstract PrintStream _next(final List<String> directory, String filename)
				throws IOException;

		protected abstract void _finishLast();

		public abstract void finish() throws IOException;

		public void close()
			{
			if (last == null) return;
			_finishLast();
			last.close();
			last = null;
			}
		}

	public static class EGMZip extends EGMOutputStream
		{
		public static final String SEPARATOR = "/"; //$NON-NLS-1$

		protected ZipOutputStream os;

		public EGMZip(OutputStream os)
			{
			this(new ZipOutputStream(os));
			}

		public EGMZip(ZipOutputStream os)
			{
			this.os = os;
			last = new PrintStream(os);
			}

		@Override
		protected void _finishLast()
			{
			last.flush();
			}

		@Override
		protected PrintStream _next(List<String> directory, String filename) throws IOException
			{
			StringBuilder path = new StringBuilder();
			for (String s : directory)
				path.append(s).append(SEPARATOR);
			path.append(filename);
			os.putNextEntry(new ZipEntry(path.toString()));
			return last;
			}

		@Override
		public void finish() throws IOException
			{
			last.flush();
			os.finish();
			}
		}

	public static class EGMFolder extends EGMOutputStream
		{
		File root;

		public EGMFolder(File root)
			{
			this.root = root;
			if (!root.mkdir())
			/* FIXME: Unable to mkdir, what do? */;
			}

		@Override
		protected void _finishLast()
			{
			last.close();
			}

		@Override
		protected PrintStream _next(List<String> directory, String filename) throws IOException
			{
			File f = root;
			for (String s : directory)
				f = new File(root,s);
			return new PrintStream(new File(f,filename));
			}

		@Override
		public void finish() throws IOException
			{
			last.close();
			}
		}

	/**
	 * Interface for registering a method for writing for a kind of resource. An
	 * inheritor would get mapped to a Kind via the <code>writers</code> map.
	 */
	public static interface ResourceWriter
		{
		public void write(EGMOutputStream os, ResNode child, List<String> dir) throws IOException;
		}

	/**
	 * Convenience wrapper module manager for standard resources. A standard
	 * resource will have an ey properties file which points to a data file with
	 * a designated extension. This eliminates the need to deal with ResNodes
	 * and Zip technicalities and just focus on writing the respective files.
	 */
	public static abstract class DataResourceWriter implements ResourceWriter
		{
		public void write(EGMOutputStream os, ResNode child, List<String> dir) throws IOException
			{
			String name = (String) child.getUserObject();
			Resource<?,?> r = (Resource<?,?>) Util.deRef((ResourceReference<?>) child.getRes());
			String fn = name + getExt(r);

			PrintStream ps = os.next(dir,name + EY);
			ps.println("Data: " + fn); //$NON-NLS-1$
			writeProperties(ps,r);

			ps = os.next(dir,fn);
			writeData(ps,r);
			}

		/**
		 * The extension, which must include the preceding dot, e.g. ".ext".
		 * Resource provided in case extension is chosen on an individual
		 * resource basis.
		 */
		public abstract String getExt(Resource<?,?> r);

		public abstract void writeProperties(PrintStream os, Resource<?,?> r) throws IOException;

		public abstract void writeData(PrintStream os, Resource<?,?> r) throws IOException;
		}

	/**
	 * Convenience wrapper for automatically writing allowed properties.
	 * 
	 * @see DataResourceWriter
	 */
	static abstract class DataPropWriter extends DataResourceWriter
		{
		@Override
		public void writeProperties(PrintStream os, Resource<?,?> r) throws IOException
			{
			PropertyMap<? extends Enum<?>> p = r.properties;
			for (Entry<? extends Enum<?>,Object> e : p.entrySet())
				if (allowProperty(e.getKey().name())) os.println(e.getKey().name() + ": " + e.getValue()); //$NON-NLS-1$
			}

		/**
		 * Returns whether the following property should be allowed in the
		 * properties file
		 */
		public abstract boolean allowProperty(String name);
		}

	// Module maps
	/** Used to register writers with their resource kinds. */
	static Map<Kind,ResourceWriter> writers = new HashMap<Kind,ResourceWriter>();
	static Map<Kind,String> typestrs = new HashMap<Kind,String>();
	static
		{
		// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS
		writers.put(Kind.SPRITE,new SpriteIO());
		writers.put(Kind.SOUND,new SoundIO());
		writers.put(Kind.BACKGROUND,new BackgroundIO());
		// writers.put(Kind.PATH,new PathIO());
		writers.put(Kind.SCRIPT,new ScriptIO());
		// writers.put(Kind.FONT,new FontIO());
		// writers.put(Kind.TIMELINE,new TimelineIO());
		// writers.put(Kind.OBJECT,new ObjectIO());
		// writers.put(Kind.ROOM,new RoomIO());
		// TODO: MOAR

		typestrs.put(Kind.SPRITE,"spr"); //$NON-NLS-1$
		typestrs.put(Kind.SOUND,"snd"); //$NON-NLS-1$
		typestrs.put(Kind.BACKGROUND,"bkg"); //$NON-NLS-1$
		typestrs.put(Kind.PATH,"pth"); //$NON-NLS-1$
		typestrs.put(Kind.SCRIPT,"scr"); //$NON-NLS-1$
		typestrs.put(Kind.FONT,"fnt"); //$NON-NLS-1$
		typestrs.put(Kind.TIMELINE,"tml"); //$NON-NLS-1$
		typestrs.put(Kind.OBJECT,"obj"); //$NON-NLS-1$
		typestrs.put(Kind.ROOM,"rom"); //$NON-NLS-1$
		typestrs.put(Kind.GAMEINFO,"inf"); //$NON-NLS-1$
		typestrs.put(Kind.GAMESETTINGS,"set"); //$NON-NLS-1$
		typestrs.put(Kind.EXTENSIONS,"ext"); //$NON-NLS-1$
		}

	// Constructors
	public static void writeEgmFile(File loc, ResNode tree, boolean zip)
		{
		try
			{
			EGMOutputStream out;
			if (zip)
				out = new EGMZip(new FileOutputStream(loc));
			else
				out = new EGMFolder(loc);
			writeEgmFile(out,tree);
			out.close();
			}
		catch (IOException e)
			{
			// TODO Auto-generated catch block
			e.printStackTrace();
			}
		}

	public static void writeEgmFile(EGMOutputStream os, ResNode tree) throws IOException
		{
		writeNodeChildren(os,tree,new ArrayList<String>());
		}

	// Workhorse methods
	/** Recursively writes out the tree nodes into the zip. */
	public static void writeNodeChildren(EGMOutputStream os, ResNode node, List<String> dir)
			throws IOException
		{
		PrintStream ps = os.next(dir,"toc.txt"); //$NON-NLS-1$

		int children = node.getChildCount();
		for (int i = 0; i < children; i++)
			{
			ResNode child = (ResNode) node.getChildAt(i);
			ps.println((String) child.getUserObject());
			}

		for (int i = 0; i < children; i++)
			{
			ResNode child = ((ResNode) node.getChildAt(i));

			if (child.status == ResNode.STATUS_SECONDARY)
				{
				writeResource(os,child,dir);
				}
			else
				{
				String cn = (String) child.getUserObject();
				// String cdir = dir + child.getUserObject() + SEPARATOR;
				ArrayList<String> newDir = new ArrayList<String>(dir);
				newDir.add(cn);
				//				os.next(newDir, null);
				writeNodeChildren(os,child,newDir);
				}
			}
		}

	/**
	 * Looks up the registered writer for this resource and invokes the write
	 * method
	 */
	public static void writeResource(EGMOutputStream os, ResNode child, List<String> dir)
			throws IOException
		{
		ResourceWriter writer = writers.get(child.kind);
		if (writer == null)
			{
			System.err.println(Messages.format("EFileWriter.NO_WRITER",child.kind)); //$NON-NLS-1$
			return;
			}
		writer.write(os,child,dir);
		}

	// Modules
	// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS

	static class SpriteIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".apng"; //$NON-NLS-1$
			}

		@Override
		public void writeData(PrintStream os, Resource<?,?> r) throws IOException
			{
			ArrayList<BufferedImage> subs = ((Sprite) r).subImages;
			//TODO: Convert subs to APNG, write to os
			}
		

		@Override
		public boolean allowProperty(String name)
			{
			return true;
			}
		}

	static class SoundIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ((Sound) r).properties.get(PSound.FILE_TYPE);
			}

		@Override
		public void writeData(PrintStream os, Resource<?,?> r) throws IOException
			{
			os.write(((Sound) r).data);
			}

		@Override
		public boolean allowProperty(String name)
			{
			return true;
			}
		}

	static class BackgroundIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".png"; //$NON-NLS-1$
			}

		@Override
		public void writeData(PrintStream os, Resource<?,?> r) throws IOException
			{
			ImageIO.write(((Background) r).getBackgroundImage(),"png",os); //$NON-NLS-1$
			}

		@Override
		public boolean allowProperty(String name)
			{
			return true;
			}
		}

	static class ScriptIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".scr"; //$NON-NLS-1$
			}

		@Override
		public void writeData(PrintStream os, Resource<?,?> r) throws IOException
			{
			os.print(((Script) r).getCode());
			}

		@Override
		public boolean allowProperty(String name)
			{
			return false;
			}
		}

	// TODO: MOAR MODULES

	public static void main(String[] args) throws GmFormatException
		{
		File home = new File(System.getProperty("user.home")); //$NON-NLS-1$

		File in = new File(home,"inputGmFile.gm81"); // any of gmd,gm6,gmk,gm81
		File out = new File(home,"outputEgmFile.egm"); // must be egm

		ResNode root = new ResNode("Root",(byte) 0,null,null); //$NON-NLS-1$;

		GmFileReader.readGmFile(in.getPath(),root);

		writeEgmFile(out,root,true);
		}
	}
