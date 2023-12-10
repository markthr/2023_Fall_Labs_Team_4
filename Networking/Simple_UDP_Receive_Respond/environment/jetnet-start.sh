ifconfig wlan0 up 192.168.2.1 netmask 255.255.255.0 &&
	route add -net 192.168.2.0 netmask 255.255.255.0 gw 192.168.2.1

iptables -A INPUT -p udp --dport 4000 -j ACCEPT

dnsmasq -C /etc/dnsmasq.conf

hostapd /etc/hostapd/hostapd.conf -B
