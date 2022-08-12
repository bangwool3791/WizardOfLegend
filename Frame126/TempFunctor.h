#pragma once

template<typename T>
T	inline Displacement(T _x, T _y)
{
	return _y - _x;
}

template<typename T>
T	inline Distance(T _x, T _y)
{
	return abs(sqrt(_x*_x + _y*_y));
}
template<typename T>
T	inline Angle(T _x1,T _x2, T _y1, T _y2)
{
	T fX = Displacement<T>(_x1, _x2);
	T fY = Displacement<T>(_y2, _y1);
	T fR = Distance<T>(fX, fY);

	T angle = acosf(fX / fR);

	if (_y1 < _y2)
		angle = 2 * PI - angle;

	return angle;
}
template<typename T>
void	Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void	Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

// Functor

class CDeleteObj
{
public:
	template<typename T>
	void		operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void		operator()(T& Temp)
	{
		if (Temp.second)
		{
			delete Temp.second;
			Temp.second = nullptr;
		}
	}
};

class CTagFinder
{
public:
	CTagFinder(const TCHAR* pTag)
		: m_pTag(pTag)
	{

	}

public:
	template<typename T>
	bool		operator()(T& Pair)
	{
		if (!lstrcmp(Pair.first, m_pTag))
			return true;

		return false;
	}

private:
	const TCHAR*		m_pTag;
};