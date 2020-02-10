pragma solidity >= 0.6.0;
//pragma experimental ABIEncoderV2;

contract C {
    function g(uint n) external pure returns (uint) {
        return n + 1;
    }

    function f(uint n) public view returns (uint) {
        return this.g(2 * n);
    }

    // TODO: See "Multi-slot types not yet implemented." from IRGeneratorContext::addLocalVariable().
    // h(uint256[],uint256): 0x40, 2, 3, 1, 2, 3 -> 0x20, 3, 2, 4, 6
    // function h(uint[] calldata a, uint b) external pure returns (uint[] memory c)
    // {
    //     c = new uint256[](a.length);
    //     for (uint i = 0; i < a.length; ++i)
    //         c[i] = a[i] * b;
    // }

    // i(uint256): 2 -> 12
    // function i(uint _i) public view returns (uint) {
    //     //uint256[] memory r = new uint256[](3);
    //     //(r[0], r[1], r[2]) = (1, 2, 3);
    //     // r[0] = 1;
    //     // r[1] = 2;
    //     // r[2] = 3;
    //
    //     uint[] memory s = this.h(r, _i);
    //     return s[0] + s[1] + s[2];
    // }
}

// ====
// compileViaYul: also
// ----
// g(uint256): 4 -> 5
// f(uint256): 2 -> 5
