/*
 * Copyright (C) 2008, 2009, 2010 IsmAvatar <IsmAvatar@gmail.com>
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

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Scanner;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;
import java.util.prefs.Preferences;
import java.util.regex.Pattern;

import javax.swing.AbstractListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.enigma.EYamlParser.YamlNode;
import org.enigma.backend.EnigmaCallbacks;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.enigma.backend.EnigmaSettings;
import org.enigma.backend.EnigmaStruct;
import org.lateralgm.components.ErrorDialog;
import org.lateralgm.components.GMLTextArea;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.jedit.GMLKeywords;
import org.lateralgm.jedit.GMLKeywords.Construct;
import org.lateralgm.jedit.GMLKeywords.Function;
import org.lateralgm.main.LGM;
import org.lateralgm.main.LGM.ReloadListener;
import org.lateralgm.messages.Messages;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.Script;
import org.lateralgm.subframes.ActionFrame;
import org.lateralgm.subframes.CodeFrame;
import org.lateralgm.subframes.ScriptFrame;
import org.lateralgm.subframes.SubframeInformer;
import org.lateralgm.subframes.SubframeInformer.SubframeListener;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.StringArray;

public class EnigmaRunner implements ActionListener,SubframeListener,ReloadListener
	{
	public static final String ENIGMA = "compileEGMf.exe";
	public static boolean ENIGMA_READY = false, ENIGMA_FAIL = false, SHUTDOWN = false;
	public static final int MODE_RUN = 0, MODE_DEBUG = 1, MODE_DESIGN = 2;
	public static final int MODE_COMPILE = 3, MODE_REBUILD = 4;
	public static final File WORKDIR = LGM.workDir.getParentFile();

	/** This is static because it belongs to EnigmaStruct's dll, which is statically loaded. */
	public static EnigmaDriver DRIVER;

	public EnigmaFrame ef = new EnigmaFrame();
	/** This is global scoped so that it doesn't get GC'd */
	private EnigmaCallbacks ec = new EnigmaCallbacks(ef);
	public EnigmaSettings es;
	public EnigmaSettingsFrame esf;
	public JMenuItem run, debug, design, compile, rebuild;
	public JMenuItem showFunctions, showGlobals, showTypes;
	public final EnigmaNode node = new EnigmaNode();

	public EnigmaRunner()
		{
		populateMenu();
		populateTree();
		LGM.addReloadListener(this);
		SubframeInformer.addSubframeListener(this);
		applyBackground("org/enigma/enigma.png");

		Runtime.getRuntime().addShutdownHook(new Thread()
			{
				public void run()
					{
					SHUTDOWN = true;
					}
			});

		new Thread()
			{
				public void run()
					{
					//Disable updates by removing plugins/shared/svnkit.jar (e.g. linux packages)
					Preferences root = Preferences.userRoot().node("/org/enigma");
					boolean rebuild = root.getBoolean("NEEDS_REBUILD",false);
					root.putBoolean("NEEDS_REBUILD",false);
					int updates = attemptUpdate(); //displays own error
					if (updates == -1)
						{
						ENIGMA_FAIL = true;
						return;
						}
					if (updates == 1 || rebuild || attemptLib() != null)
						{
						if (!make()) //displays own error
							{
							ENIGMA_FAIL = true;
							return;
							}
						}
					Error e = attemptLib();
					if (e != null)
						{
						String err = "ENIGMA: Unable to communicate with the library,\n"
								+ "either because it could not be found or uses methods different from those expected.\n"
								+ "The exact error is:\n" + e.getMessage();
						JOptionPane.showMessageDialog(null,err);
						ENIGMA_FAIL = true;
						return;
						}

					System.out.println("Initializing Enigma: ");
					String err = DRIVER.libInit(ec);
					if (err != null)
						{
						JOptionPane.showMessageDialog(null,err);
						ENIGMA_FAIL = true;
						return;
						}

					ENIGMA_READY = true;
					es = new EnigmaSettings();
					esf = new EnigmaSettingsFrame(es);
					LGM.mdi.add(esf);
					es.commitToDriver();
					populateKeywords();
					}
			}.start();
		}

	private UnsatisfiedLinkError attemptLib()
		{
		try
			{
			String lib = "compileEGMf";
			NativeLibrary.addSearchPath(lib,".");
			NativeLibrary.addSearchPath(lib,LGM.workDir.getParent());
			DRIVER = (EnigmaDriver) Native.loadLibrary(lib,EnigmaDriver.class);
			return null;
			}
		catch (UnsatisfiedLinkError e)
			{
			return e;
			}
		}

	public boolean make()
		{
		String make, path;

		//try to read the YAML definition for `make` on this platform
		try
			{
			try
				{
				File gccey = new File(new File("Compilers",TargetHandler.getOS()),"gcc.ey");
				YamlNode n = EYamlParser.parse(new Scanner(gccey));
				make = n.getMC("Make"); //or OOB
				path = n.getMC("Path","");
				//replace starting \ with root
				if (make.startsWith("\\")) make = new File("/").getAbsolutePath() + make.substring(1);
				}
			catch (FileNotFoundException e)
				{
				throw new GmFormatException(null,e);
				}
			catch (IndexOutOfBoundsException e)
				{
				throw new GmFormatException(null,e);
				}
			}
		catch (GmFormatException e2)
			{
			e2.printStackTrace();
			new ErrorDialog(null,"Unable to Update Enigma",
					"Enigma cannot run because it does not recognize your platform,"
							+ " or does not have `make` definitions for your platform.",Messages.format(
							"Listener.DEBUG_INFO", //$NON-NLS-1$
							e2.getClass().getName(),e2.getMessage(),e2.stackAsString())).setVisible(true);
			return false;
			}

		//run make
		Process p = null;
		try
			{
			p = Runtime.getRuntime().exec(make + " eTCpath=\"" + path + "\"",null,
					LGM.workDir.getParentFile());
			}
		catch (IOException e)
			{
			GmFormatException e2 = new GmFormatException(null,e);
			e2.printStackTrace();
			new ErrorDialog(
					null,
					"Unable to Update Enigma",
					"Enigma cannot run because it requires the `make` tool, which could not be found.\n"
							+ "Please ensure that `make` is properly installed and then restart the application.",
					Messages.format("Listener.DEBUG_INFO", //$NON-NLS-1$
							e2.getClass().getName(),e2.getMessage(),e2.stackAsString())).setVisible(true);
			return false;
			}

		//Set up listeners, waitFor, finish successfully
		System.out.println("Calling `" + make + " eTCpath=\"" + path + "\"" + "`");
		ef.ta.append("Calling `" + make + " eTCpath=\"" + path + "\"" + "`");
		new EnigmaThread(ef,p.getInputStream());
		new EnigmaThread(ef,p.getErrorStream());
		ef.setVisible(true);
		try
			{
			System.out.println(p.waitFor());
			}
		catch (InterruptedException e)
			{
			System.out.println("wut?");
			e.printStackTrace();
			}
		ef.setVisible(false);
		return true;
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
		design = new JMenuItem("Design");
		design.addActionListener(this);
		menu.add(design);
		compile = new JMenuItem("Compile");
		compile.addActionListener(this);
		menu.add(compile);
		rebuild = new JMenuItem("Rebuild All");
		rebuild.addActionListener(this);
		menu.add(rebuild);

		menu.addSeparator();

		JMenuItem mi = new JMenuItem("Settings");
		mi.addActionListener(node);
		menu.add(mi);

		JMenu sub = new JMenu("Keyword List");
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

	public void populateKeywords()
		{
		SortedSet<Function> fl = new TreeSet<Function>(new Comparator<Function>()
			{
				@Override
				public int compare(Function o1, Function o2)
					{
					return o1.getName().compareTo(o2.getName());
					}
			});
		for (Function f : GMLKeywords.FUNCTIONS)
			fl.add(f);
		Set<Construct> cl = new HashSet<Construct>();
		String res = DRIVER.first_available_resource();
		while (res != null)
			{
			if (nameRegex.matcher(res).matches())
				{
				if (DRIVER.resource_isFunction())
					{
					int min = DRIVER.resource_argCountMin();
					int max = DRIVER.resource_argCountMax();
					String args = Integer.toString(min);
					if (min != max) args += "-" + max;
					fl.add(new Function(res,args,""));
					}
				//					else if (DRIVER.resource_isGlobal()) rl.add(res);
				else if (DRIVER.resource_isTypeName()) cl.add(new Construct(res));
				}
			res = DRIVER.next_available_resource();
			}
		GMLKeywords.FUNCTIONS = fl.toArray(new Function[0]);
		for (Construct c : GMLKeywords.CONSTRUCTS)
			cl.add(c);
		GMLKeywords.CONSTRUCTS = cl.toArray(new Construct[0]);
		}

	public void applyBackground(String bgloc)
		{
		ImageIcon bg = findIcon(bgloc);
		LGM.mdi.add(new MDIBackground(bg),JLayeredPane.FRAME_CONTENT_LAYER);
		}

	/** Attempts to checkout/update. Returns 0/1 on success, -1 on aborted checkout, -2 on failure, -3 on missing SvnKit */
	public int attemptUpdate()
		{
		try
			{
			int up = EnigmaUpdater.checkForUpdates(ef);
			if (EnigmaUpdater.needsRestart)
				{
				Preferences.userRoot().node("/org/enigma").putBoolean("NEEDS_REBUILD",true);
				JOptionPane.showMessageDialog(null,
						"Update completed. The program will need to be restarted to complete the process.");
				System.exit(120); //exit code 120 lets our launcher know to restart us.
				}
			return up;
			}
		/**
		 * Usually you shouldn't catch an Error, however,
		 * in this case we catch it to abort the module,
		 * rather than allowing the failed module to cause
		 * the entire program/plugin to fail
		 */
		catch (NoClassDefFoundError e)
			{
			String error = "Auto-update disabled: SvnKit missing, corrupted, or unusable. "
					+ "Please download to plugins/shared/svnkit.jar in order to enable auto-update.";
			System.err.println(error);
			return -3;
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
			if (ENIGMA_READY) esf.toTop();
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
			return new DataFlavor[0];
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

	public void compile(final int mode)
		{
		if (!ENIGMA_READY)
			{
			JOptionPane.showMessageDialog(null,
					ENIGMA_FAIL ? "ENIGMA is not functional due to prior errors." : "ENIGMA isn't ready yet.");
			return;
			}

		if (es.selCompiler == null || es.selPlatform == null)
			{
			JOptionPane.showMessageDialog(null,
					"Unknown compilation settings for your platform. Perhaps your platform isn't supported.");
			return;
			}

		String exe = es.selPlatform.ext;

		//determine `outname` (rebuild has no `outname`)
		File outname = null;
		try
			{
			if (mode < MODE_DESIGN) //run/debug
				outname = File.createTempFile("egm",exe);
			else if (mode == MODE_DESIGN) outname = File.createTempFile("egm",".emd"); //design
			if (outname != null) outname.deleteOnExit();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			return;
			}
		if (mode == MODE_COMPILE)
			{
			JFileChooser fc = new JFileChooser();
			fc.setFileFilter(new CustomFileFilter(exe,"Executable files"));
			if (fc.showSaveDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			outname = fc.getSelectedFile();
			}

		LGM.commitAll();
		//System.out.println("Compiling with " + enigma);

		final File efi = outname;
		new Thread()
			{
				public void run()
					{
					ef.setVisible(true);
					ef.append("Populating communication buffer. This may take a while on large files...");
					EnigmaStruct es = EnigmaWriter.prepareStruct(LGM.currentFile);
					ef.append("Calling compiler.");
					System.out.println(DRIVER.compileEGMf(es,efi == null ? null : efi.getAbsolutePath(),mode));
					}
			}.start();

		if (mode == MODE_DESIGN) //design
			{
			try
				{
				EnigmaReader.readChanges(outname);
				}
			catch (Exception e)
				{
				e.printStackTrace();
				}
			}
		}

	//This can be static since the ENIGMA_READY and Enigma dll are both static.
	public static SyntaxError checkSyntax(String code)
		{
		if (!ENIGMA_READY)
			{
			JOptionPane.showMessageDialog(null,
					ENIGMA_FAIL ? "ENIGMA is not functional due to prior errors." : "ENIGMA isn't ready yet.");
			return null;
			}

		String osl[] = new String[LGM.currentFile.scripts.size()];
		Script isl[] = LGM.currentFile.scripts.toArray(new Script[0]);
		for (int i = 0; i < osl.length; i++)
			osl[i] = isl[i].getName();
		return DRIVER.syntaxCheck(osl.length,new StringArray(osl),code);
		}

	public void actionPerformed(ActionEvent e)
		{
		if (!ENIGMA_READY)
			{
			JOptionPane.showMessageDialog(null,
					ENIGMA_FAIL ? "ENIGMA is not functional due to prior errors." : "ENIGMA isn't ready yet.");
			return;
			}

		Object s = e.getSource();
		if (s == run) compile(MODE_RUN);
		if (s == debug) compile(MODE_DEBUG);
		if (s == design) compile(MODE_DESIGN);
		if (s == compile) compile(MODE_COMPILE);
		if (s == rebuild) compile(MODE_REBUILD);

		if (s == showFunctions) showKeywordListFrame(0);
		if (s == showGlobals) showKeywordListFrame(1);
		if (s == showTypes) showKeywordListFrame(2);
		}

	private MDIFrame keywordListFrames[] = new MDIFrame[3];
	private KeywordListModel keywordLists[] = new KeywordListModel[3];
	private final static Pattern nameRegex = Pattern.compile("[a-zA-Z][a-zA-Z0-9_]*");

	public void showKeywordListFrame(final int mode)
		{
		String[] modes = { "Functions","Globals","Types" };
		if (keywordListFrames[mode] == null)
			{
			keywordListFrames[mode] = new MDIFrame(modes[mode],true,true,true,true);
			keywordListFrames[mode].setSize(200,400);
			keywordListFrames[mode].setDefaultCloseOperation(JInternalFrame.HIDE_ON_CLOSE);
			LGM.mdi.add(keywordListFrames[mode]);
			keywordLists[mode] = new KeywordListModel();
			final JList list = new JList(keywordLists[mode]);
			//			keywordLists[mode].setEditable(false);
			//			keywordLists[mode].setCursor(Cursor.getPredefinedCursor(Cursor.TEXT_CURSOR));
			list.setFont(new Font(Font.MONOSPACED,Font.PLAIN,12));
			final JTextField filter = new JTextField();
			filter.getDocument().addDocumentListener(new DocumentListener()
				{
					@Override
					public void changedUpdate(DocumentEvent e)
						{
						keywordLists[mode].setFilter(filter.getText());
						}

					@Override
					public void insertUpdate(DocumentEvent e)
						{
						keywordLists[mode].setFilter(filter.getText());
						}

					@Override
					public void removeUpdate(DocumentEvent e)
						{
						keywordLists[mode].setFilter(filter.getText());
						}
				});
			keywordListFrames[mode].add(new JScrollPane(filter),BorderLayout.NORTH);
			keywordListFrames[mode].add(new JScrollPane(list),BorderLayout.CENTER);
			//			keywordListFrames[mode].setFocusTraversalPolicy(new TextAreaFocusTraversalPolicy(list));
			}
		//TODO: should only repopulate when whitespace changes
		keywordLists[mode].setKeywords(getKeywordList(mode));
		keywordListFrames[mode].toTop();
		}

	public class KeywordListModel extends AbstractListModel
		{
		private static final long serialVersionUID = 1L;
		public List<String> keywords;
		private List<String> filteredKeywords = new ArrayList<String>();
		private String filter = "";

		public void setKeywords(List<String> keywords)
			{
			this.keywords = keywords;
			applyFilter();
			}

		public void setFilter(String filter)
			{
			this.filter = filter;
			applyFilter();
			}

		private void applyFilter()
			{
			filteredKeywords.clear();
			if (filter.isEmpty() && keywords != null)
				filteredKeywords.addAll(keywords);
			else
				{
				for (String s : keywords)
					if (s.startsWith(filter)) filteredKeywords.add(s);
				for (String s : keywords)
					if (s.contains(filter) && !s.startsWith(filter)) filteredKeywords.add(s);
				}
			this.fireContentsChanged(this,0,getSize());
			}

		@Override
		public Object getElementAt(int index)
			{
			return filteredKeywords.get(index);
			}

		@Override
		public int getSize()
			{
			return filteredKeywords.size();
			}
		}

	/**
	 * Generates a newline (\n) delimited list of keywords of given type
	 * @param type 0 for functions, 1 for globals, 2 for types
	 * @return The keyword list
	 */
	public static List<String> getKeywordList(int type)
		{
		List<String> rl = new ArrayList<String>();
		String res = DRIVER.first_available_resource();
		while (res != null)
			{
			if (nameRegex.matcher(res).matches()) switch (type)
				{
				case 0:
					if (DRIVER.resource_isFunction())
						{
						int overloads = DRIVER.resource_overloadCount();
						if (overloads > 0)
							{
							rl.add(res + DRIVER.resource_parameters(0));
							break;
							}
						int min = DRIVER.resource_argCountMin();
						int max = DRIVER.resource_argCountMax();
						res += "(" + min;
						if (min != max) res += "-" + max;
						res += ")";
						rl.add(res);
						}
					break;
				case 1:
					if (DRIVER.resource_isGlobal()) rl.add(res);
					break;
				case 2:
					if (DRIVER.resource_isTypeName()) rl.add(res);
					break;
				}
			res = DRIVER.next_available_resource();
			}
		return rl;
		}

	public void subframeAppeared(MDIFrame source)
		{
		JToolBar tool;
		final GMLTextArea code;
		final JPanel status;
		if (source instanceof ScriptFrame)
			{
			ScriptFrame sf = (ScriptFrame) source;
			tool = sf.tool;
			code = sf.code;
			status = sf.status;
			}
		else if (source instanceof CodeFrame)
			{
			CodeFrame cf = (CodeFrame) source;
			tool = cf.tool;
			code = cf.code;
			status = cf.status;
			}
		else if (source instanceof ActionFrame && ((ActionFrame) source).code != null)
			{
			ActionFrame af = (ActionFrame) source;
			tool = af.tool;
			code = af.code;
			status = af.status;
			}
		else
			return;

		status.add(new JLabel(" | ")); //$NON-NLS-1$
		//visible divider       ^   since JSeparator isn't visible and takes up the whole thing...
		final JLabel errors = new JLabel("No errors");
		status.add(errors);

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
					SyntaxError se = checkSyntax(code.getText());
					if (se == null) return;
					int max = code.getDocumentLength() - 1;
					if (se.absoluteIndex > max) se.absoluteIndex = max;
					if (se.absoluteIndex != -1) //-1 = no error
						{
						code.setCaretPosition(se.absoluteIndex);
						code.setSelectionStart(se.absoluteIndex);
						code.setSelectionEnd(se.absoluteIndex + 1);
						errors.setText(se.line + ":" + se.position + "::" + se.errorString);
						}
					else
						errors.setText("No errors");
					code.requestFocus();
					}
			});
		tool.add(syntaxCheck,5);
		}

	@Override
	public void reloadPerformed(boolean newRoot)
		{
		if (newRoot) populateTree();
		if (ENIGMA_READY)
			{
			es = new EnigmaSettings();
			esf.setComponents(es);
			}
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
