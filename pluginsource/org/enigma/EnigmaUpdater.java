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
import org.tmatesoft.svn.core.wc.SVNWCClient;
import org.tmatesoft.svn.core.wc.SVNWCUtil;

//TODO: Thread
public class EnigmaUpdater
	{
	public static final String svn = "https://enigma-dev.svn.sourceforge.net/svnroot/enigma-dev/";
	public static final String svn_trunk = svn + "tags/stable-update";
	public static final boolean SUBFOLDER = false;

	public static void checkForUpdates()
		{
		EnigmaUpdater svn = new EnigmaUpdater();
		try
			{
			if (svn.needsCheckout())
				{
				if (JOptionPane.showConfirmDialog(null,
						"Enigma is missing libraries. Would you like us to fetch these libraries for you?",
						"Checkout",JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION)
					{
					svn.checkout();
					svn.revert();
					}
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
			showUpdateError(e);
			}
		}

	private EnigmaUpdater()
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

	private SVNClientManager cliMan = null;
	private File path = null;

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

	/**
	 * This is debug code that checks out a given revision.
	 * This allows me to test that update works.
	 * @param rev - the revision to check out
	 * @throws SVNException
	 */
	@SuppressWarnings("unused")
	private void checkoutRev(int rev) throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(svn_trunk);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.create(rev),SVNDepth.INFINITY,
				true);
		System.out.println("Checked out " + r);
		}

	private void checkout() throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(svn_trunk);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.HEAD,SVNDepth.INFINITY,true);
		System.out.println("Checked out " + r);
		}

	private void revert() throws SVNException
		{
		SVNWCClient wcCli = cliMan.getWCClient();
		wcCli.doRevert(new File[] { path },SVNDepth.INFINITY,null);
		System.out.println("Reverted");
		}

	private void update() throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
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
