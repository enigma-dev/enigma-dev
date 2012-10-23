/*
 * Copyright (C) 2011,2012 IsmAvatar <IsmAvatar@gmail.com>
 * Copyright (C) 2011 Josh Ventura <JoshV10@gmail.com>
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

public final class ApngIO
	{
	public static final byte[] PNG_SIGNATURE = { (byte) 0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A };
	public static final String IO_FMT = "png"; //$NON-NLS-1$

	//Convenience byte[]/int methods
	private static byte bite(int i, int p)
		{
		return (byte) (i >> p & 0xFF);
		}

	private static byte[] i2b(int i)
		{
		return new byte[] { bite(i,24),bite(i,16),bite(i,8),bite(i,0) };
		}

	/** Convenience method to fully read a buffer, since in.read(buf) can fall short. */
	private static void readFully(InputStream in, byte[] buffer) throws IOException
		{
		readFully(in,buffer,0,buffer.length);
		}

	private static void readFully(InputStream in, byte[] buffer, int off, int len) throws IOException
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

	//Chunk classes
	private static class ChunkType
		{
		private int val;
		private byte[] data;

		public ChunkType(byte[] bytes)
			{
			val = bytesToInt(data = bytes);
			}

		public byte[] getBytes()
			{
			return data;
			}

		public static int bytesToInt(byte[] b)
			{
			if (b.length != 4) throw new ArrayIndexOutOfBoundsException();
			return b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
			}

		public boolean equals(ChunkType other)
			{
			return val == other.val;
			}
		}

	private static abstract class PNG_Chunk
		{
		protected int length = 0;
		protected ChunkType chunkType;

		protected byte[] data;
		protected byte[] crc;

		public PNG_Chunk(ChunkType type)
			{
			chunkType = type;
			}

		void updateCRC()
			{
			CRC32 crc32 = new CRC32();
			crc32.update(chunkType.getBytes());
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
				baos.write(chunkType.getBytes());
				baos.write(data);
				baos.write(crc);
				}
			catch (IOException e)
				{
				e.printStackTrace();
				}
			return baos.toByteArray();
			}

		/*		boolean isType2(String t)
					{
					if (chunkType == null || t.length() != 4) return false;
					return chunkType[0] == t.charAt(0) && chunkType[1] == t.charAt(1)
							&& chunkType[2] == t.charAt(2) && chunkType[3] == t.charAt(3);
					}*/

		boolean isType(ChunkType t)
			{
			return chunkType == null ? false : chunkType.equals(t);
			}

		boolean read(InputStream dis) throws IOException
			{
			int b1 = dis.read();
			if (b1 == -1) return false;
			length = (b1 << 24) | (dis.read() << 16) | (dis.read() << 8) | dis.read();
			byte[] chunkTypeBytes = new byte[4];
			readFully(dis,chunkTypeBytes);
			chunkType = new ChunkType(chunkTypeBytes);
			data = new byte[length];
			readFully(dis,data);
			crc = new byte[4];
			readFully(dis,crc);
			return true;
			}
		}

	private static class Generic_Chunk extends PNG_Chunk
		{
		public Generic_Chunk()
			{
			super(null);
			}

		@Override
		void repopulate()
			{
			System.err.println("Repopulate called on generic chunk.");
			}
		}

	private static class IHDR_Dummy extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'I','H','D','R' });

		public IHDR_Dummy(byte[] png)
			{
			super(type);
			data = new byte[13];
			System.arraycopy(png,16,data,0,data.length);
			repopulate();
			}

		@Override
		void repopulate()
			{
			updateCRC();
			}
		}

	private static class acTL extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'a','c','T','L' });

		int numFrames;
		int numPlays;

		public acTL(int nf, int np)
			{
			super(type);
			numFrames = nf;
			numPlays = np;
			repopulate();
			}

		public void repopulate()
			{
			data = new byte[] { bite(numFrames,24),bite(numFrames,16),bite(numFrames,8),
					bite(numFrames,0),bite(numPlays,24),bite(numPlays,16),bite(numPlays,8),bite(numPlays,0) };
			updateCRC();
			}
		}

	private static class fcTL extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'f','c','T','L' });

		/** Sequence number of the animation chunk, 0-based */
		protected int sequenceNumber;
		/** Width of the following frame */
		protected int width;
		/** Height of the following frame */
		protected int height;
		/** X position at which to render the following frame */
		protected int xOffset;
		/** Y position at which to render the following frame */
		protected int yOffset;
		/** Frame delay fraction numerator */
		protected short delayNum;
		/** Frame delay fraction denominator */
		protected short delayDen;
		/** Type of frame area disposal to be done after rendering this frame */
		protected Dispose disposeOp;
		/** Type of frame area rendering for this frame */
		protected Blend blendOp;

		public fcTL(int sn, int w, int h, int xo, int yo, short dn, short dd, Dispose dop, Blend bop)
			{
			super(type);
			sequenceNumber = sn;
			width = w;
			height = h;
			xOffset = xo;
			yOffset = yo;
			delayNum = dn;
			delayDen = dd;
			disposeOp = dop;
			blendOp = bop;
			repopulate();
			}

		public static enum Dispose
			{
			/**
			 * No disposal is done on this frame before rendering the next;
			 * the contents of the output buffer are left as-is.
			 */
			NONE(0),
			/**
			 * The frame's region of the output buffer is to be cleared
			 * to fully transparent black before rendering the next frame.
			 */
			BACKGROUND(1),
			/**
			 * The frame's region of the output buffer is to be reverted
			 * to the previous contents before rendering the next frame.
			 */
			PREVIOUS(2);

			private byte value;

			private Dispose(int v)
				{
				value = (byte) v;
				}

			public byte getValue()
				{
				return value;
				}
			}

		public static enum Blend
			{
			/**
			 * All color components of the frame, including alpha, overwrite
			 * the current contents of the frame's output buffer region.
			 */
			SOURCE(0),
			/**
			 * The frame should be composited onto the output buffer
			 * based on its alpha, using a simple OVER operation
			 * as described in the "Alpha Channel Processing" section
			 * of the PNG specification [PNG-1.2].
			 */
			OVER(1);

			private byte value;

			private Blend(int v)
				{
				value = (byte) v;
				}

			public byte getValue()
				{
				return value;
				}
			}

		@Override
		void repopulate()
			{
			data = new byte[] { bite(sequenceNumber,24),bite(sequenceNumber,16),bite(sequenceNumber,8),
					bite(sequenceNumber,0),bite(width,24),bite(width,16),bite(width,8),bite(width,0),
					bite(height,24),bite(height,16),bite(height,8),bite(height,0),bite(xOffset,24),
					bite(xOffset,16),bite(xOffset,8),bite(xOffset,0),bite(yOffset,24),bite(yOffset,16),
					bite(yOffset,8),bite(yOffset,0),bite(delayNum,8),bite(delayNum,0),bite(delayDen,8),
					bite(delayDen,0),disposeOp.getValue(),blendOp.getValue() };
			updateCRC();
			}
		}

	private static class IDAT extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'I','D','A','T' });

		/** Creates a new IDAT chunk with the given frame data. */
		public IDAT(byte[] dat)
			{
			super(type);
			data = dat;
			repopulate();
			}

		@Override
		public void repopulate()
			{
			updateCRC();
			}
		}

	private static class fdAT extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'f','d','A','T' });

		protected int sequenceNumber; // Sequence number, 0-based
		protected byte[] frameData;

		/**
		 * Creates an fdAT from a PNG_Chunk that is already in fdAT format.
		 * Namely, it extracts the sequence number from the frame data.
		 */
		public fdAT(PNG_Chunk pc)
			{
			super(pc.chunkType);
			length = pc.length;
			sequenceNumber = pc.data[0] << 24 | pc.data[1] << 16 | pc.data[2] << 8 | pc.data[3];
			frameData = new byte[pc.data.length - 4];
			System.arraycopy(pc.data,4,frameData,0,frameData.length);
			crc = pc.crc;
			}

		/** Creates an fdAT from a PNG_Chunk that is in another format (converts it) */
		public fdAT(PNG_Chunk src, int sn)
			{
			super(type);
			length = src.length;
			sequenceNumber = sn;
			frameData = src.data; //will just be arraycopy'd anyways
			repopulate();
			}

		@Override
		public void repopulate()
			{
			data = new byte[frameData.length + 4];
			System.arraycopy(i2b(sequenceNumber),0,data,0,4);
			System.arraycopy(frameData,0,data,4,frameData.length);
			updateCRC();
			}

		public IDAT toIDAT()
			{
			return new IDAT(frameData);
			}
		}

	private static class IEND extends PNG_Chunk
		{
		public static final ChunkType type = new ChunkType(new byte[] { 'I','E','N','D' });
		public static final IEND instance = new IEND();

		/**
		 * IEND is a singleton class and should not be instantiated.
		 * Use IEND.instance instead.
		 */
		private IEND()
			{
			super(type);
			repopulate();
			}

		@Override
		public void repopulate()
			{
			data = new byte[] {};
			updateCRC();
			}
		}

	//Functionality
	private static void transferIDATs(InputStream dis, OutputStream os, int sn, boolean ignoreOthers)
			throws IOException
		{
		Generic_Chunk chunk = new Generic_Chunk();
		while (chunk.read(dis) && !chunk.isType(IEND.type))
			{
			if (chunk.isType(IDAT.type))
				os.write((sn == -1 ? chunk : new fdAT(chunk,sn)).getBytes());
			else if (!ignoreOthers) os.write(chunk.getBytes());
			}
		}

	public static void imagesToApng(ArrayList<BufferedImage> imgs, OutputStream fullFile)
			throws IOException
		{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		ImageIO.write(imgs.get(0),IO_FMT,baos);
		byte buf[] = baos.toByteArray();
		IHDR_Dummy myhdr = new IHDR_Dummy(buf);
		acTL myactl = new acTL(imgs.size(),0);
		fcTL fctl = new fcTL(0,imgs.get(0).getWidth(),imgs.get(0).getHeight(),0,0,(short) 1,(short) 30,
				fcTL.Dispose.BACKGROUND,fcTL.Blend.OVER);

		fullFile.write(PNG_SIGNATURE);
		fullFile.write(myhdr.getBytes());
		fullFile.write(myactl.getBytes());
		fullFile.write(fctl.getBytes());

		//Transfer the IDAT chunks
		ByteArrayInputStream bais = new ByteArrayInputStream(buf);
		bais.skip(8);
		transferIDATs(bais,fullFile,-1,true);

		int indx = -1;
		for (BufferedImage bi : imgs)
			{
			if (indx++ == -1) // Continues once to skip the first image;
				continue; // index will be 1 on first iteration

			baos.reset(); // Clear our buffer,
			ImageIO.write(bi,IO_FMT,baos); // Fetch new image.

			// Create the frame control. IDs will be 2, 4, 6, 8...
			fctl = new fcTL(indx++,bi.getWidth(),bi.getHeight(),0,0,(short) 1,(short) 30,
					fcTL.Dispose.BACKGROUND,fcTL.Blend.OVER);
			fullFile.write(fctl.getBytes()); // Write it into the file

			// Now make our frame data. IDs will be 3, 5, 7, 9...
			buf= baos.toByteArray();
			bais = new ByteArrayInputStream(buf);
			bais.skip(8);
			transferIDATs(bais,fullFile,indx,true);
			}
		fullFile.write(IEND.instance.getBytes());
		}

	public static ArrayList<BufferedImage> apngToBufferedImages(InputStream is)
		{
		PNG_Chunk genChunk = new Generic_Chunk();
		ByteArrayOutputStream png = new ByteArrayOutputStream();
		ArrayList<BufferedImage> ret = new ArrayList<BufferedImage>();
		byte[] imageHeader=null;
		try
			{
			png.write(PNG_SIGNATURE);
			byte pngBase[] = new byte[8];

			is.read(pngBase);
			if (!Arrays.equals(pngBase,PNG_SIGNATURE)) throw new IOException("Not APNG");

			boolean hasData = false;

			while (genChunk.read(is))
				{

				if (genChunk.isType(acTL.type))
					{
					//Look at all the fucks I give
					}
				else if (genChunk.isType(fcTL.type))
					{
					if (hasData)
						{
						png.write(IEND.instance.getBytes());
						ByteArrayInputStream bais = new ByteArrayInputStream(png.toByteArray());
						ret.add(ImageIO.read(bais));

						png.reset();
						png.write(pngBase);
						png.write(imageHeader);
						hasData = false;
						}
					}
				else if (genChunk.isType(IDAT.type))
					{
					png.write(genChunk.getBytes());
					hasData = true;
					}
				else if (genChunk.isType(fdAT.type))
					{
					byte[] a = new fdAT(genChunk).toIDAT().getBytes();
					png.write(a);
					hasData = true;
					}
				else if (genChunk.isType(IEND.type))
					{
					png.write(genChunk.getBytes());
					ByteArrayInputStream bais = new ByteArrayInputStream(png.toByteArray());
					ret.add(ImageIO.read(bais));
					break;
					}
				else if (genChunk.isType(IHDR_Dummy.type)) {
					//save the image header as it is used for all subimages
					imageHeader=genChunk.getBytes();
					png.write(imageHeader);
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
	}
