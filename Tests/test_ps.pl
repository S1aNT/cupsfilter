#!/usr/bin/perl  -w
use strict;
use locale;
use Encode;
use Encode 'from_to';
#use Unicode::String qw(utf8 latin1 utf16be);

sub from_digit2utf{
	#Arg: digit_str example 1069;;1090;;1086;;32;;1084;;1086;;1081;;32;;1076;;1086;;1082;;1091;;1084;;1077;;1085;;1090;;44;;32;;1089;;1090;;1088;;46;;321
	#Returns: wide char string
	my ($str)=@_;
	return pack("U*",split (';;',$str));
	 
}

my $str ="1069;;1090;;1086;;32;;1084;;1086;;1081;;32;;1076;;1086;;1082;;1091;;1084;;1077;;1085;;1090;;44;;32;;1089;;1090;;1088;;46;;321";

print from_digit2utf($str);



 
