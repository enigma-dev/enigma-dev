#!/usr/bin/python
import os, urllib, hashlib, sys,zipfile,shutil
print "Enigma package manager v1.0.0"
url="https://raw.github.com/amorri40/Enigma-packages/master/packages.md5"
webFile = urllib.urlopen(url)
updateText=webFile.read().split("\n")
webFile.close();

g_packageToInstall="main"
g_packageToShow=""
g_currentPackage="main"
g_OS=sys.platform

if (len(sys.argv) > 2): g_packageToShow=sys.argv[2]; g_packageToInstall="" #user wants to show packages in a category
elif (len(sys.argv) > 1): g_packageToInstall= sys.argv[1]

if g_packageToInstall.endswith("SDK"): g_packageToInstall+="-"+sys.platform #SDKs are platform specific
print "Installing "+g_packageToInstall+" please wait..."

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d): os.makedirs(d)
    
def extract_epackage(epackage):
    print "INFO: Extracting "+epackage
    savedPath = os.getcwd()
    os.chdir(os.path.dirname(epackage))
    z = zipfile.ZipFile(os.path.basename(epackage))
    
    for f in z.namelist():
        if f.endswith('/'):
            os.makedirs(f)
        else:
            z.extract(f)
    try: shutil.rmtree("__MACOSX")  #mac zips have a __MACOSX folder which needs to be removed
    except: pass      
    os.chdir(savedPath)

def downloadPackage(packageToInstall):
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
        
        if g_currentPackage == g_packageToShow: print str(show_iterator)+") "+packageName; show_iterator+=1
        
        if packageName != packageToInstall: continue
        
        #loop through dependencies and download them
        for dependency in packageDeps.split(","):
            if dependency == "none": print "INFO: no dependencies for "+packageName;break
            downloadPackage(dependency)
        
        try:
            ensure_dir("./"+packageLocalPath)
            localfile=open("./"+packageLocalPath, 'r')
            if hashlib.md5(localfile.read()).hexdigest() == packageHash: print "INFO: "+packageName + " already up-to-date (same hash)"; break #exit now that we have what we are looking for
            else: print "INFO: hash did not match (probably needs updated) localhash:"+hashlib.md5(localfile.read()).hexdigest()+ "remotehash:"+packageHash
        except IOError, err: "INFO: File doesn't exist so downloading:"+packageLocalPath
        webFile = urllib.urlopen(packageURL)
        localfile=open("./"+packageLocalPath, 'w')
        localfile.write(webFile.read())
        localfile.close()
        webFile.close()
        if packageLocalPath.endswith(".epackage"): extract_epackage("./"+packageLocalPath)
        
downloadPackage(g_packageToInstall)
print "Finished updating "+g_packageToInstall