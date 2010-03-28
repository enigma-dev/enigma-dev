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

import java.awt.Point;
import java.io.File;

import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamDecoder;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.sub.Instance;
import org.lateralgm.resources.sub.Instance.PInstance;

public final class EnigmaReader
	{
	private EnigmaReader()
		{
		}

	public static void readChanges(File emd) throws Exception
		{
		GmStreamDecoder f = new GmStreamDecoder(emd);
		int ver = f.read4();
		if (ver != 1) throw new GmFormatException(LGM.currentFile,"Unsupported version " + ver);
		Room r = LGM.currentFile.rooms.getUnsafe(f.read4());
		if (r == null) r = LGM.currentFile.rooms.add();
		int mods = f.read4();
		for (int i = 0; i < mods; i++)
			{
			Instance inst;
			int action = f.read4();
			switch (action)
				{
				case 0:
					inst = r.addInstance();
					GmObject temp = LGM.currentFile.gmObjects.getUnsafe(f.read4());
					if (temp != null) inst.properties.put(PInstance.OBJECT,temp.reference);
					//					inst.setObject(LGM.currentFile.gmObjects.getUnsafe(f.read4()).reference);
					inst.setPosition(new Point(f.read4(),f.read4()));
					break;
				case 1:
					inst = r.instances.get(f.read4());
					inst.setPosition(new Point(f.read4(),f.read4()));
					break;
				case 2:
					inst = r.instances.remove(f.read4());
					f.skip(8);
					break;
				default:
					throw new GmFormatException(LGM.currentFile,"Unsupported action type " + action);
				} //type
			} //mods
		} //readChanges
	} //class
