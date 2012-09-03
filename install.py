#!/usr/bin/python
# This python script runs in both Python 2.6 and Python 3+
# Please make sure any changes are valid for both versions of Python
import os, hashlib, sys,zipfile,shutil, getopt

try:
   from urllib.request import urlopen
   from urllib.parse import urlparse
except ImportError:
   from urlparse import urlparse
   from urllib import urlopen

print("Enigma package manager")
url="https://raw.github.com/enigma-dev/Enigma-packages/master/packages.md5"
webFile = urlopen(url)
updateText=webFile.read().decode().split('\n')
webFile.close();

g_packageToInstall="main"
g_packageToShow=""
g_currentPackage="main"
g_OS=sys.platform
g_globalInstall=False
g_showOnly=False
g_installLocation="./"

optlist, args = getopt.getopt(sys.argv[1:], 'gh',['show=','global','help'])

if len(args)>0: g_packageToInstall=args[0]
for o, a in optlist:
        if o == "--show": #only show the packages 
            g_packageToInstall=""; g_packageToShow=a; g_showOnly=True
        if o == "--global" or o == "-g": #install this globally for this user to .ENIGMA
            g_globalInstall=True; g_installLocation=os.getenv('HOME')+"/.ENIGMA/"
        if o == "--help" or o == "-h": #show the help
            print ("To see all packages use --show=all \nTo install globally use --global (useful for large sdks) \nTo see all packages in a category use --show=categoryname")
            sys.exit(0)

if g_packageToInstall.endswith("SDK"): g_packageToInstall+="-"+sys.platform #SDKs are platform specific
if not g_showOnly: print ("Installing "+g_packageToInstall+" please wait...")

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d): os.makedirs(d)
    
def extract_epackage(epackage):
    print ("INFO: Extracting "+epackage)
    savedPath = os.getcwd()
    os.chdir(os.path.dirname(epackage))
    z = zipfile.ZipFile(os.path.basename(epackage))
    
    for f in z.namelist():
        if f.endswith('/'):
            try:
               os.makedirs(f)
            except: print ("already exists, overwriting")
        else:
            z.extract(f)
    try: shutil.rmtree("__MACOSX")  #mac zips have a __MACOSX folder which needs to be removed
    except: pass      
    os.chdir(savedPath)

def downloadPackage(packageToInstall):
    global g_currentPackage
    show_iterator=1 #only used with the show argument to neatly print out number) packagename
    for package in updateText:
        if package.startswith("#Category:"): g_currentPackage=package.split(" ")[0][10:]; continue
        if (len(package) < 1) or package.startswith("#"): continue
        
        #split the pakage into its components
        packageProperties=package.split(" ")
        packageName = packageProperties[0]
        packageHash = packageProperties[1]
        packageLocalPath = packageProperties[2]
        packageURL = packageProperties[3]
        packageDeps = packageProperties[4]

        if g_currentPackage == g_packageToShow or g_packageToShow=='all': print (str(show_iterator)+") "+packageName); show_iterator+=1
        
        if packageName != packageToInstall: continue
        
        #loop through dependencies and download them
        for dependency in packageDeps.split(","):
            if dependency == "none": print ("INFO: no dependencies for "+packageName);break
            downloadPackage(dependency)
        
        try:
            ensure_dir(g_installLocation+packageLocalPath)
            localfile=open(g_installLocation+packageLocalPath, 'rb')
            file_contents=localfile.read()
            if hashlib.md5(file_contents).hexdigest() == packageHash: print ("INFO: "+packageName + " already up-to-date (same hash)"); break #exit now that we have what we are looking for
            else: print ("INFO: "+packageName+" hash did not match (probably needs updated) localhash:"+hashlib.md5(file_contents).hexdigest()+ " remotehash:"+packageHash)
        except IOError as err: "INFO: File doesn't exist so downloading:"+packageLocalPath
        webFile = urlopen(packageURL)
        localfile=open(g_installLocation+packageLocalPath, 'wb')
        localfile.write(webFile.read())
        localfile.close()
        webFile.close()
        if packageLocalPath.endswith(".epackage"): extract_epackage(g_installLocation+packageLocalPath)
        
downloadPackage(g_packageToInstall)
if g_showOnly: print ("Finished showing all packages for category: "+g_packageToShow)
else: print ("Finished updating "+g_packageToInstall)
