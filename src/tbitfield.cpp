// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len) : bitLen(len)
{
    typeSize = sizeof(uint);
    memLen = len / (typeSize * 8);
    if (memLen * typeSize * 8 != len)
        memLen++;;
    pMem = new uint[memLen];
    
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    *this = bf;
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    if(n > bitLen)
        throw - 1;

    return (n / (sizeof(uint) * 8));
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    if (n > bitLen)
        throw - 1;

    uint mask = 1;
    return (mask <<= n);
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n > bitLen)
        throw - 1;

    pMem[getIndex(n)] |= getMask(n);
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n > bitLen)
        throw - 1;

    pMem[getIndex(n)] &= (~getMask(n));
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n > bitLen)
        throw - 1;

    if (this->pMem[getIndex(n)] & getMask(n))
        return true;
    else return false;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this == bf)
        return *this;
    else
    {
        typeSize = bf.typeSize;
        memLen = bf.memLen;
        bitLen = bf.bitLen;

        pMem = new uint[memLen];

        for (int i = 0; i < memLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bitLen != bf.bitLen) return false;
    if (typeSize != bf.typeSize) return false;
    int i = 0;
    for (;i < bitLen; i++)
    {
        if (this->getBit(i) != bf.getBit(i)) return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

bool TBitField::isGreater(int x, int y)
{
    bool greater;
    if (x >= y)
        return 0;
    else
        return 1;
}

TBitField TBitField::bitOperator(const TBitField& bf, bool (TBitField::*someOperation)(bool , bool))
{
    bool greater = isGreater(bitLen, bf.bitLen);

    const TBitField* greaterBf;
    const TBitField* smallerBf;

    if (greater == 0)
    {
        greaterBf = this;
        smallerBf = &bf;
    }
    else
    {
        greaterBf = &bf;
        smallerBf = this;
    }

    TBitField result(greaterBf->bitLen);

    int i = 0;
    for (;i < smallerBf->bitLen; i++)
    {
        if ((this->*someOperation)(smallerBf->getBit(i), greaterBf->getBit(i)))
            result.setBit(i);
    }
    for (; i < greaterBf->bitLen; i++)
    {
        if ((this->*someOperation)(0, greaterBf->getBit(i)))
            result.setBit(i);
    }

    return result;
}

bool TBitField::operatorOr(bool another, bool anotherOne)
{
    if (another + anotherOne > 0)
        return true;
    else
        return false;
}
TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return this->bitOperator(bf, &TBitField::operatorOr);
}

bool TBitField::operatorAnd(bool another, bool anotherOne)
{
    if (another + anotherOne == 2)
        return true;
    else
        return false;
}
TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return this->bitOperator(bf, &TBitField::operatorAnd);;
}

TBitField TBitField::operator~() // отрицание
{
    TBitField reverseBf(*this);
    for (int i = 0; i < memLen; i++)
    {
        reverseBf.pMem[i] = ~pMem[i];
    }
    return reverseBf;
}

TBitField::~TBitField()
{
    delete[] pMem;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    size_t maxInputLen = bf.bitLen;
    char* symbols = new char[bf.bitLen];
    char currentSymb;

    int i = 0;
    for (; i < maxInputLen + 1; i++)
    {
        currentSymb = istr.get();
        if (currentSymb == '\n' || currentSymb == ' ')
            break;
        else if (currentSymb == '0' || currentSymb == '1')
            symbols[i] = currentSymb;
        else
            throw - 1;
    }

    if (i > maxInputLen) throw - 1;

    i--;
    for (int j = 0; i >= 0 && j < bf.bitLen; i--, j++)
    {
        if(symbols[i] == '1')
            bf.setBit(j);
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.bitLen - 1; i >= 0; i--)
    {
        ostr << bf.getBit(i);
    }
    return ostr;
}
