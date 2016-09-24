#ifndef SCARD_H
#define SCARD_H

#include <QObject>
#include <debuglog.h>
#include <reader.h>
//#include <pcsclite.h>
#include <winscard.h>
#include <wintypes.h>
#include "scardcl.h"

//#include "/usr/include/openssl/rsa.h"
//#include "/usr/include/openssl/sha.h"

//#include "/usr/include/openssl/rand.h"
/*
 *access global variables declared in main()
 */

extern SCARDCONTEXT hContext;
extern LONG rv;



class scard : public QObject
{
    Q_OBJECT

//    LONG rv;

//    LPTSTR mszReaders;
//    SCARDHANDLE hCard;
//    DWORD dwReaders, dwActiveProtocol, dwRecvLength;

//    SCARD_IO_REQUEST pioSendPci;

public:
    explicit scard(QObject *parent = 0);

    PCSC_API LONG establishContext();
    PCSC_API LONG cardReleaseContext();
    PCSC_API LONG cardIsValidContext();
    PCSC_API LONG cardConnect();

    PCSC_API LONG cardReconnect(SCARDHANDLE hCard,
        DWORD dwShareMode,
        DWORD dwPreferredProtocols,
        DWORD dwInitialization, /*@out@*/ LPDWORD pdwActiveProtocol);

    PCSC_API LONG cardDisconnect(SCARDHANDLE hCard, DWORD dwDisposition);

    PCSC_API LONG cardBeginTransaction(SCARDHANDLE hCard);

    PCSC_API LONG cardEndTransaction(SCARDHANDLE hCard, DWORD dwDisposition);

    PCSC_API LONG cardStatus(SCARDHANDLE hCard,
        /*@null@*/ /*@out@*/ LPSTR mszReaderName,
        /*@null@*/ /*@out@*/ LPDWORD pcchReaderLen,
        /*@null@*/ /*@out@*/ LPDWORD pdwState,
        /*@null@*/ /*@out@*/ LPDWORD pdwProtocol,
        /*@null@*/ /*@out@*/ LPBYTE pbAtr,
        /*@null@*/ /*@out@*/ LPDWORD pcbAtrLen);

    PCSC_API LONG cardGetStatusChange(SCARDCONTEXT hContext,
        DWORD dwTimeout,
        LPSCARD_READERSTATE rgReaderStates, DWORD cReaders);

    PCSC_API LONG cardControl(SCARDHANDLE hCard, DWORD dwControlCode,
        LPCVOID pbSendBuffer, DWORD cbSendLength,
        /*@out@*/ LPVOID pbRecvBuffer, DWORD cbRecvLength,
        LPDWORD lpBytesReturned);

    PCSC_API LONG cardTransmit(SCARDHANDLE hCard,
        const SCARD_IO_REQUEST *pioSendPci,
        LPCBYTE pbSendBuffer, DWORD cbSendLength,
        /*@out@*/ SCARD_IO_REQUEST *pioRecvPci,
        /*@out@*/ LPBYTE pbRecvBuffer, LPDWORD pcbRecvLength);


    PCSC_API LONG cardListReaderGroups(SCARDCONTEXT hContext,
        /*@out@*/ LPSTR mszGroups, LPDWORD pcchGroups);

    PCSC_API LONG cardListReaders();

    PCSC_API LONG cardFreeMemory(SCARDCONTEXT hContext, LPCVOID pvMem);

    PCSC_API LONG cardCancel(SCARDCONTEXT hContext);


    //reader properties
    PCSC_API LONG cardGetAttribSerialNumber(DWORD dwAttrId);
    //card properties
    PCSC_API LONG cardGetAttribATR(DWORD dwAttrId);
    PCSC_API LONG cardGetAttribDeviceSystemName(DWORD dwAttrId);

    PCSC_API LONG cardSetAttrib(SCARDHANDLE hCard, DWORD dwAttrId,
        LPCBYTE pbAttr, DWORD cbAttrLen);



    void cardGetProp();

    void cardReadData();


    QString var_serialNumber;
    void set_serialNumber(QString serial);
    QString get_serialNumber();

   QString var_ATR;
   void setATR(QString str);
   QString getATR();

   QString var_UID;
   void set_UID(QString UID);
   QString get_UID();

signals:

public slots:

};

#endif // SCARD_H
