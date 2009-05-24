#!/usr/bin/perl  -w
use strict;
use DBI;

my $DEBUG=1;
my $RACF_UTIL_BIN;
my $ip="192.168.11.151";
my $printer_name="PDF";
my $mandat;

if ($DEBUG){
	$RACF_UTIL_BIN="racf.pl";
        $mandat=get_data_from_racf($ip,$printer_name);
print $mandat;
}else {
        $RACF_UTIL_BIN="/usr/bin/racf.pl";
}




##----------------------------------------------------------------------

sub get_data_from_racf{
    #Arg: printer_ip,printer_name
    #Returns Mandat
    my ($ip,$printer_name)=@_;
    my $pid = open (F,"$RACF_UTIL_BIN $ip $printer_name|") or die "Can't find and execute $!\n";
    my @res = <F>;
    #print @res;
    close (F);
    return $res[1]; 
}

