package org.enigma;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import org.enigma.messages.Messages;
import org.enigma.utility.EEFReader;
import org.enigma.utility.EEFReader.EEFNode;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Resource.Kind;

public class EFileReader
	{
	public static final String EY = ".ey"; //$NON-NLS-1$
	public static final boolean ADD_EY_ORPHANS = true;

	/**
	 * Interface for registering a method for reading for a kind of resource. An
	 * inheritor would get mapped to a Kind via the <code>readers</code> map.
	 */
	public static interface ResourceReader
		{
		public Resource<?,?> read(EGMFile f, InputStream os, String pathname) throws IOException;
		}

	static abstract class EGMFile
		{
		public abstract EGMFile getEntry(String name);

		public abstract boolean exists();

		public abstract boolean isDirectory(String pathname);

		public abstract InputStream asInputStream() throws IOException;

		public abstract SortedSet<String> getEntries();

		public abstract Set<String> getEntries(String dir);
		}

	static class EGMZipFile extends EGMFile
		{
		ZipFile z;
		private ZipEntry ent;

		Map<String,SortedSet<String>> tree = new TreeMap<String,SortedSet<String>>();

		EGMZipFile(File f) throws IOException
			{
			z = new ZipFile(f);
			Enumeration<? extends ZipEntry> e = z.entries();
			while (e.hasMoreElements())
				{
				ZipEntry ze = e.nextElement();
				String name = ze.getName();
				String[] path = name.split("/");
				String key = new String();
				for (String dir : path)
					{
					SortedSet<String> val = tree.get(key);
					if (val == null) tree.put(key,val = new TreeSet<String>());
					val.add(dir);
					key += '/' + dir;
					}
				}
			}

		public EGMFile getEntry(String name)
			{
			ent = z.getEntry(name);
			return this;
			}

		public boolean exists(String directory, String name)
			{
			Set<String> children = tree.get(directory);
			return (children != null && children.contains(name));
			}

		public boolean exists()
			{
			return ent != null;
			}

		public InputStream asInputStream() throws IOException
			{
			return z.getInputStream(ent);
			}

		public boolean isDirectory(String directory, String name)
			{
			return isDirectory(directory + '/' + name);
			}

		public boolean isDirectory(String pathname)
			{
			if (!pathname.isEmpty() && pathname.charAt(0) != '/') pathname = '/' + pathname;
			Set<String> children = tree.get(pathname);
			return children != null && !children.isEmpty();
			}

		public SortedSet<String> getEntries()
			{
			Enumeration<? extends ZipEntry> ent = z.entries();
			SortedSet<String> r = new TreeSet<String>();
			while (ent.hasMoreElements())
				r.add(ent.nextElement().getName().split("/")[0]);
			return r;
			}

		public SortedSet<String> getEntries(String dir)
			{
			if (!dir.isEmpty() && dir.charAt(0) != '/') dir = '/' + dir;
			return tree.get(dir);
			}
		}

	static class EGMFolderFile extends EGMFile
		{
		File f;
		private File last;

		EGMFolderFile(File f)
			{
			if (!f.isDirectory()) throw new IllegalArgumentException(f + " not directory");
			this.f = f;
			}

		@Override
		public EGMFile getEntry(String name)
			{
			last = new File(f,name);
			return this;
			}

		public boolean exists()
			{
			return last.exists();
			}

		public boolean isDirectory()
			{
			return last.isDirectory();
			}

		public boolean isDirectory(String pathname)
			{
			return new File(f,pathname).isDirectory();
			}

		public InputStream asInputStream() throws IOException
			{
			return new FileInputStream(last);
			}

		public SortedSet<String> getEntries()
			{
			SortedSet<String> r = new TreeSet<String>();
			for (String n : f.list())
				r.add(n);
			return r;
			}

		public SortedSet<String> getEntries(String dir)
			{
			File c = new File(f,dir);
			SortedSet<String> r = new TreeSet<String>();
			for (String n : c.list())
				r.add(n);
			return r;
			}
		}

	//probably won't need this, but in case I do (if not, delete)
	static class ZipOrFile extends ZipEntry
		{
		File f;

		public ZipOrFile(File f)
			{
			super(f.getName());
			this.f = f;
			}

		@Override
		public boolean isDirectory()
			{
			return f.isDirectory();
			}

		@Override
		public String getName()
			{
			return f.getName();
			}

		@Override
		public long getSize()
			{
			return f.length();
			}
		}

	// Module maps
	/** Used to register readers with their resource kinds. */
	static Map<Kind,ResourceReader> readers = new HashMap<Kind,ResourceReader>();
	static Map<String,Kind> strtypes = new HashMap<String,Kind>();
	static
		{
		// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS
		//		readers.put(Kind.SPRITE,new SpriteApngIO());
		//		readers.put(Kind.SOUND,new SoundIO());
		//		readers.put(Kind.BACKGROUND,new BackgroundIO());
		//		readers.put(Kind.PATH,new PathTextIO());
		//		readers.put(Kind.SCRIPT,new ScriptIO());
		//		readers.put(Kind.FONT,new FontRawIO());
		//		readers.put(Kind.TIMELINE,new TimelineIO());
		//		readers.put(Kind.OBJECT,new ObjectIO());
		//		readers.put(Kind.ROOM,new RoomGmDataIO());
		// TODO: MOAR

		strtypes.put("spr",Kind.SPRITE); //$NON-NLS-1$
		strtypes.put("snd",Kind.SOUND); //$NON-NLS-1$
		strtypes.put("bkg",Kind.BACKGROUND); //$NON-NLS-1$
		strtypes.put("pth",Kind.PATH); //$NON-NLS-1$
		strtypes.put("scr",Kind.SCRIPT); //$NON-NLS-1$
		strtypes.put("fnt",Kind.FONT); //$NON-NLS-1$
		strtypes.put("tml",Kind.TIMELINE); //$NON-NLS-1$
		strtypes.put("obj",Kind.OBJECT); //$NON-NLS-1$
		strtypes.put("rom",Kind.ROOM); //$NON-NLS-1$
		strtypes.put("inf",Kind.GAMEINFO); //$NON-NLS-1$
		strtypes.put("set",Kind.GAMESETTINGS); //$NON-NLS-1$
		strtypes.put("ext",Kind.EXTENSIONS); //$NON-NLS-1$
		}

	static Map<String,Kind> folderKinds = new HashMap<String,Kind>();
	static
		{
		folderKinds.put("Sprites",Kind.SPRITE);
		folderKinds.put("Sounds",Kind.SOUND);
		folderKinds.put("Backgrounds",Kind.BACKGROUND);
		folderKinds.put("Paths",Kind.PATH);
		folderKinds.put("Scripts",Kind.SCRIPT);
		folderKinds.put("Fonts",Kind.FONT);
		folderKinds.put("Timelines",Kind.TIMELINE);
		folderKinds.put("Objects",Kind.OBJECT);
		folderKinds.put("Rooms",Kind.ROOM);
		folderKinds.put("Game Information",Kind.GAMEINFO);
		folderKinds.put("Global Game Settings",Kind.GAMESETTINGS);
		folderKinds.put("Extensions",Kind.EXTENSIONS);
		}

	public static void readEgmFile(File f, boolean zip) throws IOException
		{
		readEgmFile(zip ? new EGMZipFile(f) : new EGMFolderFile(f));
		}

	static class Entry
		{
		String name;
		Kind kind;

		Entry(String name, Kind kind)
			{
			this.name = name;
			this.kind = kind;
			}
		}

	public static void readEgmFile(EGMFile f) throws IOException
		{
		GmFile gf = new GmFile();
		ResNode root = new ResNode("Root",(byte) 0,null,null);
		readNodeChildren(f,gf,root,null,new String());
		}

	public static void readNodeChildren(EGMFile f, GmFile gf, ResNode parent, Kind k, String dir)
			throws IOException
		{
		System.out.println(">" + dir);

		Set<String> orphans = f.getEntries(dir);
		Set<String> eyOrphans = new TreeSet<String>();
		for (String s : orphans)
			if (s.endsWith(EY)) eyOrphans.add(s);
		orphans.removeAll(eyOrphans);

		List<Entry> toc = new ArrayList<Entry>();
		f.getEntry(dir.isEmpty() ? "toc.txt" : (dir + "/toc.txt"));
		if (f.exists())
			{
			orphans.remove("toc.txt");
			BufferedReader in = new BufferedReader(new InputStreamReader(f.asInputStream()));

			for (String entry = in.readLine(); entry != null; entry = in.readLine())
				{
				Kind ck = k;
				if (entry.length() > 4 && entry.charAt(3) == ' ')
					{
					ck = strtypes.get(entry.substring(0,3));
					if (ck == null)
						ck = k;
					else
						entry = entry.substring(4);
					}
				if (!orphans.remove(entry)) eyOrphans.remove(entry + EY);
				toc.add(new Entry(entry,ck));
				}
			}

		if (!eyOrphans.isEmpty())
			{
			if (ADD_EY_ORPHANS)
				{
				for (String s : eyOrphans)
					if (s.endsWith(EY)) toc.add(new Entry(s,null));
				System.out.print("Added EY Orphans: ");
				}
			else
				System.out.print("Skipped EY Orphans: ");
			System.out.println(eyOrphans);
			}

		processEntries(f,gf,parent,toc,dir);
		}

	static void processEntries(EGMFile f, GmFile gf, ResNode parent, Iterable<Entry> entries,
			String dir) throws IOException
		{
		for (Entry e : entries)
			{
			String entry = e.name;
			if (!dir.isEmpty()) entry = dir + '/' + entry;
			//			f.getEntry(e.name + "/");
			//			System.out.println(e.name + " " + f.isDirectory(e.name));
			if (f.isDirectory(dir + '/' + e.name))
				{
				byte status = parent.isRoot() ? ResNode.STATUS_PRIMARY : ResNode.STATUS_GROUP;
				readNodeChildren(f,gf,parent.addChild(entry,status,e.kind),e.kind,entry);
				}
			else
				{
				f.getEntry(entry + EY);
				if (f.exists())
					{
					ResourceReference<? extends Resource<?,?>> res = readResource(f,parent,e.kind,
							f.asInputStream(),entry);
					if (res != null) parent.add(new ResNode(e.name,ResNode.STATUS_SECONDARY,e.kind,res));
					}
				else
					System.out.println("Extraneous TOC entry: " + e.name + " (" + e.kind + ")");
				}
			}
		}

	public static ResourceReference<? extends Resource<?,?>> readResource(EGMFile f, ResNode parent,
			Kind kind, InputStream is, String pathname) throws IOException
		{
		System.out.println("_" + pathname);
		ResourceReader reader = readers.get(kind);
		if (reader == null)
			{
			System.err.println(Messages.format("EFileReader.NO_READER",kind)); //$NON-NLS-1$
			return null;
			}
		Resource<?,?> r = reader.read(f,is,pathname);
		return r == null ? null : r.reference;
		}

	public static void mainEef(String args[])
		{
		File home = new File(System.getProperty("user.home")); //$NON-NLS-1$
		File in = new File(home,"inputEEF.eef"); // any of gmd,gm6,gmk,gm81
		try
			{
			EEFReader.Reader reader = new EEFReader.Reader(new FileInputStream(in));
			EEFNode en = reader.getRoot();
			System.out.println(en);
			}
		catch (FileNotFoundException e2)
			{
			// TODO Auto-generated catch block
			e2.printStackTrace();
			}
		}

	public static void main(String args[]) throws IOException
		{
		File f = new File(System.getProperty("user.home"),"outputEgmFile.egm");
		readEgmFile(f,true);
		}
	}
