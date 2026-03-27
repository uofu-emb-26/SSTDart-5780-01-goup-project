import argparse
import serial
import matplotlib.pyplot as pyplot

parser = argparse.ArgumentParser()
parser.add_argument('device', help='Serial port, e.g. /dev/ttyUSB0')
parser.add_argument('--baud', type=int, default=115200)
args = parser.parse_args()

port = serial.Serial(args.device, baudrate=args.baud)
port.reset_input_buffer()

print('Sending sweep command...')
port.write(b's')

frequencies = []
magnitudes = []
i = 0

while True:
    line = port.readline().decode().strip()
    if line == 'END':
        break
    frequency, magnitude = line.split()
    frequencies.append(int(frequency))
    magnitudes.append(int(magnitude))
    print(f'  {i} {frequency} Hz -> {magnitude}')
    i += 1

port.close()

pyplot.plot(frequencies, magnitudes)
pyplot.xlabel('Frequency (Hz)')
pyplot.ylabel('Magnitude (ADC counts)')
pyplot.title('Sweep Response')
pyplot.tight_layout()
pyplot.show()
