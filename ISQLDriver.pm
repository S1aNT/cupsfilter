package ISQLDriver;
use Carp;
use strict;
use utf8;

# Это интерфейс. Все функции интерфейса должны быть реализованы.
# Иначе - кто-то сдохнет. 
#TODO Написать вянтное сообщения об ошибке и может быть глобальный обработчик ошибок ?

our $VERSION   = '0.02';
$VERSION = eval $VERSION;
our $AUTHORITY = 'freelance:SLANT';

sub connect { 
	croak ("I'm pure!"); 
};
sub select { 
	croak "I'm pure!" 
}
sub insert { 
	croak "I'm pure!" 
}

sub update { 
	croak "I'm pure!" 
}

sub delete { 
	croak "I'm pure!" 
}

sub disconnect { 
	croak "I'm pure!" 
}
sub _doexec { 
	croak "I'm pure!" 
}

sub get_func_res { 
	croak "I'm pure!" 
}

1;
