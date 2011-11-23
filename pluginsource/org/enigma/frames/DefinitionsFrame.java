package org.enigma.frames;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;

import javax.swing.BoxLayout;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;

import org.enigma.backend.Definitions;
import org.enigma.backend.Definitions.PDefinitions;
import org.lateralgm.components.GMLTextArea;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.components.impl.TextAreaFocusTraversalPolicy;
import org.lateralgm.subframes.InstantiableResourceFrame;
import org.lateralgm.ui.swing.propertylink.ComboBoxLink.IndexComboBoxConversion;

public class DefinitionsFrame extends InstantiableResourceFrame<Definitions,PDefinitions>
	{
	private static final long serialVersionUID = 1L;
	public JToolBar tool;
	public JComboBox kinds;
	public GMLTextArea code;
	public JPanel status;

	public DefinitionsFrame(Definitions res, ResNode node)
		{
		super(res,node);
		setSize(600,400);
		setLayout(new BorderLayout());

		// Setup the toolbar
		tool = new JToolBar();
		tool.setFloatable(false);
		tool.setAlignmentX(0);
		add(tool,BorderLayout.NORTH);

		tool.add(save);
		tool.addSeparator();

		code = new GMLTextArea((String) res.get(PDefinitions.CODE));
		add(code,BorderLayout.CENTER);
		code.addEditorButtons(tool);

		tool.addSeparator();
		String kindNames[] = { "Definitions","Global Locals","Initialization","Cleanup" };
		tool.add(kinds = new JComboBox(kindNames));
		kinds.setMaximumSize(new Dimension(120,25));
		plf.make(kinds,PDefinitions.KIND,new IndexComboBoxConversion());

		tool.addSeparator();
		name.setColumns(5);
		name.setMaximumSize(name.getPreferredSize());
		tool.add(new JLabel("Name: "));
		tool.add(name);

		status = new JPanel(new FlowLayout());
		status.setLayout(new BoxLayout(status,BoxLayout.X_AXIS));
		status.setMaximumSize(new Dimension(Integer.MAX_VALUE,11));
		final JLabel caretPos = new JLabel();
		status.add(caretPos);
		CaretListener cl = new CaretListener()
			{
				public void caretUpdate(CaretEvent e)
					{
					caretPos.setText((code.getCaretLine() + 1) + ":" + (code.getCaretColumn() + 1));
					}
			};
		code.addCaretListener(cl);
		cl.caretUpdate(null);
		add(status,BorderLayout.SOUTH);

		setFocusTraversalPolicy(new TextAreaFocusTraversalPolicy(code));
		}

	@Override
	public void commitChanges()
		{
		res.put(PDefinitions.CODE,code.getTextCompat());
		res.setName(name.getText());
		}
	}
