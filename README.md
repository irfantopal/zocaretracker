[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stars][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


<br />
<p align="center">

  <h3 align="center">ZoCareTracker</h3>

  <p align="center">
    A remote tracker for corona patients in home quarantine.
    <br />
    <a href="https://github.com/irfantopal/zocaretracker"><strong>Explore the project »</strong></a>
    <br />
    <br />
    <a href="https://zocaretracker.tk">Visit website</a>
    ·
    <a href="https://github.com/irfantopal/zocaretracker/issues">Report Bug</a>
    ·
    <a href="https://github.com/irfantopal/zocaretracker/issues">Request Feature</a>
  </p>
</p>

<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>

## About The Project

![ZoCareTrackerPhoto][zocaretracker-photo]

The corona patients in quarantine aren't monitored and a sudden death will be found after long time as it would look normal if they wouldn't go outside for a long time bacuse they're in the quarantine.

That' why i created a device which can
* monitor the patients pulse
* watch if the patient falls
* call emergency if the patients feels bad

These project was made for the application process of the Kulturakademi by me.

### Built With

* [Arduino](https://arduino.cc)
* [Laravel](https://laravel.com)

## Getting Started

You will need different microcontrollers and sensors, but don't worry they're cheap & found in starter packs. 

### Prerequisites

* Arduino Mega 2560 (You can also use something different)
* ESP8266 ESP-01S (This is just a basic one, also you can use something different too)
* AD8232 + ECG Pads
* GY-521 (MPU6050)
* Active Buzzer
* Breadboard & Breadboard Power Supply Module & Jumper wire
* 2x Button
* 2x 10k Ohm resistor
* 1k & 2k Ohm resistor (They're required for the voltage divider, you can also use a voltage regulator which is better)
* Linux server (You can use VM or ubuntu filesystem on windows, you need to be able to compile c++ and you also need the `libmysqlclient-dev` package)

Here is the schematic for the project (10 is 10k resistor, 11 is 1k and 12 is 2k). Please check again before powering it up, I'm not responsible for any broken parts.
![ZoCareTrackerSchematic][zocaretracker-schematic]

### Installation

1. Clone the repo or install the latest release if you don't want to build it yourself.
   ```sh
   git clone https://github.com/irfantopal/zocaretracker.git
   ```
4. Switch to client folder and install the code to your Arduino.
   ```ssh
   cd zocaretracker/client
   ```
   Edit the options in the file regarding your setup.
   Install the zocaretracker-client.ino to your Arduino using your favourite software.
3. Switch to server folder and build the server and later start it.
   I've added example code to save the data to a MySQL database, you can uncomment and use it, don't forget to change the credentials!
   ```sh
   cd ../server
   g++ zocaretracker-server.cpp -o server -lmysqlcppconn
   ./server
   ```
Now you're ready to go! Put on the ECG pads and you should see the data coming to your server soon.

## Usage

The pulse will be sent every minute. The dasboard to see the data will be added soon (check roadmap).

## Roadmap

Current features under development:
* Dashboard (est. 26.5.21)

Upcoming features:
* TCP alternative for more secure data transfer
* View live ECG graph (my ESP01S couldnt handle that much packets, I've ordered a new one and will report the status soon)
* Suggest more by creating an issue!

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Irfan Topal - [Founder of Legzo Team](https://legzo.irfantopal.com) - hello@irfantopal.com

Project Link: [https://github.com/irfantopal/zocaretracker/](https://github.com/irfantopal/zocaretracker/)


## Acknowledgements
* [Stiftung Kinderland](https://www.stiftung-kinderland.de/)
* [Socket Example](http://matrixsust.blogspot.com/)

[contributors-shield]: https://img.shields.io/github/contributors/irfantopal/zocaretracker.svg?style=for-the-badge
[contributors-url]: https://github.com/irfantopal/zocaretracker/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/irfantopal/zocaretracker.svg?style=for-the-badge
[forks-url]: https://github.com/irfantopal/zocaretracker/network/members
[stars-shield]: https://img.shields.io/github/stars/irfantopal/zocaretracker.svg?style=for-the-badge
[stars-url]: https://github.com/irfantopal/zocaretracker/stargazers
[issues-shield]: https://img.shields.io/github/issues/irfantopal/zocaretracker.svg?style=for-the-badge
[issues-url]: https://github.com/irfantopal/zocaretracker/issues
[license-shield]: https://img.shields.io/github/license/irfantopal/zocaretracker.svg?style=for-the-badge
[license-url]: https://github.com/irfantopal/zocaretracker/blob/main/LICENSE
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/irfantopal
[zocaretracker-photo]: images/photo-1.jpg
[zocaretracker-schematic]: images/photo-2.png