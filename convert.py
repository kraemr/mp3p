## This is a simple script that you can use to convert for example /home/someusername/Music/somefile.mp3 
## to for example $MUSICDIR/somefile.mp3, where $MUSICDIR represents an env variable that contains a path 
## obviously the env variable should exit otherwise your playlists wont work anymore
import sys
if(len(sys.argv)<3):
    print("you need to supply a file that you want to change the string you want to replace and what should be replaced by it")
    print("Like This python3 convert.py yourplaylist.json '/home/hackerman/Music' '$MUSIC' ")
    sys.exit(0)
path=sys.argv[1]
toreplace=sys.argv[2]
replaceval=sys.argv[3]
f=open(path,"r")
str = f.read()
f.close()
str = str.replace(toreplace,replaceval)
f=open(path,"w")
f.write(str)
f.close()