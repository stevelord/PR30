#!/bin/sh
# default firewall rules

WAN="$iface"
#echo WAN="$iface"

start_firewall() {
  echo "0" > /proc/sys/net/ipv4/ip_forward
  iptables -P OUTPUT  DROP
  iptables -P INPUT   DROP
  iptables -P FORWARD DROP
  for T in filter mangle; do
    iptables -t $T -F
    # iptables -t $T -X
  done
  echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout
  echo 1800 > /proc/sys/net/ipv4/tcp_keepalive_intvl
  #iptables -t nat -A POSTROUTING -o $WAN -j MASQUERADE
  iptables -A INPUT   -m state --state ESTABLISHED,RELATED -j ACCEPT
  iptables -A OUTPUT  -m state --state ESTABLISHED,RELATED -j ACCEPT
  iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
  iptables -A INPUT   -p tcp ! --syn -m state --state NEW -j DROP
  iptables -A OUTPUT  -p tcp ! --syn -m state --state NEW -j DROP
  iptables -A FORWARD -p tcp ! --syn -m state --state NEW -j DROP
  iptables -A INPUT -i lo -j ACCEPT
  iptables -A INPUT ! -i $WAN -p icmp --icmp-type 8/0 -m limit --limit 4/second --limit-burst 5 -j ACCEPT
  iptables -A OUTPUT -m state --state NEW -j ACCEPT
  # allow all LAN routed connections through router
  iptables -A FORWARD ! -i $WAN -m state --state NEW -j ACCEPT
  # allow all new routed connections outbound to internet
  iptables -A FORWARD -o $WAN -m state --state NEW -j ACCEPT 
  iptables -A INPUT ! -i $WAN -p tcp --dport 22 -m state --state NEW -j ACCEPT 
  iptables -A INPUT ! -i $WAN -p udp --dport 53 -m state --state NEW -j ACCEPT
  iptables -A INPUT ! -i $WAN -p udp --dport 67:68 -m state --state NEW -j ACCEPT
  iptables -A INPUT ! -i $WAN -p tcp --dport 80 -m state --state NEW -j ACCEPT 
  kill -16 `pidof MonitorWan`
  echo "1" > /proc/sys/net/ipv4/ip_forward
}

stop_firewall() {
  echo "0" > /proc/sys/net/ipv4/ip_forward
  iptables -P OUTPUT ACCEPT
  iptables -P INPUT ACCEPT
  iptables -P FORWARD ACCEPT
  for T in filter mangle; do
    iptables -t $T -F
    # iptables -t $T -X
  done
  
  echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout
  echo 75 > /proc/sys/net/ipv4/tcp_keepalive_intvl
  #iptables -t nat -A POSTROUTING -o $WAN -j MASQUERADE
  iptables -A FORWARD -o $WAN -j ACCEPT
  iptables -A INPUT -p tcp --dport 22 -i "$iface" -j DROP
  iptables -A INPUT -p tcp --dport 80 -i "$iface" -j DROP
  kill -16 `pidof MonitorWan`

  echo "1" > /proc/sys/net/ipv4/ip_forward
}

# web use
up_firewall() {
   Q=`ifconfig | grep $iface`
   WANUP=$?
   FWUP=`uci get firewall.defaults.enable`
   if [ $WANUP ]; then
     if [ $FWUP = "1" ]; then
      echo "enable firewall"
      start_firewall
     fi
   fi
}

# web use
down_firewall() {
   Q=`ifconfig | grep $iface`
   WANUP=$?
   FWUP=`uci get firewall.defaults.enable`
   if [ $WANUP ]; then
     if [ $FWUP = "0" ]; then
      echo "disable firewall"
      stop_firewall
     fi
   fi
}

# init use
start_init(){
   FWUP=`uci get firewall.defaults.enable`
   if [ $FWUP = "1" ]; then
     start_firewall
   fi
}

# init use
stop_init(){
   FWUP=`uci get firewall.defaults.enable`
   if [ $FWUP = "1" ]; then
     stop_firewall
   fi
}

case $1 in
  start)
      start_init;;
  stop)
      stop_init;;
  enable)
      up_firewall;;
  disable)
      down_firewall;;
      *)
      echo unknown value;;
esac
