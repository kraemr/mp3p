# Cppmp3
A Simple mp3-player written in C++ with basic json playlists and configuration.

# Building and Setup
Make sure to Install make on your system
```bash
git clone --recurse-submodules https://github.com/kraemr/mp3p
cd mp3p
make
```
## Linux
On linux systems you can add your env values to your .bashrc or .zshrc or similiar.

```bash
export CPPMP3_USERDATA="Your/Custom/Path"
export CPPMP3_PLAYLISTS="Your/Custom/Path"
```

## Windows
Set the values in the registry OR on startup
Here is a guide: <br>
https://phoenixnap.com/kb/windows-set-environment-variable

## How to add music to a playlist?
Just drag and drop mp3 or wav files into the window with a playlist open.

## How to add an image to a new playlist?
When you have the new playlist window open, then just drag and drop your image.

## How to edit a playlist?
Press the edit button or edit the json file directly

## playlist.json
```json
{
    "name":"playlist_name",
    "songs":[
        {"songname":"some custom name","description":"","path":"some/absolutepath"},{"songname":"some custom name","path":"$SOME_ENV_VARIABLE"}
    ]
}
```


## settings.json
the mp3player takes this file to configure itself.
You are able to change the settings in the file or in the gui.

playlist_dirs and music_dirs take absolute paths and paths containing env values.
So something like "music_dirs":["$HOME/Music"] would lead to /home/user/Music on linux.

## migrating to new pc
its recommended to have all your music in a root directory with subdirectories.
Like so

```
-Music
--Metal
---Metallica
----Song1
----Song2
--Pop
--- ...
--Random
...
```

To change absolute paths in a playlist.json to migrate it you can use the convert.py script like so:
```

```




