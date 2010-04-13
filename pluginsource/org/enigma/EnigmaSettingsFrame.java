package org.enigma;

import static java.lang.Integer.MAX_VALUE;
import static javax.swing.GroupLayout.DEFAULT_SIZE;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTabbedPane;
import javax.swing.LayoutStyle.ComponentPlacement;

import org.lateralgm.components.mdi.MDIFrame;
import org.lateralgm.main.LGM;

public class EnigmaSettingsFrame extends MDIFrame implements ActionListener
	{
	private static final long serialVersionUID = 1L;

	public JTabbedPane tabbedPane = new JTabbedPane();

	public JButton saveButton, discardButton;

	public EnigmaSettingsFrame()
		{
		super("Enigma Settings",true,true,true,true); //$NON-NLS-1$
		setDefaultCloseOperation(HIDE_ON_CLOSE);
		setFrameIcon(LGM.findIcon("restree/gm.png")); //$NON-NLS-1$
		GroupLayout layout = new GroupLayout(getContentPane());
		layout.setAutoCreateGaps(true);
		setLayout(layout);
		setResizable(false);

		/*
		 //We'll use DWORDS for speed and simplicity
		struct ENIGMAsettings
		{
		//Compatibility / Progess options
		int cppStrings; // Defines what language strings are inherited from. 0 = GML, 1 = C++
		int cppOperators; // Defines what language operators ++ and -- are inherited from. 0 = GML, 1 = C++
		int literalHandling; // Determines how literals are treated. 0 = enigma::variant, 1 = C++-scalar
		int structHandling;  // Defines behavior of the closing brace of struct {}. 0 = ISO C, 1 = Implied semicolon
		
		//Advanced options
		int instanceTypes;   // Defines how to represent instances. 0 = Integer, 1 = Pointer
		int storageClass;    // Determines how instances are stored in memory. 0 = Map, 1 = List, 2 = Array
		};


		Definitions
		Initialization
		Cleanup
		Global Locals
		 */

		JPanel compatPane = new JPanel();
		compatPane.setBorder(BorderFactory.createTitledBorder("Compatability/Progress"));

		JLabel l1 = new JLabel("Inherit strings from: ");
		ButtonGroup strings = new ButtonGroup();
		JRadioButton stringsGML = new JRadioButton("GML (#)");
		JRadioButton stringsCPP = new JRadioButton("C++ (\\n)");
		strings.add(stringsGML);
		strings.add(stringsCPP);
		JLabel l2 = new JLabel("Inherit ++/-- from: ");
		JLabel l3 = new JLabel("Treat literals as: ");
		JLabel l4 = new JLabel("Closing brace of struct: ");

		//		tabbedPane.setLayout(new BoxLayout(tabbedPane,BoxLayout.PAGE_AXIS));

		//		buildTabs();

		saveButton = new JButton("Save");
		saveButton.addActionListener(this);
		discardButton = new JButton("Discard");
		discardButton.addActionListener(this);

		layout.setHorizontalGroup(layout.createParallelGroup()
		/**/.addComponent(tabbedPane)
		/**/.addGroup(layout.createSequentialGroup()
		/*		*/.addContainerGap()
		/*		*/.addComponent(saveButton,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addComponent(discardButton,DEFAULT_SIZE,DEFAULT_SIZE,MAX_VALUE)
		/*		*/.addContainerGap()));
		layout.setVerticalGroup(layout.createSequentialGroup()
		/**/.addComponent(tabbedPane)
		/**/.addPreferredGap(ComponentPlacement.UNRELATED)
		/**/.addGroup(layout.createParallelGroup()
		/*		*/.addComponent(saveButton)
		/*		*/.addComponent(discardButton))
		/**/.addContainerGap());
		pack();
		}

	private void buildTabs()
		{
		//		JComponent pane = makeSettingsPane();
		//		tabbedPane.addTab("Settings",pane);
		//		tabbedPane.setMnemonicAt(0,KeyEvent.VK_1);
		}

	@Override
	public void actionPerformed(ActionEvent e)
		{
		if (e.getSource() == saveButton)
			{
			//			commitChanges();
			setVisible(false);
			return;
			}

		if (e.getSource() == discardButton)
			{
			//			setComponents(LGM.currentFile.gameSettings);
			setVisible(false);
			return;
			}
		}
	}
