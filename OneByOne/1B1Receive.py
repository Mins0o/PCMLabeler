import serial
from os import listdir

def ChooseDevice():
    devices = [dev for dev in listdir("/dev/") if ((dev[:4] == "ttyS" or dev[:4] == "ttyU"))]
    for deviceNumber in range(len(devices)):
        print(deviceNumber,end="\t")
        print(devices[deviceNumber])
    selection=int(input("Choose your serial device\n>>> "))
    while (selection<0 or selection>len(devices)):
        selection=int(input("Choose a valid number from the list\n>>> "))
    baudRate=230400
    try:
        baudRate=int(input("What is the baud rate? ex) 230400"))
    device = serial.Serial("/dev/"+devices[selection],baudRate)
    return device
	
def RecordData(device):
	"""Pseudocode-
	file=Create_File()
	Clear_Input_Residue
	Until specified:
		if Start_Signal_Received
			Until End_Signal_Received
				Read_by_length_3
				store_datapoints
			lable=Get_Label_from_user
			
			Write datapoints,label to file
			Clear_Input_Residue
		Loop until specified
	"""
	fileName= input("Title name to save data ex)data1\n>>> ")+".tsv"
    with open("./"+fileName,'a') as dataFile:
        # Flush any incomplete transmission in the buffer.
        while (device.in_waiting>0):
            rawRead=device.reset_input_buffer()
        print("Flushed Buffer")
		
        rawRead=None
		pcmData=[]
		while True:
            if device.in_waiting>0:
                rawRead=device.readline()
            else:
                time.sleep(0.5)
            # Checking the start of a transmission
            if(len(rawread)==4):
                print("Recording")
				
				# Until End-signal, read serial
				while (len(rawread)<5):
					
					# Nothing in the buffer but transmission hasn't ended yet
					while device.in_waiting==0:
						time.sleep(0.1)
						
					# device.in_waiting!=0
					rawread=device.readline()
					# b'10' is read as EOL and causes mismatched byte length so read until 3 bytes are in.
					while len(rawread)<3:
						rawread+=device.read()
						
					pcmData.append(int.from_bytes(rawread[:2],"big"))
					
				label=input("Label in one letter\nAdd another letter to finish recording\nYou can skip by not inputting any letters\n>>> ")
				
				if len(label)==1:
					# The last of the pcmData is end signal (b'0x00 0x00 0x00..' of len 5) so we exclude it.
					for i in pcmData[:-2]: 
						dataFile.write("{0},".format(i))
					try:
						dataFile.write(str(pcmData[-2]))
					except IndexError:
						print("File write failed. len(pcmData)={0}",format(len(pcmData)))
					dataFile.write("\t{0}\n".format(label))
				elif len(label)>1: 
					return
				# Flush anything that came in during user interface
				while device.in_waiting>0:
					device.reset_input_buffer()
				print("Waiting for input...")
				pcmData=[]
		