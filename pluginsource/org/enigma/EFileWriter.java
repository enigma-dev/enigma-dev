/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.imageio.ImageIO;

import org.enigma.messages.Messages;
import org.enigma.utility.APNGExperiments;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFileReader;
import org.lateralgm.file.GmStreamEncoder;
import org.lateralgm.file.iconio.ICOFile;
import org.lateralgm.main.Util;
import org.lateralgm.resources.Background;
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
import org.lateralgm.resources.GameInformation.PGameInformation;
import org.lateralgm.resources.GameSettings.PGameSettings;
import org.lateralgm.resources.Resource.Kind;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.library.LibManager;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Argument;
import org.lateralgm.resources.sub.BackgroundDef;
import org.lateralgm.resources.sub.Event;
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.MainEvent;
import org.lateralgm.resources.sub.PathPoint;
import org.lateralgm.resources.sub.Tile;
import org.lateralgm.resources.sub.View;
import org.lateralgm.resources.sub.BackgroundDef.PBackgroundDef;
import org.lateralgm.resources.sub.Instance.PInstance;
import org.lateralgm.resources.sub.Tile.PTile;
import org.lateralgm.resources.sub.View.PView;
import org.lateralgm.util.PropertyMap;

public class EFileWriter
	{
	public static final String EY = ".ey"; //$NON-NLS-1$

	// Modularity Classes
	public static abstract class EGMOutputStream
		{
		protected OutputStream last;

		public OutputStream next(final List<String> directory, String filename) throws IOException
			{
			if (last != null) _finishLast();
			return last = _next(directory,filename);
			}

		protected abstract OutputStream _next(final List<String> directory, String filename)
				throws IOException;

		protected abstract void _finishLast() throws IOException;

		public abstract void finish() throws IOException;

		public void close() throws IOException
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
		protected void _finishLast() throws IOException
			{
			last.flush();
			}

		@Override
		protected OutputStream _next(List<String> directory, String filename) throws IOException
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
			os.finish(); //also closes the entry
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
		protected void _finishLast() throws IOException
			{
			last.close();
			}

		@Override
		protected OutputStream _next(List<String> directory, String filename) throws IOException
			{
			File f = root;
			for (String s : directory)
				f = new File(root,s);
			return new FileOutputStream(new File(f,filename));
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
		public void write(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
				throws IOException;
		}

	/**
	 * Convenience wrapper module manager for standard resources. A standard
	 * resource will have an ey properties file which points to a data file with
	 * a designated extension. This eliminates the need to deal with ResNodes
	 * and Zip technicalities and just focus on writing the respective files.
	 */
	public static abstract class DataResourceWriter implements ResourceWriter
		{
		public void write(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
				throws IOException
			{
			String name = (String) child.getUserObject();
			Resource<?,?> r = (Resource<?,?>) Util.deRef((ResourceReference<?>) child.getRes());
			String fn = name + getExt(r);

			PrintStream ps = new PrintStream(os.next(dir,name + EY));
			ps.println("Data: " + fn); //$NON-NLS-1$
			writeProperties(ps,r.properties);

			writeData(os.next(dir,fn),r);
			}

		/**
		 * The extension, which must include the preceding dot, e.g. ".ext".
		 * Resource provided in case extension is chosen on an individual
		 * resource basis.
		 */
		public abstract String getExt(Resource<?,?> r);

		public abstract void writeProperties(PrintStream os, PropertyMap<? extends Enum<?>> p)
				throws IOException;

		public abstract void writeData(OutputStream os, Resource<?,?> r) throws IOException;
		}

	/**
	 * Convenience wrapper for automatically writing allowed properties.
	 * 
	 * @see DataResourceWriter
	 */
	static abstract class DataPropWriter extends DataResourceWriter
		{
		@Override
		public void writeProperties(PrintStream os, PropertyMap<? extends Enum<?>> p)
				throws IOException
			{
			for (Entry<? extends Enum<?>,Object> e : p.entrySet())
				if (allowProperty(e.getKey()))
					os.println(e.getKey().name() + ": " + convert(e.getValue())); //$NON-NLS-1$
			}

		public static String convert(Object o)
			{
			if (o == null) return null;
			if (o instanceof String || o instanceof Boolean || o instanceof Number
					|| o instanceof Enum<?>) return o.toString();
			if (o instanceof ResourceReference<?>)
				{
				Resource<?,?> r = Util.deRef((ResourceReference<?>) o);
				return r == null ? null : r.getName();
				}
			if (o instanceof Color) return "0x" + Integer.toHexString(((Color) o).getRGB());
			System.out.println("Unknown serialization: " + o.getClass() + " -> " + o);
			return o.toString();
			}

		/**
		 * Returns whether the following property should be allowed in the
		 * properties file. Override to disallow certain properties.
		 */
		public boolean allowProperty(Enum<?> prop)
			{
			return true;
			};
		}

	// Module maps
	/** Used to register writers with their resource kinds. */
	static Map<Kind,ResourceWriter> writers = new HashMap<Kind,ResourceWriter>();
	static Map<Kind,String> typestrs = new HashMap<Kind,String>();
	static
		{
		// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS
		writers.put(Kind.SPRITE,new SpriteApngIO());
		writers.put(Kind.SOUND,new SoundIO());
		writers.put(Kind.BACKGROUND,new BackgroundIO());
		writers.put(Kind.PATH,new PathTextIO());
		writers.put(Kind.SCRIPT,new ScriptIO());
		writers.put(Kind.FONT,new FontRawIO());
		// writers.put(Kind.TIMELINE,new TimelineIO());
		writers.put(Kind.OBJECT,new ObjectIO());
		writers.put(Kind.ROOM,new RoomGmDataIO());
		writers.put(Kind.GAMEINFO,new GameInfoRtfIO());
		writers.put(Kind.GAMESETTINGS,new GameSettingsThreeIO());

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
	public static void writeEgmFile(File loc, GmFile gf, ResNode tree, boolean zip)
		{
		try
			{
			EGMOutputStream out;
			if (zip)
				out = new EGMZip(new FileOutputStream(loc));
			else
				out = new EGMFolder(loc);
			writeEgmFile(out,gf,tree);
			out.close();
			}
		catch (IOException e)
			{
			// TODO Auto-generated catch block
			e.printStackTrace();
			}
		}

	public static void writeEgmFile(EGMOutputStream os, GmFile gf, ResNode tree) throws IOException
		{
		writeNodeChildren(os,gf,tree,new ArrayList<String>());
		}

	// Workhorse methods
	/** Recursively writes out the tree nodes into the zip. */
	public static void writeNodeChildren(EGMOutputStream os, GmFile gf, ResNode node, List<String> dir)
			throws IOException
		{
		PrintStream ps = new PrintStream(os.next(dir,"toc.txt")); //$NON-NLS-1$

		int children = node.getChildCount();
		for (int i = 0; i < children; i++)
			{
			ResNode child = (ResNode) node.getChildAt(i);
			if (node.isRoot()) ps.print(typestrs.get(child.kind) + ' ');
			ps.println(child.getUserObject());
			}

		for (int i = 0; i < children; i++)
			{
			ResNode child = ((ResNode) node.getChildAt(i));

			if (child.status == ResNode.STATUS_SECONDARY)
				{
				writeResource(os,gf,child,dir);
				}
			else
				{
				String cn = (String) child.getUserObject();
				// String cdir = dir + child.getUserObject() + SEPARATOR;
				ArrayList<String> newDir = new ArrayList<String>(dir);
				newDir.add(cn);
				// os.next(newDir, null);
				writeNodeChildren(os,gf,child,newDir);
				}
			}
		}

	/**
	 * Looks up the registered writer for this resource and invokes the write
	 * method
	 */
	public static void writeResource(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
			throws IOException
		{
		ResourceWriter writer = writers.get(child.kind);
		if (writer == null)
			{
			System.err.println(Messages.format("EFileWriter.NO_WRITER",child.kind)); //$NON-NLS-1$
			return;
			}
		writer.write(os,gf,child,dir);
		}

	// Modules
	// SPRITE,SOUND,BACKGROUND,PATH,SCRIPT,FONT,TIMELINE,OBJECT,ROOM,GAMEINFO,GAMESETTINGS,EXTENSIONS

	static class SpriteApngIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".apng"; //$NON-NLS-1$
			}

		@Override
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			ArrayList<BufferedImage> subs = ((Sprite) r).subImages;
			APNGExperiments.imagesToApng(subs,os);
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
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			os.write(((Sound) r).data);
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
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			ImageIO.write(((Background) r).getBackgroundImage(),"png",os); //$NON-NLS-1$
			}
		}

	static class PathTextIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".pth"; //$NON-NLS-1$
			}

		@Override
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			PrintStream ps = new PrintStream(os);
			// space delimited, EOF terminated
			for (PathPoint p : ((Path) r).points)
				{
				ps.print(p.getX());
				ps.print(' ');
				ps.print(p.getY());
				ps.print(' ');
				ps.println(p.getSpeed());
				}
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
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			os.write(((Script) r).getCode().getBytes()); // charset?
			}

		@Override
		public boolean allowProperty(Enum<?> prop)
			{
			return false;
			}
		}

	static class FontRawIO implements ResourceWriter
		{
		public void write(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
				throws IOException
			{
			String name = (String) child.getUserObject();
			Resource<?,?> r = (Resource<?,?>) Util.deRef((ResourceReference<?>) child.getRes());
			writeProperties(new PrintStream(os.next(dir,name + EY)),r.properties);
			}

		public void writeProperties(PrintStream os, PropertyMap<? extends Enum<?>> p)
				throws IOException
			{
			for (Entry<? extends Enum<?>,Object> e : p.entrySet())
				os.println(e.getKey().name() + ": " + e.getValue()); //$NON-NLS-1$
			}
		}

	static class ObjectIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".obj"; //$NON-NLS-1$
			}

		@Override
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			GmObject obj = (GmObject) r;
			PrintStream ps = new PrintStream(os);
			int numEvents = 0;

			// Write the Events super-key
			for (MainEvent me : obj.mainEvents)
				numEvents += me.events.size();
			ps.println("Events{" + numEvents + "}");

			// Write the events
			for (MainEvent me : obj.mainEvents)
				for (Event ev : me.events)
					{
					ps.print("  Event (");

					if (ev.mainId == MainEvent.EV_COLLISION)
						{
						Resource<?,?> cr = Util.deRef(ev.other);
						ps.print(cr == null ? new String() : cr.getName());
						}
					else
						ps.print(ev.id);

					ps.println("," + ev.mainId + "): " + "Actions{" + ev.actions.size() + "}");
					for (Action action : ev.actions)
						{
						LibAction la = action.getLibAction();
						ps.print("    Action (" + la.id + ","
								+ (la.parent != null ? la.parent.id : la.parentId) + "): ");
						if (action.isNot()) ps.print("\"Not\" ");
						if (action.isRelative()) ps.print("\"Relative\" ");
						if (action.getAppliesTo().get() != null)
							ps.print("Applies(" + action.getAppliesTo().get() + ") ");
						if (la.interfaceKind != LibAction.INTERFACE_CODE)
							{
							ps.println("Fields[" + action.getArguments().size() + "]");
							for (Argument arg : action.getArguments())
								{
								ps.print("      ");
								if (arg.getRes() == null)
									ps.println(arg.getVal());
								else
									{
									ResourceReference<?> rr = arg.getRes();
									Resource<?,?> ar = Util.deRef(rr);
									ps.println(ar == null ? new String() : ar.getName());
									}
								}
							}
						else
							{
							// note: check size first
							String code = action.getArguments().get(0).getVal();
							ps.println("Code[" + code.split("\r\n|\r|\n").length + " lines]");
							ps.println(code);
							}
						}
					}
			}

		void printName(PrintStream out, ResourceReference<?> rr) throws IOException
			{
			Resource<?,?> r = Util.deRef(rr);
			out.print(r == null ? new String() : r.getName());
			}
		}

	static class RoomGmDataIO extends DataPropWriter
		{
		@Override
		public String getExt(Resource<?,?> r)
			{
			return ".rmg"; //$NON-NLS-1$
			}

		@Override
		public void writeData(OutputStream os, Resource<?,?> r) throws IOException
			{
			Room rm = (Room) r;
			GmStreamEncoder out = new GmStreamEncoder(os);
			out.write4(rm.backgroundDefs.size());
			for (BackgroundDef back : rm.backgroundDefs)
				{
				out.writeBool(back.properties,PBackgroundDef.VISIBLE,PBackgroundDef.FOREGROUND);
				writeName(out,(ResourceReference<?>) back.properties.get(PBackgroundDef.BACKGROUND));
				out.write4(back.properties,PBackgroundDef.X,PBackgroundDef.Y);
				out.writeBool(back.properties,PBackgroundDef.TILE_HORIZ,PBackgroundDef.TILE_VERT);
				out.write4(back.properties,PBackgroundDef.H_SPEED,PBackgroundDef.V_SPEED);
				out.writeBool(back.properties,PBackgroundDef.STRETCH);
				}
			// out.writeBool(rm.properties,PRoom.ENABLE_VIEWS);
			out.write4(rm.views.size());
			for (View view : rm.views)
				{
				out.writeBool(view.properties,PView.VISIBLE);
				out.write4(view.properties,PView.VIEW_X,PView.VIEW_Y,PView.VIEW_W,PView.VIEW_H,
						PView.PORT_X,PView.PORT_Y,PView.PORT_W,PView.PORT_H,PView.BORDER_H,PView.BORDER_V,
						PView.SPEED_H,PView.SPEED_V);
				writeName(out,(ResourceReference<?>) view.properties.get(PView.OBJECT));
				}
			out.write4(rm.instances.size());
			for (Instance in : rm.instances)
				{
				out.write4(in.getPosition().x);
				out.write4(in.getPosition().y);
				writeName(out,(ResourceReference<?>) in.properties.get(PInstance.OBJECT));
				out.write4((Integer) in.properties.get(PInstance.ID));
				out.writeStr(in.getCreationCode());
				out.writeBool(in.isLocked());
				}
			out.write4(rm.tiles.size());
			for (Tile tile : rm.tiles)
				{
				out.write4(tile.getRoomPosition().x);
				out.write4(tile.getRoomPosition().y);
				writeName(out,(ResourceReference<?>) tile.properties.get(PTile.BACKGROUND));
				out.write4(tile.getBackgroundPosition().x);
				out.write4(tile.getBackgroundPosition().y);
				out.write4(tile.getSize().width);
				out.write4(tile.getSize().height);
				out.write4(tile.getDepth());
				out.write4((Integer) tile.properties.get(PTile.ID));
				out.writeBool(tile.isLocked());
				}
			out.flush();
			}

		void writeName(GmStreamEncoder out, ResourceReference<?> rr) throws IOException
			{
			Resource<?,?> r = Util.deRef(rr);
			out.writeStr(r == null ? new String() : r.getName());
			}
		}

	static class GameInfoRtfIO implements ResourceWriter
		{
		public static final String RTF = ".rtf";

		public void write(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
				throws IOException
			{
			String name = (String) child.getUserObject();
			String fn = name + RTF;

			PrintStream ps = new PrintStream(os.next(dir,name + EY));
			ps.println("Data: " + fn); //$NON-NLS-1$
			writeProperties(ps,gf.gameInfo.properties);

			writeData(os.next(dir,fn),gf.gameInfo);
			}

		public void writeProperties(PrintStream os, PropertyMap<? extends Enum<?>> p)
				throws IOException
			{
			for (Entry<? extends Enum<?>,Object> e : p.entrySet())
				if (allowProperty(e.getKey()))
					os.println(e.getKey().name() + ": " + DataPropWriter.convert(e.getValue())); //$NON-NLS-1$
			}

		public boolean allowProperty(Enum<?> prop)
			{
			return prop != PGameInformation.TEXT;
			}

		public void writeData(OutputStream os, GameInformation r) throws IOException
			{
			os.write(((String) r.get(PGameInformation.TEXT)).getBytes()); // charset?
			}
		}

	static class GameSettingsThreeIO implements ResourceWriter
		{
		@Override
		public void write(EGMOutputStream os, GmFile gf, ResNode child, List<String> dir)
				throws IOException
			{
			GameSettings gs = gf.gameSettings;
			String name = (String) child.getUserObject();
			String icon = "icon.ico", sSplash = "splash.png"; //$NON-NLS-1$ //$NON-NLS-2$
			String sFiller = "filler.png", sProgress = "progress.png"; //$NON-NLS-1$ //$NON-NLS-2$

			Object splash = gs.get(PGameSettings.LOADING_IMAGE);
			Object filler = gs.get(PGameSettings.BACK_LOAD_BAR);
			Object progress = gs.get(PGameSettings.FRONT_LOAD_BAR);

			if (splash == null) sSplash = null;
			if (filler == null) sFiller = null;
			if (progress == null) sProgress = null;

			PrintStream ps = new PrintStream(os.next(dir,name + EY));
			ps.println("Icon: " + icon); //$NON-NLS-1$
			ps.println("Splash: " + sSplash); //$NON-NLS-1$
			ps.println("Filler: " + sFiller); //$NON-NLS-1$
			ps.println("Progress: " + sProgress); //$NON-NLS-1$

			//handle guid
			ps.print(PGameSettings.DPLAY_GUID.name() + ": 0x");
			byte[] dg = (byte[]) gs.get(PGameSettings.DPLAY_GUID);
			ps.println(String.format("%032x",new BigInteger(1,dg)));

			writeProperties(ps,gs.properties);

			((ICOFile) gs.get(PGameSettings.GAME_ICON)).write(os.next(dir,icon));
			if (splash != null) ImageIO.write((BufferedImage) splash,"png",os.next(dir,sSplash));
			if (filler != null) ImageIO.write((BufferedImage) filler,"png",os.next(dir,sFiller));
			if (progress != null) ImageIO.write((BufferedImage) progress,"png",os.next(dir,sProgress));
			}

		public void writeProperties(PrintStream os, PropertyMap<? extends Enum<?>> p)
				throws IOException
			{
			for (Entry<? extends Enum<?>,Object> e : p.entrySet())
				if (allowProperty(e.getKey()))
					os.println(e.getKey().name() + ": " + DataPropWriter.convert(e.getValue())); //$NON-NLS-1$
			}

		public boolean allowProperty(Enum<?> prop)
			{
			return prop != PGameSettings.DPLAY_GUID && prop != PGameSettings.GAME_ICON
					&& prop != PGameSettings.FRONT_LOAD_BAR && prop != PGameSettings.BACK_LOAD_BAR
					&& prop != PGameSettings.LOADING_IMAGE;
			}

		public void writeData(OutputStream os, GameSettings r) throws IOException
			{
			((ICOFile) r.get(PGameSettings.GAME_ICON)).write(os);
			}
		}

	public static void main(String[] args) throws Exception
		{
		File home = new File(System.getProperty("user.home")); //$NON-NLS-1$
		File in = new File(home,"Dropbox/ENIGMA/inputGmFile.gm81"); // any of gmd,gm6,gmk,gm81
		File out = new File(home,"Dropbox/ENIGMA/outputEgmFile.egm"); // must be egm

		LibManager.autoLoad();
		ResNode root = new ResNode("Root",(byte) 0,null,null); //$NON-NLS-1$;
		GmFile gf = GmFileReader.readGmFile(in,root);

		writeEgmFile(out,gf,root,true);
		}
	}
