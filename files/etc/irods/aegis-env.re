# VERSION: 1
#
# Env rules for AEGIS 
# include this file from within ipc-custom.re
#
# Create in aegisUA1Res by default

acSetRescSchemeForCreate {
  ON($objPath like "/iplant/home/shared/aegis/*") {
    msiSetDefaultResc("aegisUA1Res", "forced");
  }
}


acSetRescSchemeForRepl { 
  ON($objPath like "/iplant/home/shared/aegis/*") {
    msiSetDefaultResc("aegisReplRes", "forced"); 
  }
}

