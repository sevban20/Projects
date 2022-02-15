# Projects
It's about Sevban Kocael's projects. New projects will add.

## ARDUINO

### Fateining Farm Project
This is Arduino Nano-based project. It has 4 buttons to control menu operations. It can work in 2 modes: Automatic feed and cleaning bed of animals like cattle, sheep, etc., another mode is that manual feed and clean. It has 2 silos to store food. Every silo has its own motor that discharges food to the mixer. Mixer prepares food. After that, the conveyor starts feeding.
It has a cleaning unit to clean the ground. In addition, it has an RTC module to keep clock info. The RTC module is powered by a CR2032 battery. DHT22 sensor was used to measure humidity and temperature.To keep fresh air in the barn were used fans that control by temperature.The board has 2 power inputs, 5V, and 12V. 6 pieces of N-channel MOSFETs have been used to power the motors.

## ESP

### Home Automation Board
This is ESP8266 based project. It can control 4 pcs relays. Every relay also has an LED display to show work situations. To detect light density it has a photocell. To measure temperature and humidity to placed DHT22 sensor. It can power up to 5V-36V. The power unit of the board is based on LM2596S. It is going to connect the firebase real-time database so it can control by mobile application or any remote-controllable app.

## LED Dimmer
It is a led dimmer using LM555. There are one INPUT and one OUTPUT. It works with 12V. The brightness is adjustable with a 50K potentiometer. The maximum current is 3A.To control led used TO-220 package N channel MOSFET-IRFZ44N-.

## Web Scraping
### Get Jobs
This is a search job from timesjob.com. A user defines keyword and other options, the script search jobs from the website and shows all of them to the user. The input choices are Keyword, Number of pages, and location experience. If the user wants to save results, there is an option to save JSON format. <br />
Keyword(Required): Jobs keywords <br /> 
Number of Pages(Optional): How many pages will list
Location(Optional): Job's location
Experience(Optional): About Years
### Get Last Topic
This script is working for that when the new topic opens the donanimhaber sicak firsatlar, the script gets the topic immediately. The user can see the last opened topic. If a day passed the topic's opened date the topic automatically will delete.
