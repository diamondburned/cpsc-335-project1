{ pkgs ? import <nixpkgs> {} }:

let
	python3 = pkgs.python3.withPackages (ps: with ps; [
		black
	]);

	llvmPackages = pkgs.llvmPackages_latest;
	clang-unwrapped = llvmPackages.clang-unwrapped;
	clang  = llvmPackages.clang;
	clangd = pkgs.writeShellScriptBin "clangd" ''
		export CPATH="$(${clang}/bin/clang -E - -v <<< "" \
			|& ${pkgs.gnugrep}/bin/grep '^ /nix' \
			|  ${pkgs.gawk}/bin/awk 'BEGIN{ORS=":"}{print substr($0, 2)}' \
			|  ${pkgs.gnused}/bin/sed 's/:$//')"
		export CPLUS_INCLUDE_PATH="$(${clang}/bin/clang++ -E - -v <<< "" \
			|& ${pkgs.gnugrep}/bin/grep '^ /nix' \
			|  ${pkgs.gawk}/bin/awk 'BEGIN{ORS=":"}{print substr($0, 2)}' \
			|  ${pkgs.gnused}/bin/sed 's/:$//')"
	  ${clang-unwrapped}/bin/clangd
	'';
in

pkgs.mkShell {
	buildInputs = with pkgs; [
		gdb
		pyright
	] ++ [
		python3
		clangd
		clang
		clang-tools
	];
}
