# File-Systems

Project developed in c implementing a program that reads the metadata of filesystems of type fat16 or ext2.

To compile the project run "make" and execute the fsutils executable generated. It has to be run with the parameter --info/--tree and the filesystem to analyze or the parameter --cat, the filesystem and the filename of a file in the filesystem(). Note that cat only reads files in fat filesystems. 

Aditionally, filesystems are suposed to be in the same folder as the executable, so to run the code you just write the filesystem name as the filesystem. Otherwise, if you want to store the filesystems in folder inside the folder of the executable, then you'll have to pass as filesystem parameter: "folder/filesystem_name"

Example: ./fsutils --info libfat
         ./fsutils --tree lolext
         ./fsutils --cat libfat lastlog.h
