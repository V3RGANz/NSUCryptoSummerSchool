using namespace std;
#include <vector>
#include <iostream>
class LongNumber
{
public:
	const size_t maxsize = 10;
	LongNumber(int x)
	{
		data.reserve(1);
		sign = x > 0 ? 1 : x < 0 ? -1 : 0;
		data[0] = sign == -1 ? -x : x;
	}
	LongNumber(const LongNumber &x)
	{
		data = vector<unsigned>(x.data);
		sign = x.sign;
	}
	LongNumber(vector<unsigned> vect, int sign)
	{
		size_t size = vect.size();
		if (size > maxsize)
			throw exception("The vector size cannot be bigger than maxsize.");
		data.reserve(size);
		size_t i;
		for (i = size - 1; i != 0 && !vect[i]; --i);
		data.resize(i + 1);
		for (; i != -1; --i)
			data[i] = vect[i];
		//data = vector<unsigned>(vect.size());
		this->sign = sign;
	}
	~LongNumber() {}
	LongNumber operator-()
	{
		LongNumber ret;
		if (ret.sign)
			ret.sign = !sign;
		ret.data = vector<unsigned>(this->data);
		return ret;
	}
	LongNumber operator +(LongNumber x)
	{
		LongNumber sum, _long, _short;
		size_t ssize, lsize;
		if (data.size() < x.data.size())
		{
			_long = x;
			_short = *this;
		}
		else
		{
			_long = *this;
			_short = x;
		}
		ssize = _short.data.size();
		lsize = _long.data.size();
		sum.data.reserve(lsize);
		unsigned carry = 0;
		if (sign + x.sign)
		{
			for (size_t i = 0; i < ssize; ++i)
			{
				long t = (long)data[i] + (long)x.data[i] + carry;
				carry = t > (unsigned)-1;
				sum.data[i] = t & (unsigned)-1;
			}
			for (size_t i = ssize; i < lsize; ++i)
			{
				sum.data[i] = _long.data[i] + carry;
				carry = !sum.data[i];
			}
			if (carry)
				sum.data.push_back(1);
			sum.sign = sign + x.sign;
		}
		else
		{
			for (size_t i = 0; i < ssize; ++i)
			{
				long t = (long)_long.data[i] - (long)_short.data[i] - carry;
				carry = t < 0;
				sum.data[i] = t & (unsigned)-1;
			}
			for (size_t i = ssize; i < lsize; ++i)
			{
				sum.data[i] = _long.data[i] - carry;
				carry = sum.data[i] == -1;
			}
			if (!sum.data[lsize - 1])
				sum.data.pop_back();
			sum.sign = Compare(*this, x);
		}
		return sum;
	}
	LongNumber operator -(LongNumber x)
	{
		return *this + -x;
	}
	LongNumber operator *(LongNumber x)
	{
		LongNumber mult;
		mult.sign = sign * x.sign;
		size_t xSize = x.data.size(), thisSize = data.size();
		for (size_t i = 0; i < xSize; ++i)
		{
			mult.data.insert(data.begin(), 0);
			LongNumber term;
			term.data.resize(xSize + thisSize - 1);
			unsigned carry = 0;
			for (size_t j = thisSize - 1; j != -1; --j)
			{
				long t = (long)data[j] * (long)x.data[j] + carry;
				carry = t >> 8 * sizeof(unsigned);
				term.data[i] = t & (unsigned)-1;
			}
			if (carry)
				term.data.push_back(carry);
			mult = mult + term;
		}
		return mult;
	}
	LongNumber operator =(LongNumber &x)
	{
		return LongNumber(x);
	}
	LongNumber operator ==(LongNumber x)
	{
		return this->data == x.data && this->sign == x.sign;
	}
	LongNumber operator %(LongNumber x)
	{
		if (x.sign != 1)
			throw exception("You're not allowed to divide by a non-positive number");
		size_t divSize = data.size(), modSize = x.data.size(), n = divSize - modSize;
		if (n < divSize)
			return LongNumber(*this);
		int b = sign;
		sign = 1;
		LongNumber rest;
		rest.sign = 1;
		rest.data = vector<unsigned>(data);
		for (size_t i = n; i != -1; --i)
		{
			unsigned t = data.back() / x.data.back();
			vector<unsigned> v(i, 0);
			v.insert(v.end(), x.data.begin(), x.data.end());
			LongNumber num(v, 1), num2 = num * LongNumber(t);
			if (num > rest)
				num2 = num2 - num;
			rest = rest - num;
		}
		sign = b;
		return rest;
	}
	bool operator >(LongNumber x)
	{
		return Compare(*this, x) == 1;
	}
	bool operator <(LongNumber x)
	{
		return Compare(*this, x) == -1;
	}
	bool operator >=(LongNumber x)
	{
		return Compare(*this, x) != -1;
	}
	bool operator <=(LongNumber x)
	{
		return Compare(*this, x) != 1;
	}
private:
	LongNumber() {}
	vector<unsigned> data;
	int sign;
	static int Compare(LongNumber x, LongNumber y)
	{
		size_t xSize = x.data.size(), ySize = y.data.size();
		if (x.sign > y.sign)
			return 1;
		if (x.sign < y.sign)
			return -1;
		if (xSize > ySize)
			return x.sign;
		if (xSize < ySize)
			return -x.sign;
		else
		{
			int sign = 0;
			for (size_t i = xSize - 1; i != -1; --i)
			{
				if (x.data[i] != y.data[i])
				{
					sign = x.data[i] > y.data[i] ? x.sign : y.sign;
					break;
				}
			}
			return sign;
		}

	}
};

int main(void)
{
	return 0;
}