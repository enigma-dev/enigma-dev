package com.alasdairmorrison.LGMUtility;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.List;
import java.util.Vector;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;


import org.enigma.EnigmaCli;
import org.enigma.backend.EnigmaSettings;
import org.lateralgm.file.GmFormatException;
import org.lateralgm.main.LGM;
import org.lateralgm.resources.ResourceReference;
import org.lateralgm.resources.Font.PFont;
import org.lateralgm.resources.Script.PScript;
import org.lateralgm.resources.library.LibAction;
import org.lateralgm.resources.sub.Action;
import org.lateralgm.resources.sub.ActionContainer;
import org.lateralgm.resources.sub.Argument;

public class LGMUtilityMain implements ActionListener {
	
	JMenuItem regex,findres,runtimeregex;
	
	static Vector<String> scriptnames;
	



	static String variables[] = {"char","auto","time","width","height","in","tm","close","index","distance","select","pause","super","plus","rand","fopen","system","wait","int","try","free","float","new","register","map","this","pow","exp","round","minus","fma","power","throw","continue","setbuffer","double","random","draw_text","log","clock","write","sleep","direction_difference","friend","floor","rewind","sign","swap","pipe","goto","class","min","max","expl","exp2","gamma","transform","const",
		"volatile","long","less"};
	static String othersOriginal[] = {",[\\s]*\\)","view_left","view_top",",",":=","\\sdiv\\s",
		//"\\smod\\s",
		"(\\b)make_color(\\b)",//"([\\s=])make_color[\\s\\(]",
		"not\\s",
		//"(\\b)switch[\\s\\(]", //temp
		"([\\s\\)])then",
		"external_define2",
		"external_call2",
		"([^_a-zA-Z0-9])min([\\s\\(])",
		"([^_a-zA-Z0-9])max([\\s\\(])",
		"view_([wxyh])view([\\s]*+[^\\[])", //"view_([wxyh])view(?!\\[)",//
		"view_([wxyh])port([\\s]*+[^\\[])",
		"view_angle([\\s]*+[^\\[])", 
		"view_object([\\s]*+[^\\[])",
		//"([^_a-zA-Z0-9])floor([\\s\\(])",
		"<>",
		" +",
		"(\\b)direction[ ]*[/]([^\\*/])",
		//"(\\b)speed(\\b)[/]",
		"[;]*\\)", //for with last ';' error
		"(\\b)begin(\\b)",
		"(\\b)end(\\b)",
		"(\\b)globalvar(\\b)",
		"//",
		"};",
		"\\.alarm"
		};
	static String othersReplacement[] = {")","view_xview","view_yview"," , ",
		"="," / ",
		//" % ",
		" $1 enigma_make_color",
		"! ",
		//"goto: switch\\(", //temp while switch statements aren't done to give readable error
		"$1",
		"external_define",
		"external_call",
		"$1 enigma_min $2",
		"$1 enigma_max $2",
		"view_$1view[0] $2",
		"view_$1port[0] $2",
		"view_angle[0] $1",
		"view_object[0] $1",
		//"$1 enigma_floor $2",
		"!=",
		" ",
		"((double)direction)/$2", //this is to fix ISO ambiguos error!
		//"((double)speed)/",
		")",
		"{",
		"}",
		"global var",
		" // ", //fixed //= invalid symbol
		"}",
		".fixme_alarm" //probably remove later on
	};//"floor($1/$2)" don't ever have //s here!
	
	public LGMUtilityMain() {
	createMenu();
	}
	
	
	public void createMenu() {
		JMenu menu = new JMenu("LGMUtility");
		regex = new JMenuItem("Apply regex to help fix compile errors");
		regex.addActionListener(this);
		menu.add(regex);
		findres = new JMenuItem("Find Resource");
		findres.addActionListener(this);
		menu.add(findres);
		runtimeregex = new JMenuItem("Apply helpful Regex for runtime problems");
		runtimeregex.addActionListener(this);
		menu.add(runtimeregex);
		LGM.frame.getJMenuBar().add(menu,1);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		Object s = e.getSource();
		if (s == regex) {
			doAllRegex();
		JOptionPane.showMessageDialog(null, "Finished regex!");
		LGM.commitAll();
		}
		if (s==findres) {
			searchForResource();
		}
		if (s==runtimeregex) {
			RuntimeRegex();
		}
	}
	
	public void RuntimeRegex() {
		
		//now apply regex to scripts
		applyregexToScripts(false);
		applyregexToObjects(false);
		applyRegexToRoomCreationCode(false);
		System.out.println("Finished Runtime regex!");
	}
	
	static String[] runtimeOriginal = {"brush_color","screen_width","screen_height"};
	static String[] runtimeReplacement = {"goto:ERROR draw_set_color(","window_get_region_width()","window_get_region_height()"};
	
	public static String replace_runtime_regex(String code,boolean argument) {
		/*
		 * These regex replacesments actually cause syntax errors in order to fix potential runtime problems
		 */
		code=" "+code+" "; //starting the code with space will help regex which start with //s
		
		for (int i=0; i< runtimeOriginal.length; i++) {
			code = code.replaceAll(runtimeOriginal[i], runtimeReplacement[i]);
		}
		
		//fix arguments
		if (argument==true) {
			if (code.length()>2)
			code=code.substring(1, code.length()-1); //remove the first and last space
			else code="";
		}
		
		return code;	
	}
	
	
	
	public void searchForResource() {
		int numberFound=0;
		String tolookfor = JOptionPane.showInputDialog(null, "What resource are you looking for? NAME [TYPE] \n Optional tags: [Contains] (can use regex)", 
				"obj_player [OBJECT]");
				  if(tolookfor == null) return;
				  String actuallylookfor=tolookfor.replaceAll("\\[[A-Z]*\\]", "").replaceAll(" ", "");
				  System.out.println("actuallylookfor:"+actuallylookfor);
				  actuallylookfor=actuallylookfor.toLowerCase();
				  if (tolookfor.contains("[OBJECT]")) {
					  System.out.println("Looking for objects");
					  //search objects
					  /*
						 * Objects
						 */
						org.lateralgm.resources.GmObject[] iol = LGM.currentFile.resMap.getList(org.lateralgm.resources.GmObject.class).toArray(new org.lateralgm.resources.GmObject[0]);
						for (int s = 0; s < iol.length; s++) {
							org.lateralgm.resources.GmObject io = iol[s];
							String name =  io.getName();
							name=name.toLowerCase();
							if (name.equals(actuallylookfor)) {
								System.out.println("name equals it so open!"+name);
								io.getNode().openFrame();
								numberFound++;
							}
							if (name.contains(actuallylookfor) && (tolookfor.contains("[CONTAINS]")||tolookfor.contains("[C]")||numberFound==0)) {
								System.out.println("name contans it so open!"+name);
								io.getNode().openFrame();
								numberFound++;
							}
							
						}
				  }
				  JOptionPane.showMessageDialog(null, "Finished searching, found "+numberFound+" resources");
	}
	
	public static void doAllRegex()
	{
		//before this fix resource names
		fixResourceNames();
		/*
		 * Idea is to first get all script names
		 */
		//now apply regex to scripts
		applyregexToScripts(true);
		applyregexToObjects(true);
		applyRegexToRoomCreationCode(true);
		System.out.println("Finished compile time regex!");
	}
	
	
	public static String replace_regex(String code,boolean argument) {
		code=" "+code+" "; //starting the code with space will help regex which start with //s
		if (!code.equals("  ") && argument == false) //if the code is nothing don't add the comment tags
		code+=" /* */ ";//a number of games don't close the last comment tags!
		
		for (int i=0; i< variables.length; i++) {
			code = code.replaceAll("\\b"+variables[i]+"\\b", " enigma___"+variables[i]+" ");
		}
		//Now fix functions
		for (int i=0; i< variables.length; i++) {
			code = code.replaceAll(" enigma___"+variables[i]+"([\\s]*[\\(])",variables[i]+"$1");//"\\b"+variables[i]+"\\b", " enigma___"+variables[i]+" ");
		}
		// now fix scripts
		//scriptnames
		for (String name : scriptnames) {
			code = code.replaceAll("\\b"+name+"([\\s]*[\\(])","script___"+name+"$1");
		}
		
		for (int i=0; i< othersOriginal.length; i++) {
			code = code.replaceAll(othersOriginal[i], othersReplacement[i]);
		}
		
		//fix arguments
		if (argument==true) {
			if (code.length()>2)
			code=code.substring(1, code.length()-1); //remove the first and last space
			else code="";
		}
		
		return code;	
	}
	
	
	
	public static void applyregexToScripts(boolean compileTime) {
		System.out.println("apply regex");
		
		org.lateralgm.resources.Script[] isl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Script.class).toArray(new org.lateralgm.resources.Script[0]);
		for (int s = 0; s < isl.length; s++)
			{
			//Script oo = osl[s];
			org.lateralgm.resources.Script io = isl[s];

			//oo.name = io.getName();
			//oo.id = io.getId();
			String code = io.get(PScript.CODE);
			if (compileTime) {
			code = replace_regex(code,false); } else {code=replace_runtime_regex(code,false);}
			io.setCode(code);
			}
	}
	
	public static void applyRegexToRoomCreationCode(boolean compileTime) {
		org.lateralgm.resources.Room[] irl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Room.class).toArray(new org.lateralgm.resources.Room[0]);
		for (int s = 0; s < irl.length; s++)
			{
			org.lateralgm.resources.Room is = irl[s];
			if (compileTime) {
			is.setCode(replace_regex(is.getCode(),false)); } else {is.setCode(replace_runtime_regex(is.getCode(),false));}
			}
	}
	
	public static String changeResourceName(String name) {
		if (name.equals("")) name="ENIGMA_REASOURCEWITHNONAME"+(int)(Math.random()*1000);
		//System.out.println("name:"+name);
		if(Character.isDigit(name.charAt(0))) name = "_DONTUSENUMBERS_"+name;
		if (name.equals("room")) name = "DONTCALLAROOMSIMPLYROOM_room";
		Vector<String> variablestouse = new Vector<String>();
		variablestouse.addAll(Arrays.asList(variables));
		variablestouse.add("break");
		variablestouse.add("switch");
		variablestouse.add("exit");
		variablestouse.add("floor");
		variablestouse.add("health");
		variablestouse.add("other");
		variablestouse.add("alarm");
		for (int i=0; i< variablestouse.size(); i++) {
			//code = code.replaceAll("([^_a-zA-Z0-9])"+variables[i]+"([^_a-zA-Z0-9])", "$1 enigma"+variables[i]+" $2");
				//name = name.replaceAll(variables[i], "enigma"+variables[i]);
			if(name.equals(variablestouse.get(i))) name = "enigma"+name;
			}
		return name;
	}
	
	public static void fixResourceNames() {
		
		Hashtable<String,String> resourcenames = new Hashtable<String,String>();
		
		String illegalRegex = "[:\\*'\\s!./\\(\\)\\-&//?%//+\"<>^\\\\]";
		String replacementString = "___";
		
		/*
		 * Objects
		 */
		//org.lateralgm.resources.GmObject[] iol = LGM.currentFile.gmObjects.toArray(new org.lateralgm.resources.GmObject[0]);
		org.lateralgm.resources.GmObject[] iol = LGM.currentFile.resMap.getList(org.lateralgm.resources.GmObject.class).toArray(new org.lateralgm.resources.GmObject[0]);
		for (int s = 0; s < iol.length; s++) {
			org.lateralgm.resources.GmObject io = iol[s];
			String name =  io.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			/*
			 * check if another object of the same name exists
			 */
			if (resourcenames.containsKey(name)) {
				System.out.println("uhoh! two objects with the same name >.<, it will rename the second to add a _id at the end");
				name+="_"+io.getId();
			} 
			resourcenames.put(name, io.getName()); //newname, oldname
			
			io.setName(name);
		}
		
		/*
		 * Sprites
		 */
		org.lateralgm.resources.Sprite[] isl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Sprite.class).toArray(new org.lateralgm.resources.Sprite[0]);
		for (int s = 0; s < isl.length; s++)
			{
			org.lateralgm.resources.Sprite is = isl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="spr_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		/*
		 * Sounds
		 */
		org.lateralgm.resources.Sound[] isndl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Sound.class).toArray(new org.lateralgm.resources.Sound[0]);
		for (int s = 0; s < isndl.length; s++)
			{
			org.lateralgm.resources.Sound is = isndl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="snd_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		/*
		 * Backgrounds
		 */
		org.lateralgm.resources.Background[] ibl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Background.class).toArray(new org.lateralgm.resources.Background[0]);
		for (int s = 0; s < ibl.length; s++)
			{
			org.lateralgm.resources.Background is = ibl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="bck_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		/*
		 * Paths
		 */
		org.lateralgm.resources.Path[] ipl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Path.class).toArray(new org.lateralgm.resources.Path[0]);
		for (int s = 0; s < ipl.length; s++)
			{
			org.lateralgm.resources.Path is = ipl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="pth_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		/*
		 * Fonts
		 */
		org.lateralgm.resources.Font[] ifl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Font.class).toArray(new org.lateralgm.resources.Font[0]);
		for (int s = 0; s < ifl.length; s++)
			{
			org.lateralgm.resources.Font is = ifl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="fnt_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			is.put(PFont.ANTIALIAS, 0);
			
			is.setName(name);
			}
		
		/*
		 * Timelines
		 */
		org.lateralgm.resources.Timeline[] itl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Timeline.class).toArray(new org.lateralgm.resources.Timeline[0]);
		for (int s = 0; s < itl.length; s++)
			{
			org.lateralgm.resources.Timeline is = itl[s];
			String name =  is.getName().replaceAll(illegalRegex, replacementString);
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="tl_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		
		/*
		 * Rooms
		 */
		org.lateralgm.resources.Room[] irl = LGM.currentFile.resMap.getList(org.lateralgm.resources.Room.class).toArray(new org.lateralgm.resources.Room[0]);
		for (int s = 0; s < irl.length; s++)
			{
			org.lateralgm.resources.Room is = irl[s];
			String name =  is.getName().replaceAll(illegalRegex, "_");
			name = changeResourceName(name);
			
			if (resourcenames.containsKey(name)) {
				name="room_"+name+"_"+is.getId();
			} 
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		/*
		 * duplicate names
		 * For Òsprite0Ó it checks if its an object first then sprite then sound then background then path then font then timeline then script then room
		 * So if I used sprite0 in a script it would always go for the object
		 */
		
		
		/*
		 * Scripts
		 */
		scriptnames = new Vector<String>();
		org.lateralgm.resources.Script[] irs = LGM.currentFile.resMap.getList(org.lateralgm.resources.Script.class).toArray(new org.lateralgm.resources.Script[0]);
		for (int s = 0; s < irs.length; s++)
			{
			org.lateralgm.resources.Script is = irs[s];
			String name =  is.getName().replaceAll(illegalRegex, "_");
			name = name.replaceAll("'", "___");
			
			/* Test adding script___ */
			scriptnames.add(name);
			name= "script___"+name;
			/* Finish test adding script___ */
			
			//changeResourceName(name); //why remove changeResourcename?
			
			/*if (resourcenames.containsKey(name)) {
				name="room_"+name+"_"+is.getId();
			} */
			resourcenames.put(name, is.getName()); //newname, oldname
			
			
			is.setName(name);
			}
		
		
	}
	
	
	
	public  static void applyregexToObjects(boolean compileTime) {
		org.lateralgm.resources.GmObject[] iol = LGM.currentFile.resMap.getList(org.lateralgm.resources.GmObject.class).toArray(new org.lateralgm.resources.GmObject[0]);
		for (int s = 0; s < iol.length; s++) {
			
			
			org.lateralgm.resources.GmObject io = iol[s];
			
			
			// Use this code instead to allow 0 main events
			// and switch GmObject.mainEvents to MainEvent.ByReference
			
			if (io.mainEvents.size() == 0) continue;

			
			

			// we assume there are never 0 main events
			// oo.mainEventCount = io.mainEvents.size();
			// oo.mainEvents = new MainEvent[oo.mainEventCount];
			for (int me = 0; me < io.mainEvents.size(); me++)
				{
				
				ArrayList<org.lateralgm.resources.sub.Event> iel = io.mainEvents.get(me).events;

				
				if (iel.size() == 0) continue;

				

				for (int e = 0; e < iel.size(); e++)
					{
					org.lateralgm.resources.sub.Event ie = iel.get(e);

					String code = getActionsCode(ie,compileTime);
					}
				}
		}
	}
	
	public static boolean actionDemise = false;
	
	public static String getActionsCode(ActionContainer ac,boolean compileTime)
	{
	final String nl = System.getProperty("line.separator");
	StringBuilder code = new StringBuilder();
	for (Action act : ac.actions)
		{
		LibAction la = act.getLibAction();
		if (la == null)
			{
			if (!actionDemise)
				{
				String mess = "Warning, you have a D&D action which is unsupported by this compiler."
						+ " This and future unsupported D&D actions will be discarded. (LibAction not found"
						+ " in moment/event " + ac.toString() + ")";
				JOptionPane.showMessageDialog(null,mess);
				actionDemise = true;
				}
			JOptionPane.showMessageDialog(null,"WARNING la == null");
			continue;
			}
		if (compileTime) {
		la.execInfo=replace_regex(la.execInfo,true); //used for custom actions?
		} else {la.execInfo=replace_runtime_regex(la.execInfo,true);}
		
		List<Argument> args = act.getArguments();
		
		boolean argument=true;
		if (la.actionKind==Action.ACT_CODE) argument=false;
		
		
		for (int i = 0; i < args.size(); i++)
		{
			if(compileTime) {
			args.get(i).setVal(replace_regex(args.get(i).getVal(),argument));
			} else {args.get(i).setVal(replace_runtime_regex(args.get(i).getVal(),argument));}
		}
		}
	
	return code.toString();
	}
	
	
	public static void main(String[] args)
	{
		boolean syntax=false;
	if (args.length != 1) {System.err.println("no input file"); System.exit(-1);}

	if (args[0].equals("-?") || args[0].equals("--help"))
		{
		System.exit(-1);
		}

	if (args[0].equals("-s") || args[0].equals("--syntax")) syntax = true;

	String outname = null;

	try
		{
		EnigmaCli.initailize(args[0],null);
		//Apply regex
				//LGM.currentFile=f;
				LGMUtilityMain.doAllRegex();
				//f=LGM.currentFile;
				
		if (syntax)
			EnigmaCli.syntaxChecker(LGM.currentFile,LGM.root);
		else
			EnigmaCli.compile(LGM.currentFile,LGM.root,outname);
		}
	catch (IOException e)
		{
		System.err.println(e.getMessage());
		}
	catch (GmFormatException e)
		{
		System.err.println(e.getMessage());
		}
	System.exit(0);
	}
	
	
}
