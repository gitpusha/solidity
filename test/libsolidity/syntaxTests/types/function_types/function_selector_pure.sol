interface Banana {
    function transfer(address,uint256) external returns(bool);
}

contract Apple {
    bytes4 constant bananaTransfer1 = Banana.transfer.selector;
}
