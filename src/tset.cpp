// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp) { }

TSet::TSet(const TSet &s) : BitField(s.BitField) { }

TSet::TSet(const TBitField &bf) : BitField(bf) { }

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return BitField.GetLength();
}

int TSet::IsMember(const int Elem) const
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s)
{
    if (this != &s){
        BitField = s.BitField;
    }

    return *this;
}

int TSet::operator==(const TSet &s) const
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s)
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= GetMaxPower()){
        throw std::out_of_range("Element out of range");
    }

    TSet res = *this;
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s)
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void)
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s)
{
    int elem;
    while (istr >> elem) {
        s.InsElem(elem);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s)
{
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i)){
            ostr << i << " ";
        }
    }
    return ostr;
}
