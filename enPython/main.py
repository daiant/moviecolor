from PIL import Image
import sys, os
from os import listdir
from os.path import isfile, join
def getFiles(dir):
	path = os.path.abspath(dir)
	list = sorted(os.listdir(dir))
	final = []
	for d in list:
		final.append(join(path,d))
	return final

def getAverage(dir):
	#Read image
	list = getFiles(dir)
	#print(list)
	index = 0;
	rgb_list = []
	for file in list:
		im = Image.open(file)

		rgb_im = im.convert('RGB')
		r,g,b = 0,0,0
		width, height = im.size
		count = 0
		for w in range(width):
			for h in range(height):
				a,u,x = rgb_im.getpixel((w,h))
				r += a
				g += u
				b += x 
				count+=1
		r = int(r/count)
		g = int(g/count)
		b = int(b/count)
		rgb_list.append([[index],[r,g,b]])
		index+=1
		print(rgb_list)
	print("Done reading")
def main():
	for arg in sys.argv:
		print(arg)
		if(arg!='main.py'):
			getAverage(arg)

main()