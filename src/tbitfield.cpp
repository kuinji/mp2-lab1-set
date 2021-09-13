// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len)
{
    typeSize = sizeof(uint);
    if (len % typeSize)
    {
        memLen = len / typeSize + 1;
        bitLen = memLen * typeSize * 8;
        pMem = new uint[memLen];
    }
    else
    {
        memLen = len / typeSize;
        bitLen = memLen * typeSize * 8;
        pMem = new uint[memLen];
    }

    for (int i = 0; i < memLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    for (int i = 0; i < bf.bitLen; i++)
    {
        if (bf.getBit(i))
            this->setBit(i);
    }
     
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return (n / (sizeof(uint) * 8));
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    uint mask = 1;
    for (int i = 0; i < n; i++)
    {
        mask *= 2;
    }
    return mask;
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    int i = bitLen - 1;
    std::cout << *pMem;
    while (!this->getBit(i) && i > 0) { i--; }
    return i;
}

void TBitField::setBit(const size_t n) // установить бит
{
    pMem[getIndex(n)] |= getMask(n);
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    pMem[getIndex(n)] &= (~getMask(n));
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    uint mask = getMask(n);
    if (this->pMem[getIndex(n)] & mask)
        return true;
    else return false;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    return TBitField(1);
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    return TBitField(1);
}

TBitField TBitField::operator~() // отрицание
{
    return TBitField(1);
}

TBitField::~TBitField()
{

}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.memLen * bf.typeSize * 8; i >= 0; i--)
    {
        ostr << bf.getBit(i);
    }
    return ostr;
}
