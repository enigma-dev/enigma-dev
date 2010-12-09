package org.enigma;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.JFrame;

public class Masker
	{
	public static final int THRESH = 0;

	public static boolean isTrans(int pixel)
		{
		return (pixel >>> 24) <= THRESH;
		}

	static class ImageCanvas extends Canvas
		{
		private static final long serialVersionUID = 1L;
		BufferedImage img;
		List<Polygon> polys = new ArrayList<Polygon>();

		public ImageCanvas(BufferedImage img)
			{
			this.img = img;
			}

		public void addPolygon(Polygon p)
			{
			polys.add(p);
			}

		public void paint(Graphics g)
			{
			g.clearRect(0,0,img.getWidth(),img.getHeight());
			//			g.drawImage(img,0,0,null);
			g.setColor(Color.WHITE);
			g.fillRect(0,0,img.getWidth(),img.getHeight());
			g.setColor(Color.BLACK);
			for (Polygon poly : polys)
				g.fillPolygon(poly);
			}
		}

	static class Mask
		{
		public static final int UNSET = 0, TRANS = 1;

		int[][] mask;
		List<Point> pts;
		int[] cols = { 0x000000,0xFFFFFF,0xFF0000,0x00FF00,0x0000FF,0xFFFF00,0xFF00FF,0x00FFFF };

		public Mask(int[][] image)
			{
			mask = new int[image.length][image[0].length];
			pts = new ArrayList<Point>();
			for (int y = 0; y < image.length; y++)
				for (int x = 0; x < image[y].length; x++)
					{
					if (isTrans(image[y][x])) mask[y][x] = TRANS;
					if (mask[y][x] != UNSET) continue;
					pts.add(new Point(x,y));
					fill(image,y,x,pts.size() + 1);
					}
			}

		public Mask(int[] image, int w, int h)
			{
			mask = new int[h][w];
			pts = new ArrayList<Point>();
			for (int y = 0; y < h; y++)
				for (int x = 0; x < w; x++)
					{
					if (isTrans(image[y * w + x])) mask[y][x] = TRANS;
					if (mask[y][x] != UNSET) continue;
					pts.add(new Point(x,y));
					fill(image,y,x,h,w,pts.size() + 1);
					}
			}

		private void fill(int[][] image, int y, int x, int shapeNum)
			{
			if (y < 0 || y >= image.length || x < 0 || x >= image[y].length) return;
			if (isTrans(image[y][x]) || mask[y][x] != UNSET) return;
			mask[y][x] = shapeNum;
			fill(image,y,x - 1,shapeNum);
			fill(image,y,x + 1,shapeNum);
			fill(image,y - 1,x,shapeNum);
			fill(image,y + 1,x,shapeNum);
			}

		private void fill(int[] image, int y, int x, int h, int w, int shapeNum)
			{
			if (y < 0 || y >= h || x < 0 || x >= w || y * w + x >= image.length) return;
			if (isTrans(image[y * w + x]) || mask[y][x] != UNSET) return;
			mask[y][x] = shapeNum;
			fill(image,y,x - 1,h,w,shapeNum);
			fill(image,y,x + 1,h,w,shapeNum);
			fill(image,y - 1,x,h,w,shapeNum);
			fill(image,y + 1,x,h,w,shapeNum);
			}

		public List<Point> getMarchingOutline(Point p)
			{
			List<Point> out = new ArrayList<Point>();
			out.add(p);

			Point p2 = new Point(p.x,p.y + 1);
			while (!p2.equals(p))
				{
				out.add(p2);
				int state = (mask[p2.y - 1][p2.x - 1] != UNSET ? 8 : 0)
						+ (mask[p2.y - 1][p2.x] != UNSET ? 4 : 0) + (mask[p2.y][p2.x - 1] != UNSET ? 2 : 0)
						+ (mask[p2.y][p2.x] != UNSET ? 1 : 0);
				//84  v 1,5,13   ^ 8,9,10,11
				//21  < 2,3,6,7  > 4,12,14
				switch (state)
					{
					case 1: //    |  || X||XX|
					case 5: //  v | X|| X|| X|
					case 13:
						p2 = new Point(p.x,p.y + 1);
						break;
					case 2:
					case 3:
					case 6:
					case 7:
						p2 = new Point(p.x - 1,p.y);
						break;
					case 4:
					case 12:
					case 14:
						p2 = new Point(p.x + 1,p.y);
						break;
					case 8:
					case 9:
					case 10:
					case 11:
						p2 = new Point(p.x,p.y - 1);
						break;
					default:
						throw new IllegalStateException();
					}
				}
			return out;
			}

		public List<Point> getRayOutline2(Point p)
			{
			List<Point> out = new ArrayList<Point>();
			out.add(p);
			Point p2 = new Point(p.x + 1,p.y);
			int num = mask[p.y][p.x];
			int dir = 0;
			while (p2.x != p.x && p2.y != p.y && dir != 0)
				{
				switch (dir)
					{
					case 0:
					case 350:
						if (getNumSafe(p2.y - 1,p2.x + 1) == num)
							{
							if (dir != 50) out.add(p2);
							dir = 50;
							p2 = new Point(p2.x + 1,p2.y - 1);
							continue;
							}
						if (getNumSafe(p2.y,p2.x + 1) == num)
							{
							if (dir != 0) out.add(p2);
							dir = 0;
							p2 = new Point(p2.x + 1,p2.y);
							continue;
							}
					case 250:
					case 300:
						if (getNumSafe(p2.y + 1,p2.x + 1) == num)
							{
							out.add(p2);
							dir = 350;
							p2 = new Point(p2.x + 1,p2.y + 1);
							continue;
							}
						if (getNumSafe(p2.y + 1,p2.x) == num)
							{
							out.add(p2);
							dir = 300;
							p2 = new Point(p2.x,p2.y + 1);
							continue;
							}
					case 150:
					case 200:
						if (getNumSafe(p2.y + 1,p2.x - 1) == num)
							{
							out.add(p2);
							dir = 250;
							p2 = new Point(p2.x - 1,p2.y + 1);
							continue;
							}
						if (getNumSafe(p2.y + 1,p2.x) == num)
							{
							out.add(p2);
							dir = 200;
							p2 = new Point(p2.x,p2.y + 1);
							continue;
							}
					case 50:
					case 100:
						if (getNumSafe(p2.y - 1,p2.x - 1) == num)
							{
							out.add(p2);
							dir = 150;
							p2 = new Point(p2.x - 1,p2.y - 1);
							continue;
							}
						if (getNumSafe(p2.y - 1,p2.x) == num)
							{
							if (dir != 100) out.add(p2);
							dir = 100;
							p2 = new Point(p2.x,p2.y - 1);
							continue;
							}
					}
				}

			return out;
			}

		public static int square[][] = { { -1,-1 }, { 0,-1 }, { 1,-1 }, { 1,0 }, { 1,1 }, { 0,1 },
				{ -1,1 }, { -1,0 } };
		public static final Point dirs[] = { new Point(1,0),new Point(1,1),new Point(0,1),
				new Point(-1,1),new Point(-1,0),new Point(-1,-1),new Point(0,-1),new Point(1,-1) };

		public List<Point> getRayOutline(final Point p)
			{
			List<Point> out = new ArrayList<Point>();
			out.add(p);
			Point p2 = new Point(p);
			final int num = mask[p.y][p.x];
			int prevDir = 0, huntDir = 0;

			do
				{
				//find next clockwise pixel
				int foundDir = -1;
				for (int i = 0; i < 8 && foundDir == -1; i++)
					{
					Point v = dirs[(huntDir + i) % 8]; //vector unit point
					if (getNumSafe(p2.y + v.y,p2.x + v.x) == num) foundDir = (huntDir + i) % 8;
					}
				//handle single-pixel shapes
				if (foundDir == -1)
					{
					out.add(p2);
					return out;
					}
				//if new dir, add old pixel as vertex
				if (foundDir != prevDir)
					{
					out.add(p2);
					prevDir = foundDir;
					//get ccw semi-cardinal from dir (e.g. E and SE both yield NE)
					int d2 = (prevDir & ~1) - 1;
					if (d2 == -1) d2 = 7;
					}
				//move to found pixel
				Point v = dirs[foundDir];
				p2 = new Point(p2.x + v.x,p2.y + v.y);
				}
			while (p2.x != p.x || p2.y != p.y);

			return out;
			}

		public List<Point> getRayOutline3(Point p)
			{
			List<Point> out = new ArrayList<Point>();
			out.add(p);
			Point p2 = new Point(p.x,p.y);
			final int num = mask[p.y][p.x];
			int fromDir = 7;
			int newDir = 0;
			int count = 0;
			do
				{
				newDir = fromDir + 1;
				while (count < 8
						&& getNumSafe(p2.y + square[(newDir + 1) % 8][1],p2.x + square[(++newDir) % 8][0]) != num)
					count++;
				if (count == 8) newDir = fromDir;

				p2 = new Point(p2.x + square[newDir][0],p2.y + square[newDir][1]);
				if (p2.x != p.x && p2.y != p.y && (newDir + 4) % 8 != fromDir) out.add(p2);
				fromDir = (newDir + 4) % 8;
				count = 0;
				}
			while (p2.x != p.x && p2.y != p.y);

			return out;
			}

		public int getNumSafe(int y, int x)
			{
			if (y < 0 || y >= mask.length || x < 0 || x >= mask[y].length) return TRANS;
			return mask[y][x];
			}

		public BufferedImage toImage()
			{
			BufferedImage img = new BufferedImage(mask.length,mask[0].length,BufferedImage.TYPE_INT_RGB);
			for (int y = 0; y < mask.length; y++)
				for (int x = 0; x < mask.length; x++)
					img.setRGB(x,y,cols[mask[y][x]]);
			return img;
			}
		}

	public static void main(String[] args) throws IOException
		{
		BufferedImage i = ImageIO.read(new File("/home/ismavatar/Desktop/test.png"));
		int w = i.getWidth();
		int h = i.getHeight();
		int pixels[] = i.getRGB(0,0,w,h,null,0,w);
		int img[][] = new int[h][w];
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
				img[y][x] = pixels[y * w + x];

		JFrame f = new JFrame();
		Mask m = new Mask(pixels,w,h);
		ImageCanvas c = new ImageCanvas(m.toImage());

		for (Point p : m.pts)
			{
			List<Point> pl = m.getRayOutline(p);
			Polygon poly = new Polygon();
			for (Point pv : pl)
				{
				System.out.print("{" + pv.x + "," + pv.y + "}, ");
				poly.addPoint(pv.x,pv.y);
				}
			System.out.println();
			c.addPolygon(poly);
			}

		f.add(c);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setSize(200,200);
		f.setLocationRelativeTo(null);
		f.setVisible(true);
		}
	}