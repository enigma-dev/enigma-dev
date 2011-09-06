package org.enigma;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
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
import java.util.Properties;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.Map.Entry;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import javax.imageio.ImageIO;

import org.enigma.messages.Messages;
import org.enigma.utility.APNGExperiments;
import org.enigma.utility.EEFReader;
import org.enigma.utility.EEFReader.EEFNode;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmStreamDecoder;
import org.lateralgm.file.ResourceList;
import org.lateralgm.file.iconio.ICOFile;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.Background;
import org.lateralgm.resources.Font;
import org.lateralgm.resources.GameInformation;
import org.lateralgm.resources.GameSettings;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.Path;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Sound;
import org.lateralgm.resources.Sprite;
import org.lateralgm.resources.Background.PBackground;
import org.lateralgm.resources.Font.PFont;
import org.lateralgm.resources.GameInformation.PGameInformation;
import org.lateralgm.resources.GameSettings.PGameSettings;
import org.lateralgm.resources.GmObject.PGmObject;
import org.lateralgm.resources.Path.PPath;
import org.lateralgm.resources.Resource.Kind;
import org.lateralgm.resources.Room.PRoom;
import org.lateralgm.resources.Script.PScript;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.resources.Sprite.PSprite;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.library.LibManager;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Argument;
import org.lateralgm.resources.sub.BackgroundDef;
import org.lateralgm.resources.sub.Event;
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.PathPoint;
import org.lateralgm.resources.sub.Tile;
import org.lateralgm.resources.sub.View;
import org.lateralgm.resources.sub.BackgroundDef.PBackgroundDef;
import org.lateralgm.resources.sub.Instance.PInstance;
import org.lateralgm.resources.sub.Tile.PTile;
import org.lateralgm.resources.sub.View.PView;
import org.lateralgm.util.PropertyMap;

public class EFileReader
	{
	public static final String EY = ".ey"; //$NON-NLS-1$
	public static final boolean ADD_EY_ORPHANS = true;

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
		Kind kind;

		EgmEntry(String name, Kind kind)
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

	public static abstract class DataPropReader<R extends Resource<R,P>, P extends Enum<P>> implements
			ResourceReader
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

		@SuppressWarnings("unchecked")
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
			Resource<?,?> temp = list.get(name);
			if (temp != null) p.put(key,temp.reference);
			//FIXME: If temp is null, postpone for later
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
		readers.put(Kind.SPRITE,new SpriteApngReader());
		readers.put(Kind.SOUND,new SoundReader());
		readers.put(Kind.BACKGROUND,new BackgroundReader());
		readers.put(Kind.PATH,new PathTextReader());
		readers.put(Kind.SCRIPT,new ScriptReader());
		readers.put(Kind.FONT,new FontRawReader());
		//		readers.put(Kind.TIMELINE,new TimelineIO());
		readers.put(Kind.OBJECT,new ObjectEefReader());
		readers.put(Kind.ROOM,new RoomGmDataReader());
		readers.put(Kind.GAMEINFO,new GameInfoRtfReader());
		readers.put(Kind.GAMESETTINGS,new GameSettingsReader());

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

	// Constructors
	public static void readEgmFile(File f, boolean zip) throws IOException
		{
		readEgmFile(zip ? new EGMZipFile(f) : new EGMFolderFile(f));
		}

	public static void readEgmFile(EGMFile f) throws IOException
		{
		LGM.main(new String[0]);
		GmFile gf = new GmFile();
		ResNode root = new ResNode("Root",(byte) 0,null,null);
		readNodeChildren(f,gf,root,null,new String());
		LGM.currentFile = gf;
		LGM.root = root;
		LGM.reload(true);
		}

	// Workhorse methods
	public static void readNodeChildren(EGMFile f, GmFile gf, ResNode parent, Kind k, String dir)
			throws IOException
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
			ResNode parent, Kind kind, InputStream is, String dir, String name) throws IOException
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
				r.data = readFully(in).toByteArray();
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
				r.setCode(new String(readFully(in).toByteArray()));
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

		protected void readDataFile(EGMFile f, Font r, Properties i, String dir) throws IOException
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
			EEFReader eef = new EEFReader(in,",");
			System.out.println("EEF Contents:");
			EEFNode en = eef.getRoot();
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
						if (evnode.id == null || evnode.id.length < 2)
							{
							System.err.println("FUCK");
							return;
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
							String code = new String();
							for (int i = 0; i < actnode.lineAttribs.size(); i++)
								code += actnode.lineAttribs.get(i) + "\n";
							args[0] = new Argument(la.libArguments[0].kind,code,null);
							}
						a.setArguments(args);
						//arguments, relative, appliesTo
						}
					}
			}

		static void putAppliesRef(GmFile gf, Action act, String name)
			{
			GmObject temp = gf.gmObjects.get(name);
			if (temp != null) act.setAppliesTo(temp.reference);
			}

		static void putOtherRef(GmFile gf, Event e, String name)
			{
			GmObject temp = gf.gmObjects.get(name);
			if (temp != null) e.other = temp.reference;
			}

		static void putArgumentRef(GmFile gf, Argument arg, String name)
			{
			Resource<?,?> temp = gf.getList(Argument.getResourceKind(arg.kind)).get(name);
			if (temp != null) arg.setRes(temp.reference);
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

		protected void put(GmFile gf, PropertyMap<PGameInformation> p, PGameInformation key, Object o)
			{
			Object def = p.get(key);
			p.put(key,DataPropReader.convert(o.toString(),def == null ? null : def.getClass()));
			}

		protected void readProperties(GmFile gf, PropertyMap<PGameInformation> p, Properties i)
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

		protected boolean allowProperty(PGameInformation key)
			{
			return key != PGameInformation.TEXT;
			}

		private void readDataFile(EGMFile f, GmFile gf, GameInformation r, Properties i, String dir)
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

		private void readData(GmFile gf, GameInformation r, InputStream in)
			{
			try
				{
				r.put(PGameInformation.TEXT,readFully(in).toString());
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

		protected void put(GmFile gf, PropertyMap<PGameSettings> p, PGameSettings key, Object o)
			{
			Object def = p.get(key);
			p.put(key,DataPropReader.convert(o.toString(),def == null ? null : def.getClass()));
			}

		protected void readProperties(GmFile gf, PropertyMap<PGameSettings> p, Properties i)
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

		protected boolean allowProperty(PGameSettings prop)
			{
			//			return false;
			return prop != PGameSettings.DPLAY_GUID && prop != PGameSettings.GAME_ICON
					&& prop != PGameSettings.FRONT_LOAD_BAR && prop != PGameSettings.BACK_LOAD_BAR
					&& prop != PGameSettings.LOADING_IMAGE;
			}

		private void readDataFiles(EGMFile f, GmFile gf, GameSettings r, Properties i, String dir)
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
					System.err.println("Data file missing: " + fn);
				}
			}
		}

	/** Use Util.readFully instead. */
	@Deprecated
	public static ByteArrayOutputStream readFully(InputStream in) throws IOException
		{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();

		byte[] buffer = new byte[4096];

		// Read in the bytes
		int numRead = 0;
		while ((numRead = in.read(buffer)) >= 0)
			baos.write(buffer,0,numRead);

		// Close the input stream and return bytes
		return baos;
		}

	public static void mainEef(String args[])
		{
		File home = new File(System.getProperty("user.home")); //$NON-NLS-1$
		File in = new File(home,"inputEEF.eef"); // any of gmd,gm6,gmk,gm81
		try
			{
			EEFReader reader = new EEFReader(new FileInputStream(in),",");
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
		File f = new File(System.getProperty("user.home"),"Dropbox/ENIGMA/outputEgmFile.egm");
		readEgmFile(f,true);
		}
	}
