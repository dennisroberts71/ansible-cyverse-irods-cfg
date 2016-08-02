# VERSION 7
#
# IPC specific AMQP rules

# Sends a message to a given AMQP topic exchange
#
# PARAMETERS:
#   *Key - The topic of the message
#   *Msg - The message to send
#
ipc_amqpSend(*Key, *Msg) {
  if (ipc_AMQP_HOST != '') {
    *hostArg = execCmdArg(ipc_AMQP_HOST);
    *portArg = str(ipc_AMQP_PORT);
    *userArg = execCmdArg(ipc_AMQP_USER);
    *passwordArg = execCmdArg(ipc_AMQP_PASSWORD);
    *ephemeralArg = str(ipc_AMQP_EPHEMERAL);
    *keyArg = execCmdArg(*Key);
    *msgArg = execCmdArg(*Msg);
    *argStr = '*hostArg *portArg *userArg *passwordArg *ephemeralArg *keyArg *msgArg';
    *status = errorcode(msiExecCmd('amqptopicsend.py', *argStr, ipc_RE_HOST, 'null', 'null', *out));
    if (*status != 0) { 
      writeLine("serverLog", "Failed to send AMQP message");
      fail;
    } else {
      msiGetStderrInExecCmdOut(*out, *errMsg);
      if (strlen(*errMsg) > 0) {
        writeLine("serverLog", "Failed to send AMQP message: *errMsg");
        fail;
      }
    }
  }
}

