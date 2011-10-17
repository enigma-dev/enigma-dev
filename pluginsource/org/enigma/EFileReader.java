/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.filechooser.FileView;

import org.enigma.backend.EnigmaSettings;
import org.enigma.messages.Messages;
import org.enigma.utility.APNGExperiments;
import org.enigma.utility.EEFReader;
import org.enigma.utility.EEFReader.EEFNode;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamDecoder;
import org.lateralgm.file.ResourceList;
import org.lateralgm.file.iconio.ICOFile;
import org.lateralgm.main.LGM;
import org.lateralgm.main.Util;
import org.lateralgm.resources.Background;
import org.lateralgm.resources.Background.PBackground;
import org.lateralgm.resources.Extensions;
import org.lateralgm.resources.Font;
import org.lateralgm.resources.Font.PFont;
import org.lateralgm.resources.GameInformation;
import org.lateralgm.resources.GameInformation.PGameInformation;
import org.lateralgm.resources.GameSettings;
import org.lateralgm.resources.GameSettings.PGameSettings;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.GmObject.PGmObject;
import org.lateralgm.resources.InstantiableResource;
import org.lateralgm.resources.Path;
import org.lateralgm.resources.Path.PPath;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.Room.PRoom;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Script.PScript;
import org.lateralgm.resources.Sound;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.resources.Sprite;
import org.lateralgm.resources.Sprite.PSprite;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.library.LibManager;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Argument;
import org.lateralgm.resources.sub.BackgroundDef;
import org.lateralgm.resources.sub.BackgroundDef.PBackgroundDef;
import org.lateralgm.resources.sub.Event;
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.Instance.PInstance;
import org.lateralgm.resources.sub.PathPoint;
import org.lateralgm.resources.sub.Tile;
import org.lateralgm.resources.sub.Tile.PTile;
import org.lateralgm.resources.sub.View;
import org.lateralgm.resources.sub.View.PView;
import org.lateralgm.util.PropertyMap;

public class EFileReader
	{
	public static final String EY = ".ey"; //$NON-NLS-1$
	public static final boolean ADD_EY_ORPHANS = true;

	static List<PostponedRef> postpone = new LinkedList<PostponedRef>();

	static interface PostponedRef
		{
		boolean invoke();
		}

	static class DefaultPostponedRef<K extends Enum<K>> implements PostponedRef
		{
		ResourceList<?> list;
		String name;
		PropertyMap<K> p;
		K key;

		DefaultPostponedRef(ResourceList<?> list, PropertyMap<K> p, K key, String name)
			{
			this.list = list;
			this.p = p;
			this.key = key;
			this.name = name;
			}

		@Override
		public boolean invoke()
			{
			Resource<?,?> temp = list.get(name);
			if (temp != null) p.put(key,temp.reference);
			return temp != null;
			}
		}

	// Module maps
	/** Used to register readers with their resource kinds. */
	static Map<Class<? extends Resource<?,?>>,ResourceReader> readers = new HashMap<Class<? extends Resource<?,?>>,ResourceReader>();
	static
		{
		// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS
		readers.put(Sprite.class,new SpriteApngReader());
		readers.put(Sound.class,new SoundReader());
		readers.put(Background.class,new BackgroundReader());
		readers.put(Path.class,new PathTextReader());
		readers.put(Script.class,new ScriptReader());
		readers.put(Font.class,new FontRawReader());
		//		readers.put(Timeline.class,new TimelineIO());
		readers.put(GmObject.class,new ObjectEefReader());
		readers.put(Room.class,new RoomGmDataReader());
		readers.put(GameInformation.class,new GameInfoRtfReader());
		readers.put(GameSettings.class,new GameSettingsReader());
		readers.put(EnigmaSettings.class,new EnigmaSettingsReader());
		}

	// Modularity Classes
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

	static class EgmEntry
		{
		String name;
		Class<? extends Resource<?,?>> kind;

		EgmEntry(String name, Class<? extends Resource<?,?>> kind)
			{
			this.name = name;
			this.kind = kind;
			}
		}

	/**
	 * Interface for registering a method for reading for a kind of resource. An
	 * inheritor would get mapped to a Kind via the <code>readers</code> map.
	 */
	public static interface ResourceReader
		{
		public Resource<?,?> read(EGMFile f, GmFile gf, InputStream in, String dir, String name)
				throws IOException;
		}

	public static abstract class DataPropReader<R extends InstantiableResource<R,P>, P extends Enum<P>>
			implements ResourceReader
		{
		@Override
		public Resource<R,P> read(EGMFile f, GmFile gf, InputStream in, String dir, String name)
				throws IOException
			{
			Properties i = new Properties();
			i.load(in);

			R r = construct();
			r.setName(name);
			readProperties(gf,r.properties,i);
			readDataFile(f,gf,r,i,dir);

			getList(gf).add(r);
			return r;
			}

		protected void readProperties(GmFile gf, PropertyMap<P> p, Properties i)
			{
			for (Entry<P,Object> e : p.entrySet())
				{
				P key = e.getKey();
				if (allowProperty(key))
					{
					String kn = key.name();
					put(gf,p,key,i.get(kn));
					i.remove(kn);
					}
				}
			}

		/** Subclasses may wish to override this method for custom/no data file handling. */
		protected void readDataFile(EGMFile f, GmFile gf, R r, Properties i, String dir)
				throws IOException
			{
			String fn = i.get("Data").toString(); //$NON-NLS-1$
			if (!dir.isEmpty()) fn = dir + '/' + fn;
			f.getEntry(fn);
			if (f.exists())
				{
				InputStream in = null;
				try
					{
					readData(gf,r,in = f.asInputStream());
					}
				finally
					{
					if (in != null) in.close();
					}
				}
			else
				System.err.println("Data file missing: " + fn);
			}

		@SuppressWarnings({ "unchecked","rawtypes" })
		public static Object convert(String s, Class c)
			{
			if (c == null) throw new ClassCastException(s + " -> null??");
			if (c == String.class) return s;
			if (c == Boolean.class)
				{
				s = s.toLowerCase();
				return !(s.startsWith("0") || s.startsWith("f") || s.startsWith("n")); //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				}
			if (Number.class.isAssignableFrom(c))
				{
				if (c == Integer.class) return Integer.decode(s);
				if (c == Double.class) return Double.valueOf(s);
				if (c == Byte.class) return Byte.decode(s);
				}
			if (Enum.class.isAssignableFrom(c)) return Enum.valueOf(c,s);
			if (c == Color.class) return new Color(Long.decode(s).intValue());
			throw new ClassCastException(s + " -> " + c);
			}

		/**
		 * Subclasses should override this method to explicitly convert
		 * special keys or keys whose default value is null.
		 */
		protected void put(GmFile gf, PropertyMap<P> p, P key, Object o)
			{
			Object def = p.get(key);
			p.put(key,convert(o.toString(),def == null ? null : def.getClass()));
			}

		public static <K extends Enum<K>>void putRef(ResourceList<?> list, PropertyMap<K> p, K key,
				String name)
			{
			PostponedRef pr = new DefaultPostponedRef<K>(list,p,key,name);
			if (!pr.invoke()) postpone.add(pr);
			}

		protected abstract R construct();

		protected abstract void readData(GmFile gf, R r, InputStream in);

		protected abstract ResourceList<R> getList(GmFile gf);

		/**
		 * Returns whether the following property is expected/should be read from the properties file.
		 * Override to exclude reading certain properties.
		 */
		protected boolean allowProperty(P key)
			{
			return true;
			}
		}

	// Constructors
	//TODO: Improve the Folder Chooser
	public static void importEgmFolder()
		{
		JFileChooser chooser = new JFileChooser(new File(".")); //FIXME: Not from there!
		chooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		chooser.setAcceptAllFileFilterUsed(false);

		chooser.setAccessory(new JLabel("<html>This File Chooser is experimental. Use with care.<br/>"
				+ "Select an EGM folder for importing.<br/>" + "Folders recognized as likely EGMs<br/>"
				+ "are denoted with a special icon.</html>"));

		//Any folder containing a "toc.txt" is marked with a special icon.
		chooser.setFileView(new FileView()
			{
				public Icon getIcon(File f)
					{
					if (!new File(f,"toc.txt").exists()) return null;
					return EnigmaRunner.findIcon("syntax.png"); //$NON-NLS-1$
					}
			});
		if (chooser.showOpenDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
		File f = chooser.getSelectedFile();
		//FIXME: If this folder doesn't contain a toc.txt, ask for confirmation 
		importEgmFolder(f);
		}

	public static void importEgmFolder(File folder)
		{
		if (folder == null || !folder.exists() || !folder.isDirectory()) return;
		try
			{
			readEgmFile(folder,LGM.newRoot(),false);
			}
		catch (GmFormatException e)
			{
			e.printStackTrace();
			}
		}

	public static GmFile readEgmFile(File f, ResNode root, boolean zip) throws GmFormatException
		{
		GmFile gf = new GmFile();
		gf.uri = f.toURI();
		try
			{
			readEgmFile(zip ? new EGMZipFile(f) : new EGMFolderFile(f),gf,root);
			return gf;
			}
		catch (IOException e)
			{
			throw new GmFormatException(gf,e);
			}
		}

	public static void readEgmFile(EGMFile f, GmFile gf, ResNode root) throws IOException
		{
		gf.format = EFileWriter.FLAVOR_EGM;
		readNodeChildren(f,gf,root,null,new String());
		root.add(new ResNode(org.lateralgm.messages.Messages.getString("LGM.EXT"),
				ResNode.STATUS_SECONDARY,Extensions.class,null));
		}

	// Workhorse methods
	public static void readNodeChildren(EGMFile f, GmFile gf, ResNode parent,
			Class<? extends Resource<?,?>> k, String dir) throws IOException
		{
		System.out.println(">" + dir);

		Set<String> orphans = f.getEntries(dir);
		Set<String> eyOrphans = new TreeSet<String>();
		for (String s : orphans)
			if (s.endsWith(EY)) eyOrphans.add(s);
		orphans.removeAll(eyOrphans);

		List<EgmEntry> toc = new ArrayList<EgmEntry>();
		f.getEntry(dir.isEmpty() ? "toc.txt" : (dir + "/toc.txt"));
		if (f.exists())
			{
			orphans.remove("toc.txt");
			BufferedReader in = new BufferedReader(new InputStreamReader(f.asInputStream()));

			for (String entry = in.readLine(); entry != null; entry = in.readLine())
				{
				Class<? extends Resource<?,?>> ck = k;
				if (entry.length() > 4 && entry.charAt(3) == ' ')
					{
					String key = entry.substring(0,3);
					ck = Resource.kindsByName3.get(key);
					if (ck == null) ck = Resource.kindsByName3.get(key.toUpperCase());
					if (ck == null)
						ck = k;
					else
						entry = entry.substring(4);
					}
				if (!orphans.remove(entry)) eyOrphans.remove(entry + EY);
				toc.add(new EgmEntry(entry,ck));
				}
			}

		if (!eyOrphans.isEmpty())
			{
			if (ADD_EY_ORPHANS)
				{
				for (String s : eyOrphans)
					if (s.endsWith(EY)) toc.add(new EgmEntry(s,null));
				System.out.print("Added EY Orphans: ");
				}
			else
				System.out.print("Skipped EY Orphans: ");
			System.out.println(eyOrphans);
			}

		processEntries(f,gf,parent,toc,dir);
		}

	public static void processEntries(EGMFile f, GmFile gf, ResNode parent,
			Iterable<EgmEntry> entries, String dir) throws IOException
		{
		for (EgmEntry e : entries)
			{
			String entry = e.name;
			if (!dir.isEmpty()) entry = dir + '/' + entry;
			//			f.getEntry(e.name + "/");
			//			System.out.println(e.name + " " + f.isDirectory(e.name));
			if (f.isDirectory(dir + '/' + e.name))
				{
				byte status = parent.isRoot() ? ResNode.STATUS_PRIMARY : ResNode.STATUS_GROUP;
				readNodeChildren(f,gf,parent.addChild(e.name,status,e.kind),e.kind,entry);
				}
			else
				{
				f.getEntry(entry + EY);
				if (f.exists())
					{
					ResourceReference<? extends Resource<?,?>> res = readResource(f,gf,parent,e.kind,
							f.asInputStream(),dir,e.name);
					parent.add(new ResNode(e.name,ResNode.STATUS_SECONDARY,e.kind,res));
					}
				else
					System.out.println("Extraneous TOC entry: " + e.name + " (" + e.kind + ")");
				}
			}
		}

	public static ResourceReference<? extends Resource<?,?>> readResource(EGMFile f, GmFile gf,
			ResNode parent, Class<? extends Resource<?,?>> kind, InputStream is, String dir, String name)
			throws IOException
		{
		System.out.println("_" + dir + '/' + name);
		ResourceReader reader = readers.get(kind);
		if (reader == null)
			{
			System.err.println(Messages.format("EFileReader.NO_READER",kind)); //$NON-NLS-1$
			return null;
			}
		Resource<?,?> r = reader.read(f,gf,is,dir,name);
		return r == null ? null : r.reference;
		}

	// Modules
	// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS

	static class SpriteApngReader extends DataPropReader<Sprite,PSprite>
		{
		@Override
		protected Sprite construct()
			{
			return new Sprite();
			}

		protected void readData(GmFile gf, Sprite r, InputStream in)
			{
			try
				{
				r.subImages.addAll(APNGExperiments.apngToBufferedImages(in));
				}
			catch (Exception e)
				{
				}
			}

		@Override
		protected ResourceList<Sprite> getList(GmFile gf)
			{
			return gf.sprites;
			}
		}

	static class SoundReader extends DataPropReader<Sound,PSound>
		{
		@Override
		protected Sound construct()
			{
			return new Sound();
			}

		@Override
		protected ResourceList<Sound> getList(GmFile gf)
			{
			return gf.sounds;
			}

		@Override
		protected void readData(GmFile gf, Sound r, InputStream in)
			{
			try
				{
				r.data = Util.readFully(in).toByteArray();
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class BackgroundReader extends DataPropReader<Background,PBackground>
		{
		@Override
		protected Background construct()
			{
			return new Background();
			}

		@Override
		protected ResourceList<Background> getList(GmFile gf)
			{
			return gf.backgrounds;
			}

		@Override
		protected void readData(GmFile gf, Background r, InputStream in)
			{
			try
				{
				((Background) r).setBackgroundImage(ImageIO.read(in));
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class PathTextReader extends DataPropReader<Path,PPath>
		{
		@Override
		protected Path construct()
			{
			return new Path();
			}

		@Override
		protected ResourceList<Path> getList(GmFile gf)
			{
			return gf.paths;
			}

		@Override
		protected void readData(GmFile gf, Path r, InputStream in)
			{
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			try
				{
				for (String s = br.readLine(); s != null; s = br.readLine())
					{
					String spt[] = s.split(" ");
					if (spt.length != 3) continue;
					try
						{
						r.points.add(new PathPoint(Integer.parseInt(spt[0]),Integer.parseInt(spt[1]),
								Integer.parseInt(spt[2])));
						}
					catch (NumberFormatException e)
						{
						continue;
						}
					}
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class ScriptReader extends DataPropReader<Script,PScript>
		{
		@Override
		protected boolean allowProperty(PScript key)
			{
			return false;
			}

		@Override
		protected Script construct()
			{
			return new Script();
			}

		@Override
		protected ResourceList<Script> getList(GmFile gf)
			{
			return gf.scripts;
			}

		@Override
		protected void readData(GmFile gf, Script r, InputStream in)
			{
			try
				{
				r.setCode(new String(Util.readFully(in).toByteArray()));
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class FontRawReader extends DataPropReader<Font,PFont>
		{
		@Override
		protected Font construct()
			{
			return new Font();
			}

		@Override
		protected ResourceList<Font> getList(GmFile gf)
			{
			return gf.fonts;
			}

		@Override
		protected void readDataFile(EGMFile f, GmFile gf, Font r, Properties i, String dir)
				throws IOException
			{
			//Override as no-op. Raw fonts have no data file.
			}

		@Override
		protected void readData(GmFile gf, Font r, InputStream in)
			{
			//Raw fonts have no data file.
			}
		}

	static class ObjectEefReader extends DataPropReader<GmObject,PGmObject>
		{
		@Override
		protected GmObject construct()
			{
			return new GmObject();
			}

		protected void readData(GmFile gf, GmObject r, InputStream in)
			{
			EEFNode en = EEFReader.parse(in);
			System.out.println("EEF Contents:");
			//return;

			GmObject obj = (GmObject) r;
			//			int numEvents = 0;
			// Write the Events super-key
			for (EEFNode mevnode : en.children)
				for (EEFNode evnode : mevnode.children)
					{
					//MainEvent ev = new MainEvent();
					if (evnode.id == null || evnode.id.length < 2)
						{
						System.err.println("FUCK");
						return;
						}
					int mid = Integer.parseInt(evnode.id[1]);
					Event e;

					try
						{
						int id = Integer.parseInt(evnode.id[0]);
						e = new Event(mid,id);
						}
					catch (NumberFormatException nfe)
						{
						e = new Event();
						e.mainId = mid;
						putOtherRef(gf,e,evnode.id[0]);
						}

					obj.mainEvents.get(mid).events.add(e);

					for (EEFNode actnode : evnode.children)
						{
						if (actnode.id == null || actnode.id.length < 2)
							{
							System.err.println("actFUCK");
							continue;
							}
						int aid = Integer.parseInt(actnode.id[0]), lid = Integer.parseInt(actnode.id[1]);
						//LibAction la = new LibAction();
						//Argument args[] = new Argument[1];
						//Action action = new Action(la,args);
						System.out.println("actnode(" + actnode.id[0] + " in " + actnode.id[1] + "): "
								+ actnode.lineAttribs.size() + " arguments");

						LibAction la = LibManager.getLibAction(lid,aid);
						if (la == null)
							{
							System.err.println("FUCK! UNDEFINED ACTION AND THEN SOME.");
							return;
							}
						Action a = e.addAction(la); //assuming the Library was found and la != null

						Map<String,String[]> attribs = actnode.namedAttributes;
						if (attribs.containsKey("not")) a.setNot(true);
						if (attribs.containsKey("relative")) a.setRelative(true);
						String[] targets = attribs.get("applies");
						if (targets != null && targets.length == 1) putAppliesRef(gf,a,targets[0]);

						Argument args[];
						if (la.interfaceKind != LibAction.INTERFACE_CODE)
							{
							args = new Argument[actnode.lineAttribs.size()];
							System.out.println("Name: " + la.name);
							for (int i = 0; i < actnode.lineAttribs.size(); i++)
								{
								switch (la.libArguments[i].kind)
									{
									case Argument.ARG_BACKGROUND:
									case Argument.ARG_FONT:
									case Argument.ARG_PATH:
									case Argument.ARG_ROOM:
									case Argument.ARG_SCRIPT:
									case Argument.ARG_SOUND:
									case Argument.ARG_SPRITE:
									case Argument.ARG_TIMELINE:
										args[i] = new Argument(la.libArguments[i].kind,
												actnode.lineAttribs.get(i).trim(),null);
										putArgumentRef(gf,args[i],actnode.lineAttribs.get(i).trim());
										break;
									default:
										args[i] = new Argument(la.libArguments[i].kind,
												actnode.lineAttribs.get(i).trim(),null);
										System.out.println("  Argument(" + actnode.lineAttribs.get(i).trim() + ")");
										break;
									}
								}
							}
						else
							{
							args = new Argument[1];
							int size = actnode.lineAttribs.size();
							String code = size == 0 ? new String() : actnode.lineAttribs.get(0);
							for (int i = 1; i < size; i++)
								code += '\n' + actnode.lineAttribs.get(i);
							args[0] = new Argument(la.libArguments[0].kind,code,null);
							}
						a.setArguments(args);
						//arguments, relative, appliesTo, not
						}
					}
			}

		static void putAppliesRef(final GmFile gf, final Action act, final String name)
			{
			PostponedRef pr = new PostponedRef()
				{
					@Override
					public boolean invoke()
						{
						if (name.equals("self") || name.equals("-1")) return true; //already self
						if (name.equals("other") || name.equals("-2"))
							{
							act.setAppliesTo(GmObject.OBJECT_OTHER);
							return true;
							}
						GmObject temp = gf.gmObjects.get(name);
						if (temp != null) act.setAppliesTo(temp.reference);
						return temp != null;
						}
				};
			if (!pr.invoke()) postpone.add(pr);
			}

		static void putOtherRef(final GmFile gf, final Event e, final String name)
			{
			PostponedRef pr = new PostponedRef()
				{
					@Override
					public boolean invoke()
						{
						GmObject temp = gf.gmObjects.get(name);
						if (temp != null) e.other = temp.reference;
						return temp != null;
						}
				};
			if (!pr.invoke()) postpone.add(pr);
			}

		static void putArgumentRef(final GmFile gf, final Argument arg, final String name)
			{
			PostponedRef pr = new PostponedRef()
				{
					@Override
					public boolean invoke()
						{
						GmObject temp = gf.gmObjects.get(name);
						if (temp != null) arg.setRes(temp.reference);
						return temp != null;
						}
				};
			if (!pr.invoke()) postpone.add(pr);
			}

		@Override
		protected ResourceList<GmObject> getList(GmFile gf)
			{
			return gf.gmObjects;
			}

		@Override
		protected void put(GmFile gf, PropertyMap<PGmObject> p, PGmObject key, Object o)
			{
			if (key == PGmObject.SPRITE || key == PGmObject.MASK)
				{
				putRef(gf.sprites,p,key,o.toString());
				return;
				}
			if (key == PGmObject.PARENT)
				{
				putRef(gf.gmObjects,p,key,o.toString());
				return;
				}
			super.put(gf,p,key,o);
			}
		}

	static class RoomGmDataReader extends DataPropReader<Room,PRoom>
		{
		@Override
		protected Room construct()
			{
			return new Room();
			}

		@Override
		protected ResourceList<Room> getList(GmFile gf)
			{
			return gf.rooms;
			}

		@Override
		protected void readData(GmFile gf, Room rm, InputStream in2)
			{
			try
				{
				GmStreamDecoder in = new GmStreamDecoder(in2);
				int nobackgrounds = in.read4();
				for (int j = 0; j < nobackgrounds; j++)
					{
					BackgroundDef bk = rm.backgroundDefs.get(j);
					in.readBool(bk.properties,PBackgroundDef.VISIBLE,PBackgroundDef.FOREGROUND);
					putRef(gf.backgrounds,bk.properties,PBackgroundDef.BACKGROUND,in.readStr());
					//					Background temp = gf.backgrounds.getUnsafe(in.read4());
					//					if (temp != null) bk.properties.put(PBackgroundDef.BACKGROUND,temp.reference);
					in.read4(bk.properties,PBackgroundDef.X,PBackgroundDef.Y);
					in.readBool(bk.properties,PBackgroundDef.TILE_HORIZ,PBackgroundDef.TILE_VERT);
					in.read4(bk.properties,PBackgroundDef.H_SPEED,PBackgroundDef.V_SPEED);
					bk.properties.put(PBackgroundDef.STRETCH,in.readBool());
					}
				//				rm.put(PRoom.ENABLE_VIEWS,in.readBool());
				int noviews = in.read4();
				for (int j = 0; j < noviews; j++)
					{
					View vw = rm.views.get(j);
					in.readBool(vw.properties,PView.VISIBLE);
					//vw.properties.put(PView.VISIBLE,in.readBool());
					in.read4(vw.properties,PView.VIEW_X,PView.VIEW_Y,PView.VIEW_W,PView.VIEW_H,PView.PORT_X,
							PView.PORT_Y,PView.PORT_W,PView.PORT_H,PView.BORDER_H,PView.BORDER_V,PView.SPEED_H,
							PView.SPEED_V);
					putRef(gf.gmObjects,vw.properties,PView.OBJECT,in.readStr());
					//					GmObject temp = f.gmObjects.getUnsafe(in.read4());
					//					if (temp != null) vw.properties.put(PView.OBJECT,temp.reference);
					}
				int noinstances = in.read4();
				for (int j = 0; j < noinstances; j++)
					{
					Instance inst = rm.addInstance();
					inst.setPosition(new Point(in.read4(),in.read4()));
					putRef(gf.gmObjects,inst.properties,PInstance.OBJECT,in.readStr());
					//					GmObject temp = f.gmObjects.getUnsafe(in.read4());
					//					if (temp != null) inst.properties.put(PInstance.OBJECT,temp.reference);
					inst.properties.put(PInstance.ID,in.read4());
					inst.setCreationCode(in.readStr());
					inst.setLocked(in.readBool());
					}
				int notiles = in.read4();
				for (int j = 0; j < notiles; j++)
					{
					Tile t = new Tile(rm);
					t.setRoomPosition(new Point(in.read4(),in.read4()));
					putRef(gf.backgrounds,t.properties,PTile.BACKGROUND,in.readStr());
					//					Background temp = f.backgrounds.getUnsafe(in.read4());
					//					ResourceReference<Background> bkg = null;
					//					if (temp != null) bkg = temp.reference;
					//					t.properties.put(PTile.BACKGROUND,bkg);
					t.setBackgroundPosition(new Point(in.read4(),in.read4()));
					t.setSize(new Dimension(in.read4(),in.read4()));
					t.setDepth(in.read4());
					t.properties.put(PTile.ID,in.read4());
					t.setLocked(in.readBool());
					rm.tiles.add(t);
					}
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class GameInfoRtfReader implements ResourceReader
		{
		@Override
		public Resource<?,?> read(EGMFile f, GmFile gf, InputStream in, String dir, String name)
				throws IOException
			{
			Properties i = new Properties();
			i.load(in);

			GameInformation r = gf.gameInfo;
			readProperties(gf,r.properties,i);
			readDataFile(f,gf,r,i,dir);

			return null;
			}

		protected static void put(GmFile gf, PropertyMap<PGameInformation> p, PGameInformation key, Object o)
			{
			Object def = p.get(key);
			p.put(key,DataPropReader.convert(o.toString(),def == null ? null : def.getClass()));
			}

		protected static void readProperties(GmFile gf, PropertyMap<PGameInformation> p, Properties i)
			{
			for (Entry<PGameInformation,Object> e : p.entrySet())
				{
				PGameInformation key = e.getKey();
				if (allowProperty(key))
					{
					String kn = key.name();
					put(gf,p,key,i.get(kn));
					i.remove(kn);
					}
				}
			}

		protected static boolean allowProperty(PGameInformation key)
			{
			return key != PGameInformation.TEXT;
			}

		private static void readDataFile(EGMFile f, GmFile gf, GameInformation r, Properties i, String dir)
				throws IOException
			{
			String fn = i.get("Data").toString(); //$NON-NLS-1$
			if (!dir.isEmpty()) fn = dir + '/' + fn;
			f.getEntry(fn);
			if (f.exists())
				{
				InputStream in = null;
				try
					{
					readData(gf,r,in = f.asInputStream());
					}
				finally
					{
					if (in != null) in.close();
					}
				}
			else
				System.err.println("Data file missing: " + fn);
			}

		private static void readData(GmFile gf, GameInformation r, InputStream in)
			{
			try
				{
				r.put(PGameInformation.TEXT,Util.readFully(in).toString());
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			}
		}

	static class GameSettingsReader implements ResourceReader
		{
		@Override
		public Resource<?,?> read(EGMFile f, GmFile gf, InputStream in, String dir, String name)
				throws IOException
			{
			Properties i = new Properties();
			i.load(in);

			GameSettings r = gf.gameSettings;
			//			for (Entry<?,?> e : r.properties.entrySet())
			//				System.out.println(e.getKey() + ": " + e.getValue());
			//
			readProperties(gf,r.properties,i);
			readDataFiles(f,gf,r,i,dir);

			return null;
			}

		protected static void put(GmFile gf, PropertyMap<PGameSettings> p, PGameSettings key, Object o)
			{
			Object def = p.get(key);
			p.put(key,DataPropReader.convert(o.toString(),def == null ? null : def.getClass()));
			}

		protected static void readProperties(GmFile gf, PropertyMap<PGameSettings> p, Properties i)
			{
			for (Entry<PGameSettings,Object> e : p.entrySet())
				{
				PGameSettings key = e.getKey();
				if (allowProperty(key))
					{
					String kn = key.name();
					put(gf,p,key,i.get(kn));
					i.remove(kn);
					}
				}
			}

		protected static boolean allowProperty(PGameSettings prop)
			{
			//			return false;
			return prop != PGameSettings.DPLAY_GUID && prop != PGameSettings.GAME_ICON
					&& prop != PGameSettings.FRONT_LOAD_BAR && prop != PGameSettings.BACK_LOAD_BAR
					&& prop != PGameSettings.LOADING_IMAGE;
			}

		private static void readDataFiles(EGMFile f, GmFile gf, GameSettings r, Properties i, String dir)
				throws IOException
			{
			String[] entries = { "Icon","Splash","Filler","Progress" }; //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$ //$NON-NLS-4$
			PGameSettings[] keys = { PGameSettings.GAME_ICON,PGameSettings.LOADING_IMAGE,
					PGameSettings.BACK_LOAD_BAR,PGameSettings.FRONT_LOAD_BAR };

			for (int j = 0; j < entries.length; j++)
				{
				Object o = i.get(entries[j]);
				if (o == null) continue;
				String fn = o.toString();
				if (fn.isEmpty() || fn.equals("null")) continue;
				if (!dir.isEmpty()) fn = dir + '/' + fn;
				f.getEntry(fn);
				if (f.exists())
					{
					InputStream in = f.asInputStream();
					try
						{
						if (j == 0)
							r.put(keys[j],new ICOFile(in));
						else
							r.put(keys[j],ImageIO.read(in));
						}
					finally
						{
						if (in != null) in.close();
						}
					}
				else
					System.err.println("Game Settings Data file missing: " + fn);
				}
			}
		}

	static class EnigmaSettingsReader implements ResourceReader
		{
		@Override
		public Resource<?,?> read(EGMFile f, GmFile gf, InputStream in, String dir, String name)
				throws IOException
			{
			//			YamlNode node = YamlParser.parse(new Scanner(in));
			Properties i = new Properties();
			i.load(in);

			EnigmaSettings es = EnigmaRunner.es;
			es.fromProperties(i);
			readDataFile(f,gf,es,i,dir);
			return null;
			}

		protected static void readDataFile(EGMFile f, GmFile gf, EnigmaSettings r, Properties i, String dir)
				throws IOException
			{
			String fn = i.getProperty("Data"); //$NON-NLS-1$
			if (!dir.isEmpty()) fn = dir + '/' + fn;
			f.getEntry(fn);
			if (!f.exists())
				{
				System.err.println("Enigma Settings Data file missing: " + fn);
				return;
				}
			InputStream in = null;
			try
				{
				readData(f,gf,r,in = f.asInputStream());
				}
			finally
				{
				if (in != null) in.close();
				}
			}

		private static void readData(EGMFile f, GmFile gf, EnigmaSettings r, InputStream in)
			{
			Set<String> names = new HashSet<String>();
			Collections.addAll(names,"definitions","globallocals","initialization","cleanup");

			EEFNode en = EEFReader.parse(in);
			if (en.children.size() < 1)
				{
				System.err.println("Enigma Settings Data file empty");
				return;
				}

			en = en.children.get(0);
			for (EEFNode code : en.children)
				{
				if (code.id.length == 0) continue;
				String id = code.id[0].toLowerCase();
				if (!names.remove(id)) continue;

				int size = code.lineAttribs.size();
				String str = size == 0 ? new String() : code.lineAttribs.get(0);
				for (int i = 1; i < size; i++)
					str += '\n' + code.lineAttribs.get(i);

				if (id.equals("definitions"))
					r.definitions = str;
				else if (id.equals("globallocals"))
					r.globalLocals = str;
				else if (id.equals("initialization"))
					r.initialization = str;
				else if (id.equals("cleanup")) r.cleanup = str;
				}
			}
		}
	}
