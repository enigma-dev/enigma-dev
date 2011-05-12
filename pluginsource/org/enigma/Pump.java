package org.enigma;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import org.enigma.backend.EnigmaCallbacks.OutputHandler;

public class Pump extends Thread
	{
	private InputStream is;
	private OutputStream os = null;
	private OutputHandler oh = null;

	public Pump(InputStream is, OutputStream os)
		{
		this.is = is;
		this.os = os;
		}

	public Pump(InputStream is, OutputHandler oh)
		{
		this.is = is;
		this.oh = oh;
		}

	@Override
	public void run()
		{
		if (os == null)
			if (oh == null)
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
				oh.append(line);
			}
		catch (IOException e)
			{
			}
		}
	}
