#include "scard.h"
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QDebug>

//extern SCARDCONTEXT hContext;
#include "globals.h"
scard::scard(QObject *parent) :
    QObject(parent)
{
}

LONG scard::establishContext()
{
    scardresp = SCardEstablishContext(SCARD_SCOPE_SYSTEM, //scope of resource manager context (user or system)
                                      NULL, //reserved for future usages and must be null
                                      NULL, //reserved for future usages and must be null
                                      &hContext); //handle to the established resource manager context


    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardEstablishContext returned error 0x%08X", scardresp);
        qDebug() << str;
    }
    qDebug() << "hContext: " << hContext;
}

LONG scard::cardReleaseContext()
{

}

LONG scard::cardIsValidContext()
{

}

LONG scard::cardConnect()
{
    //TODO: GUI selection of reader
    mszCurrentReader = readerList.at(1);//hardcoded to get the CL reader

    scardresp = SCardConnect(hContext,// Handle that identifies the resource manager context
                             mszCurrentReader,	// Name of the reader containing the target card.
                             SCARD_SHARE_SHARED,// ShareMode - Flag that indicates whether other applications may form connections to the card.
                             SCARD_PROTOCOL_T0|SCARD_PROTOCOL_T1,	// Bit mask of acceptable protocols for the connection.
                             &hCard,// Handle that identifies the connection to the smart card
                             &dwActiveProtocol);// Flag that indicates the established active protocol.
    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;
        SCardReleaseContext(hContext);			// Handle that identifies the resource manager context.
    }
}

LONG scard::cardListReaders()
{

#ifdef SCARD_AUTOALLOCATE
    dwReaders = SCARD_AUTOALLOCATE; //define a buffer with autoallocated size
#endif
    scardresp = SCardListReaders(hContext,// Handle that identifies the resource manager context
                                 NULL,// Names of the reader groups defined to the system (NULL = all)
                                 (LPTSTR)&mszReaders,// Multi-string that lists the card readers within the supplied reader groups
                                 &dwReaders);// Length of the mszReaders buffer in characters.)
    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        if(scardresp == SCARD_E_NO_READERS_AVAILABLE) {
            qDebug() << "No readers available.";
        }
        else
        {
            QString str;
            str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
            qDebug() << str;
        }

        SCardReleaseContext(hContext);			// Handle that identifies the resource manager context.
        //        return;
    }


    /*
     *  2.b. display all reader names
     */

    unsigned int idx = 0;
    //mszReaders contains reader names in a multi-string list (delimited by \0)
    mszCurrentReader = mszReaders;
    //    QStringList readers;
    //    QList<LPTSTR> readerList;
    readerList.clear(); // reset list of readers so that the do not accumulate.
    while ('\0' != *mszCurrentReader)
    {
        //Display the value
        //        QString readerVal;
        //        readerVal.sprintf("%u. Reader: %s", idx, mszCurrentReader);
        //        qDebug() << readerVal;

        //        qDebug() << "-----------";
        //        for(unsigned int i=0; i<strlen(mszCurrentReader); i++)
        //        {
        //            qDebug() << i << ". " << mszCurrentReader[i];
        //        }
        readerList.append(mszCurrentReader);//push the reader in a char array (multidimensional)
        //Advance to next value

        mszCurrentReader = mszCurrentReader + strlen(mszCurrentReader) + 1;
        idx++;
    }

    //    qDebug() << "mszReaders length: " << strlen(mszReaders);
    //    qDebug() << mszReaders;

    qDebug() << "Readers found: " << readerList.count();
    for (int i=0; i < readerList.count(); i++)
    {
        qDebug() << i+1 <<". " << readerList.at(i);
    }

}

LONG scard::cardGetAttribSerialNumber(DWORD dwAttrId)
{
    // 4. call any function required
    //    for example, we want to know the ATR of the inserted smartcard
    LPBYTE pbAtr = NULL;// Pointer to a buffer that receives the attribute (e.g. ATR)
    DWORD dwAtr = 0;// Length of the pbAtr buffer

    // a) determine length of ATR
    //Call the command with a NULL pointer to get the buffer size first.
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               NULL,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes

    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    // b) allocate memory
    pbAtr = new unsigned char[dwAtr];

    // c) get ATR
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               pbAtr,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes
    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    QString string;
    string.sprintf("%s",pbAtr);//convert from LPBYTE to QString
    scard::set_serialNumber(string);
}

LONG scard::cardGetAttribATR(DWORD dwAttrId)
{
    // 4. call any function required
    //    for example, we want to know the ATR of the inserted smartcard
    LPBYTE pbAtr = NULL;// Pointer to a buffer that receives the attribute (e.g. ATR)
    DWORD dwAtr = 0;// Length of the pbAtr buffer

    // a) determine length of ATR
    //Call the command with a NULL pointer to get the buffer size first.
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               NULL,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes

    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    // b) allocate memory
    pbAtr = new unsigned char[dwAtr];

    // c) get ATR
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               pbAtr,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes
    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    //printf("ATR length: %ld\n", dwAtrLen);
    qDebug() << "Length of the ATR buffer in bytes: " << &dwAtr;
    QString atrLen;
    atrLen.sprintf("ATR Length %ld", dwAtr); //Outputs 24!
    qDebug() << atrLen;

    qDebug() << "ATR: " << pbAtr;

    //     Output the bytes of ATR
    QString finalStr  =NULL;
    for (int i = 0; i < dwAtr; i++) {
        QString ATRstr;
        ATRstr.sprintf("%02X", pbAtr[i]);
        //            qDebug() << ATRstr;
        finalStr +=  " " + ATRstr;
        //        printf("%c", *(pbAtt+i));
    }

    //    qDebug() << finalStr;
    scard::setATR(finalStr);
}

LONG scard::cardGetAttribDeviceSystemName(DWORD dwAttrId)
{
    // 4. call any function required
    //    for example, we want to know the ATR of the inserted smartcard
    LPBYTE pbAtr2 = NULL;// Pointer to a buffer that receives the attribute (e.g. ATR)
    DWORD dwAtr = 0;// Length of the pbAtr buffer

    // a) determine length of ATR
    //Call the command with a NULL pointer to get the buffer size first.
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               NULL,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes

    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("(step 1) SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    // b) allocate memory
    pbAtr2 = new unsigned char[dwAtr];

    // c) get ATR
    scardresp = SCardGetAttrib(hCard,// Handle returned from SCardConnect
                               dwAttrId,// Identifier for the attribute to get (e.g. ATR)
                               pbAtr2,// Pointer to a buffer that receives the attribute
                               &dwAtr);// Length of the pbAttr buffer in bytes
    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("(step 2) SCardListReaders returned error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }

    qDebug() << "Length of the ATR buffer in bytes: " << &dwAtr;
    QString atrLen;
    atrLen.sprintf("ATR Length %ld", dwAtr); //Outputs 24!
    qDebug() << atrLen;

    QString string;
    string.sprintf("%s", pbAtr2);
    qDebug() << string;

    qDebug() << "ATR: " << pbAtr2;

    //     Output the bytes of ATR .
    //    QString finalStr  =NULL;
    //    for (int i = 0; i < dwAtr; i++) {
    //        QString ATRstr;
    //        ATRstr.sprintf("%02X", pbAtr[i]);
    //        //            qDebug() << ATRstr;
    //        finalStr +=  " " + ATRstr;
    //        //        printf("%c", *(pbAtt+i));
    //    }
}

void scard::cardGetProp()
{

    //    scardresp = SCardBeginTransaction( hCard );
    //    qDebug() << "SCARD resp: " << scardresp;
    //    if ( SCARD_S_SUCCESS != scardresp )
    //        qDebug() << "failed to begin transaction.";

    // send command sequence
    SCARD_IO_REQUEST sioreq;
    sioreq.dwProtocol = 0x2;
    sioreq.cbPciLength = 8;
    SCARD_IO_REQUEST rioreq;
    rioreq.cbPciLength = 8;
    rioreq.dwProtocol = 0x2;

    SCARD_IO_REQUEST pioRecvPci;
    BYTE pbRecvBuffer[255];
    //    BYTE pbSendBuffer[255];
    BYTE pbSendBuffer[] = { 0xFF, 0xCA, 0x00, 0x00, 0x00};
    dwRecvLength = 255;
    dwSendLength = 0x5;


    scardresp = SCardTransmit(hCard,
                              &sioreq,
                              pbSendBuffer,
                              dwSendLength,
                              &rioreq,
                              pbRecvBuffer,
                              &dwRecvLength);// Supplies the length of the abResponse parameter (in bytes) and receives the actual number of bytes received from the smart card

    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("command error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }
    else
    {

        qDebug() << "Transmit succesfull";

        qDebug() << "Received buffer: " << pbRecvBuffer;



        if(pbRecvBuffer[dwRecvLength-2] != 0x90 || pbRecvBuffer[dwRecvLength-1] != 0x00)
        {
        qDebug() << "Wrong return code";
//        ucByteReceive[dwRecvLength-2],ucByteReceive[dwRecvLength-1]);
//        return FALSE;
        }

    qDebug() << "Received buffer LENGTH is " << dwRecvLength-2;

        //     Output the bytes of UID
        QString finalStr = NULL;
        for (int i = 0; i < dwRecvLength-2; i++) {
            QString str;
            str.sprintf("%02X", pbRecvBuffer[i]);
            finalStr +=  " " + str;
        }
        scard::set_UID(finalStr);
        qDebug() << "UID is " << finalStr;
    }


    //    scardresp = SCardEndTransaction(hCard );
    //    qDebug() << "SCARD resp: " << scardresp;
    //    if ( SCARD_S_SUCCESS != scardresp )
    //        qDebug() << "failed to end  transaction.";
}

void scard::cardReadData()
{
    //Description: Read the data from the card
    //APDU Description: ClassByte bcla = 0xFF, Instruction Byte bins=0xB0 ,Parameter P1=Address MSB , Parameter P2=Address LSB
    //                  Le=Number of Bytes to be Read

//    SCARD_IO_REQUEST sioreq;
//    sioreq.dwProtocol = 0x2;
//    sioreq.cbPciLength = 8;
//    SCARD_IO_REQUEST rioreq;
//    rioreq.cbPciLength = 8;
//    rioreq.dwProtocol = 0x2;

//    byte bcla = 0xFF;
//    byte bins = 0xB0;
//    byte bp1 = 0x0;
//    byte bp2 = currentBlock;
//    byte len = 0x5;
//    byte sendBuffer[255],receiveBuffer[255];
//    ULONG sendbufferlen,receivebufferlen;

//    sendBuffer[0] = bcla;
//    sendBuffer[1] = bins;
//    sendBuffer[2] = bp1;
//    sendBuffer[3] = bp2;
//    sendBuffer[4] = 0x0;
//    sendbufferlen = len;
//    receivebufferlen = 0x12;


    // send command sequence
    SCARD_IO_REQUEST sioreq;
    sioreq.dwProtocol = 0x2;
    sioreq.cbPciLength = 8;
    SCARD_IO_REQUEST rioreq;
    rioreq.cbPciLength = 8;
    rioreq.dwProtocol = 0x2;

    SCARD_IO_REQUEST pioRecvPci;
    BYTE pbRecvBuffer[255];
    //    BYTE pbSendBuffer[255];
    //See paragraph 5.2.1
    BYTE pbSendBuffer[] = {0xFF, 0xB0, 0x00, 0x00, 0x08};//read  command
    dwRecvLength = 255;
    dwSendLength = sizeof(pbSendBuffer);


    BYTE pbSelectPageBuffer[] = {0XFF, 0xA6, 0x01, 0x00, 0x01,
                                 0x01};// Data field format for page number & book selection on a 32KS card.
//    dwSelectLength = sizeof(pbSelectPageBuffer);

    //Issue a select page command
//    scardresp = SCardTransmit(hCard,
//                              &sioreq,
//                              pbSelectPageBuffer,
//                              sizeof(pbSelectPageBuffer),
//                              &rioreq,
//                              pbRecvBuffer,
//                              &dwRecvLength);// Supplies the length of the abResponse parameter (in bytes) and receives the actual number of bytes received from the smart card
//    if (scardresp != SCARD_S_SUCCESS) //if an error occured
//    {
//        QString str;
//        str.sprintf("command error 0x%08X", scardresp);
//        qDebug() << str;

//        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
//        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
//        //        return;
//    }
//    else
//    {
//        qDebug() << "Received length for SELECT BOOK/PAGE is " << dwRecvLength;
//    }

    //Issue a read command
    scardresp = SCardTransmit(hCard,
                              &sioreq,
                              pbSendBuffer,
                              dwSendLength,
                              &rioreq,
                              pbRecvBuffer,
                              &dwRecvLength);// Supplies the length of the abResponse parameter (in bytes) and receives the actual number of bytes received from the smart card

//    scardresp = SCardCLICCTransmit (hCard,
//                                    pbSendBuffer,
//                                    dwSendLength,
//                                    pbRecvBuffer,
//                                       &dwRecvLength);

    if (scardresp != SCARD_S_SUCCESS) //if an error occured
    {
        QString str;
        str.sprintf("command error 0x%08X", scardresp);
        qDebug() << str;

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);// Action to take on the card in the connected reader on close.
        SCardReleaseContext(hContext);// Handle that identifies the resource manager context.
        //        return;
    }
    else
    {
        qDebug() << "Received length is " << dwRecvLength;

        for(int i=0; i < dwRecvLength; i++)
        {
QString str;
        str.sprintf("0x%02X", pbRecvBuffer[i]);
        qDebug() << str;
        }
    }
}

void scard::set_serialNumber(QString serial)
{
    var_serialNumber = "";
    var_serialNumber = serial;
}

QString scard::get_serialNumber()
{
    return var_serialNumber;
}

void scard::setATR(QString str)
{
    var_ATR = "";
    var_ATR = str.trimmed();
}

QString scard::getATR()
{
    //    qDebug() << "ATR is " << var_ATR;
    return var_ATR;
}

void scard::set_UID(QString UID)
{
    var_UID = "";
    var_UID = UID.trimmed();
}

QString scard::get_UID()
{
    return var_UID;
}
