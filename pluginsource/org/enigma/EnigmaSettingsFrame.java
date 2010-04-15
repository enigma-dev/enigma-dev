package org.enigma;

import static java.lang.Integer.MAX_VALUE;
import static javax.swing.GroupLayout.DEFAULT_SIZE;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.event.InternalFrameAdapter;
import javax.swing.event.InternalFrameEvent;
import javax.swing.event.InternalFrameListener;

import org.enigma.backend.EnigmaStruct;
import org.lateralgm.components.impl.IndexButtonGroup;
import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;
import org.lateralgm.subframes.CodeFrame;
import org.lateralgm.subframes.CodeFrame.CodeHolder;

public class EnigmaSettingsFrame extends MDIFrame implements ActionListener
	{
	private static final long serialVersionUID = 1L;
	private static final ImageIcon CODE_ICON = LGM.getIconForKey("Resource.SCRIPT"); //$NON-NLS-1$

	private EnigmaSettings es;

	private IndexButtonGroup strings, increment, literal, struct;
	private IndexButtonGroup instance, storage;
	private JButton bDef, bGlobLoc;
	private JButton bInit, bClean;
	private CodeFrame cfDef, cfGlobLoc, cfInit, cfClean;
	private CodeHolder sDef, sGlobLoc, sInit, sClean;
	private JButton bSave, bDiscard;

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
		JPanel compatPane = new JPanel(new GridLayout(4,3));
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

		JLabel l3 = new JLabel("Treat literals as:");
		literal = new IndexButtonGroup(2,true,false);
		JRadioButton literalVar = new JRadioButton("Enigma (Variant)");
		JRadioButton literalScalar = new JRadioButton("C++ (Scalar)");
		literal.add(literalVar);
		literal.add(literalScalar);
		literal.setValue(es.literalHandling);

		JLabel l4 = new JLabel("Closing brace of struct:");
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
		/*	*/.addComponent(l4))
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(stringsGML)
		/*	*/.addComponent(incrementGML)
		/*	*/.addComponent(literalVar)
		/*	*/.addComponent(structSemi))
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(stringsCPP)
		/*	*/.addComponent(incrementCPP)
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
		/*	*/.addComponent(literalVar)
		/*	*/.addComponent(literalScalar))
		/**/.addGroup(compat.createParallelGroup(Alignment.BASELINE)
		/*	*/.addComponent(l4)
		/*	*/.addComponent(structSemi)
		/*	*/.addComponent(structISO)));

		return compatPane;
		}

	private JPanel makeAdvPane()
		{
		JPanel advPane = new JPanel(new GridLayout(2,4));
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
		super("Enigma Settings",true,true,true,true); //$NON-NLS-1$
		setDefaultCloseOperation(HIDE_ON_CLOSE);
		setFrameIcon(LGM.findIcon("restree/gm.png")); //$NON-NLS-1$
		GroupLayout layout = new GroupLayout(getContentPane());
		setLayout(layout);
		setResizable(false);
		this.es = es;

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

		bSave = new JButton("Save");
		bSave.addActionListener(this);
		bDiscard = new JButton("Discard");
		bDiscard.addActionListener(this);

		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bDef)
		/*	*/.addComponent(bGlobLoc))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bInit)
		/*	*/.addComponent(bClean))
		/**/.addGroup(layout.createSequentialGroup()
		/*		*/.addContainerGap()
		/*		*/.addComponent(bSave,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addComponent(bDiscard,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addContainerGap()));
		layout.setVerticalGroup(layout.createSequentialGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bDef)
		/*	*/.addComponent(bGlobLoc))
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bInit)
		/*	*/.addComponent(bClean))
		/**/.addPreferredGap(ComponentPlacement.UNRELATED)
		/**/.addGroup(layout.createParallelGroup()
		/*		*/.addComponent(bSave)
		/*		*/.addComponent(bDiscard))
		/**/.addContainerGap());
		pack();
		}

	@Override
	public void actionPerformed(ActionEvent e)
		{
		Object s = e.getSource();

		if (s == bSave)
			{
			commitChanges();
			setVisible(false);
			return;
			}

		if (s == bDiscard)
			{
			setComponents(es);
			setVisible(false);
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
		}

	private final InternalFrameListener ifl = new CodeFrameListener();

	private class CodeFrameListener extends InternalFrameAdapter
		{
		public void internalFrameClosed(InternalFrameEvent e)
			{
			CodeFrame cf = (CodeFrame) e.getSource();
			if (cf == cfDef) cfDef = null;
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

	public void commitChanges()
		{
		es.cppStrings = strings.getValue();
		es.cppOperators = increment.getValue();
		es.literalHandling = literal.getValue();
		es.structHandling = struct.getValue();

		es.instanceTypes = instance.getValue();
		es.storageClass = storage.getValue();

		if (!es.definitions.equals(sDef.getCode()))
			{
			es.definitions = sDef.getCode();
			if (EnigmaRunner.GCC_LOCATED) EnigmaStruct.whitespaceModified(es.definitions);
			}
		es.globalLocals = sGlobLoc.getCode();
		es.initialization = sInit.getCode();
		es.cleanup = sClean.getCode();
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

	public static class EnigmaSettings
		{
		//Compatibility / Progess options
		int cppStrings = 0; // Defines what language strings are inherited from.               0 = GML,               1 = C
		int cppOperators = 0; // Defines what language operators ++ and -- are inherited from. 0 = GML,               1 = C
		int literalHandling = 0; // Determines how literals are treated.                       0 = enigma::variant,   1 = C-scalar
		int structHandling = 0; // Defines behavior of the closing brace of struct {}.         0 = Implied semicolon, 1 = ISO C

		//Advanced options
		int instanceTypes = 0; // Defines how to represent instances.           0 = Integer, 1 = Pointer
		int storageClass = 0; // Determines how instances are stored in memory. 0 = Map,     1 = List,    2 = Array

		String definitions = "", globalLocals = "";
		String initialization = "", cleanup = "";
		}
	}
