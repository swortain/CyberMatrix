#ifndef NTP_TIME_H
#define NTP_TIME_H

#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// NTP Servers:
static const char ntpServerName[] = "0.cn.pool.ntp.org";  // static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
const int         timeZone        = 8;

const int NTP_PACKET_SIZE = 48;  // NTP time stamp is in the first 48 bytes of the message

void   Ntp_Init();
void   sendNTPpacket(IPAddress& address);
time_t getNtpTime();
void   NTPTime_Init();

#endif