/*
 * Copyright (C) 2008-2011 IsmAvatar <IsmAvatar@gmail.com>
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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;
import java.net.URL;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
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
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileView;

import org.enigma.backend.EnigmaCallbacks;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaSettings;
import org.enigma.backend.EnigmaStruct;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.enigma.messages.Messages;
import org.enigma.utility.EnigmaBuildReader;
import org.enigma.utility.YamlParser;
import org.enigma.utility.YamlParser.YamlNode;
import org.lateralgm.components.ErrorDialog;
import org.lateralgm.components.GMLTextArea;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmFile.FormatFlavor;
import org.lateralgm.jedit.GMLKeywords;
import org.lateralgm.jedit.GMLKeywords.Construct;
import org.lateralgm.jedit.GMLKeywords.Function;
import org.lateralgm.jedit.GMLKeywords.Keyword;
import org.lateralgm.main.FileChooser;
import org.lateralgm.main.LGM;
import org.lateralgm.main.FileChooser.FileReader;
import org.lateralgm.main.FileChooser.FileWriter;
import org.lateralgm.main.FileChooser.GroupFilter;
import org.lateralgm.main.LGM.ReloadListener;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.ResourceReference;
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
	public static final String ENIGMA = "compileEGMf.exe"; //$NON-NLS-1$
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
	public JMenuItem busy, run, debug, design, compile, rebuild;
	public JMenuItem showFunctions, showGlobals, showTypes;
	public final EnigmaNode node = new EnigmaNode();

	public EnigmaRunner()
		{
		addReaderAndWriter();
		populateMenu();
		populateTree();
		LGM.addReloadListener(this);
		SubframeInformer.addSubframeListener(this);
		applyBackground("org/enigma/enigma.png"); //$NON-NLS-1$

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
					int updates = attemptUpdate(); //displays own error
					if (updates == -1)
						{
						ENIGMA_FAIL = true;
						return;
						}
					//Make checks for changes itself
					if (!make()) //displays own error
						{
						ENIGMA_FAIL = true;
						return;
						}
					Error e = attemptLib();
					if (e != null)
						{
						String err = Messages.getString("EnigmaRunner.ERROR_LIBRARY") //$NON-NLS-1$
								+ Messages.format("EnigmaRunner.ERROR_LIBRARY_EXACT",e.getMessage()); //$NON-NLS-1$
						JOptionPane.showMessageDialog(null,err);
						ENIGMA_FAIL = true;
						return;
						}

					System.out.println(Messages.getString("EnigmaRunner.INITIALIZING")); //$NON-NLS-1$
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
					es.commitToDriver(DRIVER);
					populateKeywords();
					}
			}.start();
		}

	private UnsatisfiedLinkError attemptLib()
		{
		try
			{
			String lib = "compileEGMf"; //$NON-NLS-1$
			NativeLibrary.addSearchPath(lib,"."); //$NON-NLS-1$
			NativeLibrary.addSearchPath(lib,LGM.workDir.getParent());
			DRIVER = (EnigmaDriver) Native.loadLibrary(lib,EnigmaDriver.class);
			return null;
			}
		catch (UnsatisfiedLinkError e)
			{
			return e;
			}
		}

	//This can be static since the ENIGMA_READY and Enigma dll are both static.
	public static boolean assertReady()
		{
		if (!ENIGMA_READY)
			JOptionPane.showMessageDialog(null,
					ENIGMA_FAIL ? Messages.getString("EnigmaRunner.UNABLE_ERROR") //$NON-NLS-1$
							: Messages.getString("EnigmaRunner.UNABLE_BUSY")); //$NON-NLS-1$
		return ENIGMA_READY;
		}

	public boolean make()
		{
		String make, path;

		//try to read the YAML definition for `make` on this platform
		try
			{
			try
				{
				File gccey = new File(new File("Compilers",TargetHandler.getOS()),"gcc.ey"); //$NON-NLS-1$ //$NON-NLS-2$
				YamlNode n = YamlParser.parse(gccey);
				make = n.getMC("Make"); //or OOB  //$NON-NLS-1$
				path = n.getMC("Path",new String()); //$NON-NLS-1$
				if (path == null) path = new String();
				//replace starting \ with root
				if (make.startsWith("\\")) make = new File("/").getAbsolutePath() + make.substring(1); //$NON-NLS-1$ //$NON-NLS-2$
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
			new ErrorDialog(null,Messages.getString("EnigmaRunner.ERROR_YAML_MAKE_TITLE"), //$NON-NLS-1$
					Messages.getString("EnigmaRunner.ERROR_YAML_MAKE"), //$NON-NLS-1$
					org.lateralgm.messages.Messages.format("Listener.DEBUG_INFO", //$NON-NLS-1$
							e2.getClass().getName(),e2.getMessage(),e2.stackAsString())).setVisible(true);
			return false;
			}

		//run make
		Process p = null;
		String cmd = make + " eTCpath=\"" + path + "\" -j"; //$NON-NLS-1$
		try
			{
			p = Runtime.getRuntime().exec(cmd,null,LGM.workDir.getParentFile());
			}
		catch (IOException e)
			{
			GmFormatException e2 = new GmFormatException(null,e);
			e2.printStackTrace();
			new ErrorDialog(null,Messages.getString("EnigmaRunner.ERROR_MAKE_TITLE"), //$NON-NLS-1$
					Messages.getString("EnigmaRunner.ERROR_MAKE"), //$NON-NLS-1$
					org.lateralgm.messages.Messages.format("Listener.DEBUG_INFO", //$NON-NLS-1$
							e2.getClass().getName(),e2.getMessage(),e2.stackAsString())).setVisible(true);
			return false;
			}

		//Set up listeners, waitFor, finish successfully
		String calling = Messages.format("EnigmaRunner.EXEC_CALLING",cmd); //$NON-NLS-1$
		System.out.println(calling);
		ef.append(calling);
		new EnigmaThread(ef,p.getInputStream());
		new EnigmaThread(ef,p.getErrorStream());
		ef.open();
		try
			{
			System.out.println(p.waitFor());
			System.out.println("Process terminated");
			}
		catch (InterruptedException e)
			{
			e.printStackTrace();
			}
		ef.close();
		return true;
		}

	void addReaderAndWriter()
		{
		EgmIO io = new EgmIO();
		FileChooser.readers.add(io);
		FileChooser.writers.add(io);

		LGM.listener.fc.addOpenFilters(io);
		LGM.listener.fc.addSaveFilters(io);

		FileChooser.fileViews.add(io);
		}

	class EgmIO extends FileView implements FileReader,FileWriter,GroupFilter
		{
		String ext = ".egm"; //$NON-NLS-1$
		CustomFileFilter filter = new CustomFileFilter(
				Messages.getString("EnigmaRunner.FORMAT_READER"),ext); //$NON-NLS-1$

		public FileFilter getGroupFilter()
			{
			return filter;
			}

		public FileFilter[] getFilters()
			{
			return new FileFilter[0];
			}

		//Reader
		public boolean canRead(URI uri)
			{
			return filter.accept(new File(uri));
			}

		public GmFile read(InputStream in, URI uri, ResNode root) throws GmFormatException
			{
			return EFileReader.readEgmFile(new File(uri),root,true);
			}

		//Writer
		@Override
		public String getExtension()
			{
			return ext;
			}

		@Override
		public String getSelectionName()
			{
			return "EGM";
			}

		@Override
		public void write(OutputStream out, GmFile gf, ResNode root) throws IOException
			{
			EFileWriter.writeEgmZipFile(out,gf,root);
			}

		@Override
		public FormatFlavor getFlavor()
			{
			return EFileWriter.FLAVOR_EGM;
			}

		//TODO: FileView
		}

	public void populateMenu()
		{
		JMenu menu = new JMenu(Messages.getString("EnigmaRunner.MENU_ENIGMA")); //$NON-NLS-1$

		busy = new JMenuItem(Messages.getString("EnigmaRunner.MENU_BUSY")); //$NON-NLS-1$
		busy.setEnabled(false);
		busy.setVisible(false);
		menu.add(busy);
		run = new JMenuItem(Messages.getString("EnigmaRunner.MENU_RUN")); //$NON-NLS-1$
		run.addActionListener(this);
		menu.add(run);
		debug = new JMenuItem(Messages.getString("EnigmaRunner.MENU_DEBUG")); //$NON-NLS-1$
		debug.addActionListener(this);
		menu.add(debug);
		design = new JMenuItem(Messages.getString("EnigmaRunner.MENU_DESIGN")); //$NON-NLS-1$
		design.addActionListener(this);
		menu.add(design);
		compile = new JMenuItem(Messages.getString("EnigmaRunner.MENU_COMPILE")); //$NON-NLS-1$
		compile.addActionListener(this);
		menu.add(compile);
		rebuild = new JMenuItem(Messages.getString("EnigmaRunner.MENU_REBUILD_ALL")); //$NON-NLS-1$
		rebuild.addActionListener(this);
		menu.add(rebuild);

		menu.addSeparator();

		JMenuItem mi = new JMenuItem(Messages.getString("EnigmaRunner.MENU_SETTINGS")); //$NON-NLS-1$
		mi.addActionListener(new ActionListener()
			{
				@Override
				public void actionPerformed(ActionEvent e)
					{
					node.openFrame();
					}
			});
		menu.add(mi);

		JMenu sub = new JMenu(Messages.getString("EnigmaRunner.MENU_KEYWORDS")); //$NON-NLS-1$
		menu.add(sub);

		showFunctions = new JMenuItem(KEY_MODES[FUNCTIONS]);
		showFunctions.addActionListener(this);
		sub.add(showFunctions);
		showGlobals = new JMenuItem(KEY_MODES[GLOBALS]);
		showGlobals.addActionListener(this);
		sub.add(showGlobals);
		showTypes = new JMenuItem(KEY_MODES[TYPES]);
		showTypes.addActionListener(this);
		sub.add(showTypes);

		LGM.frame.getJMenuBar().add(menu,1);
		}

	public void populateTree()
		{
		if (!LGM.root.isNodeChild(node)) LGM.root.add(node);

		/*EnigmaGroup node = new EnigmaGroup();
		LGM.root.add(node);
		node.add(new EnigmaNode("Whitespace"));
		node.add(new EnigmaNode("Enigma Init"));
		node.add(new EnigmaNode("Enigma Term"));*/

		LGM.tree.updateUI();
		}

	public void populateKeywords()
		{
		Comparator<Keyword> nameComp = new Comparator<Keyword>()
			{
				@Override
				public int compare(Keyword o1, Keyword o2)
					{
					return o1.getName().compareTo(o2.getName());
					}
			};

		Set<Function> fl = new TreeSet<Function>(nameComp);
		for (Function f : GMLKeywords.FUNCTIONS)
			fl.add(f);
		Set<Construct> cl = new TreeSet<Construct>(nameComp);
		String res = DRIVER.first_available_resource();
		while (res != null)
			{
			if (nameRegex.matcher(res).matches())
				{
				if (DRIVER.resource_isFunction())
					{
					int overloads = DRIVER.resource_overloadCount();
					if (overloads > 0)
						{
						String args = DRIVER.resource_parameters(0);
						fl.add(new Function(res,args.substring(1,args.length() - 1),null));
						}
					else
						{
						int min = DRIVER.resource_argCountMin();
						int max = DRIVER.resource_argCountMax();
						String args = Integer.toString(min);
						if (min != max) args += "-" + max; //$NON-NLS-1$
						fl.add(new Function(res,args,null));
						}
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
				JOptionPane.showMessageDialog(null,Messages.getString("EnigmaRunner.INFO_UPDATE_RESTART")); //$NON-NLS-1$
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
			System.err.println(Messages.getString("EnigmaRunner.WARN_UPDATE_MISSING_SVNKIT")); //$NON-NLS-1$
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

	public class EnigmaNode extends ResNode
		{
		private static final long serialVersionUID = 1L;

		public EnigmaNode()
			{
			super(Messages.getString("EnigmaRunner.RESNODE_NAME"),ResNode.STATUS_SECONDARY, //$NON-NLS-1$
					EnigmaSettings.class);
			}

		public ResourceReference<? extends Resource<?,?>> getRes()
			{
			return es == null ? null : es.reference;
			}

		public void openFrame(boolean newRes)
			{
			if (ENIGMA_READY) esf.toTop();
			}
		}

	public void setMenuEnabled(boolean en)
		{
		busy.setVisible(!en);
		run.setEnabled(en);
		debug.setEnabled(en);
		design.setEnabled(en);
		compile.setEnabled(en);
		rebuild.setEnabled(en);
		}

	public void compile(final int mode)
		{
		if (!assertReady()) return;

		if (es.targets.get(TargetHandler.COMPILER) == null
				|| es.targets.get(TargetHandler.ids[1]) == null)
			{
			JOptionPane.showMessageDialog(null,Messages.getString("EnigmaRunner.UNABLE_SETTINGS_NULL")); //$NON-NLS-1$
			return;
			}

		String ext = es.targets.get(TargetHandler.ids[1]).ext;

		//determine `outname` (rebuild has no `outname`)
		File outname = null;
		try
			{
			String outputexe = es.targets.get(TargetHandler.COMPILER).outputexe;
			if (!outputexe.equals("$tempfile")) //$NON-NLS-1$
				outname = new File(outputexe);
			else if (mode < MODE_DESIGN) //run/debug
				outname = File.createTempFile("egm",ext); //$NON-NLS-1$
			else if (mode == MODE_DESIGN) outname = File.createTempFile("egm",".emd"); //$NON-NLS-1$ //$NON-NLS-2$
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
			fc.setFileFilter(new CustomFileFilter(ext,
					Messages.getString("EnigmaRunner.CHOOSER_EXE_DESCRIPTION"))); //$NON-NLS-1$
			if (fc.showSaveDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			outname = fc.getSelectedFile();
			}

		setMenuEnabled(false);
		LGM.commitAll();
		esf.updateResource();
		es.commitToDriver(DRIVER);
		//System.out.println("Compiling with " + enigma);

		final File efi = outname;
		new Thread()
			{
				public void run()
					{
					ef.open();
					ef.progress(10,Messages.getString("EnigmaRunner.POPULATING")); //$NON-NLS-1$
					EnigmaStruct es = EnigmaWriter.prepareStruct(LGM.currentFile,LGM.root);
					ef.progress(20,Messages.getString("EnigmaRunner.CALLING")); //$NON-NLS-1$
					System.out.println(DRIVER.compileEGMf(es,efi == null ? null : efi.getAbsolutePath(),mode));

					setMenuEnabled(true);
					}
			}.start();

		if (mode == MODE_DESIGN) //design
			{
			try
				{
				EnigmaBuildReader.readChanges(outname);
				}
			catch (Exception e)
				{
				e.printStackTrace();
				}
			}
		}

	public static SyntaxError checkSyntax(String code)
		{
		if (!assertReady()) return null;

		String osl[] = new String[LGM.currentFile.scripts.size()];
		Script isl[] = LGM.currentFile.scripts.toArray(new Script[0]);
		for (int i = 0; i < osl.length; i++)
			osl[i] = isl[i].getName();
		return DRIVER.syntaxCheck(osl.length,new StringArray(osl),code);
		}

	public void actionPerformed(ActionEvent e)
		{
		if (!assertReady()) return;

		Object s = e.getSource();
		if (s == run) compile(MODE_RUN);
		if (s == debug) compile(MODE_DEBUG);
		if (s == design) compile(MODE_DESIGN);
		if (s == compile) compile(MODE_COMPILE);
		if (s == rebuild) compile(MODE_REBUILD);

		if (s == showFunctions) showKeywordListFrame(FUNCTIONS);
		if (s == showGlobals) showKeywordListFrame(GLOBALS);
		if (s == showTypes) showKeywordListFrame(TYPES);
		}

	private static final int FUNCTIONS = 0, GLOBALS = 1, TYPES = 2;
	private static final String[] KEY_MODES = { Messages.getString("EnigmaRunner.KEY_FUNCTIONS"), //$NON-NLS-1$
			Messages.getString("EnigmaRunner.KEY_GLOBALS"), //$NON-NLS-1$
			Messages.getString("EnigmaRunner.KEY_TYPES") }; //$NON-NLS-1$
	private MDIFrame keywordListFrames[] = new MDIFrame[3];
	private KeywordListModel keywordLists[] = new KeywordListModel[3];
	private final static Pattern nameRegex = Pattern.compile("[a-zA-Z][a-zA-Z0-9_]*"); //$NON-NLS-1$

	public void showKeywordListFrame(final int mode)
		{
		if (keywordListFrames[mode] == null)
			{
			keywordListFrames[mode] = new MDIFrame(KEY_MODES[mode],true,true,true,true);
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
		private String filter;

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
			if (filter == null || filter.isEmpty() && keywords != null)
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
						res += "(" + min; //$NON-NLS-1$
				if (min != max) res += "-" + max; //$NON-NLS-1$
				res += ")"; //$NON-NLS-1$
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
			if (esf != null && cf.codeHolder == esf.sDef) return;
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
		final JLabel errors = new JLabel(Messages.getString("EnigmaRunner.LABEL_ERRORS_UNSET")); //$NON-NLS-1$
		status.add(errors);

		JButton syntaxCheck;
		ImageIcon i = findIcon("syntax.png"); //$NON-NLS-1$
		if (i == null)
			syntaxCheck = new JButton(Messages.getString("EnigmaRunner.BUTTON_SYNTAX")); //$NON-NLS-1$
		else
			{
			syntaxCheck = new JButton(i);
			syntaxCheck.setToolTipText(Messages.getString("EnigmaRunner.BUTTON_SYNTAX_TIP")); //$NON-NLS-1$
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
						errors.setText(se.line + ":" + se.position + "::" + se.errorString); //$NON-NLS-1$ //$NON-NLS-2$
						}
					else
						errors.setText(Messages.getString("EnigmaRunner.LABEL_ERRORS_UNSET")); //$NON-NLS-1$
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

		loc = "org/enigma/" + loc; //$NON-NLS-1$
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
