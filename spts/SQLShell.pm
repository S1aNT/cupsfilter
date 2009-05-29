package SQLShell;
use strict;
use Carp;
use DBI;
use base qw[ISQLDriver];

BEGIN {
	our $VERSION   = '0.04';
	our $AUTHORITY = 'freelance:SLANT';
}

my $Count = 0;

sub new {
  my $proto = shift;                 # извлекаем им€ класса или указатель на объект
  my $class = ref($proto) || $proto; # если указатель, то вз€ть из него им€ класса
  $Count++;							 # содержаща€ количество существующих экземпл€ров класса	
  my $self  = {};
  $self->{DB_TRACE} =[];			 # –ежим отладки соединени€ с Ѕƒ log_level,filename
  $self->{OPTIONS} = [];			 # ссылка на безым€нный пустой массив
  $self->{DBH}=undef;				 # Pointer to database handler  	
  bless($self, $class);              # гибкий вызов функции bless
  return $self;
}

sub just_do { 
	my ($self,$func,$param)=@_;
    if (defined $self->{DBH}){
     	$self->{DBH}->begin_work();
     	my $p =undef;
     	
     	foreach my $item(@$param){
     		$item ="\'$item\'";
     	}
     	$p= join(",",@$param);
     	
    	my $q="SELECT $func($p)";
    	my $sth = $self->{DBH}->prepare($q);
        my $rv = $sth->execute();
        $self->{DBH}->commit();
         if (!defined $rv) {
         	my $e="SQLShell.pm:: When execute query $q:".$self->{DBH}->errstr."\n";
         	#$self->{DBH}->begin_work();
         	#$self->insert("debug_log","info_str",$self->{DBH}->quote($e));
         	#$self->{DBH}->commint();
            return ;
        }else{
            return $sth->fetchall_arrayref();# Returns pointer to array of result
        }
    }else{
    	croak("SQLShell.pm:: Can't execute sql query. Database not connected \n");
    }
	 
}

sub debug {
 my $self = shift;
  if (@_) { 
  	@{ $self->{DB_TRACE} } = @_;
  	#TODO Add validation log_level must from 1 to 7
  	if ($self->{DB_TRACE}->[0] < 0){
  		$self->{DB_TRACE}->[0]=0;
  	}
	if ($self->{DB_TRACE}->[0] > 7){
  		$self->{DB_TRACE}->[0]=7;
  	}
	if ((scalar(@{$self->{DB_TRACE}})== 2)&& defined $self->{DBH}){
  		$self->{DBH}->trace($self->{DB_TRACE}->[0],$self->{DB_TRACE}->[1]);
  	}	
	#TODO check file name or not what's the question ?
  }
  return $self->{DB_TRACE};
}

sub disconnect {
	my ($self) = @_;
	$self->{DBH}->disconnect() if defined($self->{DBH});
}
sub connect{
	my $self = shift;
	if (scalar(@{$self->{OPTIONS}})== 7){
		my ($dbname,$username,$password,$dbhost,$dbport,$dboptions,$dbtty)=@{$self->{OPTIONS}};
		#TODO возможно стоит сделать так - вначале коннект к базе по умолчанию postgres затем попытка смены базы
		$self->{DBH} = DBI->connect("dbi:PgPP:dbname=$dbname;host=$dbhost;port=$dbport;options=$dboptions;tty=$dbtty","$username","$password",{PrintError => 0,AutoCommit => 0});							
		#Test connection
		croak("SQLShell.pm::connect Erorr $DBI::errstr\n") unless (defined $self->{DBH});
  		#чтобы дата была в формате DD.MM.YYYY, а не как по умолчанию в формате ISO где год и мес€ц идут впереди числа.
		$self->{DBH}->do("SET DATESTYLE TO GERMAN");
	}else{
		croak("SQLShell.pm::connect Can't connect to BD. becouse arument's list not full");
	}
}

sub select {
    my($self, $fields,$table, $cond) = @_;
    #TODO add check $fields,$table,$cond
    if (defined $self->{DBH}){
    	$self->{DBH}->begin_work();
    	my $q="SELECT $fields FROM $table WHERE $cond";
    	my $sth = $self->{DBH}->prepare($q);
        my $rv = $sth->execute();
        $self->{DBH}->commit();
         if (!defined $rv) {
         	my $e="SQLShell.pm:: When execute query $q:".$self->{DBH}->errstr."\n";
            croak ($e);
        }else{
            return $sth->fetchall_arrayref();# Returns pointer to array of result
        }
    }else{
    	croak("SQLShell.pm:: Can't execute sql query. Database not connected \n");
    }
}
#TODO need function for packet's data insert, update(use ??? and prepare)
sub _doexec{# Private function !!!
	my ($self,$query)=@_;
    if (defined $self->{DBH}){
    	$self->{DBH}->begin_work();
    	my $rv = $self->{DBH}->do($query);
        $self->{DBH}->commit();
         if (!defined $rv) {
         	my $e="SQLShell.pm:: When execute query $query\n:".$self->{DBH}->errstr."\n";
            croak ($e);
        }else{
            return $rv;# Returns count records
        }
    }else{
    	croak("SQLShell.pm:: Can't execute sql query. Database not connected \n");
    }
}

sub delete{
	my($self,$table,$cond) = @_;
	#TODO add check $table,$cond
	my $q="DELETE FROM $table WHERE $cond";
	return $self->_doexec($q);
}
sub update{
	my($self,$table,$field,$value,$cond) = @_;
	#TODO add check $table, $fields,$values,$cond
	my $q="UPDATE $table SET $field='$value' WHERE $cond";
    return $self->_doexec($q);
}

sub insert {
    my($self, $table, $fields,$values) = @_;
    #TODO add check $table, $fields,$values
    my $q="INSERT INTO $table ($fields) VALUES (E$values)";
    return $self->_doexec($q);
}

sub options {
  my $self = shift;# 
  if (@_) { @{ $self->{OPTIONS} } = @_ }
  return $self->{OPTIONS};
}

sub DESTROY {
  $Count--;
}
sub total {
  return $Count;
}

END{
	#Dirty hack, but reliably!!! 
	SQLShell::disconnect();
}
1;  # дл€ функций require или use