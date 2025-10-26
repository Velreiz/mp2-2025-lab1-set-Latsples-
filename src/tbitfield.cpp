// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
  if (len < 0){
    throw std::invalid_argument("Negative length");
  }
    
  BitLen = len;
  MemLen = (BitLen + sizeof(TELEM)*8 - 1) / (sizeof(TELEM) * 8);
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; i++){
    pMem[i] = 0;
  }
}

TBitField::TBitField(const TBitField &bf)
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];

  for (int i = 0; i < MemLen; i++){
    pMem[i] = bf.pMem[i];
  }
    
}

TBitField::~TBitField()
{
  delete[] pMem;
  pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const
{
  return static_cast<int>(n/(sizeof(TELEM)*8));
}

TELEM TBitField::GetMemMask(const int n) const
{
  return static_cast<TELEM>(1 << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const
{
  return BitLen;
}

void TBitField::SetBit(const int n)
{
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Out of range");
  }

  if (n >= 0 && n < BitLen){
    pMem[GetMemIndex(n)] |= GetMemMask(n);
  }
   
}

void TBitField::ClrBit(const int n)
{ 
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Out of range");
  }

  if (n >= 0 && n < BitLen){
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
  }
    
}

int TBitField::GetBit(const int n) const
{
  if (n < 0 || n >= BitLen){
    throw std::out_of_range("Out of range");
  }

  if (n >= 0 && n < BitLen){
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
  }

  return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf)
{
  if (this != &bf){
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (int i = 0; i < MemLen; i++){
      pMem[i] = bf.pMem[i];
    }
  }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const
{
  if (BitLen != bf.BitLen){
    return 0;
  }

  for (int i = 0; i < MemLen; i++) {
    if (pMem[i] != bf.pMem[i]){
      return 0;
    }
  }

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf)
{

  int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
  TBitField result(maxLen);
  int minMem = MemLen < bf.MemLen ? MemLen : bf.MemLen;

  for (int i = 0; i < minMem; i++){
    result.pMem[i] = pMem[i] | bf.pMem[i];
  }

  if (MemLen > bf.MemLen){
    for (int i = bf.MemLen; i < MemLen; i++){
      result.pMem[i] = pMem[i];
    }
  }

  else if (bf.MemLen > MemLen){
    for (int i = MemLen; i < bf.MemLen; i++){
      result.pMem[i] = bf.pMem[i];
    }
  }
  return result;
}

TBitField TBitField::operator&(const TBitField &bf)
{
  int maxLen = BitLen > bf.BitLen ? BitLen : bf.BitLen;
  TBitField result(maxLen);
  int minMem = MemLen < bf.MemLen ? MemLen : bf.MemLen;

  for (int i = 0; i < minMem; i++){
    result.pMem[i] = pMem[i] & bf.pMem[i];
  }

  return result;
}

TBitField TBitField::operator~(void)
{
  TBitField res(BitLen);
  for (int i = 0; i < MemLen; i++){
    res.pMem[i] = ~pMem[i];
  }

  int extraBits = MemLen * sizeof(TELEM) * 8 - BitLen;

  if (extraBits > 0){
    res.pMem[MemLen - 1] &= (~0u >> extraBits);
  }

  return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf)
{
  for (int i = 0; i < bf.MemLen; i++){
    bf.pMem[i] = 0;
  }

  int pos = 0;
  char c;

  while (pos < bf.BitLen && istr.get(c)) {
    if (c == '1') {
        bf.pMem[bf.GetMemIndex(pos)] |= bf.GetMemMask(pos);
    }
    pos++;
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf)
{
  for (int i = 0; i < bf.BitLen; i++) {
    TELEM mask = bf.GetMemMask(i);
    TELEM val = bf.pMem[bf.GetMemIndex(i)] & mask;
    if (val)
        ostr << '1';
    else
        ostr << '0';
  }
  return ostr;
}
