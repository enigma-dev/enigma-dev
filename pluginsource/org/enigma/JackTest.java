package org.enigma;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.text.JTextComponent;

public class JackTest
	{
	static JTextComponent t1 = new JAckOff("Hello world");
	static JTextComponent t2 = new JTextArea("Hello world");

	public static void main(String[] args)
		{
		JFrame f = new JFrame();
		JTabbedPane tabs = new JTabbedPane();
		tabs.add("JAckOff",new JScrollPane(t1));
		tabs.add("JTextArea",new JScrollPane(t2));
		f.setContentPane(tabs);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setSize(400,400);
		f.setLocationRelativeTo(null);
		f.setVisible(true);
		}
	}
