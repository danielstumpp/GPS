def main():
	tx_lat = 0
	tx_long = 0
	tx_alt = 0
	data_points = 0
	dist = 0

	filename = raw_input("Enter the filename: ")

	# Read GPS data from user about receiver location
	rx_lat  = input("Enter receiver latitude: ")
	rx_long = input("Enter receiver longitude: ")
	rx_alt  = input("Enter receiver altitude: ")

	file = open(filename, "r")
	for line in file:
		if line.startswith('Got'):
			data = line.split(',')

			# Exit if GPS didn't get a fix
			# if data[5] != 1:
			# 	print "Error: no fix"
			# 	quit()
			
			# Read GPS data about transceiver location
			tx_lat  = data[1] + data[2]
			tx_long = data[3] + data[4]
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