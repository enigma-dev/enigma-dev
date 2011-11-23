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

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;

import javax.swing.SwingUtilities;

import org.enigma.frames.ProgressFrame;
import org.enigma.utility.EnigmaBuildReader;

public class EnigmaThread extends Thread
	{
	private ProgressFrame ef;
	//gobbler
	private BufferedReader g_in = null;
	private PrintStream g_out = null;
	//process
	private Process p = null;
	private byte p_mode;
	private File p_exef;
	//returner
	private byte r_mode;
	private int r_val;
	private File r_exef = null;

	//gobbler
	public EnigmaThread(ProgressFrame ef, InputStream in)
		{
		this.ef = ef;
		g_in = new BufferedReader(new InputStreamReader(in));
		start();
		}

	public EnigmaThread(InputStream in, PrintStream out)
		{
		g_in = new BufferedReader(new InputStreamReader(in));
		g_out = out;
		start();
		}

	//process
	public EnigmaThread(ProgressFrame ef, Process p, byte mode, File exef)
		{
		this.ef = ef;
		this.p = p;
		p_mode = mode;
		p_exef = exef;
		start();
		}

	//returner
	public EnigmaThread(ProgressFrame ef, byte mode, int val, File exef)
		{
		this.ef = ef;
		r_mode = mode;
		r_val = val;
		r_exef = exef;
		}

	private void runGobbler()
		{
		String line;
		try
			{
			while ((line = g_in.readLine()) != null)
				{
				if (g_out == null)
					{
					System.out.println("> " + line); //$NON-NLS-1$
					ef.append(line + '\n'); //internally stored as \n according to java
					}
				else
					g_out.print(line);
				}
			g_in.close();
			if (g_out != null) g_out.close();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			}
		}

	private void runProcess()
		{
		try
			{
			System.out.println("Waiting for Enigma... ");
			int r = p.waitFor();
			System.out.println("Enigma returned " + r);
			SwingUtilities.invokeLater(new EnigmaThread(ef,p_mode,r,p_exef));
			}
		catch (InterruptedException e)
			{
			e.printStackTrace();
			}
		}

	private void runReturn()
		{
		if (r_val == 0)
			{
			if (r_mode == 3)
				{
				ef.progress(99,"Updating build changes");
				try
					{
					EnigmaBuildReader.readChanges(r_exef);
					ef.progress(100,"Finished");
					}
				catch (Exception e)
					{
					ef.progress(100,"Finished with errors",e.getMessage());
					}
				}
			else
				{
				ef.progress(100,"Finished. Running");
				ef.dispose();
				if (r_mode != 4) try
					{
					Process p = Runtime.getRuntime().exec(r_exef.getPath());
					new EnigmaThread(p.getInputStream(),System.out);
					new EnigmaThread(p.getErrorStream(),System.err);
					}
				catch (IOException e)
					{
					e.printStackTrace();
					}
				}
			}
		else if (r_val == -6 || r_val == -7)
			ef.progress(100,"Finished with errors","G++ error. Compile failed (" + r_val + ")");
		else if (r_val < 0)
			ef.progress(100,"Finished with errors","Enigma Communication Error (" + r_val + ")");
		else if (r_val > 0) ef.progress(100,"Finished with errors","Syntax error (" + r_val + ")");
		}

	public void run()
		{
		if (g_in != null)
			{
			runGobbler();
			return;
			}
		if (p != null)
			{
			runProcess();
			return;
			}
		if (r_exef != null)
			{
			runReturn();
			return;
			}
		}
	}
