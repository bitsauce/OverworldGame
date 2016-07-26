import shutil
import glob

name = input("Set name: ").strip();
files = glob.glob("NewSet/*");
for file in files:
	shutil.copyfile(file, name + "_" + file[7:]);