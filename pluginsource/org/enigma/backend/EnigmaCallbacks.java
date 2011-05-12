package org.enigma.backend;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.zip.Deflater;

import org.enigma.EnigmaRunner;
import org.enigma.Pump;
import org.enigma.backend.util.Image;

import com.sun.jna.Callback;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

public class EnigmaCallbacks extends Structure
	{
	public Callback coo = new OutputOpen(); //void (*coo) (void)
	public Callback coa = new OutputAppend(); //void (*coa) (String)
	public Callback cock = new OutputClear(); //void (*cock) (void)
	public Callback cop = new OutputProgress(); //void (*cop) (int)
	public Callback cot = new OutputTip(); //void (*cot) (String)

	public Callback cof = new OpenFile(); //void (*cof) (String)
	public Callback ccf = new CloseFile(); //void (*ccf) (void)
	public Callback cex = new Execute(); //int (*cex) (String, String[], boolean)
	public Callback ccd = new CompressData(); //Image* (*ccd) (byte[], int)

	public static interface OutputHandler
		{
		public abstract void open();

		public abstract void append(String s);

		public abstract void clear();

		public abstract void progress(int i);

		public abstract void tip(String s);
		}

	public EnigmaCallbacks(OutputHandler out)
		{
		((OutputHolder) coo).out = out;
		((OutputHolder) coa).out = out;
		((OutputHolder) cock).out = out;
		((OutputHolder) cop).out = out;
		((OutputHolder) cot).out = out;
		((OutputHolder) cof).out = out;
		((OutputHolder) cex).out = out;
		}

	public static class OutputHolder
		{
		OutputHandler out;
		}

	public static class OutputOpen extends OutputHolder implements Callback
		{
		public void callback()
			{
			out.open();
			}
		}

	public static class OutputAppend extends OutputHolder implements Callback
		{
		public void callback(String msg)
			{
			out.append(msg);
			}
		}

	public static class OutputClear extends OutputHolder implements Callback
		{
		public void callback()
			{
			out.clear();
			}
		}

	public static class OutputProgress extends OutputHolder implements Callback
		{
		public void callback(int amt)
			{
			out.progress(amt);
			}
		}

	public static class OutputTip extends OutputHolder implements Callback
		{
		public void callback(String tip)
			{
			out.tip(tip);
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
								size = in.available();
								if (size > 0)
									{
									data = new byte[size];
									size = in.read(data,0,size);
									if (size == -1) break;
									sb.append(new String(data,0,size,"UTF-8"));
									int p = sb.indexOf("\n");
									while (p != -1)
										{
										String dat = sb.substring(0,p + 1);
										out.append(dat);
										sb.delete(0,p + 1);
										p = sb.indexOf("\n");
										}
									}
								else
									{
									try
										{
										Thread.sleep(100);
										}
									catch (InterruptedException e1)
										{
										}
									}
								}
							out.append(sb.toString() + "\n");
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

	public static class CloseFile implements Callback
		{
		public void callback()
			{
			OpenFile.running = false;
			}
		}

	public static class CompressData implements Callback
		{
		public Image callback(Pointer data, int size)
			{
			byte[] inData = data.getByteArray(0,size);
			Image ret = new Image.ByReference();

			Deflater compresser = new Deflater();
			compresser.setInput(inData,0,size);
			compresser.finish();
			byte[] buffer = new byte[100];
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			while (!compresser.finished())
				{
				int len = compresser.deflate(buffer);
				baos.write(buffer,0,len);
				}
			baos.toByteArray();

			ret.dataSize = baos.size();
			ret.data = ByteBuffer.allocateDirect(baos.size()).put(baos.toByteArray());

			return ret;
			}
		}

	public static class Execute extends OutputHolder implements Callback
		{
		//returns -1 bad redirect, -2 redirect can't open, -3 cmd fail, -4 wait interrupted
		//on success, returns 0 for no wait, or the executed return value on wait
		public int callback(String cmd, String[] envp, boolean wait)
			{
			int p = cmd.lastIndexOf(">");
			if (p < 2) return -1; //no > or starts with >
			char mod = cmd.charAt(p - 1);
			cmd = cmd.substring(0,p - 1);
			String redir = cmd.substring(p + 1).trim();
			final String REDIR_IDE = "IDE_CONSOLE";

			Pattern regex = Pattern.compile("^\\s*(\"?)\\\\"); //^\s*("?)\\
			String replace = "$1" + Matcher.quoteReplacement(new File("/").getAbsolutePath());
			cmd = regex.matcher(cmd).replaceFirst(replace);
			redir = regex.matcher(redir).replaceFirst(replace);

			try
				{
				final Process pr = Runtime.getRuntime().exec(cmd,envp,EnigmaRunner.WORKDIR);
				InputStream in = pr.getInputStream();
				InputStream er = pr.getErrorStream();
				if (redir.equals(REDIR_IDE))
					{
					new Pump(in,mod == '2' ? null : out).start();
					new Pump(er,mod == '2' || mod == '&' ? out : null).start();
					}
				else
					{
					OutputStream os = new FileOutputStream(redir);
					new Pump(in,mod == '2' ? null : os).start();
					new Pump(er,mod == '2' || mod == '&' ? os : null).start();
					}

				if (wait) return pr.waitFor();
				new Thread()
					{
						public void run()
							{
							try
								{
								pr.wait();
								}
							catch (InterruptedException e)
								{
								System.err.println("Exec: " + e.getLocalizedMessage());
								}
							}
					}.start();
				}
			catch (FileNotFoundException e)
				{
				System.err.println("Exec: " + e.getLocalizedMessage());
				return -2;
				}
			catch (IOException e)
				{
				System.err.println("Exec: " + e.getLocalizedMessage());
				return -3;
				}
			catch (InterruptedException e)
				{
				System.err.println("Exec: " + e.getLocalizedMessage());
				return -4;
				}

			if (true) return 0;

			int state = 0; //0 _
			boolean space = true;
			boolean quoted = false;
			boolean escaped = false;

			for (char c : cmd.toCharArray())
				{
				if (escaped)
					escaped = false;
				else if (c == '\\')
					{
					if (space)
						{
						//insert root
						space = false;
						continue;
						}
					else
						escaped = true;
					}
				else if (space && c != ' ')
					{
					space = false;
					quoted = c == '"';
					}

				if (state == 2 || state == 4)
					{
					state--;
					continue;
					}
				switch (c)
					{
					case ' ':
						if (state == 2)
							state = 1;
						else if (state == 3)
							state = 3;
						else if (state == 4)
							state = 3;
						else
							state = 0; //(1,_) |- 0
						break;
					case '"':

					} //switch
				} //for

			return -2;
			}
		}
	}
