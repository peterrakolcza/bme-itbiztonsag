import os
import base64
import zlib
from zipfile import ZipFile

def decompress_layer(javaString, number):
    base64Strings = []
    for line in lines:
        if "stringBuilder.append" in line:
            temp = line[26:-4]
            base64Strings.append(temp)
    if len(base64Strings) == 0:
        return False

    base64String = ''.join(base64Strings)
    decompressed = zlib.decompress(base64.b64decode(base64String), 15)

    with open('Class' + str(number) + '.class', 'wb') as file_to_save:
        file_to_save.write(decompressed)
    
    return True

filename = "Class1"
i = 2
while (True):
    try:
        stream = os.popen('java -jar jd-cli.jar -oz ' + filename + ".zip " + filename + ".class")
        output = stream.read()
        
        ZipFile(filename + ".zip").extractall(os.getcwd())
        os.remove(filename + ".zip")
        os.remove(filename + ".class")

        with open(os.getcwd() + "/" + filename + ".java") as f:
            lines = f.readlines()
    
        if not decompress_layer(lines, i):
            print("Done")
            break

        print("Layer " + str(i) + " is decompressed...")
        filename = "Class" + str(i)
        i += 1
    except:
        print("Legyenek a jd-cli.jar és a Class1.class fájlok ugyanebben a mappában.")
        break
    
    
