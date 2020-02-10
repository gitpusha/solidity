// Used to segfault.
contract C {
	struct S {
		mapping(S => uint) a;
	}
	function g (S calldata) external view {}
 }
// ----
// TypeError: (56-57): Only elementary types, contract types or enums are allowed as mapping keys.
// TypeError: (86-96): Recursive type not allowed for public or external contract functions.
