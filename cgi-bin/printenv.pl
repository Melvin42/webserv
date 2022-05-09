#!/usr/bin/perl

print "Content-type: text/html\r\nstatus: 200\r\n\r\n";
print "<font size=+1>Environment</font>\n";

foreach (sort keys %ENV) {
   print "<b>$_</b>: $ENV{$_}<br>\n";
}

1;