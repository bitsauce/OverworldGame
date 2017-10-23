import os
import zipfile

zipf = zipfile.ZipFile('Overworld.zip', 'w', zipfile.ZIP_DEFLATED)

# Zip the binary files
binary_dir = "Binaries/Win32/Release"
for file in os.listdir(binary_dir):
    if file.endswith(".dll") or file.endswith(".exe"):
        zipf.write(os.path.join(binary_dir, file), file)

# Zip all files in Data/
for root, dirs, files in os.walk("Data"):
    for file in files:
        if not file.endswith(".psd"):
            path = os.path.join(root, file)
            zipf.write(path, path[5:])

zipf.close()
