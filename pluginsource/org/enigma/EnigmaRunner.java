/*
 * Copyright (C) 2008-2011 IsmAvatar <IsmAvatar@gmail.com>
 * Copyright (C) 2013, Robert B. Colton
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
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URI;
import java.net.URL;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Set;
import java.util.SortedSet;
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
import javax.swing.KeyStroke;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.tree.TreeNode;

import org.enigma.backend.Definitions;
import org.enigma.backend.EnigmaCallbacks;
import org.enigma.backend.EnigmaDriver;
import org.enigma.backend.EnigmaDriver.SyntaxError;
import org.enigma.backend.EnigmaSettings;
import org.enigma.backend.EnigmaStruct;
import org.enigma.file.EFileReader;
import org.enigma.file.EgmIO;
import org.enigma.file.YamlParser;
import org.enigma.file.YamlParser.YamlNode;
import org.enigma.frames.EnigmaSettingsFrame;
import org.enigma.frames.ProgressFrame;
import org.enigma.messages.Messages;
import org.enigma.utility.EnigmaBuildReader;
import org.lateralgm.components.ErrorDialog;
import org.lateralgm.components.CodeTextArea;
import org.lateralgm.components.GmMenu;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.file.ProjectFile.ResourceHolder;
import org.lateralgm.file.ProjectFile.SingletonResourceHolder;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.joshedit.lexers.GMLKeywords;
import org.lateralgm.joshedit.lexers.GMLKeywords.Construct;
import org.lateralgm.joshedit.lexers.GMLKeywords.Function;
import org.lateralgm.joshedit.lexers.GMLKeywords.Keyword;
import org.lateralgm.main.FileChooser;
import org.lateralgm.main.LGM;
import org.lateralgm.main.LGM.ReloadListener;
import org.lateralgm.main.LGM.SingletonPluginResource;
import org.lateralgm.main.Listener;
import org.lateralgm.resources.Resource;
import org.lateralgm.resources.Script;
import org.lateralgm.subframes.ActionFrame;
import org.lateralgm.subframes.CodeFrame;
import org.lateralgm.subframes.ResourceFrame;
import org.lateralgm.subframes.ResourceFrame.ResourceFrameFactory;
import org.lateralgm.subframes.ScriptFrame;
import org.lateralgm.subframes.SubframeInformer;
import org.lateralgm.subframes.SubframeInformer.SubframeListener;

import com.sun.jna.Native;
import com.sun.jna.NativeLibrary;
import com.sun.jna.StringArray;

public class EnigmaRunner implements ActionListener,SubframeListener,ReloadListener
	{
	public static boolean NEW_DEFINITIONS_READY_YET = false;

	public static boolean ENIGMA_READY = false, ENIGMA_FAIL = false, SHUTDOWN = false;
	public static final int MODE_RUN = 0, MODE_DEBUG = 1, MODE_DESIGN = 2;
	public static final int MODE_COMPILE = 3, MODE_REBUILD = 4;
	public static final File WORKDIR = LGM.workDir.getParentFile();

	/** This is static because it belongs to EnigmaStruct's dll, which is statically loaded. */
	public static EnigmaDriver DRIVER;

	public ProgressFrame ef = new ProgressFrame();
	/** This is global scoped so that it doesn't get GC'd */
	private EnigmaCallbacks ec = new EnigmaCallbacks(ef);
	public EnigmaSettingsFrame esf;
	public JMenuItem busy, run, debug, design, compile, rebuild, stop;
	public JButton stopb, runb, debugb, compileb;
	public JMenuItem mImport, showFunctions, showGlobals, showTypes;
	public ResNode node = new ResNode(Messages.getString("EnigmaRunner.RESNODE_NAME"), //$NON-NLS-1$
			ResNode.STATUS_SECONDARY,EnigmaSettings.class);
	
	public EnigmaRunner()
		{
		addResourceHook();
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

		setMenuEnabled(false);
		stop.setVisible(false);
		stopb.setVisible(false);
		
		final Thread initthread = new Thread()
			{
				public void run()
					{
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
					// Delay compiler until reload performed is ready
					while (!LGM.isloaded) {
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e1) {
							e1.printStackTrace();
						}
					}
					ResourceHolder<EnigmaSettings> rh = LGM.currentFile.resMap.get(EnigmaSettings.class);
					if (rh == null)
						LGM.currentFile.resMap.put(EnigmaSettings.class,
								rh = new SingletonResourceHolder<EnigmaSettings>(new EnigmaSettings()));
					esf = new EnigmaSettingsFrame(rh.getResource());
					LGM.mdi.add(esf);
					rh.getResource().commitToDriver(DRIVER);
					setupBaseKeywords();
					populateKeywords();
					setMenuEnabled(true);
					}
			};
			initthread.start();
		}

	private static UnsatisfiedLinkError attemptLib()
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
		String make, tcpath, path;

		//try to read the YAML definition for `make` on this platform
		try
			{
			try
				{
				File gccey = new File(new File("Compilers",TargetHandler.getOS()),"gcc.ey"); //$NON-NLS-1$ //$NON-NLS-2$
				YamlNode n = YamlParser.parse(gccey);
				make = n.getMC("Make"); //or OOB  //$NON-NLS-1$
				tcpath = n.getMC("TCPath",new String()); //$NON-NLS-1$
				path = n.getMC("Path",new String()); //$NON-NLS-1$
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
					Messages.getString("EnigmaRunner.ERROR_YAML_MAKE"),e2).setVisible(true); //$NON-NLS-1$
			return false;
			}

		//run make
		Process p = null;
		String cmd = make + " eTCpath=\"" + tcpath + "\""; //$NON-NLS-1$
		String[] env = null;
		if (path != null) env = new String[] { "PATH=" + path };
		try
			{
			p = Runtime.getRuntime().exec(cmd,env,LGM.workDir.getParentFile());
			}
		catch (IOException e)
			{
			GmFormatException e2 = new GmFormatException(null,e);
			e2.printStackTrace();
			new ErrorDialog(null,Messages.getString("EnigmaRunner.ERROR_MAKE_TITLE"), //$NON-NLS-1$
					Messages.getString("EnigmaRunner.ERROR_MAKE"),e2).setVisible(true); //$NON-NLS-1$
			return false;
			}

		//Set up listeners, waitFor, finish successfully
		String calling = Messages.format("EnigmaRunner.EXEC_CALLING",cmd); //$NON-NLS-1$
		System.out.println(calling);
		ef.append(calling + "\n");
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

	void addResourceHook()
		{
		EgmIO io = new EgmIO();
		FileChooser.fileViews.add(io);
		FileChooser.readers.add(io);
		FileChooser.writers.add(io);

		Listener.getInstance().fc.addOpenFilters(io);
		Listener.getInstance().fc.addSaveFilters(io);

		LGM.addPluginResource(new EnigmaSettingsPluginResource());
		}

	private class EnigmaSettingsPluginResource extends SingletonPluginResource<EnigmaSettings>
		{
		@Override
		public Class<? extends Resource<?,?>> getKind()
			{
			return EnigmaSettings.class;
			}

		@Override
		public ImageIcon getIcon()
			{
			return LGM.findIcon("restree/gm.png");
			}

		@Override
		public String getName3()
			{
			return "EGS";
			}

		@Override
		public String getName()
			{
			return Messages.getString("EnigmaRunner.RESNODE_NAME");
			}

		@Override
		public ResourceFrameFactory getResourceFrameFactory()
			{
			return new ResourceFrameFactory()
				{
					@Override
					public ResourceFrame<?,?> makeFrame(Resource<?,?> r, ResNode node)
						{
						return esf;
						}
				};
			}

		@Override
		public EnigmaSettings getInstance()
			{
			return new EnigmaSettings();
			}
		}

	public void populateMenu()
		{
		stopb = new JButton(); //$NON-NLS-1$
		stopb.addActionListener(this);
		stopb.setToolTipText(Messages.getString("EnigmaRunner.MENU_STOP"));
		stopb.setIcon(LGM.getIconForKey("EnigmaPlugin.STOP"));
		LGM.tool.add(new JToolBar.Separator(), 4);
		LGM.tool.add(stopb, 5);
		runb = new JButton(); //$NON-NLS-1$
		runb.addActionListener(this);
		runb.setToolTipText(Messages.getString("EnigmaRunner.MENU_RUN"));
		runb.setIcon(LGM.getIconForKey("EnigmaPlugin.EXECUTE"));
		LGM.tool.add(runb, 6);
		debugb = new JButton(); //$NON-NLS-1$
		debugb.addActionListener(this);
		debugb.setToolTipText(Messages.getString("EnigmaRunner.MENU_DEBUG"));
		debugb.setIcon(LGM.getIconForKey("EnigmaPlugin.DEBUG"));
		LGM.tool.add(debugb, 7);
		compileb = new JButton(); //$NON-NLS-1$
		compileb.addActionListener(this);
		compileb.setToolTipText(Messages.getString("EnigmaRunner.MENU_COMPILE"));
		compileb.setIcon(LGM.getIconForKey("EnigmaPlugin.COMPILE"));
		LGM.tool.add(compileb, 8);
		
		JMenu menu = new GmMenu(Messages.getString("EnigmaRunner.MENU_BUILD")); //$NON-NLS-1$
		if (LGM.themename.equals("Quantum")) {
		  menu.setFont(LGM.lnfFont);
		}
		menu.setMnemonic('B');
		busy = addItem(Messages.getString("EnigmaRunner.MENU_BUSY"));
		busy.setEnabled(false);
		busy.setVisible(false);
		menu.add(busy);
		run = addItem(Messages.getString("EnigmaRunner.MENU_RUN")); //$NON-NLS-1$
		run.addActionListener(this);
		run.setIcon(LGM.getIconForKey("EnigmaPlugin.EXECUTE"));
		run.setAccelerator(KeyStroke.getKeyStroke("F5"));
		menu.add(run);
		debug = addItem(Messages.getString("EnigmaRunner.MENU_DEBUG")); //$NON-NLS-1$
		debug.addActionListener(this);
		debug.setIcon(LGM.getIconForKey("EnigmaPlugin.DEBUG"));
		debug.setAccelerator(KeyStroke.getKeyStroke("F6"));
		menu.add(debug);
		design = addItem(Messages.getString("EnigmaRunner.MENU_DESIGN")); //$NON-NLS-1$
		design.addActionListener(this);
		design.setAccelerator(KeyStroke.getKeyStroke("F7"));
		menu.add(design);
		compile = addItem(Messages.getString("EnigmaRunner.MENU_COMPILE")); //$NON-NLS-1$
		compile.addActionListener(this);
		compile.setIcon(LGM.getIconForKey("EnigmaPlugin.COMPILE"));
		compile.setAccelerator(KeyStroke.getKeyStroke("F8"));
		menu.add(compile);
		menu.addSeparator();
		stop = addItem(Messages.getString("EnigmaRunner.MENU_STOP")); //$NON-NLS-1$
		stop.addActionListener(this);
		stop.setIcon(LGM.getIconForKey("EnigmaPlugin.STOP"));
		stop.setAccelerator(KeyStroke.getKeyStroke("F9"));
		menu.add(stop);
		rebuild = addItem(Messages.getString("EnigmaRunner.MENU_REBUILD_ALL")); //$NON-NLS-1$
		rebuild.addActionListener(this);
		rebuild.setIcon(LGM.getIconForKey("EnigmaPlugin.REBUILD_ALL"));
		rebuild.setAccelerator(KeyStroke.getKeyStroke("F10"));
		menu.add(rebuild);

		menu.addSeparator();

		mImport = addItem(Messages.getString("EnigmaRunner.MENU_IMPORT")); //$NON-NLS-1$
		mImport.addActionListener(this);
		menu.add(mImport);

		menu.addSeparator();

		JMenuItem mi = addItem(Messages.getString("EnigmaRunner.MENU_SETTINGS")); //$NON-NLS-1$
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
		if (LGM.themename.equals("Quantum")) {
		  sub.setFont(LGM.lnfFont);
		}
		menu.add(sub);

		showFunctions = addItem(KEY_MODES[FUNCTIONS]);
		showFunctions.addActionListener(this);
		sub.add(showFunctions);
		showGlobals = addItem(KEY_MODES[GLOBALS]);
		showGlobals.addActionListener(this);
		sub.add(showGlobals);
		showTypes = addItem(KEY_MODES[TYPES]);
		showTypes.addActionListener(this);
		sub.add(showTypes);

		LGM.frame.getJMenuBar().add(menu,1);
		}

	private JMenuItem addItem(String string) {
		JMenuItem ret = new JMenuItem(string);
		if (LGM.themename.equals("Quantum")) {
		  ret.setFont(LGM.lnfFont);
		}
		return ret;
	}

	public void firstSetup()
		{
		LGM.root.add(node); //EnigmaSettings node
		if (NEW_DEFINITIONS_READY_YET)
			{
			LGM.currentFile.resMap.addList(Definitions.class);
			String name = Resource.kindNamesPlural.get(Definitions.class);
			LGM.root.addChild(name,ResNode.STATUS_PRIMARY,Definitions.class);
			}
		LGM.tree.updateUI();
		}

	public void populateTree()
		{
		if (!LGM.root.isNodeChild(node))
			{
			boolean found = false;
			for (int i = 0; i < LGM.root.getChildCount() && !found; i++)
				{
				TreeNode n = LGM.root.getChildAt(i);
				if (n instanceof ResNode && ((ResNode) n).kind == EnigmaSettings.class)
					{
					node = (ResNode) n;
					found = true;
					}
				}
			if (!found) firstSetup();
			}
		}

	private static SortedSet<Function> BASE_FUNCTIONS;
	private static SortedSet<Construct> BASE_CONSTRUCTS;
	private static final Comparator<Keyword> KEYWORD_COMP = new Comparator<Keyword>()
		{
			@Override
			public int compare(Keyword o1, Keyword o2)
				{
				return o1.getName().compareTo(o2.getName());
				}
		};

	private static void setupBaseKeywords()
		{
		BASE_FUNCTIONS = new TreeSet<Function>(KEYWORD_COMP);
		for (Function f : GMLKeywords.FUNCTIONS)
			BASE_FUNCTIONS.add(f);
		BASE_CONSTRUCTS = new TreeSet<Construct>(KEYWORD_COMP);
		for (Construct f : GMLKeywords.CONSTRUCTS)
			BASE_CONSTRUCTS.add(f);
		}

	public static void populateKeywords()
		{
		Set<Function> fl = new TreeSet<Function>(BASE_FUNCTIONS);
		Set<Construct> cl = new TreeSet<Construct>(BASE_CONSTRUCTS);
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

	public void setMenuEnabled(boolean en)
		{
		busy.setVisible(!en);
		run.setEnabled(en);
		debug.setEnabled(en);
		design.setEnabled(en);
		stop.setEnabled(!en);
		compile.setEnabled(en);
		rebuild.setEnabled(en);
		stopb.setEnabled(!en);
		runb.setEnabled(en);
		debugb.setEnabled(en);
		compileb.setEnabled(en);
		}

    class CompilerThread extends Thread {
        final int mode;
        final File efi;
        CompilerThread(final int m, File outname) {
            mode = m;
            efi = outname;
        }

        public void run() {
        	ef.open();
        	ef.progress(10,Messages.getString("EnigmaRunner.POPULATING")); //$NON-NLS-1$
        	EnigmaStruct es = EnigmaWriter.prepareStruct(LGM.currentFile,LGM.root);
        	ef.progress(20,Messages.getString("EnigmaRunner.CALLING")); //$NON-NLS-1$
        	System.out.println("Plugin: Delegating to ENIGMA (out of my hands now)");
        	System.out.println(DRIVER.compileEGMf(es,efi == null ? null : efi.getAbsolutePath(),mode));
        	setupBaseKeywords();
        	populateKeywords();
			
        	setMenuEnabled(true);
        	stop.setEnabled(false);
        	stopb.setEnabled(false);
        }
    }
    
	private static CompilerThread cthread;
	public void compile(final int mode)
		{
		if (!assertReady()) return;

		stop.setEnabled(true);
		stopb.setEnabled(true);
		EnigmaSettings es = LGM.currentFile.resMap.get(EnigmaSettings.class).getResource();

		if (es.targets.get(TargetHandler.COMPILER) == null)
			{
			JOptionPane.showMessageDialog(null,Messages.getString("EnigmaRunner.UNABLE_SETTINGS_NULL")); //$NON-NLS-1$
			return;
			}

		String ext = es.targets.get(TargetHandler.COMPILER).ext;
		String os = TargetHandler.normalize(System.getProperty("os.name")); //$NON-NLS-1$
		if (os.contains("win") && ext == null) {
			ext = ".exe";
		}
		
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
			CustomFileFilter exefilter = new CustomFileFilter(
					Messages.getString("EnigmaRunner.CHOOSER_EXE_DESCRIPTION"), ext);
			fc.setFileFilter(exefilter); //$NON-NLS-1$
			fc.setDialogTitle(Messages.getString("EnigmaRunner.COMPILETO"));
			if (fc.showSaveDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			outname = fc.getSelectedFile();
			if (!fc.getFileFilter().equals(exefilter)) {
				ext = "";
			}
			if (ext != null)
				if (!outname.getName().endsWith(ext))
					outname = new File(outname.getPath() + ext);
			else
				outname = new File(outname.getPath());
		}

		setMenuEnabled(false);
		LGM.commitAll();
		//Don't need to update ESF since commitAll traverses all nodes
		//esf.updateResource();
		es.commitToDriver(DRIVER);
		//System.out.println("Compiling with " + enigma);
		
		cthread = new CompilerThread(mode, outname);
		try {
			cthread.join();
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		//cthread.run(outname);
		cthread.start();
		
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

		String osl[] = new String[LGM.currentFile.resMap.getList(Script.class).size()];
		Script isl[] = LGM.currentFile.resMap.getList(Script.class).toArray(new Script[0]);
		for (int i = 0; i < osl.length; i++)
			osl[i] = isl[i].getName();
		return DRIVER.syntaxCheck(osl.length,new StringArray(osl),code);
		}

	public void actionPerformed(ActionEvent e)
		{
		if (!assertReady()) return;
		Object s = e.getSource();
		if (s == stop || s == stopb)  {
			cthread.interrupt();
			setMenuEnabled(true);
		}
		if (s == run || s == runb) compile(MODE_RUN);
		if (s == debug || s == debugb) compile(MODE_DEBUG);
		if (s == design) compile(MODE_DESIGN);
		if (s == compile || s == compileb) compile(MODE_COMPILE);
		if (s == rebuild) compile(MODE_REBUILD);

		if (s == mImport) EFileReader.importEgmFolder();
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
		final CodeTextArea code;
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
					SyntaxError se = checkSyntax(code.getTextCompat());
					if (se == null) return;
					if (se.absoluteIndex != -1) //-1 = no error
						{
						code.markError(se.line - 1,se.position - 1,se.absoluteIndex);
						errors.setText(se.line + ":" + se.position + "::" + se.errorString); //$NON-NLS-1$ //$NON-NLS-2$
						}
					else
						errors.setText(Messages.getString("EnigmaRunner.LABEL_ERRORS_UNSET")); //$NON-NLS-1$
					code.requestFocusInWindow();
					}
			});
		tool.add(syntaxCheck,5);
		}

	@Override
	public void reloadPerformed(boolean newRoot)
		{
		populateTree();
		new Thread() {
		public void run() {	
			// Delay reload performed until the compiler is ready
			while (!ENIGMA_READY && !LGM.isloaded) {
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			ResourceHolder<EnigmaSettings> rh = LGM.currentFile.resMap.get(EnigmaSettings.class);
			if (rh == null)
				LGM.currentFile.resMap.put(EnigmaSettings.class,
						rh = new SingletonResourceHolder<EnigmaSettings>(new EnigmaSettings()));
			
			esf.resOriginal = rh.getResource();
			esf.revertResource(); //updates local res copy as well
		}
		}.start();

		}
	public static ImageIcon findIcon(String loc)
		{
		ImageIcon ico = new ImageIcon(loc);
		if (ico.getIconWidth() != -1) return ico;

		URL url = EnigmaRunner.class.getClassLoader().getResource(loc);
		if (url != null) ico = new ImageIcon(url);
		if (ico.getIconWidth() != -1) return ico;

		loc = "org/enigma/" + loc; //$NON-NLS-1$
		ico = new ImageIcon(loc);
		if (ico.getIconWidth() != -1) return ico;

		url = EnigmaRunner.class.getClassLoader().getResource(loc);
		if (url != null) ico = new ImageIcon(url);
		return ico;
		}

	public boolean subframeRequested(Resource<?,?> res, ResNode node)
		{
		return false;
		}

	public static void main(String[] args)
		{
		try {
			LGM.main(args);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		new EnigmaRunner();
		}
	}
