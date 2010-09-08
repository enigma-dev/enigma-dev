package org.enigma.backend;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.enigma.EnigmaFrame;

import com.sun.jna.Callback;
import com.sun.jna.Structure;

public class EnigmaCallbacks extends Structure
	{
	public Callback coo = new OutputOpen(); //void (*coo) (void)
	public Callback coa = new OutputAppend(); //void (*coa) (String)
	public Callback cock = new OutputClear(); //void (*cock) (void)
	public Callback cop = new OutputProgress(); //void (*cop) (int)
	public Callback cot = new OutputTip(); //void (*cot) (String)
	public Callback cof = new OpenFile(); //void (*cof) (String)
	public Callback ccf = new CloseFile(); //void (*ccf) ()

	public EnigmaCallbacks(EnigmaFrame ef)
		{
		((OutputHolder) coo).ef = ef;
		((OutputHolder) coa).ef = ef;
		((OutputHolder) cock).ef = ef;
		((OutputHolder) cop).ef = ef;
		((OutputHolder) cot).ef = ef;
		((OutputHolder) cof).ef = ef;
		((OutputHolder) ccf).ef = ef;
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
			ef.ta.setCaretPosition(ef.ta.getDocument().getLength());
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

	public static class OpenFile extends OutputHolder implements Callback
		{
		public void callback(final String file)
			{
			new Thread()
				{
					public void run()
						{
						int data;
						try
							{
							InputStream in = new FileInputStream(new File(file));
							while (true)
								{
								if (in.available() > 0)
									{
									data = in.read();
									if (data == -1) break;
									ef.ta.append("" + (char) data);
									ef.ta.setCaretPosition(ef.ta.getDocument().getLength());
									System.out.print("" + (char) data);
									}
								}
							in.close();
							}
						catch (IOException e)
							{
							e.printStackTrace();
							}
						}
				}.start();
			}
		}

	public static class CloseFile extends OutputHolder implements Callback
		{
		public void callback()
			{

			}
		}
	}
