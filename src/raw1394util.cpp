
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libavc1394/avc1394.h>
#include <libavc1394/avc1394_vcr.h>
#include <libavc1394/rom1394.h>

#include "raw1394util.h"


/** Open the raw1394 device and get a handle.
 *  
 * \param port A 0-based number indicating which host adapter to use.
 * \return a raw1394 handle.
 */
int raw1394_get_num_ports( void )
{
	int n_ports;
	struct raw1394_portinfo pinf[ 16 ];
	raw1394handle_t handle;

	/* get a raw1394 handle */
	if ( !( handle = raw1394_new_handle() ) )
	{
		fprintf( stderr, "raw1394 - failed to get handle: %s.\n", strerror( errno ) );
		exit( EXIT_FAILURE );
	}

	if ( ( n_ports = raw1394_get_port_info( handle, pinf, 16 ) ) < 0 )
	{
		fprintf( stderr, "raw1394 - failed to get port info: %s.\n", strerror( errno ) );
		raw1394_destroy_handle( handle );
		exit( EXIT_FAILURE );
	}
	raw1394_destroy_handle( handle );

	return n_ports;
}

/** Open the raw1394 device and get a handle.
 *  
 * \param port A 0-based number indicating which host adapter to use.
 * \return a raw1394 handle.
 */
raw1394handle_t raw1394_open( int port )
{
	int n_ports;
	struct raw1394_portinfo pinf[ 16 ];
	raw1394handle_t handle;

	/* get a raw1394 handle */
#ifdef RAW1394_V_0_8

	handle = raw1394_get_handle();
#else

	handle = raw1394_new_handle();
#endif

	if ( !handle )
	{
		fprintf( stderr, "raw1394 - failed to get handle: %s.\n", strerror( errno ) );
		exit( EXIT_FAILURE );
	}

	if ( ( n_ports = raw1394_get_port_info( handle, pinf, 16 ) ) < 0 )
	{
		fprintf( stderr, "raw1394 - failed to get port info: %s.\n", strerror( errno ) );
		raw1394_destroy_handle( handle );
		exit( EXIT_FAILURE );
	}

	/* tell raw1394 which host adapter to use */
	if ( raw1394_set_port( handle, port ) < 0 )
	{
		fprintf( stderr, "raw1394 - failed to set set port: %s.\n", strerror( errno ) );
		exit( EXIT_FAILURE );
	}

	return handle;
}


void raw1394_close( raw1394handle_t handle )
{
	raw1394_destroy_handle( handle );
}

int discoverAVC( int* port, octlet_t * guid )
{
	rom1394_directory rom_dir;
	raw1394handle_t handle;
	int device = -1;
	int i, j = 0;
	int m = raw1394_get_num_ports();

	if ( *port >= 0 )
	{
		/* search on explicit port */
		j = *port;
		m = *port + 1;
	}

	for ( ; j < m && device == -1; j++ )
	{
		handle = raw1394_open( j );
		for ( i = 0; i < raw1394_get_nodecount( handle ); ++i )
		{
			if ( *guid != 0 )
			{
				/* select explicitly by GUID */
				if ( *guid == rom1394_get_guid( handle, i ) )
				{
					device = i;
					*port = j;
					break;
				}
			}
			else
			{
				/* select first AV/C Tape Reccorder Player node */
				if ( rom1394_get_directory( handle, i, &rom_dir ) < 0 )
				{
					fprintf( stderr, "error reading config rom directory for node %d\n", i );
					continue;
				}
//				fprintf(stderr, avc1394_vcr_decode_status(avc1394_vcr_status(handle, i)));
//				fprintf(stderr, "\n");
//				fprintf(stderr, rom_dir.label);
//				fprintf(stderr, "\n");
//				fprintf(stderr, "%d, %d\n", rom1394_get_node_type(&rom_dir), ROM1394_NODE_TYPE_AVC);
//				fprintf(stderr, "%d, %X\n", avc1394_check_subunit_type(handle, i, /*AVC1394_SUBUNIT_TYPE_VCR*/AVC1394_SUBUNIT_TYPE_VIDEO_CAMERA), AVC1394_SUBUNIT_TYPE_VIDEO_CAMERA);
//				fprintf(stderr, "%d, %X\n", avc1394_check_subunit_type(handle, i, AVC1394_SUBUNIT_TYPE_VCR), AVC1394_SUBUNIT_TYPE_VCR);
//				fprintf(stderr, "%X, %X, %X, %X\n", rom_dir.node_capabilities, rom_dir.unit_spec_id, rom_dir.unit_sw_version, rom_dir.model_id);
				if ( ( rom1394_get_node_type( &rom_dir ) == ROM1394_NODE_TYPE_AVC ) /*&&
				        avc1394_check_subunit_type( handle, i, AVC1394_SUBUNIT_TYPE_VCR)*/ )
				{
					*guid = rom1394_get_guid( handle, i );
					fprintf( stderr, "Found AV/C device with GUID 0x%08x%08x\n",
						(quadlet_t) (*guid>>32), (quadlet_t) (*guid & 0xffffffff));
					device = i;
					*port = j;
					break;
				}
			}
		}
		raw1394_close( handle );
	}

	return device;
}
