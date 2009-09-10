package org.enigma;

import java.io.File;
import java.util.Collection;
import java.util.Iterator;

import javax.swing.JOptionPane;

import org.lateralgm.components.ErrorDialog;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.main.LGM;
import org.lateralgm.messages.Messages;
import org.tmatesoft.svn.core.SVNDepth;
import org.tmatesoft.svn.core.SVNException;
import org.tmatesoft.svn.core.SVNLogEntry;
import org.tmatesoft.svn.core.SVNURL;
import org.tmatesoft.svn.core.internal.io.dav.DAVRepositoryFactory;
import org.tmatesoft.svn.core.io.SVNRepository;
import org.tmatesoft.svn.core.io.SVNRepositoryFactory;
import org.tmatesoft.svn.core.wc.SVNClientManager;
import org.tmatesoft.svn.core.wc.SVNRevision;
import org.tmatesoft.svn.core.wc.SVNStatus;
import org.tmatesoft.svn.core.wc.SVNStatusType;
import org.tmatesoft.svn.core.wc.SVNUpdateClient;
import org.tmatesoft.svn.core.wc.SVNWCUtil;

public class EnigmaUpdater
	{
	final static String server = "http://www.enigma-dav.org/update5";
	final static String svn = "https://enigma-dev.svn.sourceforge.net/svnroot/enigma-dev";
	final static int version = 1;

	public EnigmaUpdater()
		{
		if (path == null)
			{
			path = LGM.workDir.getParentFile();
			if (SUBFOLDER) path = new File(path,"enigma");
			}

		if (cliMan == null)
			{
			DAVRepositoryFactory.setup();
			cliMan = SVNClientManager.newInstance();
			}
		}

	final static boolean SUBFOLDER = true;

	SVNClientManager cliMan = null;
	File path = null;

	private boolean needsCheckout()
		{
		return !SVNWCUtil.isVersionedDirectory(path);
		}

	private boolean needsUpdate() throws SVNException
		{
		SVNStatus stat = cliMan.getStatusClient().doStatus(path,true);
		SVNStatusType st = stat.getRemoteContentsStatus();
		System.out.println(st);
		return stat.getRemoteContentsStatus() != SVNStatusType.STATUS_NONE;
		}

	private void checkout() throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(svn);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.HEAD,SVNDepth.INFINITY,true);
		System.out.println("Checked out " + r);
		}

	private void update() throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		long r = upCli.doUpdate(path,SVNRevision.HEAD,SVNDepth.INFINITY,true,false);
		System.out.println("Updated to " + r);
		}

	private static void svnLog() throws SVNException
		{
		SVNURL url = SVNURL.parseURIDecoded(svn);
		SVNRepository repository = SVNRepositoryFactory.create(url,null);

		Collection<?> entries = repository.log(null,null,0,-1,true,true);

		Iterator<?> i = entries.iterator();
		while (i.hasNext())
			{
			SVNLogEntry sle = (SVNLogEntry) i.next();
			System.out.format("R%d by %s - %s\n",sle.getRevision(),sle.getAuthor(),sle.getMessage());
			}
		repository.closeSession();
		}

	public static void main(String[] args)
		{
		EnigmaUpdater svn = new EnigmaUpdater();
		try
			{
			if (svn.needsCheckout())
				{
				if (JOptionPane.showConfirmDialog(null,
						"Enigma is missing libraries. Would you like us to fetch these libraries for you?",
						"Checkout",JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) svn.checkout();
				return;
				}
			if (svn.needsUpdate())
				{
				if (JOptionPane.showConfirmDialog(
						null,
						"Enigma has detected that newer libraries may exist. Would you like us to fetch these for you?",
						"Update",JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) svn.update();
				return;
				}
			}
		catch (SVNException e)
			{
			showError(e);
			}
		}

	public static void showError(Exception e)
		{
		showError(new GmFormatException(null,e));
		}

	public static void showError(GmFormatException e)
		{
		new ErrorDialog(
				null,
				"Unable to Update Enigma",
				"The Enigma Auto-Updater was unable to run due to some miscommunication with the server.\n"
						+ "This may be temporary, or you may be required to visit http://www.enigma-dev.org\n"
						+ "to find out what's going on or to get a more up-to-date version of the Lgm-Enigma plugin.",
				Messages.format("Listener.DEBUG_INFO", //$NON-NLS-1$
						e.getClass().getName(),e.getMessage(),e.stackAsString())).setVisible(true);
		}
	}
