import subprocess
import shutil
from pathlib import Path

name="copy-depends"
version='1.2'
arch='x86_64'
url="https://github.com/qw0er/copyDepends#"
desc="Copy depends"
lic="MIT"
installDir=Path("AppDir")

cmdBase=['fpm',
         '-s','dir',
         '-n',name,
         '--license',lic,
         '-v',version,
         '-a',arch,
         '--description',desc,
         '--url',url,
         '--force'
         ]

files=[installDir/'usr/bin/copy-depends=/usr/bin/copy-depends',
       installDir/'usr/share/copy-depends/excludelist.txt=/usr/share/copy-depends/excludelist.txt']

subprocess.run(cmdBase+[
    '-t','rpm',
    '-d','boost-program-options'
]+files)

subprocess.run(cmdBase+[
    '-t','deb',
    '-d','libboost-program-options1.83.0'
]+files)

subprocess.run(['copy-depends','-e',installDir/'usr/bin/copy-depends',
               '-o',installDir/'usr/lib'])
shutil.copy('../other/AppRun',installDir)
shutil.copy('../other/copy-depends.desktop',installDir)
shutil.copy('../other/icon.png',installDir)
subprocess.run([Path.home()/'opt/appimagetool-x86_64.AppImage',installDir])
print("Created AppImage")