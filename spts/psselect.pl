#!/usr/bin/perl  -w
use strict;
use feature "switch";

my @argv = @ARGV;			#keep hands off the argument array

my $i=0;
while ($i < 1000){
	$i++;
	print STDERR $argv[0],"\n";
}