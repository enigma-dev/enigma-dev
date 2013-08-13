/*
 * Copyright (C) 2008, 2009 IsmAvatar <IsmAvatar@gmail.com>
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

package org.enigma.frames;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JFrame;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;

import org.enigma.backend.EnigmaCallbacks.OutputHandler;
import org.enigma.messages.Messages;
import org.lateralgm.main.LGM;

public class ProgressFrame extends JFrame implements OutputHandler
	{
	private static final long serialVersionUID = 1L;
	protected JTextPane ta;
	protected JProgressBar pb;
	private static final SimpleAttributeSet ORANGE = new SimpleAttributeSet();
	private static final SimpleAttributeSet RED = new SimpleAttributeSet();

	static
		{
		//because Color.ORANGE looks like it was done by Mark Rothko
		StyleConstants.setForeground(ORANGE,new Color(255,128,0));
		StyleConstants.setForeground(RED,Color.RED);
		}

	public AbstractAction aCopy = new AbstractAction("COPY")
	{
		private static final long serialVersionUID = 1L;

		/** @see AbstractAction#actionPerformed(ActionEvent) */
	//r@Override
		public void actionPerformed(ActionEvent e)
		{
			ta.copy();
		}
	};
	
	public AbstractAction aSelAll = new AbstractAction("SELALL")
	{
		private static final long serialVersionUID = 1L;

		/** @see AbstractAction#actionPerformed(ActionEvent) */
	//r@Override
		public void actionPerformed(ActionEvent e)
		{
			ta.selectAll();
		}
	};
	
	private static JMenuItem makeContextButton(Action a)
	{
	  String key = "EnigmaPlugin." + a.getValue(Action.NAME);
	  JMenuItem b = new JMenuItem();
	  b.setIcon(LGM.getIconForKey(key));
	  b.setText(Messages.getString(key));
	  b.setRequestFocusEnabled(false);
	  b.addActionListener(a);
		return b;
	}
	
	public ProgressFrame()
		{
		super(Messages.getString("EnigmaFrame.TITLE")); //$NON-NLS-1$
		this.setIconImage(LGM.getIconForKey("EnigmaPlugin.COMPILE").getImage());
		setLocationRelativeTo(LGM.frame);
		JPanel p = new JPanel(new BorderLayout());
		ta = new JTextPane();
		ta.setEditable(false);
		ta.setPreferredSize(new Dimension(440,150));
		p.add(new JScrollPane(ta,JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED),BorderLayout.CENTER);
		pb = new JProgressBar();
		pb.setStringPainted(true);
		pb.setString(Messages.getString("EnigmaFrame.STARTING")); //$NON-NLS-1$
		p.add(pb,BorderLayout.SOUTH);

	    // build popup menu
	    final JPopupMenu popup = new JPopupMenu();
	    

		popup.add(makeContextButton(aCopy));
		popup.addSeparator();
		popup.add(makeContextButton(aSelAll));
			
	    ta.setComponentPopupMenu(popup);
	    ta.addMouseListener(new MouseAdapter() {

	    	@Override
	    	public void mousePressed(MouseEvent e) {
	        showPopup(e);
	    	}

	    	@Override
	    	public void mouseReleased(MouseEvent e) {
	        showPopup(e);
	    	}

	    	private void showPopup(MouseEvent e) {
	    		if (e.isPopupTrigger()) {
	    			popup.show(e.getComponent(), e.getX(), e.getY());
	    		}
	    	}
		});
		
		setContentPane(p);
		pack();
		setLocationRelativeTo(null);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		}

	public void append(String text)
		{
		StyledDocument doc = ta.getStyledDocument();
		//assuming they actually pass us a full
		//warning/error string, this will highlight it
		AttributeSet style = null;
		String lower = text.toLowerCase();
		if (lower.startsWith("warning:"))
			{
			style = ORANGE;
			postWarning(text);
			}
		if (lower.startsWith("error:"))
			{
			style = RED;
			postError(text);
			}
		//do the actual append
		try
			{
			doc.insertString(doc.getLength(),text,style);
			}
		catch (BadLocationException e)
			{ //This can never happen (also, JTextArea does this)
			}
		ta.setCaretPosition(doc.getLength());
		}

	public void postWarning(String text)
		{
		//TODO: Register warnings to a new panel
		}

	public void postError(String text)
		{
		//TODO: Register errors to a new panel?
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

	public void close()
		{
		dispose();
		}

	@Override
	public void tip(String tip)
		{
		pb.setString(tip);
		}

	public void progress(int pos, String tip)
		{
		progress(pos,tip,tip);
		}

	public void progress(int pos, String tip, String text)
		{
		pb.setValue(pos);
		pb.setString(tip);
		append(text + '\n'); //\n is internal representation of newlines, according to java
		}
	}
