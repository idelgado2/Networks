-- Networking Project -- 
COSC4342.001
Dr. Katangur
Project by Isaac Delagdo
-------------------------

Included in this directory submission are:

******source files******
balanceHeatPad.cpp
getTemperature.py
makefile
Networks_Project_Report (document)
NetworksProject_Presentation (power point)
Images (directory contating images for presentation)
This README.txt file
************************

The C++ program "balanceHeatPad.cpp" is the main program that will adjust the hating pad and collet data
from the DHT11 sensor for temperature and humidity

The Python program "getTemperature.py" will access the API from openweathermap.org to collect the current temperature
in Corpus Christi, TX.

**---(Note: These files will not work unless raspberry pi is set up with all the necessay hardware, like the heating
pad and the dht11 sensor, additionally if everything is setup correctly then make sure to input correct GPIOs in the 
according programs)---**

*****---What is working and what is not---*****
The sensor is working as well as the python program accessing the API to get the correct weather data. The heating pad
does not properly work as I could not figure out how to control 5 volts from the raspberry pi to give to the heating pad.
It would correctly work if attach the heating pad with one of the pins although it would not get hot enough as the pins 
only give off 3 volts. Attached in the image directory is a screen shot of the ouput of the program while manually attaching
the heating pad to the 5V and putting it next to the sensor

Additionally the CPP program can succesfully extract the correct weather data from the python program's request to the API.

(**---The Networks Report document in this directory will have more detail associated with this project--**) 
