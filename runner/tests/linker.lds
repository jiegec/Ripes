SECTIONS {
	. = 0x0;
	.text 0x0 : {
		*(.init)
		*(.text)
	}
	. = 0x10000000;
	.data : {
		*(.data)
		*(.rodata*)
	}
}
