# Running the Network Demo
## Installation
If you have not installed the local files, first follow the tutorial [here](Networking/environment)

## Setup
First, connect to the internet on a network adapater aside from `wlan0` which will be used for the hotspot
In the wifi settings menu, this will likely appear as the second option as shown below. This can be verified by checking the devices with `ifconfig`.
![alt text](Image_Embeds/multiple-adaptors-wifi-linux.png)
Do not connect using `wlan0` which corresponds to "Realtek RTL8822CE" on thje Jetson Xavier NX. This adapter will be used for hosting the hotspot.

## Starting up the Host Network
The commands for configuring routing and ports for the Jetson are contained in [jetnet-start.sh](Networking/environment/jetnet-start.sh). Execute this script using:
```
sudo bash jetnet-start.sh
```

This should make a network appear when checked on another device. In the case of this project, the network is named `JetNet*` where the `*` is a wildcard that is
replaced in each instance to prevent conflict e.g. `JetNet2`.

## Configure the Network for the Client Device
The demo code expects a header named `passwd.h` to declare the SSID and password for the hotspot. This file is not committed for security reasons.
Create a header file named `passwd.h` in the same directory as [main.cpp](Networking/src/main.cpp) For this project, an esp32-v2 client is used.

This header should have form:
```
#ifndef __PASSWD_H__
#define __PASSWD_H__

#include<string>

std::string wifi_ssid = "__PLACEHOLDER__";
std::string pass = "__PLACEHOLDER__";

#endif
```
Replace the `__PLACEHOLDER__` with the real values. 

The demo code can then be built and uploaded using platformio.

## Get the Client IP
To get the client IP, check what IPs have been leased out (assigned) by the DHCP server
```
cat /var/lib/misc/dnsmasq.leases
```
If this shows multiple devices, the MAC address of currently connected devices can be found by entering `hostapd_cli all_sta` in ther terminal. 
The lease information will display which devices are a esp32-v2 so each esp32-v2 can be manually pinged to narrow down devices.
Another option is to check the leases before each device is connected to see which IP is added for the new device.

## Sending Packets from the Host
Note: For this section, an IP of 192.168.2.130 will be assumed as the IP of the client device.

Running the project without interaction from the host (this project uses a Jetson Xavier NX) will just show the network and status on the OLED of the device.

To initiate sending a packet, use netcat, `nc`, to send a packet and then receive a response using the following command
```
echo "Hello!" | nc -u -s 192.168.2.1 -p 4000 192.168.2.130 2390
```

* `-u` indicates that UDP is to be used instead of TCP
* `-s 192.168.2.1` specifies the source address for the packet which is the address set in `jetnet-startup.sh`
* `-p 4000` specifies the source port which is the port opened in `jetnet-startup.sh`
* `192.168.2.130 2390` specifies the target address and port for the client receiving the packet with the IP set by the DHCP server and the port specified in `main.cpp` of the demo code.
