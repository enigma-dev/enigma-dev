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

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JOptionPane;

import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamEncoder;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Sprite;
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
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.MainEvent;
import org.lateralgm.resources.sub.View;
import org.lateralgm.resources.sub.Instance.PInstance;
import org.lateralgm.resources.sub.View.PView;

public final class EnigmaWriter
	{
	private EnigmaWriter()
		{
		}

	public static boolean writeEgmFile(EnigmaFrame ef, GmFile f, File egmf)
		{
		return new EnigmaWriter().writeFile(ef,f,egmf);
		}

	public static void writeStr(GmStreamEncoder out, Object data) throws IOException
		{
		writeStr(out,(String) data);
		}

	public static void writeStr(GmStreamEncoder out, byte[] data) throws IOException
		{
		out.write4(data.length);
		out.write(data);
		out.write4(0);
		}

	public static void writeStr(GmStreamEncoder out, String data) throws IOException
		{
		out.writeStr(data);
		out.write4(0);
		}

	private boolean writeFile(EnigmaFrame ef, GmFile f, File egmf)
		{
		ef.progress(0,"Sending Enigma resource names");
		GmStreamEncoder out = null;
		try
			{
			out = new GmStreamEncoder(egmf);
			out.write("EGMf".getBytes());
			out.write4(4); //version
			writeStr(out,f.gameSettings.gameIconData);

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
			return true;
			}
		catch (Exception e)
			{
			e.printStackTrace();
			ef.progress(100,"Failed to prepare Enigma",e.getMessage());
			}
		if (out != null)
			{
			try
				{
				out.close();
				egmf.delete();
				}
			catch (IOException e)
				{
				egmf.deleteOnExit();
				e.printStackTrace();
				ef.progress(100,"Failed to prepare Enigma",e.getMessage());
				}
			}
		return false;
		}

	public static ArrayList<LibAction> getQuestionLibActions()
		{
		ArrayList<LibAction> ala = new ArrayList<LibAction>();
		for (Library lib : LibManager.libs)
			for (LibAction act : lib.libActions)
				if (act.question && act.execType == Action.EXEC_CODE) ala.add(act);
		return ala;
		}

	public void writeScripts(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("scr".getBytes());
		out.write(0);
		for (Script s : f.scripts)
			writeStr(out,s.get(PScript.CODE));
		}

	public void writeObjects(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("obj".getBytes());
		out.write(0);
		for (GmObject o : f.gmObjects)
			{
			out.writeId((ResourceReference<?>) o.get(PGmObject.SPRITE));
			out.writeBool(o.properties,PGmObject.SOLID,PGmObject.VISIBLE);
			out.write4(o.properties,PGmObject.DEPTH);
			out.writeBool(o.properties,PGmObject.PERSISTENT);
			out.writeId((ResourceReference<?>) o.get(PGmObject.PARENT)); //or -1 (not -100)
			out.writeId((ResourceReference<?>) o.get(PGmObject.MASK)); //or -1
			for (int j = 0; j < 11; j++)
				for (Event ev : o.mainEvents.get(j).events)
					{
					String s = getActionsCode(ev);
					if (s == null || s.length() == 0) continue;
					if (writeEvent(out,ev)) writeStr(out,s);
					}
			out.write4(0);
			}
		}

	public void writeSprites(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("spr".getBytes());
		out.write(0);
		for (Sprite s : f.sprites)
			{
			out.write4(s.subImages.getWidth());
			out.write4(s.subImages.getHeight());
			out.write4(s.properties,PSprite.ORIGIN_X,PSprite.ORIGIN_Y);
			out.write4(s.subImages.size());
			boolean transp = (Boolean) s.get(PSprite.TRANSPARENT);
			for (BufferedImage bi : s.subImages)
				{
				int width = bi.getWidth();
				int height = bi.getHeight();
				out.write4(transp ? bi.getRGB(0,height - 1) : 0);
				ByteArrayOutputStream data = new ByteArrayOutputStream();
				for (int y = 0; y < height; y++)
					for (int x = 0; x < width; x++)
						{
						int rgb = bi.getRGB(x,y);
						//GM way???
						//						data.write(rgb & 255);
						//						data.write((rgb >> 8) & 255);
						//						data.write((rgb >> 16) & 255);
						//						data.write((rgb >> 24) & 255);

						//Old way
						data.write(rgb >> 16 & 255);
						data.write(rgb >> 8 & 255);
						data.write(rgb & 255);
						}
				out.write4(data.size());
				out.compress(data.toByteArray());
				out.write4(0);
				}
			}
		}

	public void writeRooms(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("rmm".getBytes());
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
*/				}
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
		}

	public boolean writeEvent(GmStreamEncoder out, Event ev) throws IOException
		{
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
		/*
		create; game start; room start; draw; begin step; alarm; keyboard; key press;
		key release; step; path end; outside room; intersect boundary; collision;
		no more lives; no more health; end step; draw; end animation; begin step;
		alarm; destroy; room end; game end.
		*/

		if (e.length() == 0) return false;
		writeStr(out,e);
		if (ev.mainId == MainEvent.EV_STEP)
			out.write4(0);
		else if (ev.mainId == MainEvent.EV_COLLISION)
			out.writeId(ev.other);
		else
			out.write4(ev.id);
		return true;
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
					{
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
					}
					break;
				}
			}
		return code;
		}
	}
