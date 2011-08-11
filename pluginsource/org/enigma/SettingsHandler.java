package org.enigma;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.enigma.messages.Messages;
import org.enigma.utility.YamlParser;
import org.enigma.utility.YamlParser.YamlElement;
import org.enigma.utility.YamlParser.YamlNode;

public final class SettingsHandler
	{
	private SettingsHandler()
		{
		}

	public static List<OptionGroupSetting> optionGroups;
	public static Set<ExtensionSetting> extensions;

	public static class OptionGroupSetting
		{
		public String name;
		public int columns;
		public List<OptionSetting> opts;

		public OptionGroupSetting(String name, int columns, List<OptionSetting> opts)
			{
			this.name = name;
			this.columns = columns;
			this.opts = opts;
			}
		}

	public static class OptionSetting
		{
		public String id, label, type;
		public int columns;
		public String options, def;

		public OptionSetting(String id, String label, String type, int columns, String options,
				String def)
			{
			this.id = id;
			this.label = label;
			this.type = type;
			this.columns = columns;
			this.options = options;
			this.def = def;
			}
		}

	public static class ExtensionSetting
		{
		public File icon;
		public String path, name, desc;

		public ExtensionSetting(File icon, String path, String name, String desc)
			{
			this.icon = icon;
			this.path = path;
			this.name = name;
			this.desc = desc;
			}
		}

	public static class YamlException extends Exception
		{
		private static final long serialVersionUID = 1L;

		YamlException(String s)
			{
			super(s);
			}
		}

	static
		{
		try
			{
			parseOptions();
			}
		catch (YamlException e)
			{
			// TODO Auto-generated catch block
			e.printStackTrace();
			}
		parseExtensions();
		}

	private static void parseOptions() throws YamlException
		{
		optionGroups = new ArrayList<OptionGroupSetting>();
		File settings = new File(EnigmaRunner.WORKDIR,"settings.ey"); //$NON-NLS-1$
		YamlNode n;
		try
			{
			n = YamlParser.parse(settings);
			}
		catch (FileNotFoundException fnfe)
			{
			throw new YamlException(settings.getPath()
					+ Messages.getString("SettingsHandler.YERR_NO_FILE")); //$NON-NLS-1$
			}
		if (n.chronos.isEmpty())
			throw new YamlException(settings.getName() + Messages.getString("SettingsHandler.YERR_EMPTY")); //$NON-NLS-1$

		//loop through panels
		for (YamlElement ge : n.chronos)
			{
			String name = ge.name;
			String error = settings.getName() + ": " + name + ": "; //$NON-NLS-1$ //$NON-NLS-2$

			if (ge.isScalar)
				throw new YamlException(error + Messages.getString("SettingsHandler.YERR_SCALAR")); //$NON-NLS-1$

			n = (YamlNode) ge;
			if (!n.getMC("Layout","Grid").equalsIgnoreCase("Grid")) //$NON-NLS-1$ //$NON-NLS-2$ //$NON-NLS-3$
				throw new YamlException(error + Messages.getString("SettingsHandler.YERR_LAYOUT")); //$NON-NLS-1$

			int columns;
			try
				{
				columns = Integer.parseInt(n.getMC("Columns",null)); //$NON-NLS-1$
				}
			catch (NumberFormatException nfe)
				{
				throw new YamlException(error + Messages.getString("SettingsHandler.YERR_INTEGER")); //$NON-NLS-1$
				}

			//loop through options in group
			ArrayList<OptionSetting> opts = new ArrayList<OptionSetting>(n.chronos.size());
			for (YamlElement oe : n.chronos)
				{
				if (oe.name.equalsIgnoreCase("Layout") || oe.name.equalsIgnoreCase("Columns")) continue; //$NON-NLS-1$ //$NON-NLS-2$

				if (oe.isScalar)
					{
					System.err.println(Messages.format("SettingsHandler.YWARN_SCALAR",oe.name)); //$NON-NLS-1$
					continue;
					}
				YamlNode on = (YamlNode) oe;

				OptionSetting opt;
				try
					{
					opt = new OptionSetting(on.name,on.getMC("Label",null), //$NON-NLS-1$
							on.getMC("Type",null), //$NON-NLS-1$
							columns,on.getMC("Options",null), //$NON-NLS-1$
							on.getMC("Default","0")); //$NON-NLS-1$ //$NON-NLS-2$
					}
				catch (IllegalArgumentException e)
					{
					System.err.println(error
							+ Messages.format("SettingsHandler.YWARN_OPTION_TYPE",on.name,e.getMessage())); //$NON-NLS-1$
					continue;
					}
				opts.add(opt);
				} //each option in group

			optionGroups.add(new OptionGroupSetting(name,columns,opts));
			}
		}

	private static void parseExtensions()
		{
		extensions = new HashSet<ExtensionSetting>();

		//TODO: Make this able to fetch from other locations relative to SHELL/
		File dir = new File(EnigmaRunner.WORKDIR,"ENIGMAsystem"); //$NON-NLS-1$
		dir = new File(dir,"SHELL"); //$NON-NLS-1$
		dir = new File(dir,"Universal_System"); //$NON-NLS-1$
		dir = new File(dir,"Extensions"); //$NON-NLS-1$

		for (File f : dir.listFiles())
			if (f.isDirectory())
				{
				File ey = new File(f,"About.ey"); //$NON-NLS-1$
				try
					{
					String path = "Universal_System/Extensions/" + f.getName(); //$NON-NLS-1$
					YamlNode yn = YamlParser.parse(ey);
					String name = yn.getMC("id"); //$NON-NLS-1$
					String icon = yn.getMC("icon",null); //$NON-NLS-1$
					String desc = yn.getMC("description"); //$NON-NLS-1$
					extensions.add(new ExtensionSetting(icon == null ? null : new File(f,icon),path,name,desc));
					//					tm.addRow(icon == null ? null : new File(f,icon),path,name,desc);
					}
				catch (FileNotFoundException e)
					{
					}
				}
		}
	}
