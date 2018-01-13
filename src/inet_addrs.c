#include <ifaddrs.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

static char hex[16] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
};

ssize_t print_v4( uint8_t* a )
{
	char out[16];
	char *p = out;

	char d3, d2;

	for ( size_t i = 0; i < 4; ++i )
	{
		d3 = a[i] - ( a[i] % 100 );
		if ( d3 )
		{
			a[i] -= d3;
			*p = '0';
			while ( d3 )
			{
				++(*p);
				d3 -= 100;
			}
			++p;
			d3 = 1;
		}

		d2 = a[i] - ( a[i] % 10 );
		if ( d3 || d2 )
		{
			a[i] -= d2;
			*p = '0';
			while ( d2 )
			{
				++(*p);
				d2 -= 10;
			}
			++p;
		}

		*p = '0' + a[i];
		++p;
		*p = '.';
		++p;
	}
	*(p-1) = '\n';

	return write( 1, out, p-out );
}

ssize_t print_v6( uint8_t* a )
{
	char out[40];
	char* p = out;
	uint8_t nibble;

	for ( size_t i = 0; i < 16; ++i )
	{
		nibble = ( a[i] & 0xF0 ) >> 4;
		*p = hex[ nibble ];         ++p;
		*p = hex[ a[i] & 0xF ]; ++p;

		if ( i % 2 )
		{
			*p = ':';
			++p;
		}
	}

	--p;
	*p='\n';

	return write( 1, out, 40 ) == -1;
}

int main( int argc, char** argv )
{
	struct ifaddrs *ifaddr, *ifa;
	ssize_t i;
	char match_requested_interface_flag;
	char name_buffer[9];

	if ( getifaddrs( &ifaddr ) == -1 )
	{
		i = write( 2, "Unable to fetch/enumerate addresses\n", 37 );
		return 1;
	}

	for ( ifa = ifaddr; ifa; ifa = ifa->ifa_next )
	{
		if ( ifa->ifa_addr == NULL )
		{
			continue;
		}

		if ( ifa->ifa_addr->sa_family != AF_INET && ifa->ifa_addr->sa_family != AF_INET6 )
		{
			continue;
		}

		if ( argc > 1 )
		{
			match_requested_interface_flag = 0;

			for ( i = 1; i < argc; ++i )
			{
				if ( strcmp( argv[i], ifa->ifa_name ) == 0 )
				{
					match_requested_interface_flag = 1;
					break;
				}
			}

			if ( match_requested_interface_flag == 0 )
			{
				continue;
			}
		}

		// If there is one command line arguments, there is only one interface
		// in question, so we don't need to specify the interface name.
		// This makes the tool useful for scripts that want to know a 'primary' IP.
		if ( argc != 2 )
		{
			for ( i = 0; i < 9 && ifa->ifa_name[i]; ++i )
			{
				name_buffer[i] = ifa->ifa_name[i];
			}
			for ( ; i < 10; ++i )
			{
				name_buffer[i] = ' ';
			}

			i = write( 1, name_buffer, 9 );
		}

		switch ( ifa->ifa_addr->sa_family )
		{
			case AF_INET:
				print_v4( (void*)(&((struct sockaddr_in*)ifa->ifa_addr)->sin_addr.s_addr) );
				break;

			case AF_INET6:
				print_v6( (void*)(&((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr.s6_addr) );
				break;
		}
	}

	return 0;
}

