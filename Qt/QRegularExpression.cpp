// --- single match

/*
Look for "time=148.162 ms" in:

PING ya.ru (87.250.250.242): 56 data bytes\n64 bytes from 87.250.250.242: icmp_seq=0 ttl=52 time=148.162 ms\n\n--- ya.ru ping statistics ---\n1 packets transmitted, 1 packets received, 0.0% packet loss\nround-trip min/avg/max/stddev = 148.162/148.162/148.162/0.000 ms\n
*/

QString latency;
QRegularExpression re("time=\\d+\\.\\d+ \\w+");
QRegularExpressionMatch match = re.match(pingOutput);
if (match.hasMatch())
{
    latency = match.captured();
    qDebug() << latency; // time=148.162 ms
}

// --- groups match

/*
Look for "time=76ms" and "Lost = 0" in:

Pinging ya.ru [87.250.250.242] with 32 bytes of data:\r\nReply from 87.250.250.242: bytes=32 time=76ms TTL=52\r\n\r\nPing statistics for 87.250.250.242:\r\n    Packets: Sent = 1, Received = 1, Lost = 0 (0% loss),\r\nApproximate round trip times in milli-seconds:\r\n    Minimum = 76ms, Maximum = 76ms, Average = 76ms\r\n
*/

QString latency, lost;
QRegularExpression re("time(=|<)\\d+\\w+|Lost = \\d+");
QRegularExpressionMatchIterator i = re.globalMatch(pingOutput);
if (i.hasNext())
{
    latency = i.next().captured();
    qDebug() << latency;
    lost = i.next().captured();
    qDebug() << lost;
}