/* Qrypto 2019
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** Botan 1.11 is licensed under Simplified BSD License
** CryptoPP 5.6.2 is licensed under Boost Software License 1.0
**/
#ifndef QRYPTO_CIPHER_H
#define QRYPTO_CIPHER_H

#include "qrypto.h"

namespace Qrypto
{

/// @header qryptokeymaker.h
class KeyMaker;

/**
 * @brief The Cipher class conforms to PKCS #5 PBES2
 * @ref https://tools.ietf.org/html/rfc2898#section-6.2
 */
class Cipher
{
    struct Impl;
    friend struct Impl;

    QString m_algorithmName;
    QString m_operationCode;
    QByteArray m_authentication;
    QByteArray m_initialVector;

public:
    enum Algorithm {
        AES,
        Blowfish,
        CAST_128,
        Camellia,
        DES_EDE3,
        IDEA,
        SEED,
        Serpent,
        Twofish,
        UnknownAlgorithm
    };

    enum Operation {
        CBC,    // Cipher Block Chaining
        CFB,    // Cipher Feedback
        CTR,    // Counter
        EAX,    // Encrypt Authenticate Translate
        ECB,    // Electronic Codebook
        GCM,    // Galois Counter
        OFB,    // Output Feedback
        UnknownOperation
    };

    static const QStringList AlgorithmNames;

    static const QStringList OperationCodes;

    /**
     * @brief Cipher default constructor
     * @param algorithm
     * @param operation
     */
    Cipher(Algorithm algorithm = AES, Operation operation = GCM) :
        m_algorithmName(AlgorithmNames.at(algorithm)),
        m_operationCode(OperationCodes.at(operation))
    { }

    Error decrypt(SequreBytes &plain, const QByteArray &crypt, const KeyMaker &keyMaker);

    Error encrypt(QByteArray &crypt, const SequreBytes &plain, const KeyMaker &keyMaker);

    /**
     * @brief validateKeyLength
     * @param keyLength in bytes
     * @return a valid key length for current cipher
     */
    uint validateKeyLength(uint keyLength);

    Algorithm algorithm() const
    {
        for (int i = AlgorithmNames.size(); i-- > 0; ) {
            if (AlgorithmNames.at(i).compare(m_algorithmName, Qt::CaseInsensitive) == 0)
                return Algorithm(i);
        }

        return UnknownAlgorithm;
    }

    void setAlgorithm(Algorithm algorithm)
    { m_algorithmName = AlgorithmNames.at(algorithm); }

    QString algorithmName() const
    { return m_algorithmName; }

    void setAlgorithmName(const QString &algorithmName)
    {
        if (AlgorithmNames.contains(algorithmName, Qt::CaseInsensitive))
            m_algorithmName = algorithmName;
        else
            m_algorithmName.clear();
    }

    /**
     * @brief authentication HMAC when not using authenticated Operation during encryption
     * @return
     */
    QByteArray authentication() const
    { return m_authentication; }

    /**
     * @brief setAuthentication
     * @param authentication HMAC for non-authenticated Operation
     */
    void setAuthentication(const QByteArray &authentication)
    { m_authentication = authentication; }

    void setAuthentication(const QString &authenticationHex)
    { setAuthentication(QByteArray::fromHex(authenticationHex.toLatin1())); }

    QString fullName() const
    { return QString("%1/%2").arg(algorithmName(), operationCode()); }

    void setFullName(const QString &fullName)
    {
        m_algorithmName.clear();
        m_operationCode.clear();

        for (QStringList names = fullName.split('/'); names.size() == 2; ) {
            setAlgorithmName(names.takeFirst());
            setOperationCode(names.takeLast());
        }
    }

    /**
     * @brief initialVector will always be autogenerated in encrypt
     * @return
     */
    QByteArray initialVector() const
    { return m_initialVector; }

    void setInitialVector(const QByteArray &initialVector)
    { m_initialVector = initialVector; }

    void setInitialVector(const QString &initialVectorHex)
    { setInitialVector(QByteArray::fromHex(initialVectorHex.toLatin1())); }

    Operation operation() const
    {
        for (int i = OperationCodes.size(); i-- > 0; ) {
            if (OperationCodes.at(i).compare(m_operationCode, Qt::CaseInsensitive) == 0)
                return Operation(i);
        }

        return UnknownOperation;
    }

    void setOperation(Operation operation)
    { m_operationCode = OperationCodes.at(operation); }

    QString operationCode() const
    { return m_operationCode; }

    void setOperationCode(const QString &operationCode)
    {
        if (OperationCodes.contains(operationCode, Qt::CaseInsensitive))
            m_operationCode = operationCode;
        else
            m_operationCode.clear();
    }
};

}

#endif // QRYPTO_CIPHER_H
