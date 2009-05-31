#!/usr/bin/perl  -w
use strict;
use locale;

open (my $source, "< sample.ps") or die "CUPS_FILTER. ERROR: Can't open sample.ps: $!\n";
open (my $out, "> sample.ps1") or die "CUPS_FILTER. ERROR: Can't open sample.ps: $!\n";

my @block;

while(<$source>){
	if (/<start_data>/i.../<\/start_data>/i){
		if (m/^\%{2}<key_(.*?)>(.*?)<\/key_(.*?)>$/gi){
			#print $1,"-",$2,"-",$3,"\n";
		}
	}
	print $out $_;
}
close $source;
close $out;