ansible-cyverse-irods-cfg
=========================
[![Build Status](https://travis-ci.org/CyVerse-Ansible/ansible-cyverse-irods-cfg.svg?branch=master)](https://travis-ci.org/CyVerse-Ansible/ansible-cyverse-irods-cfg)

This is a role for configuring CyVerse iRODS 4 grids.


Requirements
------------

It is assumed that iRODS 4.1.10 is already installed on the server.

Role Variables
--------------

Variable                                  | Required | Default                          | Choices | Comments
----------------------------------------- | -------- | -------------------------------- | ------------------------------- | --------
`irods_icat_host`                         | no       | localhost                        |                                 | the FQDN of the IES
`irods_client_server_negotiation`         | no       | request_server_negotiation       | request_server_negotiation, off | indicates whether or not to use advanced negotiation
`irods_default_number_of_transfer_threads | no       | 4                                |                                 | the default maximum number of transfer streams for parallel transfer 
`irods_default_resource_directory`        | no       |                                  |                                 | the absolute path to the vault on the resource server being configured (N/A when configuring IES and it doesn't host a resource)
`irods_default_resource_name`             | no       | demoResc                         |                                 | the name of the default resource
`irods_negotiation_key`                   | no       | TEMPORARY_32byte_negotiation_key |                                 | the negotiation key
`irods_server_control_plane_key`          | no       | TEMPORARY__32byte_ctrl_plane_key |                                 | the server control plane key
`irods_parallel_transfer_buffer_size      | no       | 4                                |                                 | the transfer buffer size in MiB for each stream during parallel transfer
`irods_server_port_range_start`           | no       | 20000                            |                                 | the first address in the range of auxillary TCP and UDP ports
`irods_server_port_range_end`             | no       | 20199                            |                                 | the last address in the range of auxillary TCP and UDP ports
`irods_zone_key`                          | no       | TEMPORARY_zone_key               |                                 | the zone key
`irods_zone_user`                         | no       | rods                             |                                 | the rodsadmin user to be used by the server being configured
`irods_db`                                | no       |                                  |                                 | the DBMS connection information, see below (N/A for non-IES resource servers)
`irods_federation`                        | no       | []                               |                                 | a list of other iRODS zones to federate with, see below
`irods_amqp_uri`                          | no       |                                  |                                 | the AMQP URI used to connect to the broker (N/A for non-IES resource servers)
`irods_amqp_host`                         | no       | localhost                        |                                 | the FQDN of the AMQP broker iRODS publishes to (DEPRECATED: use `irods_amqp_uri instead`)
`irods_amqp_port`                         | no       | 5672                             |                                 | the port the AMQP broker listens on (DEPRECATED: use `irods_amqp_uri instead`)
`irods_amqp_username`                     | no       | guest                            |                                 | the AMQP user iRODS user (DEPRECATED: use `irods_amqp_uri instead`)
`irods_amqp_password`                     | no       | guest                            |                                 | the password for the AMQP user (DEPRECATED: use `irods_amqp_uri instead`)
`irods_amqp_ephemeral`                    | no       | true                             |                                 | whether or not the `irods` AMQP exchange will persist when iRODS disconnects from the AMQP broker
`irods_single_threaded_resources`         | no       | []                               |                                 | a list of resources that only support single threaded transfers
`irods_bisque_irods_host`                 | no       | `irods_icat_host`                |                                 | The iRODS host to report to BisQue.
`irods_sernec_owners`                     | no       | []                               |                                 | a list of users who get ownership of sernec collections
`irods_sernec_writers`                    | no       | []                               |                                 | a list of users who get write access to sernec collections
`irods_sernec_readers`                    | no       | []                               |                                 | a list of users who get read access to sernec collections


`irods_db` fields

Variable   | Required | Default | Choices | Comments
-----------| -------- | ------- | ------- | --------
`host`     | yes      |         |         | the FQDN of the DBMS hosting the ICAT
`port`     | yes      |         |         | the port the DBMS listens on
`username` | yes      |         |         | the DBMS user iRODS uses
`password` | yes      |         |         | the password for the DBMS user iRODS uses


`irods_federate fields

Variable          | Required | Default | Choices | Comments
------------------| -------- | ------- | ------- | --------
`icat_host`       | yes      |         |         | the hostname of the IES in the federate
`zone_name`       | yes      |         |         | the name of the federated zone
`zone_key`        | yes      |         |         | the shared authentication secret of the federate
`negotiation_key` | yes      |         |         | the 32-byte encryption key of the federate


Dependencies
------------

This role depends on [CyVerse-Ansible.irods-env](https://galaxy.ansible.com/CyVerse-Ansible/irods-env/) for the generation and deposition of the server's irods_environment.json file.


Example Playbook
----------------

Including an example of how to use your role (for instance, with variables passed in as parameters) is always nice for users too:

```
- hosts: irods-servers
  become_user: irods
  gather_facts: true
  roles:
    - role: cyverse-irods-cfg
      irods_amqp_uri: amqp://guest:guest@localhost:5672/%2F
      irods_db:
        host: localhost
        port: 5432
        username: irodsuser
        password: password
```           

License
-------

See license.md


Author Information
------------------

Tony Edgin
