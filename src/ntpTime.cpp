/*
 * @功能:
 * @Author: swortain
 * @Date: 2020-08-08 14:31:52
 * @LastEditTime: 2023-03-01 12:55:24
 */

#include "ntpTime.h"

WiFiUDP      Udp;
unsigned int localNtpPort = 2390;               // local port to listen for UDP packets
byte         ntpPacketBuffer[NTP_PACKET_SIZE];  // buffer to hold incoming and outgoing packets

void Ntp_Init() {
    Serial.println("Starting UDP");
    Udp.begin(localNtpPort);
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
    Serial.println("sending NTP packet...");
    // set all bytes in the buffer to 0
    memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    ntpPacketBuffer[0] = 0b11100011;  // LI, Version, Mode
    ntpPacketBuffer[1] = 0;           // Stratum, or type of clock
    ntpPacketBuffer[2] = 6;           // Polling Interval
    ntpPacketBuffer[3] = 0xEC;        // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    ntpPacketBuffer[12] = 49;
    ntpPacketBuffer[13] = 0x4E;
    ntpPacketBuffer[14] = 49;
    ntpPacketBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123);  // NTP requests are to port 123
    Udp.write(ntpPacketBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

/*-------- NTP code ----------*/
time_t getNtpTime() {
    IPAddress ntpServerIP;  // NTP server's ip address

    while (Udp.parsePacket() > 0)
        ;  // discard any previously received packets
    // get a random server from the pool
    WiFi.hostByName(ntpServerName, ntpServerIP);
    sendNTPpacket(ntpServerIP);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1600) {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
            Udp.read(ntpPacketBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)ntpPacketBuffer[40] << 24;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[41] << 16;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[42] << 8;
            secsSince1900 |= (unsigned long)ntpPacketBuffer[43];
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    return 0;  // return 0 if unable to get the time
}

void NTPTime_Init() {
    Ntp_Init();

    setSyncProvider(getNtpTime);
    setSyncInterval(360);
    if (hour() == 0 && second() < 3) {
        setSyncProvider(getNtpTime);
    }
}