import serial
import struct

def setupSerial(baudrate, timeout):
    ser = serial.Serial('/dev/cu.usbmodem1421',baudrate,timeout=timeout)
    return ser

def npArrayToByteArray(array):
    bytearr = []
    for x in array:
        bytearr += struct.pack('f',x)
    return bytearr
    
def sendArrayToSerial(ser,array):
    bytearr = npArrayToByteArray(array.astype(float).flatten())
    rows = array.shape[0]
    columns = 1
    if (len(array.shape) > 1):
        columns = array.shape[1]
    ser.write(struct.pack('h',rows)+struct.pack('h',columns))
    ser.write(bytes(bytearr))

def recieveArrayFromSerial(ser):
    sizeOfInt = 2
    sizeOfFloat = 4
    rows = struct.unpack('h',ser.read(sizeOfInt))[0]
    cols = struct.unpack('h',ser.read(sizeOfInt))[0]
    buffer = ser.read(sizeOfFloat*rows*cols)
    array = []
    for index in range(0,len(buffer),4):
        array += [struct.unpack_from('f',bytes(buffer),index)[0]]
    return np.reshape(np.array(array),(rows,cols))

weights = dqn.layers[1].get_weights()[0]
bias = dqn.layers[1].get_weights()[1]
weights1 = dqn.layers[3].get_weights()[0]
bias1 = dqn.layers[3].get_weights()[1]
inputs = np.reshape(np.array([1.0,1.0,1.0,1.0]),(1,4))

ser = setupSerial(9600,10)
sendArrayToSerial(ser,weights)
ser.readline()
sendArrayToSerial(ser,bias)
ser.readline()
sendArrayToSerial(ser,weights1)
ser.readline()
sendArrayToSerial(ser,bias1)
ser.readline()
sendArrayToSerial(ser,inputs)
ser.readline()
ser.readline()
ser.readline()

recieveArrayFromSerial(ser)
ser.read_all()