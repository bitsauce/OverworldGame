from shutil import copyfile
import glob

name = input("Set name: ").strip();
files = glob.glob("Default_*");
for file in files:
	copyfile(file, name + "_" + file[8:]);