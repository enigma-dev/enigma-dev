/*
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.file;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;

import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileView;

import org.enigma.messages.Messages;
import org.lateralgm.components.impl.CustomFileFilter;
import org.lateralgm.components.impl.ResNode;
import org.lateralgm.file.GmFile;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.file.GmFile.FormatFlavor;
import org.lateralgm.main.FileChooser.FileReader;
import org.lateralgm.main.FileChooser.FileWriter;
import org.lateralgm.main.FileChooser.GroupFilter;

//TODO: FileView
public class EgmIO extends FileView implements FileReader,FileWriter,GroupFilter
	{
	static final String ext = ".egm"; //$NON-NLS-1$
	CustomFileFilter filter = new CustomFileFilter(
			Messages.getString("EnigmaRunner.FORMAT_READER"),ext); //$NON-NLS-1$

	public FileFilter getGroupFilter()
		{
		return filter;
		}

	public FileFilter[] getFilters()
		{
		return new FileFilter[0];
		}

	//Reader
	public boolean canRead(URI uri)
		{
		return filter.accept(new File(uri));
		}

	public GmFile read(InputStream in, URI uri, ResNode root) throws GmFormatException
		{
		return EFileReader.readEgmFile(new File(uri),root,true);
		}

	//Writer
	@Override
	public String getExtension()
		{
		return ext;
		}

	@Override
	public String getSelectionName()
		{
		return "EGM";
		}

	@Override
	public void write(OutputStream out, GmFile gf, ResNode root) throws IOException
		{
		EFileWriter.writeEgmZipFile(out,gf,root);
		}

	@Override
	public FormatFlavor getFlavor()
		{
		return EFileWriter.FLAVOR_EGM;
		}
	}
