/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.zip.CRC32;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.imageio.ImageIO;

import org.enigma.messages.Messages;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.main.Util;
import org.lateralgm.resources.Background;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Sound;
import org.lateralgm.resources.Resource.Kind;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.util.PropertyMap;

public class EFileWriter
	{
	public static final String EY = ".ey"; //$NON-NLS-1$

	//Modularity Classes
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
	 * Interface for registering a method for writing for a kind of resource.
	 * An inheritor would get mapped to a Kind via the <code>writers</code> map.
	 */
	public static interface ResourceWriter
		{
		public void write(EGMOutputStream os, ResNode child, List<String> dir) throws IOException;
		}

	/**
	 * Convenience wrapper module manager for standard resources.
	 * A standard resource will have an ey properties file which
	 * points to a data file with a designated extension.
	 * This eliminates the need to deal with ResNodes and Zip technicalities
	 * and just focus on writing the respective files.
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
		 * Resource provided in case extension is chosen on an individual resource basis.
		 */
		public abstract String getExt(Resource<?,?> r);

		public abstract void writeProperties(PrintStream os, Resource<?,?> r) throws IOException;

		public abstract void writeData(PrintStream os, Resource<?,?> r) throws IOException;
		}

	/**
	 * Convenience wrapper for automatically writing allowed properties.
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

		/** Returns whether the following property should be allowed in the properties file */
		public abstract boolean allowProperty(String name);
		}

	//Module maps
	/** Used to register writers with their resource kinds. */
	static Map<Kind,ResourceWriter> writers = new HashMap<Kind,ResourceWriter>();
	static Map<Kind,String> typestrs = new HashMap<Kind,String>();
	static
		{
		//SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS
		writers.put(Kind.SOUND,new SoundIO());
		writers.put(Kind.BACKGROUND,new BackgroundIO());
		writers.put(Kind.SCRIPT,new ScriptIO());

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

	//Constructors
	public static void writeEgmFile(File loc, ResNode tree)
		{
		try
			{
			FileOutputStream fos = new FileOutputStream(loc);
			writeEgmFile(fos,tree);
			fos.close();
			}
		catch (IOException e)
			{
			// TODO Auto-generated catch block
			e.printStackTrace();
			}
		}

	public static void writeEgmFile(OutputStream os, ResNode tree) throws IOException
		{
		EGMOutputStream z = new EGMZip(os);
		writeEgmFile(z,tree);
		z.finish();
		}

	public static void writeEgmFile(EGMOutputStream os, ResNode tree) throws IOException
		{
		writeNodeChildren(os,tree,new ArrayList<String>());
		}

	//Workhorse methods
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
				//				String cdir = dir + child.getUserObject() + SEPARATOR;
				os.next(dir,cn);
				ArrayList<String> newDir = new ArrayList<String>(dir);
				newDir.add(cn);
				writeNodeChildren(os,child,newDir);
				}
			}
		}

	/** Looks up the registered writer for this resource and invokes the write method */
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

	//Modules
	//SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS

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

	/////////////////////////////////////////////////
	// Below are experimental APNG support methods //
	/////////////////////////////////////////////////

	APNG readApng(File f) throws IOException
		{
		return readApng(new FileInputStream(f));
		}

	static final long PNG_MAGIC = 0x89504E470D0A1A0AL;
	static final int IHDR_TYPE = 0x49484452;
	static final int IEND_TYPE = 0x49454E44;
	static final int IDAT_TYPE = 0x49444154;

	static final int acTL_TYPE = 0x6163544C;
	static final int fcTL_TYPE = 0x6663544C;
	static final int fdAT_TYPE = 0x66644154;

	APNG readApng(InputStream is) throws IOException
		{
		return null;
		}

	/**
	 * This method attempts to read an animated PNG by disassembling and then repeatedly gluing for ImageIO.
	 * Basically, we pull out the aCTL chunk, and every other DAT chunk except the one we're parsing.
	 * We then repeat this process for each DAT chunk.
	 * @param is
	 * @throws IOException
	 */
	APNG readApngHack(InputStream is) throws IOException
		{
		DataInputStream in = new DataInputStream(is);
		long magic = in.readLong();
		if (magic != PNG_MAGIC) throw new IOException(String.format("%016X",magic) + " != png"); //$NON-NLS-1$ //$NON-NLS-2$

		Chunk c = readChunk(in);
		if (c.id != IHDR_TYPE) throw new IOException(String.format("%08X",c.id) + " != IHDR"); //$NON-NLS-1$ //$NON-NLS-2$

		IHDR h = new IHDR();
		DataInputStream d = new DataInputStream(c.data);
		h.width = d.readInt();
		h.height = d.readInt();
		h.bitDepth = d.readByte();
		h.colType = d.readByte();
		h.compression = d.readByte();
		h.filter = d.readByte();
		h.interlace = d.readByte();
		d.reset();

		APNG r = new APNG(h);

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		DataOutputStream dos = new DataOutputStream(baos);
		dos.writeLong(PNG_MAGIC);
		byte[] buf = new byte[d.available()];
		d.read(buf);
		dos.write(buf);

		while ((c = readChunk(in)).id != IEND_TYPE)
			{
			switch (c.id)
				{
				//			case
				}
			}

		return r;
		}

	Chunk readChunk(DataInputStream in) throws IOException
		{
		int size = in.readInt();
		byte[] buf = new byte[size + 4];
		in.read(buf);
		int crc1 = crc(buf), crc2 = in.readInt();
		if (crc1 != crc2) throw new IOException("CRC " + crc1 + " != " + crc2); //$NON-NLS-1$ //$NON-NLS-2$
		ByteArrayInputStream data = new ByteArrayInputStream(buf);

		byte[] bid = new byte[4];
		data.read(bid);
		int id = (bid[0] << 24) | (bid[1] << 16) | (bid[2] << 8) | bid[3];
		return new Chunk(size,id,data);
		}

	class APNG extends ArrayList<BufferedImage>
		{
		private static final long serialVersionUID = 1L;
		IHDR header;

		public APNG(IHDR h)
			{
			header = h;
			}
		}

	class Chunk
		{
		int size, id;
		InputStream data;

		Chunk(int s, int i, InputStream d)
			{
			size = s;
			id = i;
			data = d;
			}
		}

	class IHDR
		{
		int width, height;
		byte bitDepth, colType, compression, filter, interlace;
		}

	static int crc(byte[] b)
		{
		CRC32 crc = new CRC32();
		crc.update(b);
		return (int) crc.getValue();
		}
	}
