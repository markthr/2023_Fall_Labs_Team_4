### Prerequisites
Ensure that wlan0 is not needed for an internet connection. Either connect over wired, another external wifi adapter or forego internet connectivity.

Ensure that the target wifi adapter supports being ran as an access point (AP)

To get a list of devices enter the command `ifconfig`

Enter the command `iw list` and look for "Supported interface modes:" under the target device, in this case `wlan0` which is interchangeable with `phy0`
An entry should list `* AP`, if not this will not work.

### Installations

First, connect to the internet on an adapter that is not he one being configured. While this isn't necessary, it makes configuration much easier and maintians internet connectivity. This guide will assume that such a connection is available.

`sudo apt-get install dnsmasq`
`sudo apt-get install hostapd`
`sudo apt-get install iptables-persistent`

### Setup

#### Prevent the conflict over port 53 between dnsmasq DHCP server and systemd-resolved

edit `/etc/systemd/resolved.conf`to have the line `DNSStubListener=no`

No don't do this. Instead, add bind-interfaces to dnsmasq.conf

To restore internet run,  sudo dnsmasq

This line will likely be included in the file but commented out. Just uncomment it.
Make sure you set it to `no` as it will by default likely be `yes`.

Now, restart systemd-resolved
`sudo systemctl disable systemd-resolved`
`sudo systemctl stop systemd-resolved`
`sudo systemctl enable systemd-resolved`

#### Backup original config files

These files can be valuable to keep around for reference purposes

If a default hostapd.conf was distributed
`sudo mv /etc/hostapd/hostapd.conf /etc/hostapd/hostapd.OLD`

If a default dnsmasq.conf was distributed
`sudo mv /etc/dnsmasq.conf /etc/dnsmasq.OLD`

#### Download and place new conf files
Download or copy the file contents from REPO

Place dnsmasq.conf in /etc/ and hostapd.conf in /etc/hostapd

Edit the hostapd.conf to set the SSID and password

Run hostapd /etc/hostapd/hostapd.conf to test that the configuration file is working
if it starts with no errors, i.e. `AP-ENABLED` is output to the console, kill hostapd with [Ctrl-C] and move on to the next step.

### IP Configuration and Routing
`ifconfig wlan0 up 192.168.2.1 netmask 255.255.255.0`
`route add -net 192.168.2.0 netmask 255.255.255.0 gw 192.168.2.1`

Verify that an IP address has been assigned to `wlan0` with ifconfig
`inet 192.168.2.1` should appear beneath `wlan0` in the output.
Now start the services
`sudo dnsmasq -C /etc/dnsmasq.conf -d`
`sudo hostapd /etc/hostapd/hostapd.conf`

Use another device to verify that the network appears

https://unixhealthcheck.com/blog?id=449
## Configure port
### Check that the target port is not in use

`ss -na` will list the ports that are in use. Use `grep` to check if the output contains the port you wish to use. In this case, port 4000 will be used.
If the output of `ss -na | grep :4000` is nothing then port 4000 is good to use

Use iptables to accept UDP on port 4000
`iptables -A INPUT -p udp --dport 4000 -j ACCEPT`


https://www.digitalocean.com/community/tutorials/opening-a-port-on-linux

### Testing
#### Connect the client device

Ensure the client device connects.

Check what IP was assigned
`cat /var/lib/misc/dnsmasq.leases`

In this case, 192.168.2.130 was assigned. 
#### Send a UDP packet with netcat
`echo "Hello!" | nc -u -s 192.168.2.1 -p 4000 192.168.2.130 2390`

Executing this while running the demo on the client will result in the string "Acknowledged" being received and the client receiving "Hello!"

Verify this by checking the monitor terminal for the client and checking the response in the terminal on the Jetson.

### Make the configuration take effect on startup

#### Use iptables-persistent to save the port configuration
iptables-persistent will automatically restore (`iptables-restore`) the saved configuration on startup

Save your configuration with `sudo bash -c "iptables-save > rules.v4"`
The command is executed within a `sudo bash -c` in order to have permission for the output redirection

You can verify the changes by greping the file to search for the port configuration (in this case 4000)
cat /etc/iptables/rules.v4 | grep 4000

#### Configure startup behavior
Make dnsmasq begin on startup
`sudo systemctl enable dnsmasq`

Make hostapd begin on startup
`sudo systemctl unmask hostapd`
`sudo systemctl enable hostapd`