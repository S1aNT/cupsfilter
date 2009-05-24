#!/usr/bin/perl  -w
use strict;
use feature "switch";

my @argv = @ARGV;			#keep hands off the argument array

my ($printer_ip,$printer_name,$set_mandat)=@argv;
sub help{
    print ("usage racf: racf PRINTER_IP PRINTER_NAME\n\n");
    print ("Example: racf 192.168.11.150 HPLaserJet1120\n\nOutput:\t192.168.11.150HPLaserJet1120\n\tSYSPROG\n");
}

sub get_mandat{
    my $m=int (rand(5));
    my $mandat;
    given ($m){
	when (1) {
	  $mandat = "SYSPROG";
	}
	when (2) {
	  $mandat = "TOPSECRET";
	}
	when (3) {
	  $mandat = "SECRET";
	}
	when (4) {
	  $mandat = "CONFIDENT";
	}
	when (5) {
	  $mandat = "PUBLIC";
	}
	default {
          $mandat = "PUBLIC";
	}
    }
    return $mandat;

}
# ARG CHECKING
# If no arguments, device discovery mode
if (!$argv[0]){
        help();
        exit 1;
}
# If too many arguments, send error
if (scalar(@argv) >3 ){
        print "To many arguments\n";
        help();
        exit 1;
}
unless ($set_mandat){
	print "$printer_ip$printer_name\n",get_mandat();
}else{
	print "$printer_ip$printer_name\n$set_mandat";
}

=head1 AUTHOR

For all intents and purposes, blame:

Paul Mosentsev  C<< <p.slant@gmail.com> >>

=head1 COPYRIGHT AND LICENSE

Copyright 2009 Paul Mosentsev

This program is free software; you can redistribute it and/or modify it
under the same terms as Perl itself.

=cut
