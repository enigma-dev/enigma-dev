package org.enigma.backend;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
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
		static boolean running = true;

		public void callback(final String filename)
			{
			final File file = new File(filename);
			running = true;
			new Thread()
				{
					public void run()
						{
						InputStream in = null;
						while (in == null)
							{
							try
								{
								in = new FileInputStream(file);
								}
							catch (FileNotFoundException e)
								{
								if (!running)
									{
									e.printStackTrace();
									return;
									}
								try
									{
									Thread.sleep(100);
									}
								catch (InterruptedException e1)
									{
									}
								}
							}
						try
							{
							StringBuilder sb = new StringBuilder();
							while (true)
								{
								int size;
								byte[] data;
								//in this case we can use available() because
								//FileInputStream shouldn't return a blocking value
								size = running ? in.available() : 2048;
								if (size <= 0)
									{
									try
										{
										Thread.sleep(100);
										}
									catch (InterruptedException e1)
										{
										}
									continue;
									}
								data = new byte[size];
								size = in.read(data,0,size);
								if (size == -1) break;
								sb.append(new String(data,0,size,"UTF-8"));
								int p = sb.indexOf("\n");
								while (p != -1)
									{
									String dat = sb.substring(0,p + 1);
									ef.ta.append(dat);
									ef.ta.setCaretPosition(ef.ta.getDocument().getLength());
									System.out.print(dat);
									sb.delete(0,p + 1);
									p = sb.indexOf("\n");
									}
								}
							ef.ta.append(sb.toString() + "\n");
							ef.ta.setCaretPosition(ef.ta.getDocument().getLength());
							System.out.println(sb.toString());
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
			OpenFile.running = false;
			}
		}
	}
