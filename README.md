# Cppmp3
A Simple mp3player api for C++ with basic json playlists and configuration.
With some cool examples and a fully functional mp3player with Opengl Shader visualization.

# Building and Setup
Install make on your system

```bash
make
```
## Linux
On linux systems you can add your env values to your .bashrc or .zshrc or similiar.

```bash
export CPPMP3_USERDATA="Your/Custom/Path"
export CPPMP3_PLAYLISTS="Your/Custom/Path"
```

## Windows
Set the values in the registry.
Here is a guide: <br>
https://phoenixnap.com/kb/windows-set-environment-variable



## How to add music to a playlist?
Just drag and drop mp3 or wav files into the window with an empty playlist open.

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
the gui mp3player takes this file to configure itself.
You are able to change the settings in the file or in the gui.
playlist_dirs and music_dirs take absolute paths and paths containing env values.
So something like "music_dirs":["$HOME/Music"] would lead to /home/user/Music on linux.
```json
{
    "playlist_dirs":[

    ],
    "music_dirs":[

    ],
    "nanodelay":10000000,
    "fps":60,
    "initial-screen-size-x":800,
    "initial-screen-size-y":450,
    "start-maximized":true,
    "start-fullscreen":false
}
```