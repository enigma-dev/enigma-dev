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
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.zip.CRC32;

import javax.imageio.ImageIO;

/////////////////////////////////////////////////
// Below are experimental APNG support methods //
/////////////////////////////////////////////////
public class APNGExperiments
	{
	static final byte[] PNG_SIGNATURE = { (byte) 0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A };
	static final String IO_FMT = "png"; //$NON-NLS-1$

	static byte[] i2b(int i)
		{
		return new byte[] { bite(i,24),bite(i,16),bite(i,8),bite(i,0) };
		}

	static byte bite(int i, int p)
		{
		return (byte) (i >> p & 0xFF);
		}

	/** Convenience method to fully read a buffer, since in.read(buf) can fall short. */
	static void readFully(InputStream in, byte[] buffer) throws IOException
		{
		readFully(in,buffer,0,buffer.length);
		}

	static void readFully(InputStream in, byte[] buffer, int off, int len) throws IOException
		{
		int total = 0;
		while (true)
			{
			int n = in.read(buffer,off + total,len - total);
			if (n <= 0)
				{
				if (total == 0) total = n;
				break;
				}
			total += n;
			if (total == len) break;
			}
		}

	static abstract class PNG_Chunk
		{
		int length = 0;
		byte[] chunkType;
		byte[] data;
		byte[] crc;

		void updateCRC()
			{
			CRC32 crc32 = new CRC32();
			crc32.update(chunkType);
			crc32.update(data);
			int tcrc = (int) crc32.getValue();
			length = data.length;
			crc = i2b(tcrc);
			}

		abstract void repopulate();

		byte[] getBytes()
			{ // Get the bytes of this chunk for writing
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

		boolean isType(String t)
			{
			if (chunkType == null || t.length() != 4) return false;
			return chunkType[0] == t.charAt(0) && chunkType[1] == t.charAt(1)
					&& chunkType[2] == t.charAt(2) && chunkType[3] == t.charAt(3);
			}

		boolean read(InputStream dis) throws IOException
			{
			int b1 = dis.read();
			if (b1 == -1) return false;
			length = (b1 << 24) | (dis.read() << 16) | (dis.read() << 8) | dis.read();
			chunkType = new byte[4];
			readFully(dis,chunkType);
			data = new byte[length];
			readFully(dis,data);
			crc = new byte[4];
			readFully(dis,crc);
			return true;
			}
		}

	static class Generic_Chunk extends PNG_Chunk
		{
		@Override
		void repopulate()
			{
			System.err.println("Repopulate called on generic chunk.");
			}
		}

	static class IHDR extends PNG_Chunk
		{
		int width, height;
		byte bitDepth, colType, compression, filter, interlace;

		public IHDR(int w, int h, byte bd, byte ct, byte cm, byte ft, byte il)
			{
			chunkType = new byte[] { 'I','H','D','R' };
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
			chunkType = new byte[] { 'I','H','D','R' };
			data = new byte[13];
			System.arraycopy(png,16,data,0,13);
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

		public void repopulate()
			{
			/*			data = new byte[] { (byte) (width & 0xFF000000),
								(byte) (width & 0x00FF0000), (byte) (width & 0x0000FF00),
								(byte) (width & 0x000000FF), (byte) (height & 0xFF000000),
								(byte) (height & 0x00FF0000), (byte) (height & 0x0000FF00),
								(byte) (height & 0x000000FF), bitDepth, colType,
								compression, filter, interlace };*/

			data = new byte[] { bite(width,24),bite(width,16),bite(width,8),bite(width,0), //
					bite(height,24),bite(height,16),bite(height,8),bite(height,0), //
					bitDepth,colType,compression,filter,interlace };

			updateCRC();
			}
		}

	static class acTL extends PNG_Chunk
		{
		int num_frames;
		int num_plays;

		public acTL(int nf, int np)
			{
			chunkType = new byte[] { 'a','c','T','L' };
			num_frames = nf;
			num_plays = np;
			repopulate();
			}

		public void repopulate()
			{
			/*			data = new byte[] { (byte) (num_frames & 0xFF0000),
								(byte) (num_frames & 0x00FF0000),
								(byte) (num_frames & 0x0000FF00),
								(byte) (num_frames & 0x000000FF),
								(byte) (num_plays & 0xFF000000),
								(byte) (num_plays & 0x00FF0000),
								(byte) (num_plays & 0x0000FF00),
								(byte) (num_plays & 0x000000FF) };*/

			data = new byte[] { bite(num_frames,24),bite(num_frames,16),bite(num_frames,8),
					bite(num_frames,0),bite(num_plays,24),bite(num_plays,16),bite(num_plays,8),
					bite(num_plays,0) };

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

		public fcTL(int sn, int w, int h, int xo, int yo, short dn, short dd, byte dispose_opr,
				byte blend_opr)
			{
			chunkType = new byte[] { 'f','c','T','L' };
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
		void repopulate()
			{
			/*			data = new byte[] { (byte) (sequence_number & 0xFF000000),
								(byte) (sequence_number & 0x00FF0000),
								(byte) (sequence_number & 0x0000FF00),
								(byte) (sequence_number & 0x000000FF),
								(byte) (width >> 24 & 0xFF), (byte) (width & 0x00FF0000),
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
								(byte) (delay_den & 0x000000FF), dispose_op, blend_op };*/

			data = new byte[] { bite(sequence_number,24),bite(sequence_number,16),
					bite(sequence_number,8),bite(sequence_number,0),bite(width,24),bite(width,16),
					bite(width,8),bite(width,0),bite(height,24),bite(height,16),bite(height,8),
					bite(height,0),bite(x_offset,24),bite(x_offset,16),bite(x_offset,8),bite(x_offset,0),
					bite(y_offset,24),bite(y_offset,16),bite(y_offset,8),
					bite(y_offset,0),
					//The num/den may be screwed up. This was just transcribed from the original to fix
					//bit-shifting, which was omitted. The bit shift amount may have been incorrect too.
					bite(delay_num,8),bite(delay_num,0),bite(delay_den,8),bite(delay_den,0),dispose_op,
					blend_op };

			updateCRC();
			}
		}

	static class IDAT extends PNG_Chunk
		{
		public IDAT(byte[] png) throws IOException
			{
			// These are failsafes. They are both unnecessary.
			chunkType = new byte[] { 'I','D','A','T' };
			data = new byte[] {};

			ByteArrayInputStream dis = new ByteArrayInputStream(png);
			dis.skip(8);
			// Find our fdAT chunk in the PNG
			while (dis.available() >= 12)
				{
				read(dis);
				if (isType("IDAT")) break;
				}
			repopulate();
			}

		public IDAT(byte[] dat, boolean b)
			{
			chunkType = new byte[] { 'I','D','A','T' };
			data = dat;
			updateCRC();
			}

		@Override
		public void repopulate()
			{
			updateCRC();
			}
		}

	static class IDAT_Multi extends PNG_Chunk
		{
		public IDAT_Multi(InputStream dis) throws IOException
			{
			// These are failsafes. They are both unnecessary.
			chunkType = new byte[] { 'I','D','A','T' };
			data = new byte[] {};

			ByteArrayOutputStream baos = new ByteArrayOutputStream();

			// Find our IDAT chunks in the PNG
			while (read(dis) && !isType("IEND"))
				{
				if (!isType("IDAT")) continue; //guess we'll skip non-IDAT chunks
				baos.write(i2b(length));
				baos.write(chunkType);
				baos.write(data);
				baos.write(crc);
				}
			data = baos.toByteArray();
			repopulate();
			}

		@Override
		void repopulate()
			{
			updateCRC();
			}
		}

	static class fdAT extends PNG_Chunk
		{
		int sequence_number; // Sequence number, 0-based
		byte[] frame_data;

		public fdAT(int sn, byte[] png) throws IOException
			{
			byte[] fcTLt = chunkType = new byte[] { 'f','d','A','T' };
			sequence_number = sn;
			frame_data = null;

			ByteArrayInputStream dis = new ByteArrayInputStream(png);
			dis.skip(8);
			// Find our fdAT chunk in the PNG
			while (read(dis))
				{
				if (isType("IDAT"))
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

			data = new byte[pc.data.length - 4];
			System.arraycopy(pc.data,4,data,0,data.length);

			sequence_number = pc.data[0] << 24 | pc.data[1] << 16 | pc.data[2] << 8 | pc.data[3];
			crc = pc.crc;
			}

		@Override
		public void repopulate()
			{
			data = new byte[frame_data.length + 4];
			/*			System.arraycopy(new byte[] {
								(byte) (sequence_number & 0xFF000000),
								(byte) (sequence_number & 0x00FF0000),
								(byte) (sequence_number & 0x0000FF00),
								(byte) (sequence_number & 0x000000FF) }, 0, data, 0, 4);*/
			byte[] seqnum = new byte[] { bite(sequence_number,24),bite(sequence_number,16),
					bite(sequence_number,8),bite(sequence_number,0) };
			System.arraycopy(seqnum,0,data,0,4);
			System.arraycopy(frame_data,0,data,4,frame_data.length);
			updateCRC();
			}

		public IDAT toIDAT()
			{
			return new IDAT(data,true);
			}
		}

	static class IDAT2 extends PNG_Chunk
		{
		public IDAT2(InputStream dis) throws IOException
			{
			chunkType = new byte[] { 'f','d','A','T' };

			while (read(dis))
				{
				if (isType("IDAT")) break;
				}
			repopulate();
			}

		public IDAT2(PNG_Chunk src)
			{
			length = src.length;
			chunkType = src.chunkType;
			data = new byte[length];
			System.arraycopy(src.data,0,data,0,length);
			crc = src.crc;
			}

		@Override
		void repopulate()
			{
			updateCRC();
			}
		}

	static class fdAT2 extends PNG_Chunk
		{
		int sequence_number; // Sequence number, 0-based
		byte[] frame_data;

		public fdAT2(InputStream dis, int sn) throws IOException
			{
			byte[] fcTLt = chunkType = new byte[] { 'f','d','A','T' };
			sequence_number = sn;
			frame_data = null;

			while (read(dis))
				{
				if (isType("IDAT"))
					{
					chunkType = fcTLt;
					frame_data = data;
					break;
					}
				}
			repopulate();
			}

		public fdAT2(PNG_Chunk src, int sn)
			{
			length = src.length;
			chunkType = new byte[] { 'f','d','A','T' };
			sequence_number = sn;
			frame_data = src.data; //will just be arraycopy'd anyways
			repopulate();
			}

		@Override
		public void repopulate()
			{
			data = new byte[frame_data.length + 4];
			System.arraycopy(i2b(sequence_number),0,data,0,4);
			System.arraycopy(frame_data,0,data,4,frame_data.length);
			updateCRC();
			}
		}

	static class IEND extends PNG_Chunk
		{
		IEND()
			{
			chunkType = new byte[] { 'I','E','N','D' };
			repopulate();
			}

		@Override
		public void repopulate()
			{
			data = new byte[] {};
			updateCRC();
			}
		}

	static Generic_Chunk readChunk(InputStream dis) throws IOException
		{
		int b1 = dis.read();
		if (b1 == -1) return null;

		Generic_Chunk chunk = new Generic_Chunk();
		chunk.length = (b1 << 24) | (dis.read() << 16) | (dis.read() << 8) | dis.read();
		chunk.chunkType = new byte[4];
		readFully(dis,chunk.chunkType);
		chunk.data = new byte[chunk.length];
		readFully(dis,chunk.data);
		chunk.crc = new byte[4];
		readFully(dis,chunk.crc);

		return chunk;
		}

	static List<PNG_Chunk> parseIDATs(int sn, byte[] png, boolean ignoreOthers) throws IOException
		{
		ArrayList<PNG_Chunk> r = new ArrayList<PNG_Chunk>();
		ByteArrayInputStream dis = new ByteArrayInputStream(png);
		dis.skip(8);

		Generic_Chunk chunk = readChunk(dis);
		while (chunk != null && !chunk.isType("IEND"))
			{
			if (chunk.isType("IDAT"))
				r.add(sn == -1 ? chunk : new fdAT2(chunk,sn));
			else if (!ignoreOthers) r.add(chunk);
			chunk = readChunk(dis);
			}

		return r;
		}

	public static void imagesToApng(ArrayList<BufferedImage> imgs, OutputStream fullFile)
			throws IOException
		{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		ImageIO.write(imgs.get(0),IO_FMT,baos);
		byte buf[] = baos.toByteArray();
		IHDR myhdr = new IHDR(buf);
		acTL myactl = new acTL(imgs.size(),0);
		fcTL fctl = new fcTL(0,imgs.get(0).getWidth(),imgs.get(0).getHeight(),0,0,(short) 1,(short) 30,
				fcTL.Dispose.APNG_DISPOSE_OP_BACKGROUND,fcTL.Blend.APNG_BLEND_OP_OVER);
		//		IDAT mainidat = new IDAT(buf);

		fullFile.write(PNG_SIGNATURE);
		fullFile.write(myhdr.getBytes());
		fullFile.write(myactl.getBytes());
		fullFile.write(fctl.getBytes());

		//		fullFile.write(mainidat.getBytes());

		for (PNG_Chunk idat : parseIDATs(-1,buf,true))
			fullFile.write(idat.getBytes());

		int indx = -1;
		for (BufferedImage bi : imgs)
			{
			if (indx++ == -1) // Continues once to skip the first image;
				continue; // index will be 1 on first iteration

			baos.reset(); // Clear our buffer,
			ImageIO.write(bi,IO_FMT,baos); // Fetch new image.

			// Create the frame control. IDs will be 2, 4, 6, 8...
			fctl = new fcTL(indx++,bi.getWidth(),bi.getHeight(),0,0,(short) 1,(short) 30,
					fcTL.Dispose.APNG_DISPOSE_OP_BACKGROUND,fcTL.Blend.APNG_BLEND_OP_OVER);
			fullFile.write(fctl.getBytes()); // Write it into the file

			// Now make our frame data. IDs will be 3, 5, 7, 9...
			//			fdAT fdat = new fdAT(indx, baos.toByteArray());
			//			fullFile.write(fdat.getBytes()); // Write it into the file

			for (PNG_Chunk fdat : parseIDATs(indx,baos.toByteArray(),true))
				fullFile.write(fdat.getBytes());
			}
		IEND iend = new IEND();
		fullFile.write(iend.getBytes());
		}

	public static ArrayList<BufferedImage> apngToBufferedImages(InputStream is)
		{
		PNG_Chunk genChunk = new Generic_Chunk();
		ByteArrayOutputStream png = new ByteArrayOutputStream();
		ArrayList<BufferedImage> ret = new ArrayList<BufferedImage>();
		try
			{
			png.write(PNG_SIGNATURE);
			byte pngBase[] = new byte[8];

			is.read(pngBase);
			if (!Arrays.equals(pngBase,PNG_SIGNATURE)) throw new IOException("Not APNG");

			boolean hasData = false;
			
			while (genChunk.read(is))
				{
				if (genChunk.isType("acTL"))
					{
					//Look at all the fucks I give
					}
				else if (genChunk.isType("fcTL"))
					{
					if (hasData)
						{
						png.write(new IEND().getBytes());
						ByteArrayInputStream bais = new ByteArrayInputStream(png.toByteArray());
						ret.add(ImageIO.read(bais));

						png.reset();
						png.write(pngBase);
						hasData = false;
						}
					}
				else if (genChunk.isType("IDAT"))
					{
					png.write(genChunk.getBytes());
					hasData = true;
					}
				else if (genChunk.isType("fdAT"))
					{
					byte[] a = new fdAT(genChunk).toIDAT().getBytes();
					png.write(a);
					hasData = true;
					}
				else if (genChunk.isType("IEND"))
					{
					png.write(genChunk.getBytes());
					ByteArrayInputStream bais = new ByteArrayInputStream(png.toByteArray());
					ret.add(ImageIO.read(bais));
					break;
					}
				else
					{
					png.write(genChunk.getBytes());
					//Uncomment this if we want to include these chunks with every png
					//Otherwise, keep commented to only include with current png
					//	pngBase = png.toByteArray();
					}
				}
			}
		catch (IOException e)
			{
			e.printStackTrace();
			}
		return ret;
		}

	public static void mainTwoWay() throws IOException
		{
		File dropbox = new File(System.getProperty("user.home"),"Dropbox");
		File f = new File(dropbox,"ENIGMA");
		ArrayList<BufferedImage> list = new ArrayList<BufferedImage>();

		//convert sprite90.png to APNG
		list.add(ImageIO.read(new File(f,"sprite90.png")));
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		imagesToApng(list,baos);

		//attempt to read resulting APNG byte stream back to BufferedImage
		ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
		ArrayList<BufferedImage> list2 = apngToBufferedImages(bais);
		System.out.println("HI " + list2.size());
		}

	public static void main(String[] args) throws Exception
		{
		mainTwoWay();
		}
	}
