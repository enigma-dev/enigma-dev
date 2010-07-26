package org.enigma.backend;

import org.enigma.EnigmaFrame;

import com.sun.jna.Callback;
import com.sun.jna.Structure;

public class EnigmaCallbacks extends Structure
	{
	public OutputOpen coo = new OutputOpen(); //void (*coo) (void)
	public OutputAppend coa = new OutputAppend(); //void (*coa) (String)
	public OutputClear cock = new OutputClear(); //void (*cock) (void)
	public OutputProgress cop = new OutputProgress(); //void (*cop) (int)
	public OutputTip cot = new OutputTip(); //void (*cot) (String)

	public EnigmaCallbacks(EnigmaFrame ef)
		{
		coo.ef = ef;
		coa.ef = ef;
		cock.ef = ef;
		cop.ef = ef;
		cot.ef = ef;
		}

	public static class OutputHolder
		{
		EnigmaFrame ef;
		}

	public static class OutputOpen extends OutputHolder implements Callback
		{
		public void callback()
			{
			ef.setVisible(true);
			}
		}

	public static class OutputAppend extends OutputHolder implements Callback
		{
		public void callback(String msg)
			{
			ef.ta.append(msg);
			}
		}

	public static class OutputClear extends OutputHolder implements Callback
		{
		public void callback()
			{
			ef.ta.setText("");
			}
		}

	public static class OutputProgress extends OutputHolder implements Callback
		{
		public void callback(int amt)
			{
			ef.pb.setValue(amt);
			}
		}

	public static class OutputTip extends OutputHolder implements Callback
		{
		public void callback(String tip)
			{
			ef.pb.setString(tip);
			}
		}
	}
