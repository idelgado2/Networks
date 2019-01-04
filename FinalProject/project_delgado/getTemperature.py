import requests
import time

while True:
	print('\n** python progrma fetching weather from API **\n')

	r = requests.get('http://api.openweathermap.org/data/2.5/weather?id=4683416&appid=e505f7789beab0ee47922e2215d25186')
	json_object = r.json()
	temp_k = float(json_object['main']['temp']) #strip only the temperature
	temp_f = round(((temp_k * 1.8) - 459.67), 2) #convert  kelvins to fahrenheit

	file = open("file.txt", "w")
	file.write(str(temp_f));

	file.close()

	time.sleep(60);	#grabs temperature every minute ---> should realistically be longer (maby every 10 minutes)


