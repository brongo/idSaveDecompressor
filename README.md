# idSaveDecompressor v0.1 by SamPT

idSaveDecompressor is a tool for decompressing DOOM Eternal saved games (game_duration.dat). This makes it possible to read and edit the file in a hex editor. 

This initial release is a proof of concept. Future releases will simplify the import/export process, and include support for editing/modifying the save data.

__*Important:*__ Make sure you have a backup of your save data. This tool does not take backups. You can easily corrupt your save data by using these tools incorrectly. I do not take responsibility for any file corruption or loss of data that may occur.

## Requirements

1. **DOOMSaveManager** - this tool must be used to "Export" the save before it can be decompressed. Download here: https://github.com/GoobyCorp/DOOMSaveManager/releases/tag/1.3.0.5
2. A licensed **Steam or Bethesda.net** copy of DOOM Eternal. *Other versions may not work with DOOMSaveManager*
3. **oo2core_8_win64.dll** - you can find this file in your DOOM Eternal installation directory. It must be copied to the same directory as idSaveDecompressor.exe.

## Usage 

### To Export & Decompress Your Save:

1. Run "DOOM Save Manager.exe" and choose the "Export" option. Save the .zip archive.
2. Extract the .zip archive. Find "game_duration.dat" in the extracted files (there is one for each autosave).
3. Copy "game_duration.dat" to the same directory as idSaveDecompressor.
4. Drag & drop the "game_duration.dat" file onto "idSaveDecompressor_Auto.bat" - pay attention to the console message and verify it says *"file unpacked succesfully"*.
5. The file is now ready for viewing or editing in a hex editor.


### To Compress & Reimport:

1. Drag & drop the decompressed "game_duration.dat" file onto "idSaveDecompressor_Auto.bat" - pay attention to the console message and verify it says *"file compressed succesfully"*. Your file is now compressed and ready for reimport.
2. Copy the compressed "game_duration.dat" file back to its original location in the .zip archive - it should go in one of the autosave folders. 
3. Run "DOOM Save Manager.exe" and choose the "Import" option. Find the .zip archive containing your saved data and import the saves. You should see a message that says "Import Success!"
4. Play DOOM Eternal.

### To Import a PS4 Save to PC:

Note: This requires the use of PS4 Save Wizard (~$50) to export your saved game data on PS4.

1. Open your PS4 save using Save Wizard. Using Advanced Mode, export the entire file and save it on your PC. Copy the file to the same folder as idSaveDecompressor, and rename it as "game_duration.dat"
2. Drag & drop the "game_duration.dat" file onto "idSaveDecompressor_Auto.bat" - pay attention to the console message and verify it says *"file compressed succesfully"*. Your file is now compressed and ready for reimport.
3. Create a saved game on PC to use as your "dummy" save. This file will be replaced with the PS4 save. Make note of which autosave slot it is in, otherwise you might overwrite the incorrect save.
4. Run "DOOM Save Manager.exe" and choose the "Export" option. Save the .zip archive. 
5. Open the .zip archive, open the autosave folder containing the data you want to replace. Copy/paste your PS4 save from the idSaveDecompressor folder to this one (overwrite the "game_duration.dat" file with the one from your PS4). 
6. Run "DOOM Save Manager.exe" and choose the "Import" option. You should see a message that says "Import Success!"
7. Open DOOM Eternal. If done correctly, the autosave slot will still show your "dummy" data, but it will load your PS4 save with the correct progress. The autosave info will be correct the next time you load it.

## Command Line

If you prefer command line, you can remove the .bat file and use this command:
```
idSaveDecompressor.exe [options] source dest

	Options:
	-c	Compresses an uncompressed game_duration.dat file
	-d	Decompresses a compressed game_duration.dat file
```

## Viewing / Editing the decompressed file

game_duration.dat is a binary file and will require the use of a hex editor for viewing. I personally recommend 010 Editor, which is free to try for 30-days. https://www.sweetscape.com/010editor/.

This repository includes a 010 Editor template to make the file easier to read. The file structure is fully understood and outlined in this template. See `templates/DOOMSaveGame_Decompressed.bt`

## Troubleshooting

Most likely these files will trigger anti-virus warnings. However, they are safe to download and run.

**If you are unable to export files using DOOMSaveManager:**

1. Verify that you have a supported copy of the game. DOOMSaveManager currently supports the Steam or Bethesda.net versions of the game.
2. Make sure the files "BouncyCastle.Crypto.dll" and "ICSharpCode.SharpZipLib.dll" are in the same directory as "DOOM Save Manager.exe"
3. If your saved games are located in a non-standard directory, DOOMSaveManager may be unable to find them (if this happens, it will show an empty dropdown list). The program expects to find Bethesda.net saves in `C:\Users\%Username%\Saved Games\id Software\DoomEternal\base\savegame\`. If you have a Steam copy of the game, the save files should be in `C:\<SteamInstallDirectory>\userdata\%steamID%\782330\remote\`
4. DOOMSaveManager may not properly export your .zip files to a directory that requires elevated privileges, such as `C:\Program Files\`. Try exporting to a different folder (Desktop should work fine).

**If you see errors when using idSaveDecompressor:** 

1. Make sure you used DOOMSaveManager to "export" the file first. idSaveDecompressor will not work unless the game_duration.dat file is exported through DOOMSaveManager.
2. Open the game_duration.dat file in a hex editor. You should see the word "SlotFile" within the first 40 bytes. If this word is not visible, then the file was not decrypted successfully by DOOMSaveManager.
3. If you receive the error, `"Failed to open oo2core_8_win64.dll"` - please make sure the file oo2core_8_win64.dll is located in the same directory as idSaveDecompressor.exe. You can copy this file from your DOOM Eternal install directory.

All other errors, please make a note of the exact error message you received, and send the error message + a copy of the file to @SamPT#9513 on Discord. 

## Credits:
- Thank you to chrispy for identifying the hash function used to validate the saves.
- Thank you to GoobyCorp (https://github.com/GoobyCorp) for his project DoomSaveManager.
- Thank you to powzix (https://github.com/powzix) for his project powzix/ooz - which helped me to troubleshoot the Oodle decompression and work out the format.
- Thank you to proteh for his idFileDecompressor, which was initially used as a code template for this project.
