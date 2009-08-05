package org.enigma;

import java.io.File;
import java.lang.ref.WeakReference;

import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamDecoder;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.GmObject;
import org.lateralgm.resources.Room;
import org.lateralgm.resources.sub.Instance;

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
					inst.gmObjectId = new WeakReference<GmObject>(
							LGM.currentFile.gmObjects.getUnsafe(f.read4()));
					inst.setX(f.read4());
					inst.setY(f.read4());
					break;
				case 1:
					int iid = f.read4();
					int nx = f.read4();
					int ny = f.read4();
					for (Instance i1 : r.instances)
						if (i1.instanceId == iid)
							{
							i1.setX(nx);
							i1.setY(ny);
							break;
							}
					break;
				case 2:
					int iid1 = f.read4();
					f.skip(8);
					for (Instance i1 : r.instances)
						if (i1.instanceId == iid1)
							{
							r.instances.remove(i1);
							break;
							}
					break;	
				default:
					throw new GmFormatException(LGM.currentFile,"Unsupported action type " + action);
				} //type
			} //mods
		} //readChanges
	} //class
