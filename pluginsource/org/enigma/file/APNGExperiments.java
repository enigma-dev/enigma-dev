/*
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
 * Copyright (C) 2011 IsmAvatar <IsmAvatar@gmail.com>
 * 
 * This file is part of Enigma Plugin.
 * Enigma Plugin is free software and comes with ABSOLUTELY NO WARRANTY.
 * See LICENSE for details.
 */

package org.enigma.file;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.zip.CRC32;

import javax.imageio.ImageIO;

/////////////////////////////////////////////////
// Below are experimental APNG support methods //
/////////////////////////////////////////////////
public class APNGExperiments
{
	static final byte[] PNG_SIGNATURE = { (byte) 0x89, 'P', 'N', 'G', 0x0D,
			0x0A, 0x1A, 0x0A };
	static final String IO_FMT = "png"; //$NON-NLS-1$
	
	static byte[] i2b(int i) {
		return new byte[] { (byte) ((i & 0xFF000000) >> 24),
				(byte) ((i & 0x00FF0000) >> 16),
				(byte) ((i & 0x0000FF00) >> 8), (byte) (i & 0x000000FF) };
	}
	
	static abstract class PNG_Chunk
	{
		int length = 0;
		byte[] chunkType;
		byte[] data;
		byte[] crc;
		
		void updateCRC() {
			CRC32 crc32 = new CRC32();
			crc32.update(chunkType);
			crc32.update(data);
			int tcrc = (int) crc32.getValue();
			length = data.length;
			crc = i2b(tcrc);
		}
		
		abstract void repopulate();
		
		byte[] getBytes() { // Get the bytes of this chunk for writing
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			try
			{
				baos.write(i2b(length));
				baos.write(chunkType);
				baos.write(data);
				baos.write(crc);
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
			return baos.toByteArray();
		}
		
		boolean istype(String t) {
			if (chunkType == null || t.length() != 4)
				return false;
			return chunkType[0] == t.charAt(0) && chunkType[1] == t.charAt(1)
					&& chunkType[2] == t.charAt(2)
					&& chunkType[3] == t.charAt(3);
		}
		
		boolean read(InputStream dis) throws IOException {
			int b1 = dis.read();
			if (b1 == -1)
				return false;
			length = (b1 << 24) | (dis.read() << 16) | (dis.read() << 8)
					| dis.read();
			chunkType = new byte[4];
			dis.read(chunkType);
			data = new byte[length];
			dis.read(data);
			crc = new byte[4];
			dis.read(crc);
			return true;
		}
	}
	
	static class Generic_Chunk extends PNG_Chunk
	{
		@Override
		void repopulate() {
			System.err.println("Repopulate called on generic chunk.");
		}
	}
	
	static class IHDR extends PNG_Chunk
	{
		int width, height;
		byte bitDepth, colType, compression, filter, interlace;
		
		public IHDR(int w, int h, byte bd, byte ct, byte cm, byte ft, byte il)
		{
			chunkType = new byte[] { 'I', 'H', 'D', 'R' };
			width = w;
			height = h;
			bitDepth = bd;
			colType = ct;
			compression = cm;
			filter = ft;
			interlace = il;
			repopulate();
		}
		
		public IHDR(byte[] png)
		{
			chunkType = new byte[] { 'I', 'H', 'D', 'R' };
			data = new byte[13];
			System.arraycopy(png, 16, data, 0, 13);
			updateCRC();
		}
		
		static class ColorType
		{
			public static final byte GRAY = 0;
			public static final byte RGB = 2;
			public static final byte PALETTE = 3;
			public static final byte GRAYA = 4;
			public static final byte RGBA = 6;
		}
		
		static class Compression
		{
			public static final byte DEFLATE = 0;
		}
		
		static class Filter
		{
			public static final byte ADAPTIVE = 0;
		}
		
		static class Interlace
		{
			public static final byte NONE = 0;
			public static final byte ADAM7 = 1;
		}
		
		public void repopulate() {
			data = new byte[] { (byte) (width & 0xFF000000),
					(byte) (width & 0x00FF0000), (byte) (width & 0x0000FF00),
					(byte) (width & 0x000000FF), (byte) (height & 0xFF000000),
					(byte) (height & 0x00FF0000), (byte) (height & 0x0000FF00),
					(byte) (height & 0x000000FF), bitDepth, colType,
					compression, filter, interlace };
			updateCRC();
		}
	}
	
	static class acTL extends PNG_Chunk
	{
		int num_frames;
		int num_plays;
		
		public acTL(int nf, int np)
		{
			chunkType = new byte[] { 'a', 'c', 'T', 'L' };
			num_frames = nf;
			num_plays = np;
			repopulate();
		}
		
		public void repopulate() {
			data = new byte[] { (byte) (num_frames & 0xFF0000),
					(byte) (num_frames & 0x00FF0000),
					(byte) (num_frames & 0x0000FF00),
					(byte) (num_frames & 0x000000FF),
					(byte) (num_plays & 0xFF000000),
					(byte) (num_plays & 0x00FF0000),
					(byte) (num_plays & 0x0000FF00),
					(byte) (num_plays & 0x000000FF) };
			updateCRC();
		}
	}
	
	static class fcTL extends PNG_Chunk
	{
		int sequence_number; // Sequence number of the animation chunk, 0-based
		int width; // Width of the following frame
		int height; // Height of the following frame
		int x_offset; // X position at which to render the following frame
		int y_offset; // Y position at which to render the following frame
		short delay_num; // Frame delay fraction numerator
		short delay_den; // Frame delay fraction denominator
		byte dispose_op; // Type of frame area disposal to be done after
		// rendering this frame
		byte blend_op; // Type of frame area rendering for this frame
		
		public fcTL(int sn, int w, int h, int xo, int yo, short dn, short dd,
				byte dispose_opr, byte blend_opr)
		{
			chunkType = new byte[] { 'f', 'c', 'T', 'L' };
			sequence_number = sn;
			width = w;
			height = h;
			x_offset = xo;
			y_offset = yo;
			delay_num = dn;
			delay_den = dd;
			dispose_op = dispose_opr;
			blend_op = blend_opr;
			repopulate();
		}
		
		static class Dispose
		{
			static final byte APNG_DISPOSE_OP_NONE = 0; // No disposal is done
			// on this frame before rendering the next; the contents of the
			// output buffer are left as-is.
			static final byte APNG_DISPOSE_OP_BACKGROUND = 1; // The frame's
			// region of the output buffer is to be cleared to fully transparent
			// black before rendering the next frame.
			static final byte APNG_DISPOSE_OP_PREVIOUS = 2; // The frame's
			// region of the output buffer is to be reverted to the previous
			// contents before rendering the next frame.
		}
		
		static class Blend
		{
			static final byte APNG_BLEND_OP_SOURCE = 0; // All color components
			// of the frame, including alpha, overwrite the current contents of
			// the frame's output buffer region.
			static final byte APNG_BLEND_OP_OVER = 1; // The frame should be
			// composited onto the output buffer based on its alpha, using a
			// simple OVER operation as described in the "Alpha Channel
			// Processing" section of the PNG specification [PNG-1.2].
		}
		
		@Override
		void repopulate() {
			data = new byte[] { (byte) (sequence_number & 0xFF000000),
					(byte) (sequence_number & 0x00FF0000),
					(byte) (sequence_number & 0x0000FF00),
					(byte) (sequence_number & 0x000000FF),
					(byte) (width & 0xFF000000), (byte) (width & 0x00FF0000),
					(byte) (width & 0x0000FF00), (byte) (width & 0x000000FF),
					(byte) (height & 0xFF000000), (byte) (height & 0x00FF0000),
					(byte) (height & 0x0000FF00), (byte) (height & 0x000000FF),
					(byte) (x_offset & 0xFF000000),
					(byte) (x_offset & 0x00FF0000),
					(byte) (x_offset & 0x0000FF00),
					(byte) (x_offset & 0x000000FF),
					(byte) (y_offset & 0xFF000000),
					(byte) (y_offset & 0x00FF0000),
					(byte) (y_offset & 0x0000FF00),
					(byte) (y_offset & 0x000000FF),
					(byte) (delay_num & 0xFF000000),
					(byte) (delay_num & 0x00FF0000),
					(byte) (delay_den & 0x0000FF00),
					(byte) (delay_den & 0x000000FF), dispose_op, blend_op };
			updateCRC();
		}
	}
	
	static class IDAT extends PNG_Chunk
	{
		public IDAT(byte[] png) throws IOException
		{
			// These are failsafes. They are both unnecessary.
			chunkType = new byte[] { 'I', 'D', 'A', 'T' };
			data = new byte[] {};
			
			ByteArrayInputStream dis = new ByteArrayInputStream(png);
			dis.skip(8);
			// Find our fdAT chunk in the PNG
			while (dis.available() >= 12)
			{
				read(dis);
				if (istype("IDAT"))
					break;
			}
			repopulate();
		}
		
		public IDAT(byte[] dat, boolean b)
		{
			chunkType = new byte[] { 'I', 'D', 'A', 'T' };
			data = dat;
			updateCRC();
		}

		@Override
		public void repopulate() {
			updateCRC();
		}
	}
	
	static class fdAT extends PNG_Chunk
	{
		int sequence_number; // Sequence number, 0-based
		byte[] frame_data;
		
		public fdAT(int sn, byte[] png) throws IOException
		{
			byte[] fcTLt = chunkType = new byte[] { 'f', 'd', 'A', 'T' };
			sequence_number = sn;
			frame_data = null;
			
			ByteArrayInputStream dis = new ByteArrayInputStream(png);
			dis.skip(8);
			// Find our fdAT chunk in the PNG
			while (read(dis))
			{
				if (istype("IDAT"))
				{
					chunkType = fcTLt;
					frame_data = data;
					data = null;
					break;
				}
			}
			if (frame_data == null)
			{
				System.err.println("FUCK.");
				System.exit(0);
			}
			repopulate();
		}
		
		// TODO: I just quick generated this method as a way to copy over the
		// PNG_Chunk data.
		// Review it, delete it, or do whatever.
		public fdAT(PNG_Chunk pc)
		{
			// Note, both original and new arrays reference the same memory.
			// E.g. changing pc.data also changes this.data, and vice versa.
			length = pc.length;
			chunkType = pc.chunkType;
			
			data = new byte[pc.data.length-4];
			System.arraycopy(pc.data,4,data,0,data.length);
			
			sequence_number = pc.data[0] << 24 | pc.data[1] << 16 | pc.data[2] << 8 | pc.data[3];
			crc = pc.crc;
		}
		
		@Override
		public void repopulate() {
			data = new byte[frame_data.length + 4];
			System.arraycopy(new byte[] {
					(byte) (sequence_number & 0xFF000000),
					(byte) (sequence_number & 0x00FF0000),
					(byte) (sequence_number & 0x0000FF00),
					(byte) (sequence_number & 0x000000FF) }, 0, data, 0, 4);
			System.arraycopy(frame_data, 0, data, 4, frame_data.length);
			updateCRC();
		}
		
		public IDAT toIDAT() {
			return new IDAT(data,true);
		}
	}
	
	static class IEND extends PNG_Chunk
	{
		IEND()
		{
			chunkType = new byte[] { 'I', 'E', 'N', 'D' };
			repopulate();
		}
		
		@Override
		public void repopulate() {
			data = new byte[] {};
			updateCRC();
		}
	}
	
	public static void imagesToApng(ArrayList<BufferedImage> imgs,
			OutputStream fullFile) throws IOException {
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		ImageIO.write(imgs.get(0), IO_FMT, baos);
		byte buf[] = baos.toByteArray();
		IHDR myhdr = new IHDR(buf);
		acTL myactl = new acTL(imgs.size(), 0);
		fcTL fctl = new fcTL(0, imgs.get(0).getWidth(),
				imgs.get(0).getHeight(), 0, 0, (short) 1, (short) 30,
				fcTL.Dispose.APNG_DISPOSE_OP_BACKGROUND,
				fcTL.Blend.APNG_BLEND_OP_OVER);
		IDAT mainidat = new IDAT(buf);
		
		fullFile.write(PNG_SIGNATURE);
		fullFile.write(myhdr.getBytes());
		fullFile.write(myactl.getBytes());
		fullFile.write(fctl.getBytes());
		fullFile.write(mainidat.getBytes());
		
		int indx = -1;
		for (BufferedImage bi : imgs)
		{
			if (indx++ == -1) // Continues once to skip the first image;
				continue; // index will be 1 on first iteration
				
			baos.reset(); // Clear our buffer,
			ImageIO.write(bi, IO_FMT, baos); // Fetch new image.
			
			// Create the frame control. IDs will be 2, 4, 6, 8...
			fctl = new fcTL(indx++, bi.getWidth(), bi.getHeight(), 0, 0,
					(short) 1, (short) 30,
					fcTL.Dispose.APNG_DISPOSE_OP_BACKGROUND,
					fcTL.Blend.APNG_BLEND_OP_OVER);
			fullFile.write(fctl.getBytes()); // Write it into the file
			
			// Now make our frame data. IDs will be 3, 5, 7, 9...
			fdAT fdat = new fdAT(indx, baos.toByteArray());
			fullFile.write(fdat.getBytes()); // Write it into the file
		}
		IEND iend = new IEND();
		fullFile.write(iend.getBytes());
	}
	
	public static ArrayList<BufferedImage> apngToBufferedImages(InputStream is) {
		PNG_Chunk genChunk = new Generic_Chunk();
		ByteArrayOutputStream png = new ByteArrayOutputStream();
		ArrayList<BufferedImage> ret = new ArrayList<BufferedImage>();
		try
		{
			png.write(PNG_SIGNATURE);
			byte pngb[] = new byte[8];
			
			is.read(pngb);
			if (!Arrays.equals(pngb, PNG_SIGNATURE))
				throw new IOException("Not APNG");
			while (genChunk.read(is))
			{
				if (genChunk.istype("acTL"))
				{
					// DISREGARD FEMALES
				}
				else if (genChunk.istype("fcTL"))
				{
					// ACQUIRE CURRENCY
				}
				else if (genChunk.istype("IDAT"))
				{
					pngb = png.toByteArray();
					png.write(genChunk.getBytes());
					png.write(new IEND().getBytes());
					
					byte[] readme = png.toByteArray();
					ByteArrayInputStream bais = new ByteArrayInputStream(
							readme, 0, readme.length);
					ret.add(ImageIO.read(bais));
					
					png.reset();
					png.write(pngb);
				}
				else if (genChunk.istype("fdAT"))
				{
					pngb = png.toByteArray();
					byte[] a = new fdAT(genChunk).toIDAT().getBytes();
					System.out.println(a.length);
					png.write(a);
					png.write(new IEND().getBytes());
					
					byte[] readme = png.toByteArray();
					ByteArrayInputStream bais = new ByteArrayInputStream(
							readme, 0, readme.length);
					ret.add(ImageIO.read(bais));
					
					png.reset();
					png.write(pngb);
				}
				else if (genChunk.istype("IEND"))
				{
					break;
				}
				else
				{
					png.write(genChunk.getBytes());
				}
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		return ret;
	}
	
	public static void main(String[] args) {
		InputStream is = APNGExperiments.class
				.getResourceAsStream("spr_player.apng");
		if (is == null)
			System.out.println("missing");
		else
		{
			System.out.println("reading...");
			ArrayList<BufferedImage> bi = apngToBufferedImages(is);
			if (bi == null)
				System.out.println("Failed - null returned");
			else
			{
				System.out.println("read in " + bi.size()
						+ " image(s) of sizes: ");
				for (BufferedImage b : bi)
					System.out.print("(" + b.getWidth() + ',' + b.getHeight()
							+ ") ");
				System.out.println();
			}
		}
	}
}
