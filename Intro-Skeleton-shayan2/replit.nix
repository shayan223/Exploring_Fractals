{ pkgs }: {
	deps = [
		pkgs.xorg.libX11
		pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}