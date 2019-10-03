from PIL import Image, ImageDraw
import sys, os
from os import listdir
from os.path import isfile, join
from decimal import Decimal
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
	print("Reading thumbnails...")
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
		percent = Decimal((index*100.0)/len(list))
		print("{}% read".format(round(percent,2)))
	print("Done reading")
	return rgb_list
def writeImg(list, outfile):
	w, h = int(len(list)/1.77), len(list)
	frame = h/len(list)
	img = Image.new("RGB", (w,h))
	draw = ImageDraw.Draw(img)
	print("Started drawing...")
	for i in range(len(list)) :
		r,g,b = list[i][1][0],list[i][1][1],list[i][1][2]
		draw.rectangle((0,i,w,i+1),fill=(r,g,b))

	img.show()
	img.save(outfile)
	print("Saved in {}".format(outfile))
def main():
	list = getAverage(sys.argv[1])
	writeImg(list, os.path.abspath(sys.argv[2]))


main()
