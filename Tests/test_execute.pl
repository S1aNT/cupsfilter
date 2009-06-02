#!/usr/bin/perl -w
use DBI; 
use strict;
 
### The database handle 
my ($dbname,$username,$password,$dbhost,$dbport,$dboptions,$dbtty)=("CupsLog","postgres","pg","localhost","5432","-e","ansi");
my $dbh = DBI->connect("dbi:PgPP:dbname=$dbname;host=$dbhost;port=$dbport;options=$dboptions;tty=$dbtty","$username","$password",{PrintError => 0,AutoCommit => 0});
$dbh->trace(3,"test.log");							
### The statement handle

 
my $sth = $dbh->prepare("SELECT set_cups_log('PDF','slant77','ghf','5','SAVE_CUPS_DATA','','42')"); 
#my $sth = $dbh->prepare("SELECT id FROM cups_data_log");
### Execute the statement 
$sth->execute() || die ("Error $dbh->errstr()");
 
### Fetch all the data into a Perl data structure 
my $array_ref = $sth->fetchall_arrayref();
 
exit;