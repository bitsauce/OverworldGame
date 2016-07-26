from shutil import copyfile
import glob

name = input("Set name: ").strip();
files = glob.glob("Apparel/" + name + "_*.png");
for file in files:
	copyfile(file, file[9+len(name):]);