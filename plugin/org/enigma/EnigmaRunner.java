package org.enigma;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.Script;
import org.lateralgm.subframes.ScriptFrame;
import org.lateralgm.subframes.SubframeInformer;
import org.lateralgm.subframes.SubframeInformer.SubframeListener;

public class EnigmaRunner implements ActionListener, SubframeListener
	{
	public static final String ENIGMA = "compileEGMf.exe";
	public EnigmaFrame ef;
	public JMenuItem run, debug, build, compile;
	public JButton syntaxCheck;

	public EnigmaRunner()
		{
		JMenu menu = new JMenu("Enigma");

		run = new JMenuItem("Run");
		run.addActionListener(this);
		menu.add(run);
		debug = new JMenuItem("Debug");
		debug.addActionListener(this);
		menu.add(debug);
		build = new JMenuItem("Build");
		build.addActionListener(this);
		menu.add(build);
		compile = new JMenuItem("Compile");
		compile.addActionListener(this);
		menu.add(compile);

		LGM.frame.getJMenuBar().add(menu,1);
		SubframeInformer.addSubframeListener(this);
		}

	public String findEnigma(String expected)
		{
		String path = expected;
		File f = new File(path);
		if (!f.exists()) path = System.getProperty("user.dir") + File.separator + path;
		if (!new File(path).exists())
			{
			System.err.println("Unable to locate Enigma");
			return null;
			}
		if (!new File(path).equals(f))
			{
			System.out.println(f.getAbsolutePath() + " not found.");
			return path;
			}
		return f.getAbsolutePath();
		}

	public void compile(final byte mode)
		{
		String arg1;
		switch (mode)
			{
			case 1:
				arg1 = "-c";
				break;
			case 2:
				arg1 = "-d";
				break;
			case 3:
				arg1 = "-b";
				break;
			case 4:
				arg1 = "-c";
				break;
			default:
				return;
			}

		File egmf, exef = null;
		try
			{
			egmf = File.createTempFile("egm",".egm");
			egmf.deleteOnExit();

			if (mode < 3)
				exef = File.createTempFile("egm",".exe");
			else if (mode == 3) exef = File.createTempFile("egm",".emd");
			if (exef != null) exef.deleteOnExit();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			return;
			}
		if (mode == 4)
			{
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(new CustomFileFilter(".exe","Executable files"));
			if (fc.showSaveDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			exef = fc.getSelectedFile();
			}

		LGM.commitAll();
		ef = new EnigmaFrame();
		if (!EnigmaWriter.writeEgmFile(ef,LGM.currentFile,egmf)) return;

		String enigma = findEnigma(ENIGMA);
		if (enigma == null)
			{
			ef.progress(100,"Failed, Enigma not found");
			return;
			}
		System.out.println("Compiling with " + enigma);

		try
			{
			String[] cmd = new String[] { enigma,arg1,egmf.getPath(),exef.getPath() };
			Process p = Runtime.getRuntime().exec(cmd);
			new EnigmaThread(ef,p.getInputStream());
			new EnigmaThread(ef,p.getErrorStream());
			new EnigmaThread(ef,p,mode,exef);
			}
		catch (Exception e)
			{
			e.printStackTrace();
			ef.progress(100,"Finished with errors",e.getMessage());
			return;
			}
		}

	public static int checkSyntax(String code)
		{
		BufferedWriter f;
		try
			{
			f = new BufferedWriter(new FileWriter("syntax.txt"));
			f.write('1');
			f.newLine();
			for (Script scr : LGM.currentFile.scripts)
				f.write(scr.getName() + ",");
			f.newLine();
			f.write(code);
			f.close();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			}

		String path = "compileEGMf.exe";
		File fi = new File(path);
		if (!fi.exists()) path = System.getProperty("user.dir") + File.separator + path;
		if (!new File(path).exists())
			{
			JOptionPane.showMessageDialog(null,"Unable to locate Enigma for Syntax Check");
			return -1;
			}
		if (!new File(path).equals(fi))
			System.out.println(fi.getAbsolutePath() + " not found. Attempting " + path);
		else
			System.out.println("Checking syntax with " + fi.getAbsolutePath());
		try
			{
			Process p = Runtime.getRuntime().exec("\"" + path + "\" -s syntax.txt");
			BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));
			String line;
			String output = "";
			while ((line = input.readLine()) != null)
				if (line.length() > 0) output += line + "\n";
			input.close();
			int r = p.waitFor();
			if (r == -1) return -1;

			JOptionPane.showMessageDialog(null,output);
			return r;
			}
		catch (Exception e)
			{
			e.printStackTrace();
			}
		return -1;
		}

	public void actionPerformed(ActionEvent e)
		{
		if (e.getSource() == run)
			{
			compile((byte) 1);
			}
		if (e.getSource() == debug)
			{
			compile((byte) 2);
			}
		if (e.getSource() == build)
			{
			compile((byte) 3);
			}
		if (e.getSource() == compile)
			{
			compile((byte) 4);
			}
		if (e.getSource() == syntaxCheck)
			{
			Container c = syntaxCheck.getParent();
			if (c instanceof ScriptFrame)
				checkSyntax(((ScriptFrame)c).code.getText());
			}
		}

	public void subframeAppeared(MDIFrame source)
		{
		if (!(source instanceof ScriptFrame)) return;
		ScriptFrame sf = (ScriptFrame) source;
		syntaxCheck = new JButton("Syntax");
		syntaxCheck.addActionListener(this);
		sf.tool.add(syntaxCheck);
		}

	public boolean subframeRequested(Resource<?> res, ResNode node)
		{
		return false;
		}
	}
