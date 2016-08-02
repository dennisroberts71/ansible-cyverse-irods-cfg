# aegis-env.re
# Env rules for AEGIS 
# put this in server/config/reConfigs/aegis-env.re
# include this file from within ipc-custom.re
#
# Create in aegisUA1Res by default
acSetRescSchemeForCreate {
  ON($objPath like "/iplant/home/shared/aegis/*") {
    msiSetDefaultResc("aegisRes", "forced");
  }
}
