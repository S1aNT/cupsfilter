#!/usr/bin/perl
use DBI;

@drivers = DBI->available_drivers;
print "------------- drivers -----------\n";
for $i (@drivers) {
  print "$i\n";
}
print "---------------------------\n";