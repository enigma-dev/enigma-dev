package org.enigma;

import java.awt.BorderLayout;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class EnigmaFrame extends JFrame
	{
	private static final long serialVersionUID = 1L;
	public JTextArea ta;
	public JProgressBar pb;

	public EnigmaFrame()
		{
		super("Enigma Progress Console");
		JPanel p = new JPanel(new BorderLayout());
		ta = new JTextArea(10,40);
		ta.setEditable(false);
		p.add(new JScrollPane(ta,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED),"Center");
		pb = new JProgressBar();
		pb.setStringPainted(true);
		pb.setString("Starting");
		p.add(pb,"South");

		setContentPane(p);
		pack();
		setLocationRelativeTo(null);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setVisible(true);
		}

	void progress(int pos, String tip)
		{
		progress(pos,tip,tip);
		}

	void progress(int pos, String tip, String text)
		{
		pb.setValue(pos);
		pb.setString(tip);
		ta.append(text + "\n");
		}
	}
