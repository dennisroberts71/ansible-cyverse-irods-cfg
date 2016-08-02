# VERSION: 13
#
# All customizations done to the iRODS rule logic are placed in this file or should be included by
# this file.

# The environment specific rule customizations belong in the file ipc-env.re.  These rules have the
# highest priority.  Implementations in ipc-custom.re of rules also in ipc-env.re will be ignored.

@include "aegis-env"
@include "ipc-env"

# All iplant specific, environment independent logic goes in the file ipc-logic.re.  These rules
# will be called by the hooks implemented in ipc-custom.re.  The rule names should be prefixed with
# "ipc" and suffixed with the name of the rule hook that will call the custom rule.

@include "ipc-logic"


# THIRD PARTY RULES
#
# Third party rule logic goes in its own file, and the file should be included in this section.
# Third party rule logic should be implemented in a rule prefixed with the name of the rule file
# and suffixed with the name of the rule hook that will call the custome rule.

@include "bisque"
@include "coge"


# POLICIES

acAclPolicy { msiAclPolicy("STRICT"); }
acBulkPutPostProcPolicy { msiSetBulkPutPostProcPolicy("on"); }
acSetReServerNumProc { msiSetReServerNumProc("4"); }

acCreateCollByAdmin(*ParColl, *ChildColl) {
  msiCreateCollByAdmin(*ParColl, *ChildColl);
  ipc_acCreateCollByAdmin(*ParColl, *ChildColl);
}

acDeleteCollByAdmin(*ParColl, *ChildColl) {
  msiDeleteCollByAdmin(*ParColl, *ChildColl);
  ipc_acDeleteCollByAdmin(*ParColl, *ChildColl);
}

acDeleteCollByAdminIfPresent(*ParColl, *ChildColl) {
  *status = errormsg(ipc_acDeleteCollByAdmin(*ParColl, *ChildColl), *msg);
  if(*status < 0) { writeLine("serverLog", *msg); }

  *status = errormsg(msiDeleteCollByAdmin(*ParColl, *ChildColl),*msg);
  if(*status != 0 && *status != -808000) {
    failmsg(*status, *msg);
  } 
}

acDataDeletePolicy { ipc_acDataDeletePolicy; }


# PRE-PROC RULE HOOKS
#
# The first custom pre-proc rule that fails should cause the rest to not be executed.  Third party
# pre-proc rule effects should be rolled back if a subsequent pre-proc rule fails. Third party
# pre-proc rules should be called before any IPC pre-proc rules to ensure that third party rules
# don't invalidate IPC rules.

acPreProcForModifyAccessControl(*RecursiveFlag, *AccessLevel, *UserName, *Zone, *Path) {
  ipc_acPreProcForModifyAccessControl(*RecursiveFlag, *AccessLevel, *UserName, *Zone, *Path);
}

acPreProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit, *NAName,
                              *NAValue, *NAUnit) {
  ipc_acPreProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit,
                                    *NAName, *NAValue, *NAUnit);
}

acPreProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit) {
  ipc_acPreProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit);
}

acPreProcForModifyAVUMetadata(*Option, *SourceItemType, *TargetItemType, *SourceItemName,
                              *TargetItemName) {
  ipc_acPreProcForModifyAVUMetadata(*Option, *SourceItemType, *TargetItemType, *SourceItemName,
                                    *TargetItemName);
}

# NOTE: The camelcasing is inconsistent here
acPreprocForRmColl { ipc_acPreprocForRmColl; }

# TODO: All logic that modifies session variables needs to be changed
acPreProcForWriteSessionVariable(*Var) { succeed; }


# POST-PROC RULE HOOKS
#
# Post-proc rules cannot be rolled back on failure, so all custom post-proc rules should always be
# called.  Third part post-proc rules should be called before any IPC post-proc rules to ensure
# that third party rules don't invalidate IPC rules.

acPostProcForPut { 
  *err = errormsg(ipc_acPostProcForPut, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(bisque_acPostProcForPut, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }
}

acPostProcForCopy {
  *err = errormsg(ipc_acPostProcForCopy, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(bisque_acPostProcForCopy, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }
}

acPostProcForCollCreate { 
  *err = errormsg(ipc_acPostProcForCollCreate, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(bisque_acPostProcForCollCreate, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(coge_acPostProcForCollCreate, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }
}

acPostProcForRmColl { ipc_acPostProcForRmColl; }

acPostProcForDelete {
  *err = errormsg(ipc_acPostProcForDelete, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(bisque_acPostProcForDelete, *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }
}

acPostProcForObjRename(*SourceObject, *DestObject) {
  *err = errormsg(ipc_acPostProcForObjRename(*SourceObject, *DestObject), *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(bisque_acPostProcForObjRename(*SourceObject, *DestObject), *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }

  *err = errormsg(coge_acPostProcForObjRename(*SourceObject, *DestObject), *msg);
  if (*err < 0) { writeLine("serverLog", *msg); }
}

acPostProcForTarFileReg { ipc_acPostProcForTarFileReg; }

acPostProcForModifyAccessControl(*RecursiveFlag, *AccessLevel, *UserName, *Zone, *Path) {
  ipc_acPostProcForModifyAccessControl(*RecursiveFlag, *AccessLevel, *UserName, *Zone, *Path);
}

acPostProcForModifyDataObjMeta { ipc_acPostProcForModifyDataObjMeta; }

acPostProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit, *NAName,
                               *NAValue, *NAUnit) {
  ipc_acPostProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit,
                                     *NAName, *NAValue, *NAUnit);
}

acPostProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit) {
  ipc_acPostProcForModifyAVUMetadata(*Option, *ItemType, *ItemName, *AName, *AValue, *AUnit);
}

acPostProcForModifyAVUMetadata(*Option, *SourceItemType, *TargetItemType, *SourceItemName,
                               *TargetItemName) {
  ipc_acPostProcForModifyAVUMetadata(*Option, *SourceItemType, *TargetItemType, *SourceItemName,
                                     *TargetItemName);
}

