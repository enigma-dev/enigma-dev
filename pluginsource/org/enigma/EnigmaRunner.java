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

import java.awt.Graphics;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.URL;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JInternalFrame;
import javax.swing.JLayeredPane;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import org.enigma.EnigmaSettingsFrame.EnigmaSettings;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaStruct;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.impl.TextAreaFocusTraversalPolicy;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;
import org.lateralgm.main.LGM.ReloadListener;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.Script;
import org.lateralgm.subframes.ScriptFrame;
import org.lateralgm.subframes.SubframeInformer;
import org.lateralgm.subframes.SubframeInformer.SubframeListener;

import com.sun.jna.StringArray;

public class EnigmaRunner implements ActionListener,SubframeListener,ReloadListener
	{
	public static final String ENIGMA = "compileEGMf.exe";
	public EnigmaFrame ef;
	public EnigmaSettings es = new EnigmaSettings();
	public EnigmaSettingsFrame esf = new EnigmaSettingsFrame(es);
	public JMenuItem run, debug, build, compile;
	public JMenuItem showFunctions, showGlobals, showTypes;
	/** This is static because it belongs to EnigmaStruct's dll, which is statically loaded. */
	public static boolean GCC_LOCATED = false;
	public final EnigmaNode node = new EnigmaNode();

	public EnigmaRunner()
		{
		attemptUpdate();
		LGM.addReloadListener(this);
		initEnigmaLib();
		populateMenu();
		populateTree();
		SubframeInformer.addSubframeListener(this);
		applyBackground("org/enigma/enigma.png");

		LGM.mdi.add(esf);

		//Apparently josh wants a textbox for if it returns 0 (gcc found)?
		//I'm guessing he also wants me to get text from him and put it in here
		if (false)
			{
			ef = new EnigmaFrame();
			PrintStream oldOut = System.out;
			System.setOut(new PrintStream(new TextAreaOutputStream(ef.ta)));
			}

		if (GCC_LOCATED) EnigmaDriver.whitespaceModified(es.definitions);
		}

	private void initEnigmaLib()
		{
		System.out.print("Initializing Enigma: ");
		int ret = EnigmaDriver.libInit();
		if (ret == 0)
			{
			GCC_LOCATED = true;
			return;
			}

		if (ret == 1) ret = locateGCC();
		if (ret != 0)
			{
			String err;
			switch (ret)
				{
				case 1:
					err = "ENIGMA: GCC not found";
					break;
				case 2:
					err = "ENIGMA: No output gained from call to GCC";
					break;
				case 3:
					err = "ENIGMA: Output from GCC doesn't make sense";
					break;
				default:
					err = "ENIGMA: Undefined error " + ret;
					break;
				}
			JOptionPane.showMessageDialog(null,err);
			}
		}

	public int locateGCC()
		{
		//this is my sad attempt at trying to get the user to locate GCC
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		fc.setDialogTitle("ENIGMA: Please locate the GCC directory (containing bin/, lib/ etc)");
		fc.setAcceptAllFileFilterUsed(false);
		if (fc.showOpenDialog(null) != JFileChooser.APPROVE_OPTION) return 0;
		int ret = EnigmaDriver.gccDefinePath(fc.getSelectedFile().getAbsolutePath());
		if (ret == 0) GCC_LOCATED = true;
		return ret;
		}

	public class TextAreaOutputStream extends OutputStream
		{
		StringBuilder sb = new StringBuilder();
		JTextArea ta;

		public TextAreaOutputStream(JTextArea ta)
			{
			this.ta = ta;
			}

		@Override
		public void write(byte[] b) throws IOException
			{
			flush();
			ta.append(new String(b));
			}

		public void write(byte[] b, int off, int len) throws IOException
			{
			flush();
			ta.append(new String(b,off,len));
			}

		@Override
		public void write(int b) throws IOException
			{
			sb.append((char) b);
			if (b == '\n') flush();
			}

		public void flush() throws IOException
			{
			if (sb.length() != 0)
				{
				ta.append(sb.toString());
				sb = new StringBuilder();
				}
			}
		}

	public void populateMenu()
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

		menu.addSeparator();

		JMenuItem mi = new JMenuItem("Settings");
		mi.addActionListener(node);
		menu.add(mi);

		JMenu sub = new JMenu("Show Built-in...");
		menu.add(sub);

		showFunctions = new JMenuItem("Functions");
		showFunctions.addActionListener(this);
		sub.add(showFunctions);
		showGlobals = new JMenuItem("Globals");
		showGlobals.addActionListener(this);
		sub.add(showGlobals);
		showTypes = new JMenuItem("Types");
		showTypes.addActionListener(this);
		sub.add(showTypes);

		LGM.frame.getJMenuBar().add(menu,1);
		}

	public void populateTree()
		{
		LGM.root.add(node);

		/*EnigmaGroup node = new EnigmaGroup();
		LGM.root.add(node);
		node.add(new EnigmaNode("Whitespace"));
		node.add(new EnigmaNode("Enigma Init"));
		node.add(new EnigmaNode("Enigma Term"));*/

		LGM.tree.updateUI();
		}

	public void applyBackground(String bgloc)
		{
		ImageIcon bg = findIcon(bgloc);
		LGM.mdi.add(new MDIBackground(bg),JLayeredPane.FRAME_CONTENT_LAYER);
		}

	public void attemptUpdate()
		{
		try
			{
			EnigmaUpdater.checkForUpdates();
			}
		/**
		 * Usually you shouldn't catch an Error, however,
		 * in this case we catch it to abort the module,
		 * rather than allowing the failed module to cause
		 * the entire program/plugin to fail
		 */
		catch (NoClassDefFoundError e)
			{
			String error = "SvnKit missing, corrupted, or unusable. Please download and "
					+ "place next to the enigma plugin in order to enable auto-update.";
			System.err.println(error);
			}
		}

	public class MDIBackground extends JComponent
		{
		private static final long serialVersionUID = 1L;
		ImageIcon image;

		public MDIBackground(ImageIcon icon)
			{
			image = icon;
			if (image == null) return;
			if (image.getIconWidth() <= 0) image = null;
			}

		public int getWidth()
			{
			return LGM.mdi.getWidth();
			}

		public int getHeight()
			{
			return LGM.mdi.getHeight();
			}

		public void paintComponent(Graphics g)
			{
			super.paintComponent(g);
			if (image == null) return;
			for (int y = 0; y < getHeight(); y += image.getIconHeight())
				for (int x = 0; x < getWidth(); x += image.getIconWidth())
					g.drawImage(image.getImage(),x,y,null);
			}
		}

	public class EnigmaNode extends ResNode implements ActionListener
		{
		private static final long serialVersionUID = 1L;
		private JPopupMenu pm;

		public EnigmaNode()
			{
			super("Enigma Settings",ResNode.STATUS_SECONDARY,Resource.Kind.GAMESETTINGS);
			pm = new JPopupMenu();
			pm.add(new JMenuItem("Edit")).addActionListener(this);
			}

		public void openFrame()
			{
			esf.toTop();
			}

		public void showMenu(MouseEvent e)
			{
			pm.show(e.getComponent(),e.getX(),e.getY());
			}

		public void actionPerformed(ActionEvent e)
			{
			openFrame();
			}

		public DataFlavor[] getTransferDataFlavors()
			{
			return null;
			}

		public boolean isDataFlavorSupported(DataFlavor flavor)
			{
			return false;
			}

		public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException
			{
			throw new UnsupportedFlavorException(flavor);
			}
		}

	public void compile(final byte mode)
		{
		//modes: 0=run, 1=debug, 2=build, 3=compile
		if (!GCC_LOCATED)
			{
			JOptionPane.showMessageDialog(null,"You can't compile without GCC.");
			return;
			}

		//determine where to output the exe
		File exef = null;
		try
			{
			if (mode < 2)
				exef = File.createTempFile("egm",".exe");
			else if (mode == 2) exef = File.createTempFile("egm",".emd");
			if (exef != null) exef.deleteOnExit();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			return;
			}
		if (mode == 3)
			{
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(new CustomFileFilter(".exe","Executable files"));
			if (fc.showSaveDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			exef = fc.getSelectedFile();
			}

		LGM.commitAll();
		//		ef = new EnigmaFrame();
		//				System.out.println("Compiling with " + enigma);

		EnigmaStruct es = EnigmaWriter.prepareStruct(LGM.currentFile);
		System.out.println(EnigmaDriver.compileEGMf(es,exef.getAbsolutePath(),mode));

		if (mode == 2)
			{
			try
				{
				EnigmaReader.readChanges(exef);
				}
			catch (Exception e)
				{
				e.printStackTrace();
				}
			}

		}

	//This can be static since the GCC_LOCATED and Enigma dll are both static.
	public static SyntaxError checkSyntax(String code)
		{
		if (!GCC_LOCATED)
			{
			JOptionPane.showMessageDialog(null,"You can't compile without GCC.");
			return null;
			}

		String osl[] = new String[LGM.currentFile.scripts.size()];
		Script isl[] = LGM.currentFile.scripts.toArray(new Script[0]);
		for (int i = 0; i < osl.length; i++)
			osl[i] = isl[i].getName();
		return EnigmaDriver.syntaxCheck(osl.length,new StringArray(osl),code);
		}

	public void actionPerformed(ActionEvent e)
		{
		Object s = e.getSource();
		if (s == run) compile((byte) 1);
		if (s == debug) compile((byte) 2);
		if (s == build) compile((byte) 3);
		if (s == compile) compile((byte) 4);

		if (s == showFunctions) showStuffFrame(0);
		if (s == showGlobals) showStuffFrame(1);
		if (s == showTypes) showStuffFrame(2);
		}

	MDIFrame stuffFrames[] = new MDIFrame[3];
	JTextArea stuffLists[] = new JTextArea[3];

	public void showStuffFrame(int mode)
		{
		String[] modes = { "Functions","Globals","Types" };
		if (stuffFrames[mode] == null)
			{
			stuffFrames[mode] = new MDIFrame(modes[mode],true,true,true,true);
			stuffFrames[mode].setSize(200,400);
			stuffFrames[mode].setDefaultCloseOperation(JInternalFrame.HIDE_ON_CLOSE);
			LGM.mdi.add(stuffFrames[mode]);
			stuffLists[mode] = new JTextArea();
			stuffLists[mode].setEditable(false);
			stuffFrames[mode].getContentPane().add(new JScrollPane(stuffLists[mode]));
			stuffFrames[mode].setFocusTraversalPolicy(new TextAreaFocusTraversalPolicy(stuffLists[mode]));
			}
		stuffFrames[mode].toTop();
		}

	public void subframeAppeared(MDIFrame source)
		{
		if (!(source instanceof ScriptFrame)) return;
		final ScriptFrame sf = (ScriptFrame) source;
		JButton syntaxCheck;
		ImageIcon i = findIcon("syntax.png");
		if (i == null)
			syntaxCheck = new JButton("Syntax");
		else
			{
			syntaxCheck = new JButton(i);
			syntaxCheck.setToolTipText("Syntax");
			}
		syntaxCheck.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
					{
					SyntaxError se = checkSyntax(sf.code.getText());
					if (se == null) return;
					int max = sf.code.getDocumentLength() - 1;
					if (se.absoluteIndex > max) se.absoluteIndex = max;
					if (se.absoluteIndex != -1) //-1 = no error
						{
						sf.code.setSelectionStart(se.absoluteIndex);
						sf.code.setSelectionEnd(se.absoluteIndex + 1);
						//TODO: Statusbar with error message
						}
					}
			});
		sf.tool.add(syntaxCheck,5);
		}

	@Override
	public void reloadPerformed(boolean newRoot)
		{
		if (newRoot) populateTree();
		es = new EnigmaSettings();
		esf.setComponents(es);
		}

	public ImageIcon findIcon(String loc)
		{
		ImageIcon ico = new ImageIcon(loc);
		if (ico.getIconWidth() != -1) return ico;

		URL url = this.getClass().getClassLoader().getResource(loc);
		if (url != null) ico = new ImageIcon(url);
		if (ico.getIconWidth() != -1) return ico;

		loc = "org/enigma/" + loc;
		ico = new ImageIcon(loc);
		if (ico.getIconWidth() != -1) return ico;

		url = this.getClass().getClassLoader().getResource(loc);
		if (url != null) ico = new ImageIcon(url);
		return ico;
		}

	public boolean subframeRequested(Resource<?,?> res, ResNode node)
		{
		return false;
		}

	public static void main(String[] args)
		{
		LGM.main(args);
		new EnigmaRunner();
		}
	}
