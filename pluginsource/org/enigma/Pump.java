package org.enigma;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import javax.swing.JTextArea;

public class Pump extends Thread
	{
	private InputStream is;
	private OutputStream os = null;
	private JTextArea ot = null;

	public Pump(InputStream is, OutputStream os)
		{
		this.is = is;
		this.os = os;
		}

	public Pump(InputStream is, JTextArea ot)
		{
		this.is = is;
		this.ot = ot;
		}

	@Override
	public void run()
		{
		if (os == null)
			if (ot == null)
				runDump();
			else
				runText();
		else
			runOut();
		try
			{
			is.close();
			}
		catch (IOException e)
			{
			}
		}

	public void runOut()
		{
		int length;
		byte[] buf = new byte[1024];
		try
			{
			while ((length = is.read(buf)) > 0)
				os.write(buf,0,length);
			}
		catch (IOException e)
			{
			}
		finally
			{
			try
				{
				os.close();
				}
			catch (IOException e)
				{
				}
			}
		}

	public void runDump()
		{
		int length;
		byte[] buf = new byte[1024];
		try
			{
			do
				length = is.read(buf);
			while (length > 0);
			}
		catch (IOException e)
			{
			}
		}

	public void runText()
		{
		BufferedReader br = new BufferedReader(new InputStreamReader(is));
		String line;
		try
			{
			while ((line = br.readLine()) != null)
				{
				ot.append(line);
				ot.setCaretPosition(ot.getDocument().getLength());
				}
			}
		catch (IOException e)
			{
			}
		}
	}
