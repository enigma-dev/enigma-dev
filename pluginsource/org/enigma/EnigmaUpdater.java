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
import org.tmatesoft.svn.core.wc.ISVNStatusHandler;
import org.tmatesoft.svn.core.wc.SVNClientManager;
import org.tmatesoft.svn.core.wc.SVNRevision;
import org.tmatesoft.svn.core.wc.SVNStatus;
import org.tmatesoft.svn.core.wc.SVNUpdateClient;
import org.tmatesoft.svn.core.wc.SVNWCClient;
import org.tmatesoft.svn.core.wc.SVNWCUtil;

//TODO: Thread
public class EnigmaUpdater
	{
	public static final String svn = "https://enigma-dev.svn.sourceforge.net/svnroot/enigma-dev/";
	public static final boolean SUBFOLDER = false;
	public static boolean revert;

	private SVNClientManager cliMan = null;
	/** Path to the working copy */
	private File path = null;

	public static boolean checkForUpdates()
		{
		EnigmaUpdater svn = new EnigmaUpdater();
		try
			{
			if (svn.needsCheckout())
				{
				String repo = askCheckout();
				if (repo != null)
					{
					svn.checkout(repo);
					if (revert) svn.revert();
					}
				return true;
				}
			if (svn.needsUpdate())
				{
				if (JOptionPane.showConfirmDialog(
						null,
						"Enigma has detected that newer libraries may exist. Would you like us to fetch these for you?",
						"Update",JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) svn.update();
				return true;
				}
			}
		catch (SVNException e)
			{
			showUpdateError(e);
			}
		return false;
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

	public static String askCheckout()
		{
		JRadioButton stable, testing, trunk;
		JCheckBox noRevert;

		Box box = new Box(BoxLayout.Y_AXIS);

		//		JPanel p = new JPanel();
		//		p.setLayout(new BoxLayout(p,));

		JLabel l = new JLabel(
				"Enigma is missing libraries.\nWould you like us to fetch these libraries for you?");
		l.setAlignmentX(JLabel.CENTER_ALIGNMENT);
		box.add(l);

		Box p2 = new Box(BoxLayout.X_AXIS);
		box.setAlignmentX(Box.CENTER_ALIGNMENT);
		box.add(p2);

		ButtonGroup bg = new ButtonGroup();
		stable = new JRadioButton("Stable",true);
		bg.add(stable);
		p2.add(stable);
		testing = new JRadioButton("Testing");
		bg.add(testing);
		p2.add(testing);
		trunk = new JRadioButton("Dev Trunk");
		bg.add(trunk);
		p2.add(trunk);

		noRevert = new JCheckBox("I'm a dev, don't touch my changes.");
		noRevert.setAlignmentX(JCheckBox.CENTER_ALIGNMENT);
		box.add(noRevert);

		if (JOptionPane.showConfirmDialog(null,box,"Checkout",JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION)
			return null;

		revert = !noRevert.isSelected();
		if (stable.isSelected()) return svn + "tags/update-stable";
		if (testing.isSelected()) return svn + "tags/update-test";
		return svn + "trunk";
		}

	private boolean needsCheckout()
		{
		return !SVNWCUtil.isVersionedDirectory(path);
		}

	private boolean needsUpdate() throws SVNException
		{
		final ArrayList<SVNStatus> changes = new ArrayList<SVNStatus>();
		ISVNStatusHandler ish = new ISVNStatusHandler()
			{
				public void handleStatus(SVNStatus status) throws SVNException
					{
					changes.add(status);
					}
			};
		/*long revision = */cliMan.getStatusClient().doStatus(path,SVNRevision.HEAD,SVNDepth.UNKNOWN,
				true,false,false,false,ish,null);

		return !changes.isEmpty();
		}

	/**
	 * This is debug code that checks out a given revision.
	 * This allows me to test that update works.
	 * @param rev - the revision to check out
	 * @throws SVNException
	 */
	@SuppressWarnings("unused")
	private void checkoutRev(String trunk, int rev) throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(trunk);
		long r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.create(rev),SVNDepth.INFINITY,
				true);
		System.out.println("Checked out " + r);
		}

	private void checkout(String trunk) throws SVNException
		{
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(trunk);
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
