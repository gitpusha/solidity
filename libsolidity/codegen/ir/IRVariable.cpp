/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <libsolidity/codegen/ir/IRVariable.h>
#include <libsolidity/ast/AST.h>
#include <boost/range/adaptor/transformed.hpp>
#include <libsolutil/StringUtils.h>

using namespace std;
using namespace solidity;
using namespace solidity::frontend;
using namespace solidity::util;

IRVariable::IRVariable(VariableDeclaration const& _declaration):
	m_type(*_declaration.annotation().type),
	m_baseName("vloc_" + _declaration.name() + '_' + std::to_string(_declaration.id()))
{
	solAssert(!_declaration.isStateVariable(), "");
}

IRVariable::IRVariable(Type const& _type, std::string _name):
	m_type(_type), m_baseName(std::move(_name))
{
}

IRVariable::IRVariable(Expression const& _expression):
	m_type(*_expression.annotation().type),
	m_baseName("expr_" + to_string(_expression.id()))
{
}

IRVariable IRVariable::part(std::string const& _slot, Type const& _type) const
{
	return IRVariable(_type, part(_slot));
}
string IRVariable::part(std::string const& _slot) const
{
	solAssert(contains(m_type.stackSlotNames(), _slot), "Invalid stack slot name.");
	if (_slot.empty())
		return m_baseName;
	else
		return m_baseName + '_' + _slot;
}

vector<string> IRVariable::stackComponents() const
{
	vector<string> result;
	for (auto const& slot: m_type.stackSlotNames())
		result.emplace_back(part(slot));
	return result;
}

string IRVariable::commaSeparatedList() const
{
	return joinHumanReadable(stackComponents());
}

string IRVariable::name() const
{
	solUnimplementedAssert(m_type.sizeOnStack() == 1, "");
	auto const& slot = m_type.stackSlotNames().front();
	if (slot.empty())
		return m_baseName;
	else
		return m_baseName + '_' + slot;
}

IRVariable IRVariable::tupleComponent(size_t _i) const
{
	auto const* tupleType = dynamic_cast<TupleType const*>(&m_type);
	solAssert(tupleType, "Component of non-tuple requested.");
	solAssert(_i < tupleType->components().size(), "Invalid tuple component requested.");
	TypePointer component = tupleType->components().at(_i);
	solUnimplementedAssert(component, "Placeholder tuple component requested.");
	return IRVariable(*component, part("component_" + std::to_string(_i + 1)));
}
