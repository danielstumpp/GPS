import math
import Haversine

def main():
	tx_lat = 0
	tx_long = 0
	tx_alt = 0
	data_points = 0
	dist = 0

	filename = raw_input("Enter the filename: ")
	fix = raw_input("Care about fix (y/n)? ")

	# Read GPS data from user about receiver location
	rx_lat  = raw_input("Enter receiver latitude: ")
	rx_long = raw_input("Enter receiver longitude: ")
	rx_alt  = raw_input("Enter receiver altitude: ")

	file = open(filename, "r")
	for line in file:
		if line.startswith('Got'):
			data = line.split(',')

			# Exit if GPS didn't get a fix
			if (data[5] != 1) & (fix == "y"):
				print "Error: no fix"
				quit()
			
			# Read GPS data about transceiver location
			# tx_lat  = data[1] + data[2]
			# tx_long = data[3] + data[4]
			tx_lat = data[1]
			tx_long = data[3]
			tx_alt  = data[8]

			dist = calcDist(rx_lat, rx_long, rx_alt, tx_lat, tx_long, tx_alt)

		if line.startswith('RSSI'):
			data_points += 1
			print "Data point #" + str(data_points)
			print "Distance: " + str(dist) + ", " + line


# Calculate distance	
def calcDist(rx_lat, rx_long, rx_alt, tx_lat, tx_long, tx_alt):
	# TODO: perform distance calculation
	

	return -1
if __name__ == "__main__":
	main()