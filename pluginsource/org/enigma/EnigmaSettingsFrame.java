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

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.DefaultComboBoxModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
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
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;
import javax.swing.event.InternalFrameListener;

import org.enigma.EnigmaRunner.TargetSelection;
import org.enigma.backend.EnigmaSettings;
import org.lateralgm.compare.CollectionComparator;
import org.lateralgm.compare.MapComparator;
import org.lateralgm.compare.ObjectComparator;
import org.lateralgm.compare.ReflectionComparator;
import org.lateralgm.compare.SimpleCasesComparator;
import org.lateralgm.components.CustomFileChooser;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.IndexButtonGroup;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.file.GmStreamDecoder;
import org.lateralgm.main.LGM;
import org.lateralgm.messages.Messages;
import org.lateralgm.subframes.CodeFrame;
import org.lateralgm.subframes.CodeFrame.CodeHolder;

public class EnigmaSettingsFrame extends MDIFrame implements ActionListener
	{
	private static final long serialVersionUID = 1L;
	private static final ImageIcon CODE_ICON = LGM.getIconForKey("Resource.SCRIPT"); //$NON-NLS-1$

	private EnigmaSettings oldEs, es;

	protected JToolBar toolbar;
	protected JButton save, saveFile, loadFile;

	private IndexButtonGroup strings, increment, equal, literal, struct;
	private IndexButtonGroup instance, storage;
	private JButton bDef, bGlobLoc;
	private JButton bInit, bClean;
	private CodeFrame cfDef, cfGlobLoc, cfInit, cfClean;
	private CodeHolder sDef, sGlobLoc, sInit, sClean;
	private CustomFileChooser fc;

	private JComboBox targPlat, targGfx, targSound, targColl;
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

	private JPanel makeCompatPane()
		{
		JPanel compatPane = new JPanel();
		compatPane.setBorder(BorderFactory.createTitledBorder("Compatability/Progress"));
		GroupLayout compat = new GroupLayout(compatPane);
		compatPane.setLayout(compat);

		JLabel l1 = new JLabel("Inherit strings from:");
		strings = new IndexButtonGroup(2,true,false);
		JRadioButton stringsGML = new JRadioButton("GML (#)");
		JRadioButton stringsCPP = new JRadioButton("C (\\n)");
		strings.add(stringsGML);
		strings.add(stringsCPP);
		strings.setValue(es.cppStrings);

		JLabel l2 = new JLabel("Inherit ++/-- from:");
		increment = new IndexButtonGroup(2,true,false);
		JRadioButton incrementGML = new JRadioButton("GML (+)");
		JRadioButton incrementCPP = new JRadioButton("C (+=1/-=1)");
		increment.add(incrementGML);
		increment.add(incrementCPP);
		increment.setValue(es.cppOperators);

		JLabel l3 = new JLabel("Inherit = (e.g. a=b=c) from:");
		equal = new IndexButtonGroup(2,true,false);
		JRadioButton equalGML = new JRadioButton("GML (a=(b==c))");
		JRadioButton equalCPP = new JRadioButton("C (a=c; b=c)");
		equal.add(equalGML);
		equal.add(equalCPP);
		equal.setValue(es.cppEquals);

		JLabel l4 = new JLabel("Treat literals as:");
		literal = new IndexButtonGroup(2,true,false);
		JRadioButton literalVar = new JRadioButton("Enigma (Variant)");
		JRadioButton literalScalar = new JRadioButton("C++ (Scalar)");
		literal.add(literalVar);
		literal.add(literalScalar);
		literal.setValue(es.literalHandling);

		JLabel l5 = new JLabel("Closing brace of struct:");
		struct = new IndexButtonGroup(2,true,false);
		JRadioButton structSemi = new JRadioButton("Implied Semicolon");
		JRadioButton structISO = new JRadioButton("ISO C");
		struct.add(structSemi);
		struct.add(structISO);
		struct.setValue(es.structHandling);

		compat.setHorizontalGroup(compat.createSequentialGroup()
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(l1)
		/*	*/.addComponent(l2)
		/*	*/.addComponent(l3)
		/*	*/.addComponent(l4)
		/*	*/.addComponent(l5))
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(stringsGML)
		/*	*/.addComponent(incrementGML)
		/*	*/.addComponent(equalGML)
		/*	*/.addComponent(literalVar)
		/*	*/.addComponent(structSemi))
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(stringsCPP)
		/*	*/.addComponent(incrementCPP)
		/*	*/.addComponent(equalCPP)
		/*	*/.addComponent(literalScalar)
		/*	*/.addComponent(structISO)));

		compat.setVerticalGroup(compat.createSequentialGroup()
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l1)
		/*	*/.addComponent(stringsGML)
		/*	*/.addComponent(stringsCPP))
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l2)
		/*	*/.addComponent(incrementGML)
		/*	*/.addComponent(incrementCPP))
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l3)
		/*	*/.addComponent(equalGML)
		/*	*/.addComponent(equalCPP))
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l4)
		/*	*/.addComponent(literalVar)
		/*	*/.addComponent(literalScalar))
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l5)
		/*	*/.addComponent(structSemi)
		/*	*/.addComponent(structISO)));

		return compatPane;
		}

	private JPanel makeAdvPane()
		{
		JPanel advPane = new JPanel();
		advPane.setBorder(BorderFactory.createTitledBorder("Advanced"));
		GroupLayout adv = new GroupLayout(advPane);
		advPane.setLayout(adv);

		JLabel l5 = new JLabel("Represent instances as:");
		instance = new IndexButtonGroup(2,true,false);
		JRadioButton instanceInt = new JRadioButton("Integer");
		JRadioButton instancePnt = new JRadioButton("Pointer");
		instance.add(instanceInt);
		instance.add(instancePnt);
		instance.setValue(es.instanceTypes);

		JLabel l6 = new JLabel("Store instances as:");
		storage = new IndexButtonGroup(3,true,false);
		JRadioButton storageMap = new JRadioButton("Map");
		JRadioButton storageList = new JRadioButton("List");
		JRadioButton storageArray = new JRadioButton("Array");
		storage.add(storageMap);
		storage.add(storageList);
		storage.add(storageArray);
		storage.setValue(es.storageClass);

		adv.setHorizontalGroup(adv.createSequentialGroup()
		/**/.addGroup(adv.createParallelGroup()
		/*	*/.addComponent(l5)
		/*	*/.addComponent(l6))
		/**/.addGroup(adv.createParallelGroup()
		/*	*/.addComponent(instanceInt)
		/*	*/.addComponent(storageMap))
		/**/.addGroup(adv.createParallelGroup()
		/*	*/.addComponent(instancePnt)
		/*	*/.addComponent(storageList))
		/**/.addGroup(adv.createParallelGroup()
		/*	*/.addComponent(storageArray)));

		adv.setVerticalGroup(adv.createSequentialGroup()
		/**/.addGroup(adv.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l5)
		/*	*/.addComponent(instanceInt)
		/*	*/.addComponent(instancePnt))
		/**/.addGroup(adv.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l6)
		/*	*/.addComponent(storageMap)
		/*	*/.addComponent(storageList)
		/*	*/.addComponent(storageArray)));

		return advPane;
		}

	public EnigmaSettingsFrame(EnigmaSettings es)
		{
		super("Enigma Settings",false,true,true,true); //$NON-NLS-1$
		setDefaultCloseOperation(HIDE_ON_CLOSE);
		setFrameIcon(LGM.findIcon("restree/gm.png")); //$NON-NLS-1$
		this.oldEs = es;
		this.es = es.copy();

		fc = new CustomFileChooser("/org/enigma","LAST_SETTINGS_DIR"); //$NON-NLS-1$ //$NON-NLS-2$
		fc.setFileFilter(new CustomFileFilter(".esf","Enigma Settings File")); //$NON-NLS-1$

		toolbar = makeToolBar();
		add(toolbar,BorderLayout.NORTH);
		JTabbedPane tabs = new JTabbedPane();
		tabs.add("General",makeSettings());
		tabs.add("API",makeAPI());
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

		loadFile = new JButton(LGM.getIconForKey("LGM.OPEN"));
		loadFile.setToolTipText("Load from file");
		loadFile.setRequestFocusEnabled(false);
		loadFile.addActionListener(this);
		tool.add(loadFile);
		saveFile = new JButton(LGM.getIconForKey("LGM.SAVEAS"));
		saveFile.setToolTipText("Save to file");
		saveFile.setRequestFocusEnabled(false);
		saveFile.addActionListener(this);
		tool.add(saveFile);

		tool.addSeparator();
		return tool;
		}

	private JPanel makeSettings()
		{
		JPanel p = new JPanel();
		GroupLayout layout = new GroupLayout(p);
		p.setLayout(layout);

		JPanel compatPane = makeCompatPane();
		JPanel advPane = makeAdvPane();

		sDef = new SimpleCodeHolder(es.definitions);
		sGlobLoc = new SimpleCodeHolder(es.globalLocals);
		sInit = new SimpleCodeHolder(es.initialization);
		sClean = new SimpleCodeHolder(es.cleanup);

		bDef = new JButton("Definitions",CODE_ICON);
		bGlobLoc = new JButton("Global Locals",CODE_ICON);
		bInit = new JButton("Initialization",CODE_ICON);
		bClean = new JButton("Cleanup",CODE_ICON);

		bDef.addActionListener(this);
		bGlobLoc.addActionListener(this);
		bInit.addActionListener(this);
		bClean.addActionListener(this);

		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bDef)
		/*	*/.addComponent(bGlobLoc))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bInit)
		/*	*/.addComponent(bClean)));
		layout.setVerticalGroup(layout.createSequentialGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
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

		JPanel platPane = new JPanel(new GridLayout(4,3));
		platPane.setBorder(BorderFactory.createTitledBorder("Platform"));
		GroupLayout plat = new GroupLayout(platPane);
		platPane.setLayout(plat);

		String curPlat = EnigmaRunner.normalize(System.getProperty("os.name"));
		targPlat = new JComboBox(EnigmaRunner.findTargets("Platforms",curPlat).toArray());
		TargetSelection ts = (TargetSelection) targPlat.getSelectedItem();
		String defPlat = ts == null ? null : ts.id;
		targGfx = new JComboBox(EnigmaRunner.findTargets("Graphics_Systems",defPlat).toArray());
		targSound = new JComboBox(EnigmaRunner.findTargets("Audio_Systems",defPlat).toArray());
		targColl = new JComboBox(EnigmaRunner.findTargets("Collision_Systems",defPlat).toArray());
		targPlat.addActionListener(this);
		targGfx.addActionListener(this);
		targSound.addActionListener(this);
		targColl.addActionListener(this);

		tfAuth = new JTextField(ts == null ? null : ts.auth);
		taDesc = new JTextArea(ts == null ? null : ts.desc);
		tfAuth.setEditable(false);
		taDesc.setEditable(false);
		taDesc.setLineWrap(true);
		taDesc.setWrapStyleWord(true);

		JLabel lPlat = new JLabel("Platform: ");
		JLabel lGfx = new JLabel("Graphics: ");
		JLabel lSound = new JLabel("Audio: ");
		JLabel lColl = new JLabel("Collision: ");
		JLabel lAuth = new JLabel("Author: ");
		JScrollPane desc = new JScrollPane(taDesc);

		int pref = GroupLayout.PREFERRED_SIZE;
		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addGroup(layout.createParallelGroup()
		/*		*/.addComponent(lPlat)
		/*		*/.addComponent(lGfx)
		/*		*/.addComponent(lSound)
		/*		*/.addComponent(lColl))
		/*	*/.addPreferredGap(ComponentPlacement.RELATED)
		/*	*/.addGroup(layout.createParallelGroup()
		/*		*/.addComponent(targPlat)
		/*		*/.addComponent(targGfx)
		/*		*/.addComponent(targSound)
		/*		*/.addComponent(targColl)))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(lAuth)
		/*	*/.addPreferredGap(ComponentPlacement.RELATED)
		/*	*/.addComponent(tfAuth))
		/**/.addComponent(desc));
		layout.setVerticalGroup(layout.createSequentialGroup()
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lPlat)
		/*	*/.addComponent(targPlat,pref,pref,pref))
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lGfx)
		/*	*/.addComponent(targGfx,pref,pref,pref))
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lSound)
		/*	*/.addComponent(targSound,pref,pref,pref))
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lColl)
		/*	*/.addComponent(targColl,pref,pref,pref))
		/**/.addPreferredGap(ComponentPlacement.RELATED)
		/**/.addGroup(layout.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(lAuth)
		/*	*/.addComponent(tfAuth))
		/**/.addPreferredGap(ComponentPlacement.RELATED)
		/**/.addComponent(desc));

		return p;
		}

	public void loadFromFile()
		{
		fc.setDialogTitle("File to load information from");
		while (true)
			{
			if (fc.showOpenDialog(LGM.frame) != JFileChooser.APPROVE_OPTION) return;
			if (fc.getSelectedFile().exists()) break;
			JOptionPane.showMessageDialog(null,
					fc.getSelectedFile().getName() + Messages.getString("SoundFrame.FILE_MISSING"), //$NON-NLS-1$
					"File to load information from",JOptionPane.WARNING_MESSAGE);
			}
		try
			{
			FileInputStream i = new FileInputStream(fc.getSelectedFile());
			int ver = i.read();
			if (ver != 1) throw new IOException("Incorrect version: " + ver);
			es.cppStrings = i.read();
			es.cppOperators = i.read();
			es.cppEquals = i.read();
			es.literalHandling = i.read();
			es.structHandling = i.read();

			es.instanceTypes = i.read();
			es.storageClass = i.read();

			es.definitions = readStr(i);
			if (!es.definitions.equals(sDef.getCode()))
				{
				es.definitions = sDef.getCode();
				es.saveDefinitions();
				if (EnigmaRunner.GCC_LOCATED)
					EnigmaRunner.DRIVER.definitionsModified(es.definitions,EnigmaRunner.targetYaml(es));
				}
			es.globalLocals = readStr(i);
			es.initialization = readStr(i);
			es.cleanup = readStr(i);
			setComponents(es);
			i.close();
			}
		catch (IOException e)
			{
			e.printStackTrace();
			}
		}

	public void saveToFile()
		{
		commitChanges();
		fc.setDialogTitle("File to write information to"); //$NON-NLS-1$
		if (fc.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
		String name = fc.getSelectedFile().getPath();
		if (CustomFileFilter.getExtension(name) == null) name += ".esf"; //$NON-NLS-1$
		try
			{
			FileOutputStream i = new FileOutputStream(new File(name));

			i.write(1);
			i.write(es.cppStrings);
			i.write(es.cppOperators);
			i.write(es.cppEquals);
			i.write(es.literalHandling);
			i.write(es.structHandling);

			i.write(es.instanceTypes);
			i.write(es.storageClass);

			writeStr(i,es.definitions);
			writeStr(i,es.globalLocals);
			writeStr(i,es.initialization);
			writeStr(i,es.cleanup);
			i.close();

			i.close();
			}
		catch (Exception e)
			{
			e.printStackTrace();
			}
		}

	String readStr(InputStream is) throws IOException
		{
		int a = is.read();
		int b = is.read();
		int c = is.read();
		int d = is.read();
		int size = (a | (b << 8) | (c << 16) | (d << 24));

		byte data[] = new byte[size];
		is.read(data);
		return new String(data,GmStreamDecoder.CHARSET);
		}

	void writeStr(OutputStream os, String str) throws IOException
		{
		int val = str.length();
		os.write(val & 255);
		os.write((val >>> 8) & 255);
		os.write((val >>> 16) & 255);
		os.write((val >>> 24) & 255);
		os.write(str.getBytes(GmStreamDecoder.CHARSET));
		}

	public void updateResource()
		{
		commitChanges();
		oldEs = es.copy();
		}

	public void commitChanges()
		{
		es.cppStrings = strings.getValue();
		es.cppOperators = increment.getValue();
		es.literalHandling = literal.getValue();
		es.structHandling = struct.getValue();

		es.instanceTypes = instance.getValue();
		es.storageClass = storage.getValue();

		es.definitions = sDef.getCode();
		es.globalLocals = sGlobLoc.getCode();
		es.initialization = sInit.getCode();
		es.cleanup = sClean.getCode();

		TargetSelection ts = (TargetSelection) targPlat.getSelectedItem();
		es.targetPlatform = (ts == null ? null : ts.id);
		ts = (TargetSelection) targGfx.getSelectedItem();
		es.targetGraphics = (ts == null ? null : ts.id);
		ts = (TargetSelection) targSound.getSelectedItem();
		es.targetSound = (ts == null ? null : ts.id);
		ts = (TargetSelection) targColl.getSelectedItem();
		es.targetCollision = (ts == null ? null : ts.id);
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
				if (cfDef == null) cfDef = newCodeFrame(sDef,"Enigma Definitions");
				cfDef.toTop();
				}
			if (s == bGlobLoc)
				{
				if (cfDef == null) cfGlobLoc = newCodeFrame(sGlobLoc,"Enigma Global Locals");
				cfGlobLoc.toTop();
				}
			if (s == bInit)
				{
				if (cfInit == null) cfInit = newCodeFrame(sInit,"Enigma Initialization");
				cfInit.toTop();
				}
			if (s == bClean)
				{
				if (cfClean == null) cfClean = newCodeFrame(sClean,"Enigma Cleanup");
				cfClean.toTop();
				}
			return;
			}

		if (s == targPlat || s == targGfx || s == targSound || s == targColl)
			{
			TargetSelection ts = (TargetSelection) ((JComboBox) s).getSelectedItem();
			if (s == targPlat)
				{
				String defPlat = ts == null ? null : ts.id;
				targGfx.setModel(new DefaultComboBoxModel(EnigmaRunner.findTargets("Graphics_Systems",
						defPlat).toArray()));
				targSound.setModel(new DefaultComboBoxModel(EnigmaRunner.findTargets("Audio_Systems",
						defPlat).toArray()));
				targColl.setModel(new DefaultComboBoxModel(EnigmaRunner.findTargets("Collision_Systems",
						defPlat).toArray()));
				}
			tfAuth.setText(ts == null ? null : ts.auth);
			taDesc.setText(ts == null ? null : ts.desc);
			return;
			}
		}

	protected void fireInternalFrameEvent(int id)
		{
		if (id == InternalFrameEvent.INTERNAL_FRAME_CLOSING)
			{
			if (resourceChanged())
				{
				int ret = JOptionPane.showConfirmDialog(LGM.frame,
						Messages.format("ResourceFrame.KEEPCHANGES","Enigma Settings"), //$NON-NLS-1$
						Messages.getString("ResourceFrame.KEEPCHANGES_TITLE"),JOptionPane.YES_NO_CANCEL_OPTION); //$NON-NLS-1$
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
					if (EnigmaRunner.GCC_LOCATED)
						EnigmaRunner.DRIVER.definitionsModified(es.definitions,EnigmaRunner.targetYaml(es));
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
		CodeFrame cf = new CodeFrame(ch,"{0}",title);
		cf.addInternalFrameListener(ifl);
		LGM.mdi.add(cf);
		LGM.mdi.addZChild(this,cf);
		return cf;
		}

	public void setComponents(EnigmaSettings es)
		{
		this.es = es;

		strings.setValue(es.cppStrings);
		increment.setValue(es.cppOperators);
		literal.setValue(es.literalHandling);
		struct.setValue(es.structHandling);

		instance.setValue(es.instanceTypes);
		storage.setValue(es.storageClass);

		sDef.setCode(es.definitions);
		sGlobLoc.setCode(es.globalLocals);
		sInit.setCode(es.initialization);
		sClean.setCode(es.cleanup);
		}
	}
