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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import javax.swing.AbstractListModel;
import javax.swing.BorderFactory;
import javax.swing.ComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToolBar;
import javax.swing.GroupLayout.Alignment;
import javax.swing.GroupLayout.Group;
import javax.swing.GroupLayout.SequentialGroup;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;
import javax.swing.event.InternalFrameListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;

import org.enigma.TargetHandler.TargetSelection;
import org.enigma.YamlParser.YamlContent;
import org.enigma.YamlParser.YamlElement;
import org.enigma.YamlParser.YamlNode;
import org.enigma.backend.EnigmaSettings;
import org.enigma.messages.Messages;
import org.lateralgm.compare.CollectionComparator;
import org.lateralgm.compare.MapComparator;
import org.lateralgm.compare.ObjectComparator;
import org.lateralgm.compare.ReflectionComparator;
import org.lateralgm.compare.SimpleCasesComparator;
import org.lateralgm.components.CustomFileChooser;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.IndexButtonGroup;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;
import org.lateralgm.subframes.CodeFrame;
import org.lateralgm.subframes.CodeFrame.CodeHolder;

public class EnigmaSettingsFrame extends MDIFrame implements ActionListener,FocusListener,
		PopupMenuListener
	{
	private static final long serialVersionUID = 1L;
	private static final ImageIcon CODE_ICON = LGM.getIconForKey("Resource.SCRIPT"); //$NON-NLS-1$

	private final EnigmaSettings oldEs;
	private EnigmaSettings es;

	protected JToolBar toolbar;
	protected JButton save, saveFile, loadFile;

	private Map<String,Option> options;
	private Map<String,TargetSelection> userPicks = new HashMap<String,TargetSelection>();
	private Map<JComboBox,String> targets;
	private JButton bDef, bGlobLoc;
	private JButton bInit, bClean;
	CodeHolder sDef;
	private CodeFrame cfDef, cfGlobLoc, cfInit, cfClean;
	private CodeHolder sGlobLoc, sInit, sClean;
	private CustomFileChooser fc;

	private JTextField tfAuth;
	private JTextArea taDesc;

	class SimpleCodeHolder implements CodeHolder
		{
		private String code;

		public SimpleCodeHolder(String s)
			{
			code = s;
			}

		@Override
		public String getCode()
			{
			return code;
			}

		@Override
		public void setCode(String s)
			{
			code = s;
			}
		}

	class YamlException extends Exception
		{
		private static final long serialVersionUID = 1L;

		YamlException(String s)
			{
			super(s);
			}
		}

	class Option
		{
		final static String UNKNOWN = "<no type>"; //$NON-NLS-1$

		JComponent[] cChoices;

		IndexButtonGroup ibg = null;
		JComboBox cb = null;
		String other = null;

		Option(String name, String type, int cnum, String choices)
		//				throws IllegalArgumentException
			{
			if (name != null && name.isEmpty()) name = null;
			if (type.equalsIgnoreCase("Label")) //$NON-NLS-1$
				populateLabel(type,name,choices);
			else if (type.equalsIgnoreCase("Radio-1")) //$NON-NLS-1$
				populateRadio1(name,choices == null ? null : choices.split(",")); //$NON-NLS-1$
			else if (type.equalsIgnoreCase("Combobox")) //$NON-NLS-1$
				populateCombo(name,choices == null ? null : choices.split(",")); //$NON-NLS-1$
			else
				populateLabel(type,name,choices);
			//				throw new IllegalArgumentException(type);

			JComponent[] bc = cChoices;
			cChoices = new JComponent[cnum];
			System.arraycopy(bc,0,cChoices,0,bc.length);
			for (int i = bc.length; i < cnum; i++)
				cChoices[i] = new JLabel();
			}

		void populateLabel(String type, String name, String choices)
			{
			cChoices = new JComponent[1];
			JLabel lab = new JLabel(Messages.format("EnigmaSettingsFrame.OPTION_UNSUPPORTED",type,name)); //$NON-NLS-1$
			lab.setToolTipText(other = choices);
			cChoices[0] = lab;
			}

		void populateRadio1(String name, String...choices)
			{
			cChoices = new JComponent[choices.length + (name == null ? 0 : 1)];
			if (name != null) cChoices[0] = new JLabel(name);
			ibg = new IndexButtonGroup(2,true,false);
			for (int i = 0; i < choices.length; i++)
				{
				JRadioButton but = new JRadioButton(choices[i]);
				cChoices[i + (name == null ? 0 : 1)] = but;
				ibg.add(but);
				}
			}

		void populateCombo(String name, String...choices)
			{
			cChoices = new JComponent[name == null ? 1 : 2];
			if (name != null) cChoices[0] = new JLabel(name);
			cChoices[name == null ? 0 : 1] = cb = new JComboBox(choices);
			}

		void setValue(String val)
			{
			other = val;
			if (val == null) return;
			try
				{
				setValue(Integer.parseInt(val));
				}
			catch (NumberFormatException e)
				{
				return;
				}
			}

		void setValue(int val)
			{
			if (ibg != null)
				ibg.setValue(val);
			else if (cb != null) cb.setSelectedIndex(val);
			}

		String getValue()
			{
			if (ibg != null) return Integer.toString(ibg.getValue());
			if (cb != null) return Integer.toString(cb.getSelectedIndex());
			return other;
			}
		}

	public EnigmaSettingsFrame(EnigmaSettings es)
		{
		super("Enigma Settings",false,true,true,true); //$NON-NLS-1$
		setDefaultCloseOperation(HIDE_ON_CLOSE);
		setFrameIcon(LGM.findIcon("restree/gm.png")); //$NON-NLS-1$
		this.oldEs = es;
		this.es = es.copy();

		fc = new CustomFileChooser("/org/enigma","LAST_SETTINGS_DIR"); //$NON-NLS-1$ //$NON-NLS-2$
		fc.setFileFilter(new CustomFileFilter(
				".ey",Messages.getString("EnigmaSettingsFrame.EY_DESCRIPTION"))); //$NON-NLS-1$ //$NON-NLS-2$

		toolbar = makeToolBar();
		add(toolbar,BorderLayout.NORTH);
		JTabbedPane tabs = new JTabbedPane();
		tabs.add(Messages.getString("EnigmaSettingsFrame.TAB_GENERAL"),makeSettings()); //$NON-NLS-1$
		tabs.add(Messages.getString("EnigmaSettingsFrame.TAB_API"),makeAPI()); //$NON-NLS-1$
		add(tabs,BorderLayout.CENTER);
		pack();
		}

	private JToolBar makeToolBar()
		{
		JToolBar tool = new JToolBar();
		tool.setFloatable(false);

		// Setup the buttons
		save = new JButton(LGM.getIconForKey("ResourceFrame.SAVE")); //$NON-NLS-1$
		save.setRequestFocusEnabled(false);
		save.addActionListener(this);
		tool.add(save);
		tool.addSeparator();

		loadFile = new JButton(LGM.getIconForKey("LGM.OPEN")); //$NON-NLS-1$
		loadFile.setToolTipText(Messages.getString("EnigmaSettingsFrame.LOAD_TIP")); //$NON-NLS-1$
		loadFile.setRequestFocusEnabled(false);
		loadFile.addActionListener(this);
		tool.add(loadFile);
		saveFile = new JButton(LGM.getIconForKey("LGM.SAVEAS")); //$NON-NLS-1$
		saveFile.setToolTipText(Messages.getString("EnigmaSettingsFrame.SAVE_TIP")); //$NON-NLS-1$
		saveFile.setRequestFocusEnabled(false);
		saveFile.addActionListener(this);
		tool.add(saveFile);

		tool.addSeparator();
		return tool;
		}

	private JPanel makePane(String name, int choiceCount, Option...opts)
		{
		JPanel pane = new JPanel();
		pane.setBorder(BorderFactory.createTitledBorder(name));
		GroupLayout layout = new GroupLayout(pane);
		pane.setLayout(layout);

		Group g = layout.createSequentialGroup();
		Group g1 = layout.createParallelGroup();
		for (int i = 0; i < choiceCount; i++)
			{
			g1 = layout.createParallelGroup();
			for (Option o : opts)
				g1.addComponent(o.cChoices[i]);
			g.addGroup(g1);
			}
		layout.setHorizontalGroup(g);

		g = layout.createSequentialGroup();
		for (Option o : opts)
			{
			g1 = layout.createParallelGroup(Alignment.BASELINE);
			for (int i = 0; i < choiceCount; i++)
				g1.addComponent(o.cChoices[i]);
			g.addGroup(g1);
			}
		layout.setVerticalGroup(g);

		return pane;
		}

	private List<JPanel> parsePanels() throws YamlException
		{
		options = new HashMap<String,Option>();
		ArrayList<JPanel> panels = new ArrayList<JPanel>();
		File settings = new File(EnigmaRunner.WORKDIR,"settings.ey"); //$NON-NLS-1$
		YamlNode n;
		try
			{
			n = YamlParser.parse(settings);
			}
		catch (FileNotFoundException fnfe)
			{
			throw new YamlException(settings.getPath()
					+ Messages.getString("EnigmaSettingsFrame.YERR_NO_FILE")); //$NON-NLS-1$
			}
		if (n.chronos.isEmpty())
			throw new YamlException(settings.getName()
					+ Messages.getString("EnigmaSettingsFrame.YERR_EMPTY")); //$NON-NLS-1$

		//loop through panels
		for (YamlElement ge : n.chronos)
			{
			String name = ge.name;
			String error = settings.getName() + ": " + name + ": "; //$NON-NLS-1$ //$NON-NLS-2$

			if (ge.isScalar)
				throw new YamlException(error + Messages.getString("EnigmaSettingsFrame.YERR_SCALAR")); //$NON-NLS-1$

			n = (YamlNode) ge;
			if (!n.getMC("Layout","Grid").equalsIgnoreCase("Grid")) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				throw new YamlException(error + Messages.getString("EnigmaSettingsFrame.YERR_LAYOUT")); //$NON-NLS-1$

			int columns;
			try
				{
				columns = Integer.parseInt(n.getMC("Columns",null)); //$NON-NLS-1$
				}
			catch (NumberFormatException nfe)
				{
				throw new YamlException(error + Messages.getString("EnigmaSettingsFrame.YERR_INTEGER")); //$NON-NLS-1$
				}

			//loop through options in panel
			ArrayList<Option> opts = new ArrayList<Option>(n.chronos.size());
			for (YamlElement oe : n.chronos)
				{
				if (oe.name.equalsIgnoreCase("Layout") || oe.name.equalsIgnoreCase("Columns")) continue; //$NON-NLS-1$ //$NON-NLS-2$

				if (oe.isScalar)
					{
					System.err.println(Messages.format("EnigmaSettingsFrame.YWARN_Scalar",oe.name)); //$NON-NLS-1$
					continue;
					}
				YamlNode on = (YamlNode) oe;

				Option opt;
				try
					{
					opt = new Option(on.getMC("Label",null), //$NON-NLS-1$
							on.getMC("Type",Option.UNKNOWN), //$NON-NLS-1$
							columns,on.getMC("Options",null)); //$NON-NLS-1$
					opt.setValue(on.getMC("Default","0")); //$NON-NLS-1$ //$NON-NLS-2$
					}
				catch (IllegalArgumentException e)
					{
					System.err.println(error
							+ Messages.format("EnigmaSettingsFrame.YWARN_OPTION_TYPE",on.name,e.getMessage())); //$NON-NLS-1$
					continue;
					}
				opts.add(opt);
				options.put(on.name,opt);
				} //each option in panel

			panels.add(makePane(name,columns,opts.toArray(new Option[0])));
			}

		//Populate the default options so they're ready for first call to definitionsModified.
		//Technically it would make more sense to do this in EnigmaSettings c'tor,
		//but then we'd need to either parse this file twice or keep a reference to it.
		oldEs.options.clear();
		for (Entry<String,Option> entry : options.entrySet())
			oldEs.options.put(entry.getKey(),entry.getValue() == null ? null
					: entry.getValue().getValue());

		return panels;
		}

	private JPanel makeSettings()
		{
		JPanel p = new JPanel();
		GroupLayout layout = new GroupLayout(p);
		p.setLayout(layout);

		List<JPanel> panels = null;
		try
			{
			panels = parsePanels();
			}
		catch (YamlException e)
			{
			// TODO Auto-generated catch block
			e.printStackTrace();
			}

		sDef = new SimpleCodeHolder(es.definitions);
		sGlobLoc = new SimpleCodeHolder(es.globalLocals);
		sInit = new SimpleCodeHolder(es.initialization);
		sClean = new SimpleCodeHolder(es.cleanup);

		bDef = new JButton(Messages.getString("EnigmaSettingsFrame.BUTTON_DEFINITIONS"),CODE_ICON); //$NON-NLS-1$
		bGlobLoc = new JButton(Messages.getString("EnigmaSettingsFrame.BUTTON_GLOBAL_LOCALS"),CODE_ICON); //$NON-NLS-1$
		bInit = new JButton(Messages.getString("EnigmaSettingsFrame.BUTTON_INITIALIZATION"),CODE_ICON); //$NON-NLS-1$
		bClean = new JButton(Messages.getString("EnigmaSettingsFrame.BUTTON_CLEANUP"),CODE_ICON); //$NON-NLS-1$

		bDef.addActionListener(this);
		bGlobLoc.addActionListener(this);
		bInit.addActionListener(this);
		bClean.addActionListener(this);

		Group gh = layout.createParallelGroup();
		SequentialGroup gv = layout.createSequentialGroup();
		if (panels != null) for (JPanel pp : panels)
			{
			gh.addComponent(pp);
			gv.addComponent(pp);
			}

		layout.setHorizontalGroup(gh
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bDef)
		/*	*/.addComponent(bGlobLoc))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bInit)
		/*	*/.addComponent(bClean)));
		layout.setVerticalGroup(gv
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bDef)
		/*	*/.addComponent(bGlobLoc))
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bInit)
		/*	*/.addComponent(bClean))
		/**/.addContainerGap());

		return p;
		}

	private JPanel makeAPI()
		{
		JPanel p = new JPanel();
		GroupLayout layout = new GroupLayout(p);
		p.setLayout(layout);

		Group hg1 = layout.createParallelGroup();
		Group hg2 = layout.createParallelGroup();
		SequentialGroup vg = layout.createSequentialGroup();
		final int pref = GroupLayout.PREFERRED_SIZE;

		String[] labels = { "Compiler: ","Platform: ","Graphics: ","Audio: ","Collision: ","Widgets: " };
		JComponent[] targs = initializeTargets();

		for (int i = 0; i < targs.length; i++)
			{
			JLabel label = new JLabel(labels[i]);

			hg1.addComponent(label);
			hg2.addComponent(targs[i]);

			vg.addGroup(layout.createParallelGroup(Alignment.BASELINE)
			/**/.addComponent(label)
			/**/.addComponent(targs[i],pref,pref,pref));
			}

		tfAuth = new JTextField(null);
		taDesc = new JTextArea();
		tfAuth.setEditable(false);
		taDesc.setEditable(false);
		taDesc.setLineWrap(true);
		taDesc.setWrapStyleWord(true);

		JLabel lAuth = new JLabel(Messages.getString("EnigmaSettingsFrame.LABEL_AUTHOR")); //$NON-NLS-1$
		JScrollPane desc = new JScrollPane(taDesc);

		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addGroup(hg1)
		/*	*/.addPreferredGap(ComponentPlacement.RELATED)
		/*	*/.addGroup(hg2))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(lAuth)
		/*	*/.addPreferredGap(ComponentPlacement.RELATED)
		/*	*/.addComponent(tfAuth))
		/**/.addComponent(desc));
		layout.setVerticalGroup(vg
		/**/.addPreferredGap(ComponentPlacement.RELATED)
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lAuth)
		/*	*/.addComponent(tfAuth))
		/**/.addPreferredGap(ComponentPlacement.RELATED)
		/**/.addComponent(desc,0,0,Integer.MAX_VALUE));

		return p;
		}

	public void loadFromFile()
		{
		fc.setDialogTitle(Messages.getString("EnigmaSettingsFrame.LOAD_TITLE")); //$NON-NLS-1$
		while (true)
			{
			if (fc.showOpenDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			if (fc.getSelectedFile().exists()) break;
			JOptionPane.showMessageDialog(null,fc.getSelectedFile().getName()
					+ org.lateralgm.messages.Messages.getString("SoundFrame.FILE_MISSING"), //$NON-NLS-1$
					Messages.getString("EnigmaSettingsFrame.LOAD_MISSING_TITLE"),JOptionPane.WARNING_MESSAGE); //$NON-NLS-1$
			}

		es.options.clear();
		try
			{
			YamlNode n = YamlParser.parse(fc.getSelectedFile());
			for (YamlElement e : n.chronos)
				{
				if (!e.isScalar) continue;
				String val = ((YamlContent) e).getValue();
				es.options.put(e.name,val);
				}
			setComponents(es);
			}
		catch (IOException e)
			{
			e.printStackTrace();
			}
		}

	public void saveToFile()
		{
		commitChanges();
		fc.setDialogTitle(Messages.getString("EnigmaSettingsFrame.SAVE_TITLE")); //$NON-NLS-1$
		if (fc.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
		String name = fc.getSelectedFile().getPath();
		if (CustomFileFilter.getExtension(name) == null) name += ".ey"; //$NON-NLS-1$

		try
			{
			PrintStream ps = new PrintStream(new File(name));

			ps.println("%e-yaml"); //$NON-NLS-1$
			ps.println("---"); //$NON-NLS-1$

			//options
			for (Entry<String,String> entry : es.options.entrySet())
				ps.println(entry.getKey() + ": " + entry.getValue()); //$NON-NLS-1$
			ps.println();

			//targets
			for (Entry<String,TargetSelection> entry : es.targets.entrySet())
				{
				if (entry.getValue() == null) continue;
				ps.format("target-%s: %s\n",entry.getKey(),entry.getValue().id); //$NON-NLS-1$
				}
			ps.println("target-networking: None"); //$NON-NLS-1$

			ps.close();
			}
		catch (FileNotFoundException e)
			{
			e.printStackTrace();
			}
		}

	public void updateResource()
		{
		commitChanges();
		es.copyInto(oldEs);
		}

	public void commitChanges()
		{
		es.options.clear();
		for (Entry<String,Option> entry : options.entrySet())
			es.options.put(entry.getKey(),entry.getValue() == null ? null : entry.getValue().getValue());

		es.definitions = sDef.getCode();
		es.globalLocals = sGlobLoc.getCode();
		es.initialization = sInit.getCode();
		es.cleanup = sClean.getCode();

		es.targets.clear();
		for (Entry<JComboBox,String> box : targets.entrySet())
			es.targets.put(box.getValue(),(TargetSelection) box.getKey().getSelectedItem());
		}

	public void setComponents(EnigmaSettings es)
		{
		this.es = es;

		for (Entry<String,String> entry : es.options.entrySet())
			{
			Option opt = options.get(entry.getKey());
			if (opt != null) opt.setValue(entry.getValue());
			}

		sDef.setCode(es.definitions);
		sGlobLoc.setCode(es.globalLocals);
		sInit.setCode(es.initialization);
		sClean.setCode(es.cleanup);

		for (Entry<JComboBox,String> box : targets.entrySet())
			{
			TargetSelection targ = es.targets.get(box.getValue());
			if (targ != null) box.getKey().setSelectedItem(targ);
			userPicks.clear();
			}
		}

	public void revertResource()
		{
		setComponents(oldEs);
		}

	public boolean resourceChanged()
		{
		commitChanges();
		ReflectionComparator rc = new SimpleCasesComparator(new CollectionComparator(new MapComparator(
				new ObjectComparator(null))));
		return !rc.areEqual(oldEs,es);
		}

	/** A special ComboBoxModel to alleviate repopulation */
	private class TargetCombo extends AbstractListModel implements ComboBoxModel
		{
		private static final long serialVersionUID = 1L;

		private List<TargetSelection> data;
		private TargetSelection selection;

		public TargetCombo()
			{
			data = new ArrayList<TargetSelection>();
			}

		public void setData(Collection<TargetSelection> data)
			{
			this.data = new ArrayList<TargetSelection>(data);
			fireContentsChanged(this,0,data.size() - 1);
			}

		@Override
		public void setSelectedItem(Object anItem)
			{
			if (anItem instanceof TargetSelection && data.contains(anItem))
				selection = (TargetSelection) anItem;
			}

		//These methods may be called frequently...
		@Override
		public Object getElementAt(int index)
			{
			return data.get(index);
			}

		@Override
		public int getSize()
			{
			return data.size();
			}

		@Override
		public Object getSelectedItem()
			{
			return selection;
			}
		}

	private JComponent[] initializeTargets()
		{
		//first, initialize the boxes
		JComboBox[] targs = new JComboBox[TargetHandler.targets.size()];
		targets = new HashMap<JComboBox,String>(targs.length);
		for (int i = 0; i < targs.length; i++)
			targets.put(targs[i] = new JComboBox(new TargetCombo()),TargetHandler.ids[i]);

		//now populate them
		userPicks.put(TargetHandler.ids[0],TargetHandler.defaults.get(TargetHandler.ids[0]));
		targs[0].setFont(targs[0].getFont().deriveFont(Font.BOLD));
		populateTargets();

		//now set their selection and listeners
		for (int i = 0; i < targs.length; i++)
			{
			targs[i].setSelectedIndex(0);
			targs[i].addPopupMenuListener(this);
			targs[i].addActionListener(this);
			targs[i].addFocusListener(this);
			}

		return targs;
		}

	/**
	 * This method populates each combo box with valid choices for if it is activated.
	 * The boxes are populated with a choice for any system that does not violate the
	 * dependencies of a locked system. For example, if the compiler isn't locked, we
	 * can offer Win32 and X11 assuming we have a MinGW cross compiler and the Linux
	 * GCC installed. If we have locked the compiler to MinGW-cross, the only option
	 * for the windowing API is Win32.
	 * 
	 * Use this function whenever anything should change about the available or locked
	 * systems that would affect the combo box contents.
	 */
	void populateTargets()
		{
		for (Entry<JComboBox,String> target : targets.entrySet()) // Iterate our combo boxes.
			{ // We will need a handle to the combobox and the system it represents (ie, "windowing").
			Set<TargetSelection> options = new LinkedHashSet<TargetSelection>(); // Keep a list of valid systems to assign to the combobox
			for (Map<String,TargetSelection> combo : TargetHandler.combos) // Iterate our combinations.
				{
				boolean addMe = true; // Does this combination contain all the locked systems (the ones the user specifically assigned)?
				for (Entry<String,TargetSelection> estarget : userPicks.entrySet()) // For each lock (used-assigned system)
					{
					if (estarget.getKey().equals(target.getValue())) continue; // If the lock is actually for this combobox's system, ignore the lock; the user is in the process of changing its lock
					if (combo.get(estarget.getKey()) != estarget.getValue()) addMe = false; // If this combo does not employ a system the user specified, do not use it.
					}
				if (addMe) options.add(combo.get(target.getValue())); // If the system meets all criteria, add it to the list.
				}
			//			target.getKey().setModel(new DefaultComboBoxModel(options.toArray(new TargetSelection[0])));
			// Finally, set the values of the combobox to the list we just generated.
			// Since we iterated the main combo list linearly, and that list was sorted by score,
			//   we can assume that the new list is likewise sorted by score.
			((TargetCombo) target.getKey().getModel()).setData(options);
			}
		}

	/**
	 * This method is invoked when the user clicks away from the target selection dropdown menu.
	 * It behaves as more of a feature than a necessary part of the system; canceling the selection
	 * functions like a reset button for the current item. It removes the lock from the previously
	 * selected target, allowing systems to be selected in other dropdowns which are not compatible
	 * with the current system (thus leading to its replacement).
	 */
	@Override
	public void popupMenuCanceled(PopupMenuEvent e)
		{
		JComboBox changedSystem = (JComboBox) e.getSource();
		String current = targets.get(changedSystem); // Get the category of the changed system
		if (current == null) return;
		userPicks.remove(current); // Clear the lock on this system
		changedSystem.setFont(changedSystem.getFont().deriveFont(Font.PLAIN)); // Unbold the box
		populateTargets(); // Re-populate our combo boxes to reflect that they no longer need to
		// fill this system's dependencies
		}

	//prevents recursively firing a changed selection while trampling selections
	private boolean changing = false;

	@Override
	public void actionPerformed(ActionEvent e)
		{
		Object s = e.getSource();

		if (s == save)
			{
			updateResource();
			setVisible(false);
			return;
			}

		if (s == loadFile)
			{
			loadFromFile();
			return;
			}

		if (s == saveFile)
			{
			saveToFile();
			return;
			}

		if (s == bDef || s == bGlobLoc || s == bInit || s == bClean)
			{
			if (s == bDef)
				{
				if (cfDef == null)
					cfDef = newCodeFrame(sDef,
							Messages.getString("EnigmaSettingsFrame.CODE_TITLE_DEFINITIONS")); //$NON-NLS-1$
				cfDef.toTop();
				}
			if (s == bGlobLoc)
				{
				if (cfDef == null)
					cfGlobLoc = newCodeFrame(sGlobLoc,
							Messages.getString("EnigmaSettingsFrame.CODE_TITLE_GLOBAL_LOCALS")); //$NON-NLS-1$
				cfGlobLoc.toTop();
				}
			if (s == bInit)
				{
				if (cfInit == null)
					cfInit = newCodeFrame(sInit,
							Messages.getString("EnigmaSettingsFrame.CODE_TITLE_INITIALIZATION")); //$NON-NLS-1$
				cfInit.toTop();
				}
			if (s == bClean)
				{
				if (cfClean == null)
					cfClean = newCodeFrame(sClean,
							Messages.getString("EnigmaSettingsFrame.CODE_TITLE_CLEANUP")); //$NON-NLS-1$
				cfClean.toTop();
				}
			return;
			}

		String targSys = targets.get(s);
		if (targSys != null)
			{
			if (changing) return;

			JComboBox changedSystem = (JComboBox) s;
			TargetSelection changedSelection = (TargetSelection) changedSystem.getSelectedItem();

			//Update author and description
			if (changedSystem.hasFocus())
				{
				tfAuth.setText(changedSelection.auth);
				taDesc.setText(changedSelection.desc);
				}

			//repopulate the boxes
			userPicks.put(targSys,changedSelection);
			changedSystem.setFont(changedSystem.getFont().deriveFont(Font.BOLD));
			populateTargets();

			//now trample the other cancelled selections
			changing = true;
			for (Entry<JComboBox,String> systems : targets.entrySet())
				if (systems.getKey() != changedSystem && userPicks.get(systems.getValue()) == null)
					systems.getKey().setSelectedIndex(0);
			changing = false;
			}
		}

	@Override
	public void focusGained(FocusEvent e)
		{
		if (!(e.getSource() instanceof JComboBox)) return;
		Object o = ((JComboBox) e.getSource()).getSelectedItem();
		if (!(o instanceof TargetSelection)) return;
		TargetSelection ts = (TargetSelection) o;
		tfAuth.setText(ts.auth);
		taDesc.setText(ts.desc);
		}

	protected void fireInternalFrameEvent(int id)
		{
		if (id == InternalFrameEvent.INTERNAL_FRAME_CLOSING)
			{
			if (resourceChanged())
				{
				String resource = Messages.getString("EnigmaSettingsFrame.DIALOG_KEEPCHANGES_RESOURCE"); //$NON-NLS-1$
				int ret = JOptionPane.showConfirmDialog(
						LGM.frame,
						org.lateralgm.messages.Messages.format("ResourceFrame.KEEPCHANGES",resource), //$NON-NLS-1$
						org.lateralgm.messages.Messages.getString("ResourceFrame.KEEPCHANGES_TITLE"),JOptionPane.YES_NO_CANCEL_OPTION); //$NON-NLS-1$
				if (ret == JOptionPane.YES_OPTION)
					{
					updateResource();
					setVisible(false);
					}
				else if (ret == JOptionPane.NO_OPTION)
					{
					revertResource();
					setVisible(false);
					}
				}
			else
				{
				updateResource();
				setVisible(false);
				}
			}
		super.fireInternalFrameEvent(id);
		}

	private final InternalFrameListener ifl = new CodeFrameListener();

	private class CodeFrameListener extends InternalFrameAdapter
		{
		public void internalFrameClosed(InternalFrameEvent e)
			{
			CodeFrame cf = (CodeFrame) e.getSource();
			if (cf == cfDef)
				{
				if (!es.definitions.equals(sDef.getCode()))
					{
					es.definitions = sDef.getCode();
					es.saveDefinitions();
					if (EnigmaRunner.ENIGMA_READY) es.commitToDriver(EnigmaRunner.DRIVER);
					}
				cfDef = null;
				}
			if (cf == cfGlobLoc) cfGlobLoc = null;
			if (cf == cfInit) cfInit = null;
			if (cf == cfClean) cfClean = null;
			}
		}

	private CodeFrame newCodeFrame(CodeHolder ch, String title)
		{
		CodeFrame cf = new CodeFrame(ch,"{0}",title); //$NON-NLS-1$
		cf.addInternalFrameListener(ifl);
		LGM.mdi.add(cf);
		LGM.mdi.addZChild(this,cf);
		return cf;
		}

	//Unused
	@Override
	public void focusLost(FocusEvent e)
		{
		}

	@Override
	public void popupMenuWillBecomeVisible(PopupMenuEvent e)
		{
		}

	@Override
	public void popupMenuWillBecomeInvisible(PopupMenuEvent e)
		{
		}
	}
