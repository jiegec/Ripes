SECTIONS {
	. = 0x0;
	.text 0x0 : {
		*(.init)
		*(.text)
	}
	.data : {
		*(.data)
	}
}
