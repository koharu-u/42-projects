{
  pkgs ? import <nixpkgs> { },
}:

let
  cFormatter42 = pkgs.python3Packages.buildPythonApplication rec {
    pname = "c-formatter-42";
    version = "0.2.8";
    pyproject = true;

    src = pkgs.fetchPypi {
      pname = "c_formatter_42";
      inherit version;
      hash = "sha256-2jGxq0zckt02OueRJEzwY5guKcn8VNveDyL84gorBXs=";
    };

    build-system = [ pkgs.python3Packages.setuptools ];
    buildInputs = with pkgs; [
      ncurses
      zlib
    ];
    nativeBuildInputs = [ pkgs.autoPatchelfHook ];
    pythonImportsCheck = [ "c_formatter_42" ];

    postInstall = ''
      ln -s "$out/bin/c_formatter_42" "$out/bin/42_formatter_c"
    '';
  };
in
pkgs.mkShell {
  packages = with pkgs; [
    cFormatter42
    clang-tools
    fd
    gcc
    gdb
    git
    gnumake
    neovim
    norminette
    ripgrep
    valgrind
  ];

  shellHook = ''
        export _42_ORIGINAL_XDG_CONFIG_HOME="''${XDG_CONFIG_HOME:-$HOME/.config}"
        export _42_NVIM_CONFIG_ROOT="$(mktemp -d "''${TMPDIR:-/tmp}/42-nvim.XXXXXX")"

        if [ -e "$_42_ORIGINAL_XDG_CONFIG_HOME/nvim" ]; then
          cp -aL "$_42_ORIGINAL_XDG_CONFIG_HOME/nvim" "$_42_NVIM_CONFIG_ROOT/nvim"
          chmod -R u+w "$_42_NVIM_CONFIG_ROOT/nvim"
        else
          mkdir -p "$_42_NVIM_CONFIG_ROOT/nvim"
        fi

        mkdir -p "$_42_NVIM_CONFIG_ROOT/nvim/lua/plugins"
        ${pkgs.coreutils}/bin/install -m 0644 /dev/null \
          "$_42_NVIM_CONFIG_ROOT/nvim/lua/plugins/zz-norminette42-shell.lua"
        ${pkgs.coreutils}/bin/tee \
          "$_42_NVIM_CONFIG_ROOT/nvim/lua/plugins/zz-norminette42-shell.lua" \
          >/dev/null <<'LUA'
    return {
      {
        "Chayanon-Ninyawee/norminette42.nvim",
        enabled = true,
        init = function()
          vim.api.nvim_create_autocmd("FileType", {
            group = vim.api.nvim_create_augroup("42ShellDisableCFormatting", { clear = true }),
            pattern = { "c", "cpp" },
            callback = function(event)
              vim.b[event.buf].autoformat = false
            end,
          })
        end,
        opts = {
          user = "kpiwan",
          mail = "kpiwan@student.42bangkok.com",
          formatOnSave = false,
        },
      },
    }
    LUA

        export XDG_CONFIG_HOME="$_42_NVIM_CONFIG_ROOT"

        _42_cleanup_nvim_config() {
          if [ -n "''${_42_NVIM_CONFIG_ROOT:-}" ] &&
            [ -d "$_42_NVIM_CONFIG_ROOT" ]; then
            ${pkgs.coreutils}/bin/rm -rf -- "$_42_NVIM_CONFIG_ROOT"
          fi
        }
        trap _42_cleanup_nvim_config EXIT

        if [ ! -f "$XDG_CONFIG_HOME/nvim/init.lua" ]; then
          echo "42 shell: no existing LazyVim config found; plugin spec staged but cannot load"
        fi
  '';
}
