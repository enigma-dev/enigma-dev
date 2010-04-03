/*
 * Copyright (C) 2008, 2009 IsmAvatar <IsmAvatar@gmail.com>
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

import static org.lateralgm.main.Util.deRef;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.awt.image.PixelGrabber;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;
import java.util.zip.DeflaterOutputStream;

import javax.swing.JOptionPane;

import org.enigma.backend.EnigmaStruct;
import org.enigma.backend.other.Constant;
import org.enigma.backend.other.Include;
import org.enigma.backend.resources.Background;
import org.enigma.backend.resources.GmObject;
import org.enigma.backend.resources.Room;
import org.enigma.backend.resources.Script;
import org.enigma.backend.resources.Sound;
import org.enigma.backend.resources.Sprite;
import org.enigma.backend.sub.BackgroundDef;
import org.enigma.backend.sub.Event;
import org.enigma.backend.sub.Image;
import org.enigma.backend.sub.Instance;
import org.enigma.backend.sub.MainEvent;
import org.enigma.backend.sub.Tile;
import org.enigma.backend.sub.View;
import org.lateralgm.file.GmFile;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Background.PBackground;
import org.lateralgm.resources.GmObject.PGmObject;
import org.lateralgm.resources.Room.PRoom;
import org.lateralgm.resources.Script.PScript;
import org.lateralgm.resources.Sound.PSound;
import org.lateralgm.resources.Sound.SoundKind;
import org.lateralgm.resources.Sprite.BBMode;
import org.lateralgm.resources.Sprite.PSprite;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.library.LibManager;
import org.lateralgm.resources.library.Library;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Argument;
import org.lateralgm.resources.sub.BackgroundDef.PBackgroundDef;
import org.lateralgm.resources.sub.Instance.PInstance;
import org.lateralgm.resources.sub.Tile.PTile;
import org.lateralgm.resources.sub.View.PView;

public final class EnigmaWriter
	{
	protected GmFile i;
	protected EnigmaStruct o;

	private EnigmaWriter(GmFile in, EnigmaStruct out)
		{
		i = in;
		o = out;
		}

	public static EnigmaStruct prepareStruct(GmFile f)
		{
		EnigmaWriter ew = new EnigmaWriter(f,new EnigmaStruct());
		return ew.populateStruct();
		}

	protected EnigmaStruct populateStruct()
		{
		//		ef.progress(0,"Initializing");

		o.fileVersion = i.fileVersion;
		o.filename = i.filename;

		populateSprites();
		populateSounds();
		populateBackgrounds();
		o.pathCount = 0;
		populateScripts();
		o.fontCount = 0;
		o.timelineCount = 0;
		populateObjects();
		populateRooms();

		o.triggerCount = 0;

		o.constantCount = i.gameSettings.constants.size();
		if (o.constantCount != 0)
			{
			o.constants = new Constant.ByReference();
			Constant[] ocl = (Constant[]) o.constants.toArray(o.constantCount);
			for (int c = 0; c < o.constantCount; c++)
				{
				ocl[c].name = i.gameSettings.constants.get(c).name;
				ocl[c].value = i.gameSettings.constants.get(c).value;
				}
			}

		o.includeCount = i.gameSettings.includes.size();
		if (o.includeCount != 0)
			{
			o.includes = new Include.ByReference();
			Include[] oil = (Include[]) o.includes.toArray(o.includeCount);
			for (int inc = 0; inc < o.includeCount; inc++)
				{
				System.out.println("J: " + i.gameSettings.includes.get(inc).filePath);
//				oil[inc].filepath = i.gameSettings.includes.get(inc).filePath;
				}
			}

		o.packageCount = 0;
		//String[] packages = { "Alpha","Beta","Gamma","Delta" };
		//o.packageCount = packages.length;
		//o.packages = new StringArray(packages);

		o.lastInstanceId = i.lastInstanceId;
		o.lastTileId = i.lastTileId;

		//		ef.progress(100,"Finalizing");

		return o;

		/*		o.spriteCount = i.sprites.size();
				public Sprite[] sprites;
				o.soundCount = i.sounds.size();
				public Sound[] sounds;
				o.backgroundCount = i.backgrounds.size();
				public Background[] backgrounds;
				o.pathCount = i.paths.size();
				public Path[] paths;
				o.scriptCount = i.scripts.size();
				public Script[] scripts;
				o.fontCount = i.fonts.size();
				public Font[] fonts;
				o.timelineCount = i.timelines.size();
				public Timeline[] timelines;
				o.gmObjectCount = i.gmObjects.size();
				public GmObject[] gmObjects;
				o.roomCount = i.rooms.size();
				public Room[] rooms;

				o.triggerCount = i.triggers.size();
				public Trigger[] triggers;
				o.constantCount = i.constants.size();
				public Constant[] constants;
				o.includeCount = i.includes.size();
				public Include[] includes;
				o.packageCount = i.packages.size();
				public String[] packages;*/

		/*out.write("EGMf".getBytes());
		out.write4(4); //version
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		f.gameSettings.gameIcon.write(baos);
		writeStr(out,baos.toByteArray());

		ArrayList<LibAction> ala = getQuestionLibActions();
		out.write4(f.scripts.size() + ala.size());
		for (Script s : f.scripts)
			writeStr(out,s.getName());
		for (LibAction la : ala)
			writeStr(out,"lib" + la.parentId + "_action" + la.id);

		ArrayList<String> names = new ArrayList<String>(f.gmObjects.size());
		out.write4(f.gmObjects.size());
		for (GmObject o : f.gmObjects)
			{
			out.write4(o.getId());
			String name = o.getName();
			if (names.contains(name)) throw new GmFormatException(f,"Duplicate object name: " + name);
			writeStr(out,name);
			}

		out.write4(f.rooms.size());
		for (Room o : f.rooms)
			{
			out.write4(o.getId());
			writeStr(out,o.getName());
			}

		out.write4(f.sprites.size());
		for (Sprite o : f.sprites)
			{
			out.write4(o.getId());
			writeStr(out,o.getName());
			}
		ef.progress(30,"Sending Enigma resource data");

		writeScripts(f,out);
		for (LibAction la : ala)
			writeStr(out,la.execInfo);

		ef.pb.setValue(45);
		writeObjects(f,out);
		ef.pb.setValue(60);
		writeRooms(f,out);
		ef.pb.setValue(75);

		out.write("myn".getBytes());
		out.write(0);
		writeStr(out,"");
		writeStr(out,"");

		writeSprites(f,out); //after myn
		ef.pb.setValue(90);

		out.close();
		ef.progress(95,"Executing Enigma...");
		return true;*/
		}

	protected static final SoundKind[] SOUND_KIND = { SoundKind.NORMAL,SoundKind.BACKGROUND,
			SoundKind.SPATIAL,SoundKind.MULTIMEDIA };
	protected static final Map<SoundKind,Integer> SOUND_CODE;
	static
		{
		EnumMap<SoundKind,Integer> m = new EnumMap<SoundKind,Integer>(SoundKind.class);
		for (int i = 0; i < SOUND_KIND.length; i++)
			m.put(SOUND_KIND[i],i);
		SOUND_CODE = Collections.unmodifiableMap(m);
		}
	protected static final BBMode[] SPRITE_BB_MODE = { BBMode.AUTO,BBMode.FULL,BBMode.MANUAL };
	protected static final Map<BBMode,Integer> SPRITE_BB_CODE;
	static
		{
		EnumMap<BBMode,Integer> m = new EnumMap<BBMode,Integer>(BBMode.class);
		for (int i = 0; i < SPRITE_BB_MODE.length; i++)
			m.put(SPRITE_BB_MODE[i],i);
		SPRITE_BB_CODE = Collections.unmodifiableMap(m);
		}

	protected void populateSprites()
		{
		int size = i.sprites.size();
		o.spriteCount = size;
		if (size == 0) return;

		o.sprites = new Sprite.ByReference();
		Sprite[] osl = (Sprite[]) o.sprites.toArray(size);
		org.lateralgm.resources.Sprite[] isl = i.sprites.toArray(new org.lateralgm.resources.Sprite[0]);
		for (int s = 0; s < size; s++)
			{
			Sprite os = osl[s];
			org.lateralgm.resources.Sprite is = isl[s];

			os.name = is.getName();
			os.id = is.getId();

			os.transparent = is.get(PSprite.TRANSPARENT);
			os.shape = is.get(PSprite.PRECISE) ? 0 : 1;//is.get(PSprite.SHAPE); //0*=Precise, 1=Rectangle,  2=Disk, 3=Diamond
			os.alphaTolerance = 0;//is.get(PSprite.ALPHA_TOLERANCE);
			os.separateMask = false;//is.get(PSprite.SEPARATE_MASK);
			os.smoothEdges = is.get(PSprite.SMOOTH_EDGES);
			os.preload = is.get(PSprite.PRELOAD);
			os.originX = is.get(PSprite.ORIGIN_X);
			os.originY = is.get(PSprite.ORIGIN_Y);
			os.bbMode = SPRITE_BB_CODE.get(is.get(PSprite.BB_MODE)); //0*=Automatic, 1=Full image, 2=Manual
			os.bbLeft = is.get(PSprite.BB_LEFT);
			os.bbRight = is.get(PSprite.BB_RIGHT);
			os.bbTop = is.get(PSprite.BB_TOP);
			os.bbBottom = is.get(PSprite.BB_BOTTOM);

			os.subImageCount = is.subImages.size();
			if (os.subImageCount == 0) continue;

			os.subImages = new Image.ByReference();
			Image[] osil = (Image[]) os.subImages.toArray(os.subImageCount);
			for (int i = 0; i < os.subImageCount; i++)
				populateImage(is.subImages.get(i),osil[i]);
			}
		}

	protected void populateSounds()
		{
		int size = i.sounds.size();
		o.soundCount = size;
		if (size == 0) return;

		o.sounds = new Sound.ByReference();
		Sound[] osl = (Sound[]) o.sounds.toArray(size);
		org.lateralgm.resources.Sound[] isl = i.sounds.toArray(new org.lateralgm.resources.Sound[0]);
		for (int s = 0; s < size; s++)
			{
			Sound os = osl[s];
			org.lateralgm.resources.Sound is = isl[s];

			os.name = is.getName();
			os.id = is.getId();

			os.kind = SOUND_CODE.get(is.get(PSound.KIND));
			os.fileType = is.get(PSound.FILE_TYPE);
			os.fileName = is.get(PSound.FILE_NAME);
			os.chorus = is.get(PSound.CHORUS);
			os.echo = is.get(PSound.ECHO);
			os.flanger = is.get(PSound.FLANGER);
			os.gargle = is.get(PSound.GARGLE);
			os.reverb = is.get(PSound.REVERB);
			os.volume = is.get(PSound.VOLUME);
			os.pan = is.get(PSound.PAN);
			os.preload = is.get(PSound.PRELOAD);

			if (is.data == null)
				{
				os.size = 0;
				continue;
				}
			os.size = is.data.length;
			os.data = ByteBuffer.allocateDirect(os.size).put(is.data);
			}
		}

	protected void populateBackgrounds()
		{
		int size = i.backgrounds.size();
		o.backgroundCount = size;
		if (size == 0) return;

		o.backgrounds = new Background.ByReference();
		Background[] obl = (Background[]) o.backgrounds.toArray(size);
		org.lateralgm.resources.Background[] ibl = i.backgrounds.toArray(new org.lateralgm.resources.Background[0]);
		for (int s = 0; s < size; s++)
			{
			Background ob = obl[s];
			org.lateralgm.resources.Background ib = ibl[s];

			ob.name = ib.getName();
			ob.id = ib.getId();

			ob.transparent = ib.get(PBackground.TRANSPARENT);
			ob.smoothEdges = ib.get(PBackground.SMOOTH_EDGES);
			ob.preload = ib.get(PBackground.PRELOAD);
			ob.useAsTileset = ib.get(PBackground.USE_AS_TILESET);
			ob.tileWidth = ib.get(PBackground.TILE_WIDTH);
			ob.tileHeight = ib.get(PBackground.TILE_HEIGHT);
			ob.hOffset = ib.get(PBackground.H_OFFSET);
			ob.vOffset = ib.get(PBackground.V_OFFSET);
			ob.hSep = ib.get(PBackground.H_SEP);
			ob.vSep = ib.get(PBackground.V_SEP);

			ob.backgroundImage = new Image.ByReference();
			populateImage(ib.getBackgroundImage(),ob.backgroundImage);
			}
		}

	protected void populateScripts()
		{
		List<LibAction> qs = getQuestionLibActions();

		int size = i.scripts.size() + qs.size();
		o.scriptCount = size;
		if (size == 0) return;

		o.scripts = new Script.ByReference();
		Script[] osl = (Script[]) o.scripts.toArray(size);
		org.lateralgm.resources.Script[] isl = i.scripts.toArray(new org.lateralgm.resources.Script[0]);
		for (int s = 0; s < isl.length; s++)
			{
			Script oo = osl[s];
			org.lateralgm.resources.Script io = isl[s];

			oo.name = io.getName();
			oo.id = io.getId();
			oo.code = io.get(PScript.CODE);
			}

		for (int s = 0; s < qs.size(); s++)
			{
			Script oo = osl[s + isl.length];
			oo.name = "lib" + qs.get(s).parentId + "_action" + qs.get(s).id;
			oo.id = -s - 2;
			oo.code = qs.get(s).execInfo;
			}
		}

	protected void populateObjects()
		{
		int size = i.gmObjects.size();
		o.gmObjectCount = size;
		if (size == 0) return;

		o.gmObjects = new GmObject.ByReference();
		GmObject[] ool = (GmObject[]) o.gmObjects.toArray(size);
		org.lateralgm.resources.GmObject[] iol = i.gmObjects.toArray(new org.lateralgm.resources.GmObject[0]);
		for (int s = 0; s < size; s++)
			{
			GmObject oo = ool[s];
			org.lateralgm.resources.GmObject io = iol[s];

			oo.name = io.getName();
			oo.id = io.getId();

			oo.spriteId = toId(io.get(PGmObject.SPRITE),-1);
			oo.solid = io.get(PGmObject.SOLID);
			oo.visible = io.get(PGmObject.VISIBLE);
			oo.depth = io.get(PGmObject.DEPTH);
			oo.persistent = io.get(PGmObject.PERSISTENT);
			oo.parentId = toId(io.get(PGmObject.PARENT),-100);
			oo.maskId = toId(io.get(PGmObject.MASK),-1);

			//Use this code instead to allow 0 main events
			//and switch GmObject.mainEvents to MainEvent.ByReference
			oo.mainEventCount = io.mainEvents.size();
			if (oo.mainEventCount == 0) continue;

			oo.mainEvents = new MainEvent.ByReference();
			MainEvent[] ooil = (MainEvent[]) oo.mainEvents.toArray(oo.mainEventCount);

			//we assume there are never 0 main events
			//			oo.mainEventCount = io.mainEvents.size();
			//			oo.mainEvents = new MainEvent[oo.mainEventCount];
			for (int me = 0; me < oo.mainEventCount; me++)
				{
				MainEvent ome = ooil[me];
				ArrayList<org.lateralgm.resources.sub.Event> iel = io.mainEvents.get(me).events;

				ome.id = me;
				ome.eventCount = iel.size();
				if (ome.eventCount == 0) continue;

				ome.events = new Event.ByReference();
				Event[] oel = (Event[]) ome.events.toArray(ome.eventCount);

				for (int e = 0; e < ome.eventCount; e++)
					{
					Event oe = oel[e];
					org.lateralgm.resources.sub.Event ie = iel.get(e);

					oe.id = ie.id;
					oe.otherObjectId = toId(ie.other,-1);

					oe.code = getActionsCode(ie);
					}
				}
			}
		}

	protected void populateRooms()
		{
		int size = i.rooms.size();
		o.roomCount = size;
		if (size == 0) return;

		o.rooms = new Room.ByReference();
		Room[] orly = (Room[]) o.rooms.toArray(size);
		org.lateralgm.resources.Room[] irl = i.rooms.toArray(new org.lateralgm.resources.Room[0]);
		for (int s = 0; s < size; s++)
			{
			Room or = orly[s];
			org.lateralgm.resources.Room is = irl[s];

			or.name = is.getName();
			or.id = is.getId();

			or.caption = is.get(PRoom.CAPTION);
			or.width = is.get(PRoom.WIDTH);
			or.height = is.get(PRoom.HEIGHT);

			// vvv may be useless vvv //
			or.snapX = is.get(PRoom.SNAP_X);
			or.snapY = is.get(PRoom.SNAP_Y);
			or.isometric = is.get(PRoom.ISOMETRIC);
			// ^^^ may be useless ^^^ //

			or.speed = is.get(PRoom.SPEED);
			or.persistent = is.get(PRoom.PERSISTENT);
			or.backgroundColor = ARGBtoRGBA(((Color) is.get(PRoom.BACKGROUND_COLOR)).getRGB());
			or.drawBackgroundColor = is.get(PRoom.DRAW_BACKGROUND_COLOR);
			or.creationCode = is.get(PRoom.CREATION_CODE);

			/// vvv useless stuff vvv //
			or.rememberWindowSize = is.get(PRoom.REMEMBER_WINDOW_SIZE);
			or.editorWidth = is.get(PRoom.EDITOR_WIDTH);
			or.editorHeight = is.get(PRoom.EDITOR_HEIGHT);
			or.showGrid = is.get(PRoom.SHOW_GRID);
			or.showObjects = is.get(PRoom.SHOW_OBJECTS);
			or.showTiles = is.get(PRoom.SHOW_TILES);
			or.showBackgrounds = is.get(PRoom.SHOW_BACKGROUNDS);
			or.showViews = is.get(PRoom.SHOW_VIEWS);
			or.deleteUnderlyingObjects = is.get(PRoom.DELETE_UNDERLYING_OBJECTS);
			or.deleteUnderlyingTiles = is.get(PRoom.DELETE_UNDERLYING_TILES);
			or.currentTab = is.get(PRoom.CURRENT_TAB);
			or.scrollBarX = is.get(PRoom.SCROLL_BAR_X);
			or.scrollBarY = is.get(PRoom.SCROLL_BAR_Y);
			or.enableViews = is.get(PRoom.ENABLE_VIEWS);
			// ^^^ useless stuff ^^^ //

			or.backgroundDefCount = is.backgroundDefs.size();
			if (or.backgroundDefCount != 0)
				{
				or.backgroundDefs = new BackgroundDef.ByReference();
				BackgroundDef[] obdl = (BackgroundDef[]) or.backgroundDefs.toArray(or.backgroundDefCount);
				for (int bd = 0; bd < obdl.length; bd++)
					{
					BackgroundDef obd = obdl[bd];
					org.lateralgm.resources.sub.BackgroundDef ibd = is.backgroundDefs.get(bd);

					obd.visible = ibd.properties.get(PBackgroundDef.VISIBLE);
					obd.foreground = ibd.properties.get(PBackgroundDef.FOREGROUND);
					obd.x = ibd.properties.get(PBackgroundDef.X);
					obd.y = ibd.properties.get(PBackgroundDef.Y);
					obd.tileHoriz = ibd.properties.get(PBackgroundDef.TILE_HORIZ);
					obd.tileVert = ibd.properties.get(PBackgroundDef.TILE_VERT);
					obd.hSpeed = ibd.properties.get(PBackgroundDef.H_SPEED);
					obd.vSpeed = ibd.properties.get(PBackgroundDef.V_SPEED);
					obd.stretch = ibd.properties.get(PBackgroundDef.STRETCH);

					obd.backgroundId = toId(ibd.properties.get(PBackgroundDef.BACKGROUND),-1);
					}
				}

			or.viewCount = is.views.size();
			if (or.viewCount != 0)
				{
				or.views = new View.ByReference();
				View[] ovl = (View[]) or.views.toArray(or.viewCount);
				for (int v = 0; v < ovl.length; v++)
					{
					View ov = ovl[v];
					org.lateralgm.resources.sub.View iv = is.views.get(v);

					ov.visible = iv.properties.get(PView.VISIBLE);
					ov.viewX = iv.properties.get(PView.VIEW_X);
					ov.viewY = iv.properties.get(PView.VIEW_Y);
					ov.viewW = iv.properties.get(PView.VIEW_W);
					ov.viewH = iv.properties.get(PView.VIEW_H);
					ov.portX = iv.properties.get(PView.PORT_X);
					ov.portY = iv.properties.get(PView.PORT_Y);
					ov.portW = iv.properties.get(PView.PORT_W);
					ov.portH = iv.properties.get(PView.PORT_H);
					ov.borderH = iv.properties.get(PView.BORDER_H);
					ov.borderV = iv.properties.get(PView.BORDER_V);
					ov.speedH = iv.properties.get(PView.SPEED_H);
					ov.speedV = iv.properties.get(PView.SPEED_V);
					ov.objectId = toId(iv.properties.get(PView.OBJECT),-1);
					}
				}

			or.instanceCount = is.instances.size();
			if (or.instanceCount != 0)
				{
				or.instances = new Instance.ByReference();
				Instance[] oil = (Instance[]) or.instances.toArray(or.instanceCount);
				for (int i = 0; i < oil.length; i++)
					{
					Instance oi = oil[i];
					org.lateralgm.resources.sub.Instance ii = is.instances.get(i);

					oi.x = ii.properties.get(PInstance.X);
					oi.y = ii.properties.get(PInstance.Y);
					oi.objectId = toId(ii.properties.get(PInstance.OBJECT),-1);
					oi.id = ii.properties.get(PInstance.ID);
					oi.creationCode = ii.properties.get(PInstance.CREATION_CODE);
					oi.locked = ii.properties.get(PInstance.LOCKED);
					}
				}

			or.tileCount = is.tiles.size();
			if (or.tileCount != 0)
				{
				or.tiles = new Tile.ByReference();
				Tile[] otl = (Tile[]) or.tiles.toArray(or.tileCount);
				for (int t = 0; t < otl.length; t++)
					{
					Tile ot = otl[t];
					org.lateralgm.resources.sub.Tile it = is.tiles.get(t);

					ot.bgX = it.properties.get(PTile.BG_X);
					ot.bgY = it.properties.get(PTile.BG_Y);
					ot.roomX = it.properties.get(PTile.ROOM_X);
					ot.roomY = it.properties.get(PTile.ROOM_Y);
					ot.width = it.properties.get(PTile.WIDTH);
					ot.height = it.properties.get(PTile.HEIGHT);
					ot.depth = it.properties.get(PTile.DEPTH);
					ot.backgroundId = toId(it.properties.get(PTile.BACKGROUND),-1);
					ot.id = it.properties.get(PTile.ID);
					ot.locked = it.properties.get(PTile.LOCKED);
					} //tile
				} //tiles
			} //rooms
		} //populateRooms()

	public static void populateImage(BufferedImage i, Image o)
		{
		if (i == null || o == null) return;

		o.width = i.getWidth();
		o.height = i.getHeight();

		int[] pixels = new int[o.width * o.height];
		PixelGrabber pg = new PixelGrabber(i,0,0,o.width,o.height,pixels,0,o.width);
		try
			{
			pg.grabPixels();

			ByteArrayOutputStream baos = new ByteArrayOutputStream(pixels.length * 4);
			DeflaterOutputStream dos = new DeflaterOutputStream(baos);
			//is this the most efficient way? ARGB => RGBA
			for (int p = 0; p < pixels.length; p++)
				{
				dos.write(pixels[p] >>> 16 & 0xFF);
				dos.write(pixels[p] >>> 8 & 0xFF);
				dos.write(pixels[p] & 0xFF);
				dos.write(pixels[p] >>> 24);
				}
			//pixels[p] = ARGBtoRGBA(pixels[p]);

			dos.finish();
			o.dataSize = baos.size();
			o.data = ByteBuffer.allocateDirect(baos.size()).put(baos.toByteArray());
			}
		catch (Exception e)
			{
			e.printStackTrace();
			}

		////we assume an int is 4 bytes
		//o.pixels = ByteBuffer.allocateDirect(o.width * o.height * 4).order(ByteOrder.LITTLE_ENDIAN).asIntBuffer().put(
		//		pixels);
		}

	public static int ARGBtoRGBA(int argb)
		{
		return ((argb & 0x00FFFFFF) << 8) | (argb >>> 24);
		}

	public static int toId(Object obj, int def)
		{
		ResourceReference<?> rr = (ResourceReference<?>) obj;
		if (deRef(rr) != null) return rr.get().getId();
		return def;
		}

	public static boolean actionDemise = false;

	public static String getActionsCode(org.lateralgm.resources.sub.Event ev)
		{
		String nl = System.getProperty("line.separator");
		String code = "";
		for (Action act : ev.actions)
			{
			LibAction la = act.getLibAction();
			if (la == null)
				{
				if (!actionDemise)
					{
					String mess = "Warning, you have a D&D action which is unsupported by this compiler."
							+ " This and future unsupported D&D actions will be discarded. (LibAction not found"
							+ " in event " + ev.toString() + ")";
					JOptionPane.showMessageDialog(null,mess);
					actionDemise = true;
					}
				continue;
				}
			List<Argument> args = act.getArguments();
			switch (la.actionKind)
				{
				case Action.ACT_BEGIN:
					code += "{";
					break;
				case Action.ACT_CODE:
					code += args.get(0).getVal() + nl;
					break;
				case Action.ACT_ELSE:
					code += "else ";
					break;
				case Action.ACT_END:
					code += "}";
					break;
				case Action.ACT_EXIT:
					code += "exit ";
					break;
				case Action.ACT_REPEAT:
					code += "repeat (" + args.get(0) + ") ";
					break;
				case Action.ACT_VARIABLE:
					code += args.get(0).getVal() + " = " + args.get(1).getVal() + nl;
					break;
				case Action.ACT_NORMAL:
					{
					if (la.execType == Action.EXEC_NONE) break;
					ResourceReference<org.lateralgm.resources.GmObject> apto = act.getAppliesTo();
					if (la.question)
						if (apto != org.lateralgm.resources.GmObject.OBJECT_SELF)
							{
							if (!actionDemise)
								{
								String mess = "Warning, you have a D&D action which is unsupported by this compiler."
										+ " This and future unsupported D&D actions will be discarded. (Question + Applies To"
										+ " in event " + ev.toString() + " in library ";
								if (la.parent == null || la.parent.tabCaption.length() == 0)
									mess += la.parentId;
								else
									mess += la.parent.tabCaption;
								mess += " action " + (la.name.length() == 0 ? la.id : la.name) + ")";
								JOptionPane.showMessageDialog(null,mess);
								actionDemise = true;
								}
							continue;
							}
					if (apto != org.lateralgm.resources.GmObject.OBJECT_SELF)
						{
						if (apto == org.lateralgm.resources.GmObject.OBJECT_OTHER)
							code += "with (other) {";
						else
							code += "with (" + apto.get().getName() + ") {";
						}
					if (act.isRelative()) code += "argument_relative = true" + nl;
					if (la.question) code += "if ";
					if (act.isNot()) code += "!";
					if (la.question && la.execType == Action.EXEC_CODE)
						code += "lib" + la.parentId + "_action" + la.id;
					else
						code += la.execInfo;
					if (la.execType == Action.EXEC_FUNCTION)
						{
						code += "(";
						for (int i = 0; i < args.size() - 1; i++)
							code += args.get(i).getVal() + ",";
						if (args.size() != 0) code += args.get(args.size() - 1) + ")";
						}
					code += nl;
					if (apto != org.lateralgm.resources.GmObject.OBJECT_SELF) code += "}";
					}
					break;
				}
			}
		return code;
		}

	//in order to allow question actions to get converted to code, we treat their internal code as scripts
	public static ArrayList<LibAction> getQuestionLibActions()
		{
		ArrayList<LibAction> ala = new ArrayList<LibAction>();
		for (Library lib : LibManager.libs)
			for (LibAction act : lib.libActions)
				if (act.question && act.execType == Action.EXEC_CODE) ala.add(act);
		return ala;
		}
	}
