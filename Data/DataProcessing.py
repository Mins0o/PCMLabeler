from os import listdir
import csv
	
def TsvToData(filePath):
	"""This method reads .tsv file and returns the data, XY split in a tuple"""
	with open(filePath,'r') as file:
		lines=list(csv.reader(file,delimiter='\t'))
		data=[[int(x) for x in line[0].split(',')] for line in lines]
		label=[line[1] for line in lines]
	return(data,label)
	
def LoadFiles(filePath=None):
	"""This method returns a dictionary of data which are divided into X and Y.
	For example, if <file1.tsv>, <file2.tsv>, <file3.tsv> are loaded,
	{"file1.tsv" : <data1>, "file2.tsv" : <data2>, "file3.tsv" : <data3>}
	where each <data#> is formatted as ([data_0, ..., data_n], [label_0, ..., label_n])"""
	if filePath==None:
		path=input("What is the path of your data folder?\n>>> ")
	else:
		path=filePath
	dataFiles = [file for file in listdir(path) if file[-4:]==".tsv"]
	for fileNum in range(len(dataFiles)):
		print("{0:02d}\t{1}".format(fileNum,dataFiles[fileNum]))
	selections=[int(x) for x in input("Type in indices of files, each separated by spacing\n>>> ").split()]
	filesDict={}
	for selection in selections:
		filesDict[dataFiles[selection]]=TsvToData(path+"\\"+dataFiles[selection])
	return(filesDict)

def TruncateToMin(dataCollection):
	"""This method matches the length of the data by cutting off the tails of longer files"""
	print("<<<TruncateToMinLength() in progress>>>")
	# Get minimum length and file name of it
	minLength=9999999
	fileName=""
	for name in dataCollection:
		data=dataCollection[name][0]
		for singleDataStream in range(len(data)):
			if len(data[singleDataStream])<minLength:
				minLength=len(data[singleDataStream])
				fileName="{0},Line {1}".format(name,singleDataStream)
	userAnswer=""
	while not(userAnswer.lower()=="y" or userAnswer.lower()=="n"):
		userAnswer=input("The minimum length is {0} from {1}. Would you like to truncate the data?(Y/N)\n>>> ".format(minLength,fileName))
	if userAnswer.lower()=="y":
		output=([],[])
		for dataFile in dataCollection:
			for i in range(len(dataCollection[dataFile][0])):
				output[0].append(dataCollection[dataFile][0][i][:minLength])
				output[1].append(dataCollection[dataFile][1][i])
	return output

def SaveData(data,filePath=None):
	"""This method saves an XY-split data into a tsv file"""
	if filePath==None:
		path=input("What is the path of your data folder?\n>>> ")
	else:
		path=filePath
	with open(path+"\\Truncated.tsv",'w') as file:
		for lineNumber in range(len(data[0])):
			file.write(",".join([str(x) for x in data[0][lineNumber]])+"\t"+data[1][lineNumber]+"\n")
	print("Saved the truncated and combined file")
	
	
if (__name__=="__main__"):
	filePath=input("What is the path of your data folder?\n>>> ")
	SaveData(TruncateToMin(LoadFiles(filePath)),filePath)
	
	
