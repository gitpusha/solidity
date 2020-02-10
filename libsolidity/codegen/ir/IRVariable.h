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
#pragma once

#include <optional>
#include <string>
#include <vector>

namespace solidity::frontend
{

class VariableDeclaration;
class Type;
class Expression;

/**
 * An IRVariable refers to a set of yul variables that correspond to the stack layout of a solidity variable or expression
 * of a specific solidity type. If the solidity type occupies a single stack slot, the IRVariable refers to a single yul variable.
 * Otherwise the set of yul variables it refers to is (recursively) determined by ``Type::stackItems()``.
 */
class IRVariable
{
public:
	/// IR variable referring to the declaration @a _decl.
	explicit IRVariable(VariableDeclaration const& _decl);
	/// IR variable with explicit name @a _name and type @a _type.
	IRVariable(Type const& _type, std::string _name);
	/// IR variable referring to the expression @a _expr.
	IRVariable(Expression const& _expression);

	/// @returns the name of the variable, if it occupies a single stack slot (otherwise throws).
	std::string name() const;

	/// @returns a comma-separated list of the stack components of the variable.
	std::string commaSeparatedList() const;

	/// @returns an IRVariable referring to the tuple component @a _i of a tuple variable.
	IRVariable tupleComponent(std::size_t _i) const;

	/// @returns the type of the variable.
	Type const& type() const { return m_type; }

	/// @returns an IRVariable referring to the stack component @a _slot of the variable.
	/// @a _slot must be among the stack slots in ``m_type.stackItems()``.
	/// The returned IRVariable is itself typed with the type of the stack slot as defined
	/// in ``m_type.stackItems()`` and may again occupy multiple stack slots.
	IRVariable part(std::string const& _slot) const;
private:
	/// @returns a vector containing the names of the stack components of the variable.
	std::vector<std::string> stackComponents() const;

	Type const& m_type;
	std::string m_baseName;
};


}
