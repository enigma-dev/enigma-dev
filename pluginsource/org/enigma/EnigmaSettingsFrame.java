package org.enigma;

import static java.lang.Integer.MAX_VALUE;
import static javax.swing.GroupLayout.DEFAULT_SIZE;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;

import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;

public class EnigmaSettingsFrame extends MDIFrame implements ActionListener
	{
	private static final long serialVersionUID = 1L;
	private static final ImageIcon CODE_ICON = LGM.getIconForKey("Resource.SCRIPT"); //$NON-NLS-1$

	public JButton bSave, bDiscard;
	public JButton bDefinitions, bGlobalLocals;
	public JButton bInitialization, bCleanup;

	public EnigmaSettingsFrame()
		{
		super("Enigma Settings",true,true,true,true); //$NON-NLS-1$
		setDefaultCloseOperation(HIDE_ON_CLOSE);
		setFrameIcon(LGM.findIcon("restree/gm.png")); //$NON-NLS-1$
		GroupLayout layout = new GroupLayout(getContentPane());
		setLayout(layout);
		setResizable(false);

		JPanel compatPane = new JPanel(new GridLayout(4,3));
		compatPane.setBorder(BorderFactory.createTitledBorder("Compatability/Progress"));
		GroupLayout compat = new GroupLayout(compatPane);
		layout.setAutoCreateGaps(true);
		compatPane.setLayout(compat);

		JLabel l1 = new JLabel("Inherit strings from:");
		ButtonGroup strings = new ButtonGroup();
		JRadioButton stringsGML = new JRadioButton("GML (#)");
		JRadioButton stringsCPP = new JRadioButton("C (\\n)");
		strings.add(stringsGML);
		strings.add(stringsCPP);
		JLabel l2 = new JLabel("Inherit ++/-- from:");
		ButtonGroup increment = new ButtonGroup();
		JRadioButton incrementGML = new JRadioButton("GML (+)");
		JRadioButton incrementCPP = new JRadioButton("C (+=1/-=1)");
		increment.add(incrementGML);
		increment.add(incrementCPP);
		JLabel l3 = new JLabel("Treat literals as:");
		ButtonGroup literal = new ButtonGroup();
		JRadioButton literalVar = new JRadioButton("Enigma (Variant)");
		JRadioButton literalScalar = new JRadioButton("C++ (Scalar)");
		literal.add(literalVar);
		literal.add(literalScalar);
		JLabel l4 = new JLabel("Closing brace of struct:");
		ButtonGroup struct = new ButtonGroup();
		JRadioButton structISO = new JRadioButton("ISO C");
		JRadioButton structSemi = new JRadioButton("Implied Semicolon");
		struct.add(structISO);
		struct.add(structSemi);

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
		/*	*/.addComponent(structISO))
		/**/.addGroup(compat.createParallelGroup()
		/*	*/.addComponent(stringsCPP)
		/*	*/.addComponent(incrementCPP)
		/*	*/.addComponent(literalScalar)
		/*	*/.addComponent(structSemi)));

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
		/*	*/.addComponent(structISO)
		/*	*/.addComponent(structSemi)));

		JPanel advPane = new JPanel(new GridLayout(2,4));
		advPane.setBorder(BorderFactory.createTitledBorder("Advanced"));
		GroupLayout adv = new GroupLayout(advPane);
		layout.setAutoCreateGaps(true);
		advPane.setLayout(adv);

		JLabel l5 = new JLabel("Represent instances as:");
		ButtonGroup instance = new ButtonGroup();
		JRadioButton instanceInt = new JRadioButton("Integer");
		JRadioButton instancePnt = new JRadioButton("Pointer");
		instance.add(instanceInt);
		instance.add(instancePnt);
		JLabel l6 = new JLabel("Store instances as:");
		ButtonGroup storage = new ButtonGroup();
		JRadioButton storageMap = new JRadioButton("Map");
		JRadioButton storageList = new JRadioButton("List");
		JRadioButton storageArray = new JRadioButton("Array");
		storage.add(storageMap);
		storage.add(storageList);
		storage.add(storageArray);

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

		bDefinitions = new JButton("Definitions",CODE_ICON);
		bGlobalLocals = new JButton("Global Locals",CODE_ICON);
		bInitialization = new JButton("Initialization",CODE_ICON);
		bCleanup = new JButton("Cleanup",CODE_ICON);

		//		buildTabs();

		bSave = new JButton("Save");
		bSave.addActionListener(this);
		bDiscard = new JButton("Discard");
		bDiscard.addActionListener(this);

		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bDefinitions)
		/*	*/.addComponent(bGlobalLocals))
		/**/.addGroup(layout.createSequentialGroup()
		/*	*/.addComponent(bInitialization)
		/*	*/.addComponent(bCleanup))
		/**/.addGroup(layout.createSequentialGroup()
		/*		*/.addContainerGap()
		/*		*/.addComponent(bSave,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addComponent(bDiscard,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addContainerGap()));
		layout.setVerticalGroup(layout.createSequentialGroup()
		/**/.addComponent(compatPane)
		/**/.addComponent(advPane)
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bDefinitions)
		/*	*/.addComponent(bGlobalLocals))
		/**/.addGroup(layout.createParallelGroup()
		/*	*/.addComponent(bInitialization)
		/*	*/.addComponent(bCleanup))
		/**/.addPreferredGap(ComponentPlacement.UNRELATED)
		/**/.addGroup(layout.createParallelGroup()
		/*		*/.addComponent(bSave)
		/*		*/.addComponent(bDiscard))
		/**/.addContainerGap());
		pack();
		}

	//	private void buildTabs()
	//		{
	//		JComponent pane = makeSettingsPane();
	//		tabbedPane.addTab("Settings",pane);
	//		tabbedPane.setMnemonicAt(0,KeyEvent.VK_1);
	//		}

	@Override
	public void actionPerformed(ActionEvent e)
		{
		Object s = e.getSource();
		
		if (s == bSave)
			{
			//			commitChanges();
			setVisible(false);
			return;
			}

		if (s == bDiscard)
			{
			//			setComponents(LGM.currentFile.gameSettings);
			setVisible(false);
			return;
			}

		if (s == bDefinitions || s == bGlobalLocals || s == bInitialization || s == bCleanup)
			{
			
			}
		}
	}
