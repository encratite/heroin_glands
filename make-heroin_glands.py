import os
import sys

if len(sys.argv) >= 2:
	offset = int(sys.argv[1])
else:
	offset = 0

library_directory = "heroin"
library_name = "libheroin_glands.a"
output_directory = library_directory + "/object"

try:
	os.mkdir(output_directory)
except:
	pass

try:
	os.remove(library_name)
except:
	pass

object_list = " "

i = 1
for file in os.listdir(library_directory):
	if len(file) < 4:
		continue
	extension = file[-3 : ]
	if extension != "cpp":
		continue
	#print "Run: " + str(i) + " ," + file
	base = file[0 : -4]
	object_path = output_directory + "/" + base + ".o"
	command = "g++ -c -g -I . " + library_directory + "/" + base + ".cpp -o " + object_path
	object_list += " " + output_directory + "/" + base + ".o"
	if offset == 0 or i == int(offset):
		print "Executing " + command
		os.system(command)
	i += 1

if offset == 0:
	command = "ar cq " + library_name + object_list
	print "Executing " + command
	os.system(command)

