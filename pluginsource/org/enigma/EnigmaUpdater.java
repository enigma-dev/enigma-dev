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

import java.io.File;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JRadioButton;

import org.enigma.frames.ProgressFrame;
import org.enigma.messages.Messages;
import org.lateralgm.components.ErrorDialog;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.main.LGM;
import org.tmatesoft.svn.core.SVNCancelException;
import org.tmatesoft.svn.core.SVNDepth;
import org.tmatesoft.svn.core.SVNException;
import org.tmatesoft.svn.core.SVNLogEntry;
import org.tmatesoft.svn.core.SVNURL;
import org.tmatesoft.svn.core.internal.io.dav.DAVRepositoryFactory;
import org.tmatesoft.svn.core.io.SVNRepository;
import org.tmatesoft.svn.core.io.SVNRepositoryFactory;
import org.tmatesoft.svn.core.wc.ISVNEventHandler;
import org.tmatesoft.svn.core.wc.ISVNStatusHandler;
import org.tmatesoft.svn.core.wc.SVNBasicClient;
import org.tmatesoft.svn.core.wc.SVNClientManager;
import org.tmatesoft.svn.core.wc.SVNEvent;
import org.tmatesoft.svn.core.wc.SVNEventAction;
import org.tmatesoft.svn.core.wc.SVNRevision;
import org.tmatesoft.svn.core.wc.SVNStatus;
import org.tmatesoft.svn.core.wc.SVNStatusType;
import org.tmatesoft.svn.core.wc.SVNUpdateClient;
import org.tmatesoft.svn.core.wc.SVNWCClient;
import org.tmatesoft.svn.core.wc.SVNWCUtil;

public class EnigmaUpdater
	{
	public static final String svn = "https://enigma-dev.svn.sourceforge.net/svnroot/enigma-dev/"; //$NON-NLS-1$
	public static final boolean SUBFOLDER = false;
	public static boolean revert;

	private SVNClientManager cliMan = null;
	/** Path to the working copy */
	private File path = null;

	/** Returns -2 on error, -1 on aborted checkout, 0 on no update, 1 on finished update */
	public static int checkForUpdates(final ProgressFrame ef)
		{
		EnigmaUpdater svn = new EnigmaUpdater();
		try
			{
			if (svn.needsCheckout())
				{
				String repo = askCheckout();
				if (repo == null) return -1;
				ef.append(Messages.getString("EnigmaUpdater.DO_UPDATE") + '\n'); //$NON-NLS-1$
				ef.open();

				final File me = getThisFile();
				svn.checkout(repo,new ISVNEventHandler()
					{
						@Override
						public void handleEvent(SVNEvent event, double progress) throws SVNException
							{
							if (event.getFile().equals(LGM.workDir) || event.getFile().equals(me))
								needsRestart = true;
							ef.append(event.getAction() + " " + event.getFile() + '\n'); //$NON-NLS-1$
							}

						@Override
						public void checkCancelled() throws SVNCancelException
							{
							if (EnigmaRunner.SHUTDOWN) throw new SVNCancelException();
							}
					});
				if (revert) svn.revert();
				return 1;
				}
			svn.cleanup();
			long lrev = svn.workingRev();
			long rev = svn.needsUpdate();
			if (rev != -1L)
				{
				String title = "Update" + (lrev != -1 ? " from r" + lrev : "") + " to r" + rev;
				if (JOptionPane.showConfirmDialog(null,Messages.getString("EnigmaUpdater.ASK_UPDATE"), //$NON-NLS-1$
						title,JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) svn.update();
				System.out.println("ENIGMA r" + rev);
				return 1;
				}
			System.out.println("ENIGMA r" + lrev);
			}
		catch (SVNCancelException e)
			{
			System.out.println(Messages.getString("EnigmaUpdater.CHECKOUT_CANCELLED")); //$NON-NLS-1$
			return -1;
			}
		catch (SVNException e)
			{
			showUpdateError(e);
			return -2;
			}
		return 0;
		}

	private EnigmaUpdater()
		{
		if (path == null)
			{
			path = EnigmaRunner.WORKDIR;
			if (SUBFOLDER) path = new File(path,"enigma"); //$NON-NLS-1$
			}

		if (cliMan == null)
			{
			DAVRepositoryFactory.setup();
			cliMan = SVNClientManager.newInstance();
			}
		}

	public static String askCheckout()
		{
		JRadioButton stable, testing, trunk;
		JCheckBox noRevert;

		Box box = new Box(BoxLayout.Y_AXIS);

		//		JPanel p = new JPanel();
		//		p.setLayout(new BoxLayout(p,));

		JLabel l = new JLabel(Messages.getString("EnigmaUpdater.ASK_CHECKOUT")); //$NON-NLS-1$
		l.setAlignmentX(JLabel.CENTER_ALIGNMENT);
		box.add(l);

		Box p2 = new Box(BoxLayout.X_AXIS);
		box.setAlignmentX(Box.CENTER_ALIGNMENT);
		box.add(p2);

		ButtonGroup bg = new ButtonGroup();
		stable = new JRadioButton(Messages.getString("EnigmaUpdater.CO_STABLE"),true); //$NON-NLS-1$
		bg.add(stable);
		p2.add(stable);
		testing = new JRadioButton(Messages.getString("EnigmaUpdater.CO_TESTING")); //$NON-NLS-1$
		bg.add(testing);
		p2.add(testing);
		trunk = new JRadioButton(Messages.getString("EnigmaUpdater.CO_DEV_TRUNK")); //$NON-NLS-1$
		bg.add(trunk);
		p2.add(trunk);

		noRevert = new JCheckBox(Messages.getString("EnigmaUpdater.CO_NO_REVERT")); //$NON-NLS-1$
		noRevert.setAlignmentX(JCheckBox.CENTER_ALIGNMENT);
		box.add(noRevert);

		String title = Messages.getString("EnigmaUpdater.ASK_CHECKOUT_TITLE"); //$NON-NLS-1$
		if (JOptionPane.showConfirmDialog(null,box,title,JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION)
			return null;

		revert = !noRevert.isSelected();
		if (stable.isSelected()) return svn + "tags/update-stable"; //$NON-NLS-1$
		if (testing.isSelected()) return svn + "tags/update-test"; //$NON-NLS-1$
		return svn + "trunk"; //$NON-NLS-1$
		}

	private boolean needsCheckout()
		{
		return !SVNWCUtil.isVersionedDirectory(path);
		}

	private long needsUpdate() throws SVNException
		{
		final ArrayList<SVNStatus> changes = new ArrayList<SVNStatus>();
		ISVNStatusHandler ish = new ISVNStatusHandler()
			{
				public void handleStatus(SVNStatus status) throws SVNException
					{
					SVNStatusType type = status.getContentsStatus();
					if (type == SVNStatusType.STATUS_UNVERSIONED) return;
					System.out.println(status.getContentsStatus() + " " + status.getFile()); //$NON-NLS-1$
					changes.add(status);
					}
			};
		long revision = cliMan.getStatusClient().doStatus(path,SVNRevision.HEAD,SVNDepth.UNKNOWN,true,
				false,false,false,ish,null);

		return !changes.isEmpty() ? revision : -1L;
		}

	private long workingRev()
		{
		try
			{
			SVNStatus status = cliMan.getStatusClient().doStatus(path,false);
			return status.getRevision().getNumber();
			}
		catch (SVNException e)
			{
			return -1L;
			}
		}

	/**
	 * This is debug code that checks out a given revision.
	 * This allows me to test that update works.
	 * @param rev - the revision to check out
	 * @throws SVNException
	 */
	@SuppressWarnings("unused")
	private void checkoutRev(String repo, int rev) throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(repo);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.create(rev),SVNDepth.INFINITY,
				true);
		System.out.println("Checked out " + r);
		}

	private void checkout(String repo, ISVNEventHandler eventHandler) throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(repo);
		upCli.setEventHandler(eventHandler);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.HEAD,SVNDepth.INFINITY,true);
		System.out.println("Checked out " + r);
		}

	private static File getThisFile()
		{
		try
			{
			return new File(
					EnigmaRunner.class.getProtectionDomain().getCodeSource().getLocation().toURI());
			}
		catch (Exception e)
			{
			e.printStackTrace();
			}
		return null;
		}

	private static void listenForChangesRequiringRestart(SVNBasicClient cli)
		{
		final File me = getThisFile();
		cli.setEventHandler(new ISVNEventHandler()
			{
				@Override
				public void checkCancelled() throws SVNCancelException
					{
					}

				@Override
				public void handleEvent(SVNEvent event, double progress) throws SVNException
					{
					SVNEventAction act = event.getAction();
					if (act.equals(SVNEventAction.UPDATE_UPDATE))
						if (event.getFile().equals(LGM.workDir) || event.getFile().equals(me))
							needsRestart = true;
					}
			});
		}

	/** Releases locks and finished unfinished operations. Idempotent (calling when unneeded has no effect) */
	private void cleanup() throws SVNException
		{
		SVNWCClient wcc = cliMan.getWCClient();
		listenForChangesRequiringRestart(wcc);
		wcc.doCleanup(path);
		}

	private void revert() throws SVNException
		{
		SVNWCClient wcCli = cliMan.getWCClient();
		wcCli.doRevert(new File[] { path },SVNDepth.INFINITY,null);
		System.out.println("Reverted");
		}

	public static boolean needsRestart = false;

	private void update() throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		listenForChangesRequiringRestart(upCli);
		long r = upCli.doUpdate(path,SVNRevision.HEAD,SVNDepth.INFINITY,true,false);
		System.out.println("Updated to " + r);
		}

	/**
	 * This function will eventually be adapted
	 * to allow the user to review a list of
	 * changes since their working version
	 * @throws SVNException
	 */
	@SuppressWarnings("unused")
	private static void svnLog() throws SVNException
		{
		SVNURL url = SVNURL.parseURIDecoded(svn);
		SVNRepository repository = SVNRepositoryFactory.create(url,null);

		Collection<?> entries = repository.log(null,null,0,-1,true,true);

		Iterator<?> i = entries.iterator();
		while (i.hasNext())
			{
			SVNLogEntry sle = (SVNLogEntry) i.next();
			System.out.format("r%d by %s - %s\n",sle.getRevision(),sle.getAuthor(),sle.getMessage());
			}
		repository.closeSession();
		}

	public static void showUpdateError(Exception e)
		{
		showUpdateError(new GmFormatException(null,e));
		}

	public static void showUpdateError(GmFormatException e)
		{
		new ErrorDialog(null,Messages.getString("EnigmaUpdater.ERROR_TITLE"), //$NON-NLS-1$
				Messages.getString("EnigmaUpdater.ERROR_MISCOMMUNICATION"),e).setVisible(true); //$NON-NLS-1$
		}
	}
