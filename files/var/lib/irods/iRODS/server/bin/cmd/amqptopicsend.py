#!/usr/bin/env python2.6
#
# This always exits with 0 status, as a work around for iRODS msiExecCmd microservice. To detect
# errors, monitor stderr.

import sys
import logging
import pika

try:
    logging.basicConfig(stream=sys.stdout)

    connParams = None
    ephemeral = True   # Indicates if the exchange is only temporary (for testing)
    key = ''
    body = '{}'

    if len(sys.argv) == 5:
        connParams = pika.URLParameters(sys.argv[1])
        ephemeral = sys.argv[2].lower() == 'true'  
        key = sys.argv[3]
        body = sys.argv[4]
    else:
        host = sys.argv[1]
        port = int(sys.argv[2])
        user = sys.argv[3]
        password = sys.argv[4]
        ephemeral = sys.argv[5].lower() == 'true'  
        key = sys.argv[6]
        body = sys.argv[7]

        connParams = pika.ConnectionParameters(host=host,
		      	                       port=port,
			                       credentials=pika.PlainCredentials(user, password)
	             )

    exchange = 'irods'

    connection = pika.BlockingConnection(connParams)
    channel = connection.channel()

    channel.exchange_declare(exchange=exchange,
                             exchange_type='topic',
                             durable=(not ephemeral),
                             auto_delete=ephemeral
    )

    channel.basic_publish(exchange=exchange,
                          routing_key=key,
                          body=body,
                          properties=pika.BasicProperties(delivery_mode=2)
    )

    connection.close()
except BaseException as e:
    sys.stderr.write('ERROR: %s\n' % e)
    sys.exit(1)
