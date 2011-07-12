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

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import org.enigma.backend.EnigmaCallbacks.OutputHandler;
import org.enigma.messages.Messages;
import org.lateralgm.main.LGM;

public class EnigmaFrame extends JFrame implements OutputHandler
	{
	private static final long serialVersionUID = 1L;
	public JTextArea ta;
	public JProgressBar pb;

	public EnigmaFrame()
		{
		super(Messages.getString("EnigmaFrame.TITLE")); //$NON-NLS-1$
		setLocationRelativeTo(LGM.frame);
		JPanel p = new JPanel(new BorderLayout());
		ta = new JTextArea(10,40);
		ta.setEditable(false);
		p.add(new JScrollPane(ta,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED),BorderLayout.CENTER);
		pb = new JProgressBar();
		pb.setStringPainted(true);
		pb.setString(Messages.getString("EnigmaFrame.STARTING")); //$NON-NLS-1$
		p.add(pb,BorderLayout.SOUTH);

		setContentPane(p);
		pack();
		setLocationRelativeTo(null);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		}

	public void append(String text)
		{
		ta.append(text);
		ta.setCaretPosition(ta.getDocument().getLength());
		}

	public void progress(int pos)
		{
		pb.setValue(pos);
		}

	@Override
	public void clear()
		{
		ta.setText(null);
		}

	@Override
	public void open()
		{
		setVisible(true);
		}

	@Override
	public void tip(String tip)
		{
		pb.setString(tip);
		}

	void progress(int pos, String tip)
		{
		progress(pos,tip,tip);
		}

	void progress(int pos, String tip, String text)
		{
		pb.setValue(pos);
		pb.setString(tip);
		ta.append(text + '\n'); //\n is internal representation of newlines, according to java
		}
	}
