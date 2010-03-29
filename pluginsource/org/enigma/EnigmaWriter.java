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

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JOptionPane;

import org.enigma.backend.EnigmaStruct;
import org.enigma.backend.resources.GmObject;
import org.enigma.backend.resources.Room;
import org.enigma.backend.resources.Script;
import org.enigma.backend.resources.Sprite;
import org.enigma.backend.sub.Image;
import org.enigma.backend.sub.MainEvent;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmStreamEncoder;
import org.lateralgm.resources.GmObject.PGmObject;
import org.lateralgm.resources.Room.PRoom;
import org.lateralgm.resources.Script.PScript;
import org.lateralgm.resources.Sprite.PSprite;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.library.LibManager;
import org.lateralgm.resources.library.Library;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Argument;
import org.lateralgm.resources.sub.Event;

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

	private EnigmaStruct populateStruct()
		{
		//		ef.progress(0,"Initializing");

		o.fileVersion = i.fileVersion;
		o.filename = i.filename;
		o.lastInstanceId = i.lastInstanceId;
		o.lastTileId = i.lastTileId;

		writeSprites(o,i);
		writeScripts(o,i);
		writeObjects(o,i);
		writeRooms(o,i);

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

	public static ArrayList<LibAction> getQuestionLibActions()
		{
		ArrayList<LibAction> ala = new ArrayList<LibAction>();
		for (Library lib : LibManager.libs)
			for (LibAction act : lib.libActions)
				if (act.question && act.execType == Action.EXEC_CODE) ala.add(act);
		return ala;
		}

	public void writeScripts(EnigmaStruct o, GmFile i)
		{
		int size = i.scripts.size();
		o.scriptCount = size;
		o.scripts = new Script[size];
		org.lateralgm.resources.Script[] isl = i.scripts.toArray(new org.lateralgm.resources.Script[0]);
		for (int s = 0; s < size; s++)
			{
			Script oo = o.scripts[s];
			org.lateralgm.resources.Script io = isl[s];

			oo.name = io.getName();
			oo.id = io.getId();
			oo.code = io.get(PScript.CODE);
			}
		}

	public void writeObjects(EnigmaStruct o, GmFile i)
		{
		int size = i.gmObjects.size();
		o.gmObjectCount = size;
		o.gmObjects = new GmObject[size];
		org.lateralgm.resources.GmObject[] iol = i.gmObjects.toArray(new org.lateralgm.resources.GmObject[0]);
		for (int s = 0; s < size; s++)
			{
			GmObject oo = o.gmObjects[s];
			org.lateralgm.resources.GmObject io = iol[s];

			oo.name = io.getName();
			oo.id = io.getId();

			//			oo.sprite = io.get(PGmObject.SPRITE);
			oo.solid = io.get(PGmObject.SOLID);
			oo.visible = io.get(PGmObject.VISIBLE);
			oo.depth = io.get(PGmObject.DEPTH);
			oo.persistent = io.get(PGmObject.PERSISTENT);
			//			GmObject.ByReference parent;
			//			Sprite.ByReference mask;

			oo.mainEventCount = io.mainEvents.size();
			oo.mainEvents = new MainEvent[oo.mainEventCount];
			//TODO: handle main events
			}
		}

	public void writeSprites(EnigmaStruct o, GmFile i)
		{
		int size = i.sprites.size();
		o.spriteCount = size;
		o.sprites = new Sprite[size];
		org.lateralgm.resources.Sprite[] isl = i.sprites.toArray(new org.lateralgm.resources.Sprite[0]);
		for (int s = 0; s < size; s++)
			{
			Sprite os = o.sprites[s];
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
			os.bbMode = is.get(PSprite.BB_MODE); //0*=Automatic, 1=Full image, 2=Manual
			os.bbLeft = is.get(PSprite.BB_LEFT);
			os.bbRight = is.get(PSprite.BB_RIGHT);
			os.bbTop = is.get(PSprite.BB_TOP);
			os.bbBottom = is.get(PSprite.BB_BOTTOM);

			os.subImageCount = is.subImages.size();
			os.subImages = new Image[os.subImageCount];
			//			is.subImages.get(0);
			//			Image[] subImages;
			//			int subImageCount;
			}
		}

	public void writeRooms(EnigmaStruct o, GmFile i)
		{
		int size = i.rooms.size();
		o.roomCount = size;
		o.rooms = new Room[size];
		org.lateralgm.resources.Room[] irl = i.rooms.toArray(new org.lateralgm.resources.Room[0]);
		for (int s = 0; s < size; s++)
			{
			Room os = o.rooms[s];
			org.lateralgm.resources.Room is = irl[s];

			os.name = is.getName();
			os.id = is.getId();

			os.caption = is.get(PRoom.CAPTION);
			os.width = is.get(PRoom.WIDTH);
			os.height = is.get(PRoom.HEIGHT);

			// vvv may be useless vvv //
			os.snapX = is.get(PRoom.SNAP_X);
			os.snapY = is.get(PRoom.SNAP_Y);
			os.isometric = is.get(PRoom.ISOMETRIC);
			// ^^^ may be useless ^^^ //

			os.speed = is.get(PRoom.SPEED);
			os.persistent = is.get(PRoom.PERSISTENT);
			os.backgroundColor = is.get(PRoom.BACKGROUND_COLOR);
			os.drawBackgroundColor = is.get(PRoom.DRAW_BACKGROUND_COLOR);
			os.creationCode = is.get(PRoom.CREATION_CODE);
			}

		/*out.write("rmm".getBytes());
		out.write(0);
		for (Room r : f.rooms)
			{
			writeStr(out,r.get(PRoom.CAPTION));
			out.write4(r.properties,PRoom.WIDTH,PRoom.HEIGHT,PRoom.SPEED);
			boolean dbc = (Boolean) r.get(PRoom.DRAW_BACKGROUND_COLOR);
			int bgc = ((Color) r.get(PRoom.BACKGROUND_COLOR)).getRGB();
			out.write4(dbc ? bgc : 0);
			writeStr(out,r.get(PRoom.CREATION_CODE));
			out.writeBool(r.properties,PRoom.ENABLE_VIEWS);
			if (r.views.size() != 8) throw new IOException("View count mismatch: " + r.views.size());
			for (View view : r.views)
				{

				out.writeBool(view.properties,PView.VISIBLE);
				out.write4(view.properties,PView.VIEW_X,PView.VIEW_Y,PView.VIEW_W,PView.VIEW_H,
						PView.PORT_X,PView.PORT_Y,PView.PORT_W,PView.PORT_H,PView.BORDER_H,PView.BORDER_V,
						PView.SPEED_H,PView.SPEED_V);
				out.writeId((ResourceReference<?>) view.properties.get(PView.OBJECT));
		*/

		/*				out.writeBool(view.properties..visible);
						out.write4(view.viewX);
						out.write4(view.viewY);
						out.write4(view.viewW);
						out.write4(view.viewH);
						out.write4(view.portX);
						out.write4(view.portY);
						out.write4(view.portW);
						out.write4(view.portH);
						out.write4(view.hbor);
						out.write4(view.vbor);
						out.write4(view.hspeed);
						out.write4(view.vspeed);
						out.writeId(view.objectFollowing);
						if (deRef(view.objectFollowing) != null)
							System.out.println("??" + view.objectFollowing.get().getId());
						else
							System.out.println("?-1");
		*/
		/*}
					for (Instance i : r.instances)
						{
						out.write("inst".getBytes());
						out.write4((Integer) i.properties.get(PInstance.ID));
						ResourceReference<GmObject> or = i.properties.get(PInstance.OBJECT);
						out.writeId(or);
						out.write4(i.getPosition().x);
						out.write4(i.getPosition().y);
						writeStr(out,i.getCreationCode());
						}
					out.write4(0);
					}
				*/
		}

	public void toImage(BufferedImage im)
		{
		Image i = new Image();
		i.width = im.getWidth();
		i.height = im.getHeight();
		i.pixels = new int[i.height * i.width];
		//not really sure how to do this efficiently
		/*		for (int y = 0; y < i.height; y++)
					for (int x = 0; x < i.width; x++)
						{
						i.pixels[y * i.width + x] = im.getColorModel().getRed(0);
						}*/
		}

	public boolean writeEvent(GmStreamEncoder out, Event ev) throws IOException
		{
		/*
		String e;
		switch (ev.mainId)
			{
			case MainEvent.EV_CREATE:
				e = "create";
				break;
			case MainEvent.EV_DESTROY:
				e = "destroy";
				break;
			case MainEvent.EV_ALARM:
				e = "alarm";
				break;
			case MainEvent.EV_STEP:
				switch (ev.id)
					{
					case Event.EV_STEP_BEGIN:
						e = "beginstep";
						break;
					case Event.EV_STEP_NORMAL:
						e = "step";
						break;
					case Event.EV_STEP_END:
						e = "endstep";
						break;
					default:
						return false; //don't write unknowns
					}
				break;
			case MainEvent.EV_COLLISION:
				e = "collision";
				break;
			case MainEvent.EV_KEYBOARD:
				e = "keyboard";
				break;
			case MainEvent.EV_MOUSE:
				e = "mouse";
				break;
			case MainEvent.EV_OTHER:
				return false;
			case MainEvent.EV_DRAW:
				e = "draw";
				break;
			case MainEvent.EV_KEYPRESS:
				e = "keypress";
				break;
			case MainEvent.EV_KEYRELEASE:
				e = "keyrelease";
				break;
			default:
				return false;
			}
			*/

		/*
		create; game start; room start; draw; begin step; alarm; keyboard; key press;
		key release; step; path end; outside room; intersect boundary; collision;
		no more lives; no more health; end step; draw; end animation; begin step;
		alarm; destroy; room end; game end.
		*/

		/*
		if (e.length() == 0) return false;
		writeStr(out,e);
		if (ev.mainId == MainEvent.EV_STEP)
			out.write4(0);
		else if (ev.mainId == MainEvent.EV_COLLISION)
			out.writeId(ev.other);
		else
			out.write4(ev.id);
			*/
		return false;
		}

	public static boolean actionDemise = false;

	public static String getActionsCode(Event ev)
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
					{/*
						if (la.execType == Action.EXEC_NONE) break;
						ResourceReference<GmObject> apto = act.getAppliesTo();
						if (la.question)
						if (apto != GmObject.OBJECT_SELF)
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
						if (apto != GmObject.OBJECT_SELF)
						{
						if (apto == GmObject.OBJECT_OTHER)
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
						if (apto != GmObject.OBJECT_SELF) code += "}";
						*/
					}
					break;
				}
			}
		return code;
		}
	}
