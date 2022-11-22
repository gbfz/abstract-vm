#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <gmp.h>
#include <gmpxx.h>
#include <string>
#include <stdexcept>

template <eOperandType Type>
class Operand final : virtual public IOperand
{ 
public:
// operators 
	std::unique_ptr<const IOperand> operator+ (const IOperand& rhs) const override
	{ return decide(rhs, mpz_add, mpf_add); }

	std::unique_ptr<const IOperand> operator- (const IOperand& rhs) const override
	{ return decide(rhs, mpz_sub, mpf_sub); }

	std::unique_ptr<const IOperand> operator* (const IOperand& rhs) const override
	{ return decide(rhs, mpz_mul, mpf_mul); }

	std::unique_ptr<const IOperand> operator/ (const IOperand& rhs) const override
	{ return decide(rhs, mpz_div, mpf_div); }

	std::unique_ptr<const IOperand> operator% (const IOperand& rhs) const override
	{
		if (isFloatOperation(rhs))
			throw std::runtime_error("Modulo not supported for fractions");
		return operate<mpz_class>(rhs, mpz_mod);
	}

private:
// actors for operators 
	auto decide(const IOperand& rhs, auto&& i_func, auto&& f_func) const
	{
		if (isFloatOperation(rhs))
			return operate<mpf_class>(rhs, f_func);
		return operate<mpz_class>(rhs, i_func);
	}

	template <class gmpT>
	auto operate(const IOperand& rhs, auto&& func) const
	{
		const auto type = this->getCommonType(rhs);
		gmpT a { this->toString() };
		gmpT b { rhs.toString() };
		gmpT result;
		func(get_mp_t(result), get_mp_t(a), get_mp_t(b));
		if (didOverflow(result, type))
			throw std::runtime_error("Overflow!");
		return OperandFactory::createOperand(type, toString(result));
	}

	auto get_mp_t(mpz_class& mp) const noexcept
	{ return mp.get_mpz_t(); }

	auto get_mp_t(mpf_class& mp) const noexcept
	{ return mp.get_mpf_t(); }

// utils for operators 
	bool isFloatOperation(const IOperand& rhs) const noexcept
	{
		using enum eOperandType;
		bool l = Type == Float || Type == Double;
		bool r = rhs.getType() == Float || rhs.getType() == Double;
		return l || r;
	}

	eOperandType getCommonType(const IOperand& rhs) const noexcept
	{ return std::max(getType(), rhs.getType()); }

	auto toString(const mpf_class& c) const
	{
		mp_exp_t e;
		std::string s = c.get_str(e);
		s.insert(e, 1, '.');
		if (s.back() == '.')
			s += '0';
		return s;
	}

	auto toString(const mpz_class& c) const
	{ return c.get_str(); }

	template <class T>
	using lim = std::numeric_limits<T>;

	bool didOverflow(const auto& r, eOperandType type) const
	{
		using enum eOperandType;
		switch (type)
		{
			case Int8: return r < lim<int8_t>::min() || r > lim<int8_t>::max();
			case Int16: return r < lim<int16_t>::min() || r > lim<int16_t>::max();
			case Int32: return r < lim<int32_t>::min() || r > lim<int32_t>::max();
			case Float: return r < lim<float>::lowest() || r > lim<float>::max();
			case Double: return r < lim<double>::lowest() || r > lim<double>::max();
			default: throw std::runtime_error("Type unsupported");
		}
	}

public:
// ctors, dtors 
	Operand(const std::string& value) : IOperand(), value(value) {}

	Operand(const Operand& other) : IOperand(), value(other.value) {}

	Operand(Operand&& other) : IOperand(), value()
	{ std::exchange(value, other.value); }

	Operand() = delete;
	Operand& operator= (const Operand& other) = delete;
	Operand& operator= (Operand&& other) = delete;
	~Operand() = default; 

// utils 
	int getPrecision() const override
	{
		switch (Type)
		{
			using enum eOperandType;
			case Int8:   return 8;
			case Int16:  return 16;
			case Int32:  return 32;
			case Float:  return 32;
			case Double: return 64;
			default: throw std::runtime_error("Type unsupported");
		}
	}

	eOperandType getType() const noexcept override
	{ return Type; }

	const std::string& toString() const noexcept override
	{ return value; }

private:
// member fields 
	const std::string value;
};
