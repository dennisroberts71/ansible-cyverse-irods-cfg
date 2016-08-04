ansible-cyverse-irods-cfg
=========================
[![Build Status](https://travis-ci.org/CyVerse-Ansible/ansible-cyverse-irods-cfg.svg?branch=master)](https://travis-ci.org/CyVerse-Ansible/ansible-cyverse-irods-cfg)

This is a role for configuring CyVerse iRODS 4 grids.


Requirements
------------

It is assumed that iRODS 4.1.9 is already installed on the server.

Role Variables
--------------

Variable                           | Required | Default                          | Choices | Comments
---------------------------------- | -------- | -------------------------------- | ------- | --------
`irods_icat_host`                  | no       | localhost                        |         | the FQDN of the IES
`irods_default_resource_directory` | no       |                                  |         | the absolute path to the vault on the resource server being configured (N/A when configuring IES and it doesn't host a resource)
`irods_default_resource_name`      | no       | demoResc                         |         | the name of the default resource
`irods_negotiation_key`            | no       | TEMPORARY_32byte_negotiation_key |         | the negotiation key
`irods_server_control_plane_key`   | no       | TEMPORARY__32byte_ctrl_plane_key |         | the server control plane key
`irods_server_port_range_start`    | no       | 20000                            |         | the first address in the range of auxillary TCP and UDP ports
`irods_server_port_range_end`      | no       | 20199                            |         | the last address in the range of auxillary TCP and UDP ports
`irods_zone_key`                   | no       | TEMPORARY_zone_key               |         | the zone key
`irods_zone_user`                  | no       | rods                             |         | the rodsadmin user to be used by the server being configured
`irods_db_host`                    | no       | localhost                        |         | the FQDN of the DBMS hosting the ICAT (N/A for non-IES resource servers)
`irods_db_port`                    | no       | 5432                             |         | the port the DBMS listens on (N/A for non-IES resource servers)
`irods_db_username`                | no       | irods                            |         | the DBMS user iRODS uses (N/A for non-IES resource servers)
`irods_db_password`                | no       | testpassword                     |         | the password for the DBMS user iRODS uses (N/A for non-IES resource servers)
`irods_amqp_host`                  | no       | localhost                        |         | the FQDN of the AMQP broker iRODS publishes to
`irods_amqp_port`                  | no       | 5672                             |         | the port the AMQP broker listens on
`irods_amqp_username`              | no       | guest                            |         | the AMQP user iRODS user
`irods_amqp_password`              | no       | guest                            |         | the password for the AMQP user
`irods_amqp_ephemeral`             | no       | true                             |         | whether or not the `irods` AMQP exchange will persist when iRODS disconnects from the AMQP broker
`irods_single_threaded_resources`  | no       | []                               |         | a list of resources that only support single threaded transfers


Dependencies
------------

Example Playbook
----------------

Including an example of how to use your role (for instance, with variables passed in as parameters) is always nice for users too:

```
- hosts: irods-servers
  become: true
  gather_facts: true
  roles:
    - role: cyverse.ansible-cyverse-irods-cfg
      irods_amqp_username: irods
      irods_amqp_password: crack-me
```           

License
-------

BSD

Author Information
------------------

Tony Edgin
