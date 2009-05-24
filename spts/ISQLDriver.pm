# Это интерфейс. Все функции интерфейса должны быть реализованы.
# Иначе - кто-то сдохнет. 
package ISQLDriver;
use Carp;
use strict;

our $VERSION   = '0.01';
$VERSION = eval $VERSION;
our $AUTHORITY = 'freelance:SLANT';
#TODO Написать вянтное сообщения об ошибке и может быть глобальный обработчик ошибок ?
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
1;
