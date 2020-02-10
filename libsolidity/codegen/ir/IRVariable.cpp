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

IRVariable IRVariable::part(string const& _name) const
{
	for (auto const& [itemName, itemType]: m_type.stackItems())
		if (itemName == _name)
		{
			solAssert(itemName.empty() || itemType, "");
			return IRVariable{itemType ? *itemType : m_type, itemName.empty() ? m_baseName : m_baseName + '_' + _name};
		}
	solAssert(false, "Invalid stack item name.");
}

vector<string> IRVariable::stackComponents() const
{
	vector<string> result;
	for (auto const& [itemName, itemType]: m_type.stackItems())
		if (itemType)
			result += part(itemName).stackComponents();
		else
		{
			solAssert(itemName.empty(), "");
			result.emplace_back(itemName.empty() ? m_baseName : m_baseName + '_' + itemName);
		}
	return result;
}

string IRVariable::commaSeparatedList() const
{
	return joinHumanReadable(stackComponents());
}

string IRVariable::name() const
{
	solAssert(m_type.sizeOnStack() == 1, "");
	auto const& [itemName, type] = m_type.stackItems().front();
	solAssert(!type, "");
	if (itemName.empty())
		return m_baseName;
	else
		return m_baseName + '_' + itemName;
}

IRVariable IRVariable::tupleComponent(size_t _i) const
{
	return part("component_" + std::to_string(_i + 1));
}
