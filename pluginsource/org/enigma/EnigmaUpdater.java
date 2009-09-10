package org.enigma;

import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Collection;
import java.util.Iterator;

import org.lateralgm.components.ErrorDialog;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmStreamDecoder;
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
import org.tmatesoft.svn.core.wc.SVNUpdateClient;

public class EnigmaUpdater
	{
	final static String server = "http://www.enigma-dav.org/update5";
	final static String svn = "https://enigma-dev.svn.sourceforge.net/svnroot/enigma-dev";
	final static int version = 1;

	public EnigmaUpdater()
		{
		try
			{
			oldFashionedUpdate(new URL(server));
			}
		catch (MalformedURLException e1)
			{
			e1.printStackTrace();
			return;
			}
		catch (GmFormatException e)
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

	final static boolean SUBFOLDER = false;
	final static boolean CHECKOUT = true;
	final static boolean UPDATE = true;

	private static void svnClientUpdate() throws SVNException
		{
		DAVRepositoryFactory.setup();
		SVNClientManager cliMan = SVNClientManager.newInstance();
		SVNUpdateClient upCli = cliMan.getUpdateClient();
		SVNURL url = SVNURL.parseURIDecoded(svn);
		File path = LGM.workDir.getParentFile();
		if (SUBFOLDER) path = new File(path,"enigma");
		long r = -1; //stores the resulting revision number
		if (CHECKOUT)
			{
			r = upCli.doCheckout(url,path,SVNRevision.HEAD,SVNRevision.HEAD,SVNDepth.INFINITY,true);
			System.out.println("Checked out " + r);
			}
		if (UPDATE)
			{
			r = upCli.doUpdate(path,SVNRevision.HEAD,SVNDepth.INFINITY,true,false);
			System.out.println("Updated to " + r);
			}
		}

	private static void svnLog() throws SVNException
		{
		DAVRepositoryFactory.setup();
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

	private static void oldFashionedUpdate(URL url) throws GmFormatException
		{
		GmStreamDecoder in = null;
		try
			{
			in = new GmStreamDecoder(url.openStream());
			if (in.read4() != 1146439525) //1146439525 = 'eGUD'
				throw new GmFormatException(null,
						"Update server not found. Found some other page. Maybe an error page.");
			int rversion = in.read4();
			if (rversion > version || true)
				throw new GmFormatException(null,"Incorrect update version. Expecting " + version
						+ ", found " + rversion);
			}
		catch (Exception e)
			{
			if ((e instanceof GmFormatException)) throw (GmFormatException) e;
			throw new GmFormatException(null,e);
			}
		finally
			{
			try
				{
				if (in != null)
					{
					in.close();
					in = null;
					}
				}
			catch (IOException ex)
				{
				String key = Messages.getString("GmFileReader.ERROR_CLOSEFAILED"); //$NON-NLS-1$
				throw new GmFormatException(null,key);
				}
			}
		}

	public static void main(String[] args)
		{
		//		new EnigmaUpdater();
		try
			{
			svnClientUpdate();
			}
		catch (SVNException e)
			{
			e.printStackTrace();
			}
		}
	}
