# VERSION 2
#
# bisque.re  
# Bisque related rules for > iRods 4.0
# put this in server/config/reConfigs/bisque.re
# include this file from within ipc-custom.re
###############################################

BISQUE_GROUPS = list('NEVP', 'sernec')
BISQUE_ID_ATTR = 'ipc-bisque-id'
BISQUE_URI_ATTR = 'ipc-bisque-uri'

 
logMsg(*Msg) = writeLine('serverLog', 'BISQUE: *Msg')


mkURL(*IESHost, *Path) = "irods://" ++ *IESHost ++ *Path


# Tells BisQue to create a link for a given user to a data object.
#
# bisque_ops.py ln -P permission --alias user irods://ies.host/path/to/data.object
ln(*IESHost, *Permission, *Client, *Path) {
  logMsg("scheduling linking of *Path for *Client with permission *Permission");
  delay("<PLUSET>1s</PLUSET>") {
    logMsg("linking *Path for *Client with permission *Permission");
    *pArg = execCmdArg(*Permission);
    *aliasArg = execCmdArg(*Client);
    *pathArg = execCmdArg(mkURL(*IESHost, *Path));
    *argStr = 'ln -P *pArg --alias *aliasArg *pathArg';
    *status = errorcode(msiExecCmd("bisque_ops.py", *argStr, *IESHost, "null", "null", *out));
    if (*status != 0) {
      msiGetStderrInExecCmdOut(*out, *resp);
      logMsg('FAILURE - *resp');
      logMsg('failed to link *Path for *Client with permission *Permission');
      fail;
    } else {
      # bisque_ops.py exits normally even when an error occurs.
      msiGetStderrInExecCmdOut(*out, *errMsg);
      if (strlen(*errMsg) > 0) {
        logMsg(*errMsg);
        logMsg('failed to link *Path for *Client with permission *Permission');
        fail;
      }
      msiGetStdoutInExecCmdOut(*out, *resp);
      *props = split(trimr(triml(*resp, ' '), '/'), ' ')

# This is broken. See https://github.com/irods/irods/issues/3355
#      msiStrArray2String(*props, *kvStr);
# Workaround
      *kvStr = "";
      foreach(*prop in *props) {
        if (*kvStr == "" ) {
          *kvStr = *prop;
        } else {
          *kvStr = *kvStr ++ "%" ++ *prop;
        }
      }
# End of workaround
      msiString2KeyValPair(*kvStr, *kvs);
      msiGetValByKey(*kvs, 'resource_uniq', *qId);
      *id = substr(*qId, 1, strlen(*qId) - 2);
      msiGetValByKey(*kvs, 'uri', *qURI);
      *uri = substr(*qURI, 1, strlen(*qURI) - 2);
      msiString2KeyValPair(BISQUE_ID_ATTR ++ '=' ++ *id ++ '%' BISQUE_URI_ATTR ++ '=' ++ *uri, 
                           *kv);
      msiSetKeyValuePairsToObj(*kv, *Path, '-d');
      logMsg('linked *Path for *Client with permission *Permission');
    }
  }
}


# Tells BisQue to change the path of a linked data object.
#
# bisque_ops.py mv --alias user \
#     irods://ies.host/old/path/to/data.object irods://ies.host/new/path/to/data.object
mv(*IESHost, *Client, *OldPath, *NewPath) {
  logMsg('scheduling link move from *OldPath to *NewPath for *Client');
  delay("<PLUSET>1s</PLUSET>") {
    logMsg('moving link from *OldPath to *NewPath for *Client');
    *aliasArg = execCmdArg(*Client);
    *oldPathArg = execCmdArg(mkURL(*IESHost, *OldPath));
    *newPathArg = execCmdArg(mkURL(*IESHost, *NewPath));
    *argStr = 'mv --alias *aliasArg *oldPathArg *newPathArg';
    *status = errorcode(msiExecCmd('bisque_ops.py', *argStr, *IESHost, 'null', 'null', *out));
    if (*status != 0) {
      msiGetStderrInExecCmdOut(*out, *resp);
      logMsg('FAILURE - *resp');
      logMsg('failed to move link from *OldPath to *NewPath for *Client');
      fail;
    } else {
      # bisque_ops.py exits normally even when an error occurs.
      msiGetStderrInExecCmdOut(*out, *errMsg);
      if (strlen(*errMsg) > 0) {
        logMsg(*errMsg);
        logMsg('failed to move link from *OldPath to *NewPath for *Client');
        fail;
      }
      logMsg('moved link from *OldPath to *NewPath for *Client');
    }
  }
}


# Tells BisQue to remove a link to data object.
#
# bisque_ops.py rm --alias user irods://ies.host/path/to/data.object
rm(*IESHost, *Client, *Path) {
  logMsg("scheduling removal of linking to *Path for *Client");
  delay("<PLUSET>1s</PLUSET>") {
    logMsg("Removing link from *Path for *Client");
    *aliasArg = execCmdArg(*Client);
    *pathArg = execCmdArg(mkURL(*IESHost, *Path));
    *argStr = 'rm --alias *aliasArg *pathArg';
    *status = errorcode(msiExecCmd("bisque_ops.py", *argStr, *IESHost, "null", "null", *out));
    if (*status != 0) {
      msiGetStderrInExecCmdOut(*out, *resp);
      logMsg('FAILURE - *resp');
      logMsg('failed to remove link to *Path for *Client');
      fail;
    } else {
      # bisque_ops.py exits normally even when an error occurs.
      msiGetStderrInExecCmdOut(*out, *errMsg);
      if (strlen(*errMsg) > 0) {
        logMsg(*errMsg);
        logMsg('failed to remove link to *Path for *Client');
        fail;
      }
      logMsg('removed link to *Path for *Client');
    }
  }
}


joinPath(*ParentColl, *ObjName) = *ParentColl ++ '/' ++ *ObjName


getHomeUser(*Path) =
  let *nodes = split(*Path, '/') 
  in if size(*nodes) <= 2
     then '' 
     else let *user = elem(*nodes, 2)     
          in if *user == 'shared' then '' else *user


getClient(*Path) =
   let *homeUser = getHomeUser(*Path)
   in if *homeUser == '' then $userNameClient else *homeUser


ensureBisqueWritePerm(*Path) = msiSetACL('default', 'write', 'bisque', *Path)


ensureBisqueWritePermColl(*Path) {
  logMsg('permitting bisque user RW on *Path');
  ensureBisqueWritePerm(*Path);
  msiSetACL('recursive', 'inherit', 'null', *Path);
}


isInGroup(*Group, *Path) = *Path like '/iplant/home/shared/*Group/\*'


isInGroups(*Groups, *Path) {
  *result = false;
  foreach(*group in *Groups) {
    if (isInGroup(*group, *Path)) {
      *result = true;
      break;
    }
  }
  *result;
}

isInUser(*Path) = *Path like regex '/iplant/home/[^/]\*/bisque_data($|/.\*)' 
                  && !(*Path like '/iplant/home/shared/\*')


isForBisque(*Path) = $userNameClient != "bisque" 
                     && (isInUser(*Path) || isInGroups(BISQUE_GROUPS, *Path))


handleNewObject(*IESHost, *Client, *Path) {
  ensureBisqueWritePerm(*Path);
  *perm = if isInGroups(BISQUE_GROUPS, *Path) then 'published' else 'private';
  ln(*IESHost, *perm, *Client, *Path);
}


stripTrailingSlash(*Path) = if *Path like '*/' then trimr(*Path, '/') else *Path


determineSrc(*BaseSrcColl, *BaseDestColl, *DestEntity) = 
  let *dest = stripTrailingSlash(*DestEntity) in
    stripTrailingSlash(*BaseSrcColl) ++ 
      substr(*dest, strlen(stripTrailingSlash(*BaseDestColl)), strlen(*dest))


handleCollForBisque(*IESHost, *Client, *SrcColl, *DestColl) {
  *idAttr = BISQUE_ID_ATTR;

  foreach (*collPat in list(*DestColl, *DestColl ++ '/%')) {
    foreach (*obj in SELECT COLL_NAME, DATA_NAME WHERE COLL_NAME LIKE '*collPat') {
      *collName = *obj.COLL_NAME;
      *dataName = *obj.DATA_NAME;

      foreach (*reg in SELECT COUNT(META_DATA_ATTR_VALUE)
                       WHERE COLL_NAME = '*collName' 
                         AND DATA_NAME = '*dataName' 
                         AND META_DATA_ATTR_NAME = '*idAttr') {
        *regCnt = *reg.META_DATA_ATTR_VALUE;

        if (*regCnt == '0') {
          handleNewObject(*IESHost, *Client, joinPath(*obj.COLL_NAME, *obj.DATA_NAME));
        } else {
          *srcSubColl = determineSrc(*SrcColl, *DestColl, *obj.COLL_NAME);
          *srcObj = joinPath(*srcSubColl, *obj.DATA_NAME);
          *destObj = joinPath(*obj.COLL_NAME, *obj.DATA_NAME);
          mv(*IESHost, *Client, *srcObj, *destObj);
        }                  
      }   
    }   
  }
}


mvAll(*IESHost, *Client, *SrcColl, *DestColl) {
  *idAttr = BISQUE_ID_ATTR;

  foreach(*collPat in list(*DestColl, *DestColl ++ '/%')) {
    foreach(*row in SELECT COLL_NAME, DATA_NAME
                    WHERE COLL_NAME LIKE '*collPat' AND META_DATA_ATTR_NAME = '*idAttr') {
      *srcSubColl = determineSrc(*SrcColl, *DestColl, *row.COLL_NAME);
      *srcObj = joinPath(*srcSubColl, *row.DATA_NAME);
      *destObj = joinPath(*row.COLL_NAME, *row.DATA_NAME);
      mv(*IESHost, *Client, *srcObj, *destObj);
    }
  }
}


# Add a call to this rule from inside the acPostProcForCollCreate PEP.
bisque_acPostProcForCollCreate {
  if ($collName like regex "/iplant/home/[^/]\*/bisque_data") {
    ensureBisqueWritePermColl($collName);
  }   
}

# Add a call to this rule from inside the acPostProcForPut PEP. 
bisque_acPostProcForPut(*IESHost) {
  if (isForBisque($objPath)) {
    handleNewObject(*IESHost, getClient($objPath), $objPath);
  }
}

# Add a call to this rule from inside the acPostProcForCopy PEP.
bisque_acPostProcForCopy(*IESHost) {
  if (isForBisque($objPath)) {
    handleNewObject(*IESHost, getClient($objPath), $objPath);
  }
}


# Add a call to this rule from inside the acPostProcForObjRename PEP.
bisque_acPostProcForObjRename(*SrcEntity, *DestEntity, *IESHost) {
  *client = getClient(*SrcEntity);
  msiGetObjType(*DestEntity, *type);
 
  if (*type == '-c') {
    if (isForBisque(*DestEntity)) {
      ensureBisqueWritePermColl(*DestEntity);
      handleCollForBisque(*IESHost, *client, *SrcEntity, *DestEntity);
    } else {
      mvAll(*IESHost, *client, *SrcEntity, *DestEntity);
    }
  } else if (*type == '-d') {
    msiSplitPath(*DestEntity, *coll, *obj);
    *idAttr = BISQUE_ID_ATTR;

    foreach (*reg in SELECT COUNT(META_DATA_ATTR_NAME) 
                       WHERE COLL_NAME = '*coll' 
                       AND DATA_NAME = '*obj' 
                       AND META_DATA_ATTR_NAME = '*idAttr') {
      *regCnt = *reg.META_DATA_ATTR_NAME;

      if (*regCnt != '0') {
        mv(*IESHost, *client, *SrcEntity, *DestEntity);
      } else if (isForBisque(*DestEntity)) {
        handleNewObject(*IESHost, *client, *DestEntity);
      }
    }
  }
}


# Add a call to this rule from inside the acPostProcForDelete PEP.
bisque_acPostProcForDelete(*IESHost) {
  if (isInUser($objPath) || isInGroups(BISQUE_GROUPS, $objPath)) {
    rm(*IESHost, getClient($objPath), $objPath);
  }
}

