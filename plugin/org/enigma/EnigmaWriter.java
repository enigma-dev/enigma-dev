package org.enigma;

import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.JOptionPane;

import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamEncoder;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.Script;
import org.lateralgm.resources.Sprite;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.Event;
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.MainEvent;
import org.lateralgm.resources.sub.View;

public final class EnigmaWriter
	{
	private EnigmaWriter()
		{
		}

	public static boolean writeEgmFile(EnigmaFrame ef, GmFile f, File egmf)
		{
		return new EnigmaWriter().writeFile(ef,f,egmf);
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
			out.write4(3); //version
			writeStr(out,f.gameSettings.gameIconData);

			out.write4(f.scripts.size());
			for (Script s : f.scripts)
				writeStr(out,s.getName());

			ArrayList<String> names = new ArrayList<String>(f.gmObjects.size());
			out.write4(f.gmObjects.size());
			for (GmObject o : f.gmObjects)
				{
				out.write4(o.getId());
				String name = o.getName();
				if (names.contains(name))
					throw new GmFormatException(f,"Duplicate object name: " + name);
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

	public void writeScripts(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("scr".getBytes());
		out.write(0);
		for (Script s : f.scripts)
			writeStr(out,s.scriptStr);
		}

	public void writeObjects(GmFile f, GmStreamEncoder out) throws IOException
		{
		out.write("obj".getBytes());
		out.write(0);
		for (GmObject o : f.gmObjects)
			{
			out.writeId(o.sprite);
			for (int j = 0; j < 11; j++)
				for (Event ev : o.mainEvents[j].events)
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
			out.write4(s.width);
			out.write4(s.height);
			out.write4(s.originX);
			out.write4(s.originY);
			out.write4(s.subImages.size());
			for (BufferedImage bi : s.subImages)
				{
				int width = bi.getWidth();
				int height = bi.getHeight();
				out.write4(s.transparent ? bi.getRGB(0,height - 1) : 0);
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
		  writeStr(out,r.caption); 
		  out.write4(r.width); 
		  out.write4(r.height); 
		  out.write4(r.speed); 
			out.write4(r.drawBackgroundColor ? r.backgroundColor.getRGB() : 0);
			System.out.println(r.backgroundColor.getRGB());
			writeStr(out,r.creationCode);
			out.writeBool(r.enableViews);
			if (r.views.length != 8) throw new IOException("View count mismatch: " + r.views.length);
			for (View view : r.views)
				{
				out.writeBool(view.visible);
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
				}
			for (Instance i : r.instances)
				{
				out.write("inst".getBytes());
				out.write4(i.instanceId);
				out.writeId(i.gmObjectId);
				out.write4(i.getX());
				out.write4(i.getY());
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

	public boolean actionDemise = false;

	public String getActionsCode(Event ev)
		{
		String code = "";
		for (Action act : ev.actions)
			{
			if (act.libAction.actionKind == Action.ACT_CODE)
				code += act.arguments[0].val + System.getProperty("line.separator");
			else
				{
				if (!actionDemise)
					{
					String mess = "Warning, you have a D&D action which is unsupported by this compiler."
							+ " This and future unsupported D&D actions will be discarded.";
					JOptionPane.showMessageDialog(null,mess);
					actionDemise = true;
					}
				}
			}
		return code;
		}
	}
