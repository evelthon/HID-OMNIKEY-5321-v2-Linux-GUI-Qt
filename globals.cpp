#include "globals.h"

//define vars to be accessible/defined
LONG scardresp = 0;
SCARDCONTEXT hContext = 0;
LPTSTR mszReaders = NULL;
LPTSTR mszCurrentReader = NULL;
SCARDHANDLE hCard =0;
DWORD dwReaders = 0;
DWORD dwActiveProtocol = 0;
DWORD dwRecvLength = 0;
DWORD dwSendLength = 0;

QList<LPTSTR> readerList;

globals::globals(QObject *parent) :
    QObject(parent)
{
}

